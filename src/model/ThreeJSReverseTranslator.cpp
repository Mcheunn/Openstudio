/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ThreeJSReverseTranslator.hpp"
#include "ThreeJSForwardTranslator.hpp"

#include "FileOperations.hpp"
#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "BuildingStory.hpp"
#include "BuildingStory_Impl.hpp"
#include "BuildingUnit.hpp"
#include "BuildingUnit_Impl.hpp"
#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "ConstructionAirBoundary.hpp"
#include "ConstructionAirBoundary_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup_Impl.hpp"
#include "PlanarSurfaceGroup.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "DaylightingControl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"

#include <thread>

#include <cmath>

namespace openstudio {
namespace model {

  Point3dVectorVector ThreeJSReverseTranslator::getFaces(const ThreeGeometryData& data) const {
    Point3dVectorVector result;

    const Point3dVector vertices = fromThreeVector(data.vertices());
    const std::vector<size_t> faces = data.faces();
    const size_t n = faces.size();

    if (n < 1) {
      return result;
    }

    if (faces[0] == openstudioFaceFormatId()) {
      // openstudio, all vertices belong to one face
      Point3dVector face;

      // faces[0] is format
      for (size_t i = 1; i < n; ++i) {
        face.push_back(vertices[faces[i]]);
      }

      //        try{
      //          Plane p(face);
      //        } catch (const std::exception&)
      //        {
      //          std::cout << "Vertices: " << vertices << '\n';
      //          std::cout << "faces: " << '\n';
      //          for (const auto& f : faces){
      //            std::cout << "  " << f << '\n';
      //          }
      //          bool t = false;
      //        }

      result.push_back(face);
    }

    return result;
  }

  ConstructionAirBoundary ThreeJSReverseTranslator::getAirWallConstruction(Model& model) {
    boost::optional<ConstructionAirBoundary> result = model.getConcreteModelObjectByName<ConstructionAirBoundary>("AirWall");
    if (!result) {
      ConstructionAirBoundary airWall(model);
      airWall.setName("AirWall");
      return airWall;
    }
    OS_ASSERT(result);
    return result.get();
  }

  ThreeJSReverseTranslator::ThreeJSReverseTranslator() {
    m_logSink.setLogLevel(Warn);
    //m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.ThreeJSReverseTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> ThreeJSReverseTranslator::warnings() const {
    std::vector<LogMessage> result = m_logSink.logMessages();
    result.erase(std::remove_if(result.begin(), result.end(), [](const auto& logMessage) { return logMessage.logLevel() != Warn; }), result.end());
    return result;
  }

  std::vector<LogMessage> ThreeJSReverseTranslator::errors() const {
    std::vector<LogMessage> result = m_logSink.logMessages();
    result.erase(std::remove_if(result.begin(), result.end(), [](const auto& logMessage) { return logMessage.logLevel() <= Warn; }), result.end());
    return result;
  }

  std::map<UUID, UUID> ThreeJSReverseTranslator::handleMapping() const {
    return m_handleMapping;
  }

  unsigned getIddObjectTypeOrder(const std::string& iddObjectType) {
    unsigned typeOrder = 999;
    if (istringEqual(iddObjectType, "OS:Space")) {
      typeOrder = 0;
    } else if (istringEqual(iddObjectType, "OS:ThermalZone")) {
      typeOrder = 1;
    } else if (istringEqual(iddObjectType, "OS:SpaceType")) {
      typeOrder = 2;
    } else if (istringEqual(iddObjectType, "OS:BuildingStory")) {
      typeOrder = 3;
    } else if (istringEqual(iddObjectType, "OS:BuildingUnit")) {
      typeOrder = 4;
    } else if (istringEqual(iddObjectType, "OS:DefaultConstructionSet")) {
      typeOrder = 5;
    }
    return typeOrder;
  }

  bool sortModelObjectMetadata(const ThreeModelObjectMetadata& lhs, const ThreeModelObjectMetadata& rhs) {
    unsigned leftTypeOrder = getIddObjectTypeOrder(lhs.iddObjectType());
    unsigned rightTypeOrder = getIddObjectTypeOrder(rhs.iddObjectType());

    if (leftTypeOrder == rightTypeOrder) {
      return lhs.name() < rhs.name();
    }
    return leftTypeOrder < rightTypeOrder;
  }

  unsigned getUserDataSurfaceTypeOrder(const std::string& userDataSurfaceType) {

    /// surfaceType is overloaded as a more general type:
    /// Surfaces {"Wall", "Floor", "RoofCeiling"}
    /// SubSurfaces {"FixedWindow", "OperableWindow", "GlassDoor", "Skylight", "TubularDaylightDome", "TubularDaylightDiffuser", "Door", "OverheadDoor"}
    /// ShadingSurfaces {"SiteShading", "BuildingShading", "SpaceShading"}
    /// InteriorPartitionSurfaces {"InteriorPartitionSurface"}
    /// DaylightingControl {"DaylightingControl"}

    unsigned typeOrder = 999;
    if (istringEqual(userDataSurfaceType, "Wall") || istringEqual(userDataSurfaceType, "Floor") || istringEqual(userDataSurfaceType, "RoofCeiling")) {
      typeOrder = 0;
    } else if (istringEqual(userDataSurfaceType, "FixedWindow") || istringEqual(userDataSurfaceType, "OperableWindow")
               || istringEqual(userDataSurfaceType, "GlassDoor") || istringEqual(userDataSurfaceType, "Skylight")
               || istringEqual(userDataSurfaceType, "TubularDaylightDome") || istringEqual(userDataSurfaceType, "TubularDaylightDiffuser")
               || istringEqual(userDataSurfaceType, "Door") || istringEqual(userDataSurfaceType, "OverheadDoor")) {
      typeOrder = 1;
    } else if (istringEqual(userDataSurfaceType, "SiteShading") || istringEqual(userDataSurfaceType, "BuildingShading")
               || istringEqual(userDataSurfaceType, "SpaceShading")) {
      typeOrder = 2;
    } else if (istringEqual(userDataSurfaceType, "InteriorPartitionSurface")) {
      typeOrder = 3;
    } else if (istringEqual(userDataSurfaceType, "DaylightingControl")) {
      typeOrder = 4;
    }
    return typeOrder;
  }

  bool sortSceneChildren(const ThreeSceneChild& lhs, const ThreeSceneChild& rhs) {
    unsigned leftTypeOrder = getUserDataSurfaceTypeOrder(lhs.userData().surfaceType());
    unsigned rightTypeOrder = getUserDataSurfaceTypeOrder(rhs.userData().surfaceType());

    if (leftTypeOrder == rightTypeOrder) {
      return lhs.userData().name() < rhs.userData().name();
    }
    return leftTypeOrder < rightTypeOrder;
  }

  boost::optional<RenderingColor> makeRenderingColor(const std::string& color, Model& model) {
    return RenderingColor::fromColorString(color, model);
  }

  boost::optional<Model> ThreeJSReverseTranslator::modelFromThreeJS(const ThreeScene& scene) {
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();

    /// Mapping between handles referenced in ThreeScene (keys) and handles of objects in returned model (values) for last translation
    m_handleMapping.clear();

    Model model;
    initializeModelObjects(model);

    std::map<std::string, Space> originalNameToSpaceMap;
    std::map<std::string, ShadingSurfaceGroup> originalNameToShadingSurfaceGroupMap;
    std::map<std::string, ThermalZone> originalNameToThermalZoneMap;
    std::map<std::string, SpaceType> originalNameToSpaceTypeMap;
    std::map<std::string, BuildingStory> originalNameToBuildingStoryMap;
    std::map<std::string, BuildingUnit> originalNameToBuildingUnitMap;
    std::map<std::string, DefaultConstructionSet> originalNameToDefaultConstructionSetMap;
    std::map<std::string, Surface> originalNameToSurfaceMap;
    std::map<std::string, SubSurface> originalNameToSubSurfaceMap;
    std::map<std::string, unsigned> originalNameToSpaceMultiplierMap;
    std::map<std::string, unsigned> originalNameToThermalZoneMultiplierMap;

    // create all the objects we will need
    ThreeSceneMetadata metadata = scene.metadata();
    std::vector<ThreeModelObjectMetadata> modelObjectMetadata = metadata.modelObjectMetadata();

    // set the model's north axis, opposite of the floorspace and threejs convention
    model.getUniqueModelObject<Building>().setNorthAxis(-metadata.northAxis());

    // sort object types so we make spaces first, etc
    std::sort(modelObjectMetadata.begin(), modelObjectMetadata.end(), sortModelObjectMetadata);

    for (const auto& m : modelObjectMetadata) {
      std::string iddObjectType = m.iddObjectType();
      UUID handle = toUUID(m.handle());
      std::string name = m.name();
      std::string color = m.color();

      boost::optional<ModelObject> modelObject;

      if (istringEqual(iddObjectType, "OS:Space")) {
        Space space(model);
        modelObject = space;

        unsigned multiplier = 1;
        if (m.multiplier()) {
          multiplier = m.multiplier().get();
        }

        // DLM: temp code
        if (multiplier == 0) {
          LOG(Warn, "Muliplier for Space '" << name << "' is 0, setting to 1");
          multiplier = 1;
        } else if (multiplier > 1) {
          LOG(Warn, "Multiplier translation not yet implemented for Space '" << name << "', setting to 1");
          multiplier = 1;
        }

        originalNameToSpaceMultiplierMap.insert(std::make_pair(name, multiplier));
        originalNameToSpaceMap.insert(std::make_pair(name, space));
      } else if (istringEqual(iddObjectType, "OS:ShadingSurfaceGroup")) {
        ShadingSurfaceGroup shadingSurfaceGroup(model);
        modelObject = shadingSurfaceGroup;
        originalNameToShadingSurfaceGroupMap.insert(std::make_pair(name, shadingSurfaceGroup));
      } else if (istringEqual(iddObjectType, "OS:ThermalZone")) {
        ThermalZone thermalZone(model);
        boost::optional<RenderingColor> renderingColor = makeRenderingColor(color, model);
        if (renderingColor) {
          thermalZone.setRenderingColor(*renderingColor);
        }
        modelObject = thermalZone;
        originalNameToThermalZoneMap.insert(std::make_pair(name, thermalZone));
      } else if (istringEqual(iddObjectType, "OS:SpaceType")) {
        SpaceType spaceType(model);
        boost::optional<RenderingColor> renderingColor = makeRenderingColor(color, model);
        if (renderingColor) {
          spaceType.setRenderingColor(*renderingColor);
        }
        modelObject = spaceType;
        originalNameToSpaceTypeMap.insert(std::make_pair(name, spaceType));
      } else if (istringEqual(iddObjectType, "OS:BuildingStory")) {
        BuildingStory buildingStory(model);

        // boost::optional<unsigned> multiplier = m.multiplier(); // DLM: should we apply multipliers only to Spaces in ThreeJS?
        boost::optional<double> nominalZCoordinate = m.nominalZCoordinate();
        boost::optional<double> belowFloorPlenumHeight = m.belowFloorPlenumHeight();
        boost::optional<double> floorToCeilingHeight = m.floorToCeilingHeight();
        boost::optional<double> aboveCeilingPlenumHeight = m.aboveCeilingPlenumHeight();

        if (nominalZCoordinate) {
          buildingStory.setNominalZCoordinate(*nominalZCoordinate);
        }

        if (floorToCeilingHeight) {
          buildingStory.setNominalFloortoCeilingHeight(*floorToCeilingHeight);
          if (belowFloorPlenumHeight && aboveCeilingPlenumHeight) {
            buildingStory.setNominalFloortoFloorHeight(*floorToCeilingHeight + *belowFloorPlenumHeight + *aboveCeilingPlenumHeight);
          }
        }

        boost::optional<RenderingColor> renderingColor = makeRenderingColor(color, model);
        if (renderingColor) {
          buildingStory.setRenderingColor(*renderingColor);
        }

        modelObject = buildingStory;
        originalNameToBuildingStoryMap.insert(std::make_pair(name, buildingStory));
      } else if (istringEqual(iddObjectType, "OS:BuildingUnit")) {
        BuildingUnit buildingUnit(model);
        boost::optional<RenderingColor> renderingColor = makeRenderingColor(color, model);
        if (renderingColor) {
          buildingUnit.setRenderingColor(*renderingColor);
        }
        modelObject = buildingUnit;
        originalNameToBuildingUnitMap.insert(std::make_pair(name, buildingUnit));
      } else if (istringEqual(iddObjectType, "OS:DefaultConstructionSet")) {
        DefaultConstructionSet defaultConstructionSet(model);
        modelObject = defaultConstructionSet;
        originalNameToDefaultConstructionSetMap.insert(std::make_pair(name, defaultConstructionSet));
      } else {
        LOG(Error, "Unknown IddObjectType '" << iddObjectType << "'");
      }

      if (modelObject) {
        modelObject->setName(name);

        if (!handle.isNull()) {
          m_handleMapping[handle] = modelObject->handle();
        }
      }
    }

    // now translate all the surfaces
    ThreeSceneObject sceneObject = scene.object();
    std::vector<ThreeSceneChild> children = sceneObject.children();

    // sort the children to create all surfaces before sub surfaces
    std::sort(children.begin(), children.end(), sortSceneChildren);

    for (const auto& child : children) {
      boost::optional<ThreeGeometry> geometry = scene.getGeometry(child.geometry());
      if (!geometry) {
        continue;
      }

      Point3dVectorVector faces = getFaces(geometry->data());

      boost::optional<ThreeMaterial> material = scene.getMaterial(child.material());

      ThreeUserData userData = child.userData();

      std::string handle = userData.handle();
      std::string name = userData.name();
      std::string surfaceType = userData.surfaceType();
      std::string constructionName = userData.constructionName();
      // UUID constructionHandle = toUUID(userData.constructionHandle());
      std::string surfaceName = userData.surfaceName();
      // UUID surfaceHandle = toUUID(userData.surfaceHandle());
      std::string subSurfaceName = userData.subSurfaceName();
      // UUID subSurfaceHandle = toUUID(userData.subSurfaceHandle());
      std::string spaceName = userData.spaceName();
      // UUID spaceHandle = toUUID(userData.spaceHandle());
      std::string shadingName = userData.shadingName();
      // UUID shadingHandle = toUUID(userData.shadingHandle());
      std::string thermalZoneName = userData.thermalZoneName();
      // UUID thermalZoneHandle = toUUID(userData.thermalZoneHandle());
      std::string spaceTypeName = userData.spaceTypeName();
      // UUID spaceTypeHandle = toUUID(userData.spaceTypeHandle());
      std::string buildingStoryName = userData.buildingStoryName();
      // UUID buildingStoryHandle = toUUID(userData.buildingStoryHandle());
      std::string buildingUnitName = userData.buildingUnitName();
      // UUID buildingUnitHandle = toUUID(userData.buildingUnitHandle());
      std::string constructionSetName = userData.constructionSetName();
      // UUID constructionSetHandle = toUUID(userData.constructionSetHandle());
      std::string outsideBoundaryCondition = userData.outsideBoundaryCondition();
      std::string outsideBoundaryConditionObjectName = userData.outsideBoundaryConditionObjectName();
      std::string outsideBoundaryConditionObjectHandle = userData.outsideBoundaryConditionObjectHandle();
      double illuminanceSetpoint = userData.illuminanceSetpoint();
      bool airWall = userData.airWall();
      //bool plenum = userData.plenum();
      //bool belowFloorPlenum = userData.belowFloorPlenum();
      //bool aboveFloorPlenum = userData.aboveCeilingPlenum();

      // DLM: these objects may have been renamed on import (e.g. if space and space type have the same name)
      // need to keep a map of original name to object for each type rather than relying on model name

      boost::optional<ThermalZone> thermalZone;
      if (!thermalZoneName.empty()) {
        const auto it = originalNameToThermalZoneMap.find(thermalZoneName);
        if (it != originalNameToThermalZoneMap.end()) {
          thermalZone = it->second;
        } else {
          LOG(Error, "Could not find ThermalZone '" << thermalZoneName << "'");
        }
      }

      boost::optional<SpaceType> spaceType;
      if (!spaceTypeName.empty()) {
        const auto it = originalNameToSpaceTypeMap.find(spaceTypeName);
        if (it != originalNameToSpaceTypeMap.end()) {
          spaceType = it->second;
        } else {
          LOG(Error, "Could not find SpaceType '" << spaceTypeName << "'");
        }
      }

      boost::optional<BuildingStory> buildingStory;
      if (!buildingStoryName.empty()) {
        const auto it = originalNameToBuildingStoryMap.find(buildingStoryName);
        if (it != originalNameToBuildingStoryMap.end()) {
          buildingStory = it->second;
        } else {
          LOG(Error, "Could not find BuildingStory '" << buildingStoryName << "'");
        }
      }

      boost::optional<BuildingUnit> buildingUnit;
      if (!buildingUnitName.empty()) {
        const auto it = originalNameToBuildingUnitMap.find(buildingUnitName);
        if (it != originalNameToBuildingUnitMap.end()) {
          buildingUnit = it->second;
        } else {
          LOG(Error, "Could not find BuildingUnit '" << buildingUnitName << "'");
        }
      }

      boost::optional<DefaultConstructionSet> constructionSet;
      if (!constructionSetName.empty()) {
        const auto it = originalNameToDefaultConstructionSetMap.find(constructionSetName);
        if (it != originalNameToDefaultConstructionSetMap.end()) {
          constructionSet = it->second;
        } else {
          LOG(Error, "Could not find DefaultConstructionSet '" << constructionSetName << "'");
        }
      }

      // Check if creating a surface
      if (istringEqual(surfaceType, "Wall") || istringEqual(surfaceType, "Floor") || istringEqual(surfaceType, "RoofCeiling")) {

        if (spaceName.empty()) {
          spaceName = "Default Space";
        }

        boost::optional<Space> space;
        if (!spaceName.empty()) {
          const auto it = originalNameToSpaceMap.find(spaceName);
          if (it != originalNameToSpaceMap.end()) {
            space = it->second;
          } else {
            LOG(Error, "Could not find Space '" << spaceName << "'");
            continue;
          }
        } else {
          LOG(Error, "Surface missing required Space Name");
          continue;
        }
        OS_ASSERT(space);

        unsigned multiplier = 1;
        const auto it = originalNameToSpaceMultiplierMap.find(spaceName);
        if (it != originalNameToSpaceMultiplierMap.end()) {
          multiplier = it->second;
        } else {
          LOG(Warn, "Could not find muliplier for Space '" << spaceName << "'");
        }

        if (thermalZone) {
          if (!space->thermalZone()) {

            const auto it = originalNameToThermalZoneMultiplierMap.find(thermalZoneName);
            if (it != originalNameToThermalZoneMultiplierMap.end()) {
              if (it->second != multiplier) {
                LOG(Warn, "Varying mulipliers detected for ThermalZone '" << thermalZoneName << "', setting multiplier to 1");
                thermalZone->setMultiplier(1);
              }
            } else {
              originalNameToThermalZoneMultiplierMap[thermalZoneName] = multiplier;
              thermalZone->setMultiplier(multiplier);
            }

            space->setThermalZone(*thermalZone);
          }
        } else if (multiplier > 1) {
          LOG(Warn, "Space '" << spaceName << "' has muliplier of " << multiplier << " but no ThermalZone, multiplier ignored");
        }

        if (spaceType) {
          if (!space->spaceType()) {
            space->setSpaceType(*spaceType);
          }
        }

        if (buildingStory) {
          if (!space->buildingStory()) {
            space->setBuildingStory(*buildingStory);
          }
        }

        if (buildingUnit) {
          if (!space->buildingUnit()) {
            space->setBuildingUnit(*buildingUnit);
          }
        }

        if (constructionSet) {
          if (!space->defaultConstructionSet()) {
            space->setDefaultConstructionSet(*constructionSet);
          }
        }

        for (const auto& face : faces) {
          try {
            // ensure we can create a plane before calling Surface ctor that might mess up the model
            Plane plane(face);

            bool test;

            Surface surface(face, model);
            test = surface.setName(name).has_value();
            if (!test) {
              LOG(Warn, "Could not set Surface name '" << name << "' for Surface '" << surface.nameString() << "'");
            }
            surface.setSpace(*space);
            if (!test) {
              LOG(Warn, "Could not set Space '" << space->nameString() << "' for Surface '" << surface.nameString() << "'");
            }
            surface.setSurfaceType(surfaceType);
            if (!test) {
              LOG(Warn, "Could not set SurfaceType '" << surfaceType << "' for Surface '" << surface.nameString() << "'");
            }

            if (airWall) {
              surface.setConstruction(getAirWallConstruction(model));
            }

            originalNameToSurfaceMap.insert(std::make_pair(name, surface));

            // DLM: can we use these to set adjacencies?
            //std::string outsideBoundaryCondition = userData.outsideBoundaryCondition();
            //std::string outsideBoundaryConditionObjectName = userData.outsideBoundaryConditionObjectName();
            //std::string outsideBoundaryConditionObjectHandle = userData.outsideBoundaryConditionObjectHandle();

          } catch (const std::exception&) {
            LOG(Warn, "Could not create surface for vertices " << face);
          }
        }

      } else if (istringEqual(surfaceType, "FixedWindow") || istringEqual(surfaceType, "OperableWindow") || istringEqual(surfaceType, "GlassDoor")
                 || istringEqual(surfaceType, "Skylight") || istringEqual(surfaceType, "TubularDaylightDome")
                 || istringEqual(surfaceType, "TubularDaylightDiffuser") || istringEqual(surfaceType, "Door")
                 || istringEqual(surfaceType, "OverheadDoor")) {

        boost::optional<Surface> surface;
        if (!surfaceName.empty()) {
          const auto it = originalNameToSurfaceMap.find(surfaceName);
          if (it != originalNameToSurfaceMap.end()) {
            surface = it->second;
          } else {
            LOG(Error, "Could not find Surface '" << surfaceName << "'");
            continue;
          }
        } else {
          LOG(Error, "SubSurface missing required Surface Name");
          continue;
        }
        OS_ASSERT(surface);

        for (const auto& face : faces) {
          try {
            // ensure we can create a plane before calling Surface ctor that might mess up the model
            Plane plane(face);

            bool test;

            SubSurface subSurface(face, model);
            test = subSurface.setName(name).has_value();
            if (!test) {
              LOG(Warn, "Could not set SubSurface name '" << name << "' for SubSurface '" << subSurface.nameString() << "'");
            }
            test = subSurface.setSurface(*surface);
            if (!test) {
              LOG(Warn, "Could not set SubSurface to '" << surface->nameString() << "' for SubSurface '" << subSurface.nameString() << "'");
            }
            test = subSurface.setSubSurfaceType(surfaceType);
            if (!test) {
              LOG(Warn, "Could not set SubSurfaceType to '" << surfaceType << "' for SubSurface '" << subSurface.nameString() << "'");
            }

            if (airWall) {
              subSurface.setConstruction(getAirWallConstruction(model));
            }

            originalNameToSubSurfaceMap.insert(std::make_pair(name, subSurface));

          } catch (const std::exception&) {
            LOG(Warn, "Could not create sub surface for vertices " << face);
          }
        }

      } else if (istringEqual(surfaceType, "SiteShading") || istringEqual(surfaceType, "BuildingShading")
                 || istringEqual(surfaceType, "SpaceShading")) {

        if (shadingName.empty()) {
          shadingName = "Default " + surfaceType;
        }

        boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup;
        if (istringEqual(surfaceType, "SpaceShading")) {

          boost::optional<SubSurface> subSurface;
          if (!subSurfaceName.empty()) {
            const auto it = originalNameToSubSurfaceMap.find(subSurfaceName);
            if (it != originalNameToSubSurfaceMap.end()) {
              subSurface = it->second;
            } else {
              LOG(Error, "Could not find SubSurface '" << subSurfaceName << "'");
              continue;
            }
          } else {
            LOG(Error, "SpaceShading missing required SubSurface Name");
            continue;
          }
          OS_ASSERT(subSurface);

          boost::optional<Space> space = subSurface->space();
          if (!space) {
            LOG(Error, "Cannot find Space for SubSurface '" << subSurfaceName << "'");
            continue;
          }
          OS_ASSERT(space);

          std::vector<ShadingSurfaceGroup> groups = subSurface->shadingSurfaceGroups();
          if (groups.empty()) {
            std::string shadingGroupName = subSurfaceName + " Shading Group";
            shadingSurfaceGroup = ShadingSurfaceGroup(model);
            shadingSurfaceGroup->setName(shadingGroupName);
            shadingSurfaceGroup->setSpace(*space);
            shadingSurfaceGroup->setShadedSubSurface(*subSurface);
          } else {
            shadingSurfaceGroup = groups[0];
          }
        } else {
          const auto it = originalNameToShadingSurfaceGroupMap.find(shadingName);
          if (it != originalNameToShadingSurfaceGroupMap.end()) {
            shadingSurfaceGroup = it->second;
          }

          if (!shadingSurfaceGroup) {
            shadingSurfaceGroup = ShadingSurfaceGroup(model);
            if (istringEqual(surfaceType, "SiteShading")) {
              shadingSurfaceGroup->setShadingSurfaceType("Site");
            } else if (istringEqual(surfaceType, "BuildingShading")) {
              shadingSurfaceGroup->setShadingSurfaceType("Building");
            }
            shadingSurfaceGroup->setName(shadingName);
          }
        }

        OS_ASSERT(shadingSurfaceGroup);

        for (const auto& face : faces) {
          try {
            // ensure we can create a plane before calling Surface ctor that might mess up the model
            Plane plane(face);

            bool test;

            ShadingSurface shadingSurface(face, model);
            test = shadingSurface.setName(name).has_value();
            if (!test) {
              LOG(Warn, "Could not set ShadingSurface name '" << name << "' for ShadingSurface '" << shadingSurface.nameString() << "'");
            }
            test = shadingSurface.setShadingSurfaceGroup(*shadingSurfaceGroup);
            if (!test) {
              LOG(Warn, "Could not set ShadingSurfaceGroup '" << shadingSurfaceGroup->nameString() << "' for ShadingSurface '"
                                                              << shadingSurface.nameString() << "'");
            }

          } catch (const std::exception&) {
            LOG(Warn, "Could not create shading surface for vertices " << face);
          }
        }

      } else if (istringEqual(surfaceType, "DaylightingControl")) {

        if (spaceName.empty()) {
          spaceName = "Default Space";
        }

        boost::optional<Space> space;
        if (!spaceName.empty()) {
          const auto it = originalNameToSpaceMap.find(spaceName);
          if (it != originalNameToSpaceMap.end()) {
            space = it->second;
          } else {
            LOG(Error, "Could not find Space '" << spaceName << "'");
            continue;
          }
        }
        OS_ASSERT(space);

        for (const auto& face : faces) {
          boost::optional<Point3d> centroid = getCentroid(face);

          if (centroid) {
            DaylightingControl dc(model);
            dc.setName(name);
            dc.setPositionXCoordinate(centroid->x());
            dc.setPositionYCoordinate(centroid->y());
            dc.setPositionZCoordinate(centroid->z());
            dc.setSpace(*space);
            dc.setIlluminanceSetpoint(illuminanceSetpoint);

            boost::optional<ThermalZone> zone = space->thermalZone();
            if (thermalZone) {
              if (!thermalZone->primaryDaylightingControl()) {
                thermalZone->setPrimaryDaylightingControl(dc);
              } else if (!thermalZone->secondaryDaylightingControl()) {
                thermalZone->setSecondaryDaylightingControl(dc);
              }
            }

          } else {
            LOG(Warn, "Could not create daylighting control for vertices " << face);
          }
        }
      }
    }

    // do intersections and matching between stories
    std::vector<BuildingStory> stories = model.getConcreteModelObjects<BuildingStory>();
    unsigned storiesN = stories.size();

    // sort stories by nominal z coordinate, then by name
    std::sort(stories.begin(), stories.end(), [](const BuildingStory& a, const BuildingStory& b) -> bool {
      boost::optional<double> aZ = a.nominalZCoordinate();
      boost::optional<double> bZ = b.nominalZCoordinate();
      if (aZ && bZ) {
        if (aZ.get() == bZ.get()) {
          return a.nameString() < b.nameString();
        } else {
          return aZ.get() < bZ.get();
        }
      } else if (aZ) {
        return true;
      } else if (bZ) {
        return false;
      }
      return a.nameString() < b.nameString();
    });

    // map of story index to sorted spaces
    std::map<unsigned, std::vector<Space>> sortedSpaces;
    for (unsigned i = 0; i < storiesN; ++i) {
      std::vector<Space> spaces = stories[i].spaces();

      // sort spaces by floor area then name
      std::sort(spaces.begin(), spaces.end(), [](const Space& a, const Space& b) -> bool {
        double areaA = a.floorArea();
        double areaB = b.floorArea();
        if (areaA == areaB) {
          return a.nameString() < b.nameString();
        }
        return areaA < areaB;
      });

      sortedSpaces[i] = spaces;
    }

    // loop over all the stories to intesect/match between stories
    for (unsigned i = 0; i < storiesN; ++i) {

      // bounding box for story i in building coordinates
      BoundingBox storyI_BB = stories[i].boundingBoxBuildingCoordinates();

      for (unsigned j = i + 1; j < storiesN; ++j) {

        // check if storyI_BB intersects bounding box for story j in building coordinates
        if (!storyI_BB.intersects(stories[j].boundingBoxBuildingCoordinates())) {
          continue;
        }

        // loop over sorted spaces on story i
        for (auto& spaceI : sortedSpaces[i]) {

          // bounding box for space on story i in building coordinates
          BoundingBox spaceI_BB = spaceI.boundingBoxBuildingCoordinates();

          // loop over sorted spaces on story j
          for (auto& spaceJ : sortedSpaces[j]) {

            // check if intersects bounding box for space on story j in building coordinates
            if (spaceI_BB.intersects(spaceJ.boundingBoxBuildingCoordinates())) {
              spaceI.intersectSurfaces(spaceJ);
              spaceI.matchSurfaces(spaceJ);
            }
          }
        }
      }
    }

    // loop over all the stories to intesect/match on same story
    // do surface matching for spaces on same story
    for (unsigned i = 0; i < storiesN; ++i) {
      std::vector<Space>& spaces = sortedSpaces[i];
      unsigned spacesN = spaces.size();

      // loop over spaces on story i
      for (unsigned j = 0; j < spacesN; ++j) {

        // bounding box for space j in building coordinates
        BoundingBox spaceJ_BB = spaces[j].boundingBoxBuildingCoordinates();

        // loop over remaining spaces on story i
        for (unsigned k = j + 1; k < spacesN; ++k) {

          // check if space bounding boxes intersect
          if (spaceJ_BB.intersects(spaces[k].boundingBoxBuildingCoordinates())) {
            // DLM: should not need to intersect on same floor?
            spaces[j].intersectSurfaces(spaces[k]);
            spaces[j].matchSurfaces(spaces[k]);
          }
        }
      }
    }

    return model;
  }

}  // namespace model
}  // namespace openstudio
