/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "GltfForwardTranslator.hpp"

#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "ConstructionAirBoundary.hpp"
#include "ConstructionAirBoundary_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
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
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "PlanarSurfaceGroup.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Polygon3d.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <stack>
#include <limits.h>

namespace openstudio {
namespace model {

  namespace GLTF = tinygltf;

  // For raw values for GLTF Materials
  struct MaterialData
  {
    std::string matName;
    int r;     // [0, 255]
    int g;     // [0, 255]
    int b;     // [0, 255]
    double a;  // [0, 1]
    bool isDoubleSided = false;
  };

  // For Indices of Indices, Coordinates & Normal buffers
  // against each Components
  struct ShapeComponentIds
  {
    int IndicesAccessorId;
    int VerticesAccessorId;
    int NormalsAccessorId;
  };

  // Creates a GLTF material on the basis of raw Material Values
  // i.e, R, G, B, A & isDoubleSided
  // and adds to GLTF model's materials
  // param : _materials
  // param : mat_Data
  // returns : index of the created GLTF Material
  int createMaterial(std::vector<GLTF::Material>& _materials, const MaterialData& mat_Data) {
    GLTF::Material m;
    m.name = mat_Data.matName;
    GLTF::PbrMetallicRoughness pbr;
    // this contains the red, green, blue and alpha components of the main color of the material
    pbr.baseColorFactor = {(float)(mat_Data.r / 255.0), (float)(mat_Data.g / 255.0), (float)(mat_Data.b / 255.0), (float)(mat_Data.a)};
    // this indicates that the material should have very minimum reflection characteristics
    // between that of a metal and non-metal material.
    pbr.metallicFactor = 0.0;
    // this makes the material to not be perfectly mirror-like, but instead scatter the
    // reflected light a bit.
    pbr.roughnessFactor = 1.0;
    m.pbrMetallicRoughness = pbr;
    // emissive texture describes the parts of the object surface that emit light with a certain color
    m.emissiveFactor = {0.01, 0.01, 0.00};
    // occlusion texture can be used to simulate the effect of objects self-shadowing each other.
    // m.occlusionTexture = "";
    // normal map is a texture applied to modulate the surface normal in a way that makes
    // it possible to simulate finer geometric details without the cost of a higher mesh.
    // m.normalTexture = "";
    m.alphaMode = (mat_Data.a < 1.0f) ? "BLEND" : "OPAQUE";
    //alphaCutoff should not be 1.0f else MATERIAL_ALPHA_CUTOFF_INVALID_MODE	Alpha cutoff is supported only for 'MASK' alpha mode
    m.alphaCutoff = 0.5f;
    m.doubleSided = mat_Data.isDoubleSided;

    int ret = _materials.size();
    _materials.push_back(m);
    return ret;
  }

  // Initializes the Default material for the GLTF Model materials
  // param : _materials
  void initMaterials(std::vector<GLTF::Material>& _materials) {
    // default material is always set at index 0;
    const int grey = 204;
    const double alpha = 1.0;
    createMaterial(_materials, MaterialData{.matName = "Default material", .r = grey, .g = grey, .b = grey, .a = alpha, .isDoubleSided = true});
  }

  // Initializes the SCENE for the GLTF model that will be
  // aware of all other nodes, meshes, materials, BufferViews, Accessors tightly
  // tied up in a Heirarchy using indices pointing one to other.
  // param : _scene
  // param : _topNode
  // param : _nodes
  void initScene(GLTF::Scene& _scene, std::vector<GLTF::Node>& _nodes) {
    std::vector<int> ns{0};
    _scene.nodes = ns;

    GLTF::Node& _topNode = _nodes.emplace_back();
    _topNode.name = "Z_UP";
    std::vector<double> mtrx{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    _topNode.matrix = mtrx;
  }

  // Initializes our two main Buffer Views
  // one for the indices & other for Coordinates and Normals
  // param : _indicesBv
  // param : _coordinatesBv
  // param : _bufferViews
  void initBufferViews(GLTF::BufferView& _indicesBv, GLTF::BufferView& _coordinatesBv, std::vector<GLTF::BufferView>& _bufferViews) {
    GLTF::BufferView bv_indices;
    bv_indices.buffer = 0;
    //bv_indices.byteStride = 4; //This is not required
    bv_indices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
    _indicesBv = bv_indices;
    //_bufferViews.push_back(_indicesBv);

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    GLTF::BufferView bv_coordinates;
    bv_coordinates.buffer = 0;
    bv_coordinates.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    bv_coordinates.byteStride = 12;
    //bv_coordinates.byteLength = 36;
    _coordinatesBv = bv_coordinates;
    //_bufferViews.push_back(_coordinatesBv);
  }

  // Creates a collection of raw (values: R,G,B,A & isDoubleSided)
  // for all pre-defined GLTF material
  // returns : collection of raw material Values
  std::vector<MaterialData> getAllMaterials() {
    std::vector<MaterialData> result{
      {"Undefined", 255, 255, 255, 1, true},

      {"NormalMaterial", 255, 255, 255, 1, true},
      {"NormalMaterial_Ext", 255, 255, 255, 1},
      {"NormalMaterial_Int", 255, 0, 0, 1},

      {"Floor", 140, 140, 140, 1, true},
      {"Floor_Ext", 128, 128, 128, 1},
      {"Floor_Int", 191, 191, 191, 1},

      {"Wall", 150, 131, 75, 1, true},
      {"Wall_Ext", 204, 178, 102, 1},
      {"Wall_Int", 235, 226, 197, 1},

      {"RoofCeiling", 112, 56, 57, 1, true},
      {"RoofCeiling_Ext", 153, 76, 76, 1},
      {"RoofCeiling_Int", 202, 149, 149, 1},

      {"Window", 102, 178, 204, 0.6, true},
      {"Window_Ext", 102, 178, 204, 0.6},
      {"Window_Int", 192, 226, 235, 0.6},

      {"Door", 111, 98, 56, 1, true},
      {"Door_Ext", 153, 133, 76, 1},
      {"Door_Int", 202, 188, 149, 1},

      {"SiteShading", 55, 90, 109, 1, true},
      {"SiteShading_Ext", 75, 124, 149, 1},
      {"SiteShading_Int", 187, 209, 220, 1},

      {"BuildingShading", 83, 56, 111, 1, true},
      {"BuildingShading_Ext", 113, 76, 153, 1},
      {"BuildingShading_Int", 216, 203, 229, 1},

      {"SpaceShading", 55, 81, 130, 1, true},
      {"SpaceShading_Ext", 76, 110, 178, 1},
      {"SpaceShading_Int", 183, 197, 224, 1},

      {"InteriorPartitionSurface", 117, 138, 105, 1, true},
      {"InteriorPartitionSurface_Ext", 158, 188, 143, 1},
      {"InteriorPartitionSurface_Int", 213, 226, 207, 1},

      // start textures for boundary conditions
      {"Boundary_Surface", 0, 153, 0, 1, true},
      {"Boundary_Adiabatic", 255, 0, 0, 1, true},
      {"Boundary_Space", 255, 0, 0, 1, true},
      {"Boundary_Outdoors", 163, 204, 204, 1, true},
      {"Boundary_Outdoors_Sun", 40, 204, 204, 1, true},
      {"Boundary_Outdoors_Wind", 9, 159, 162, 1, true},
      {"Boundary_Outdoors_SunWind", 68, 119, 161, 1, true},
      {"Boundary_Ground", 204, 183, 122, 1, true},
      {"Boundary_Groundfcfactormethod", 153, 122, 30, 1, true},
      {"Boundary_Groundslabpreprocessoraverage", 255, 191, 0, 1, true},
      {"Boundary_Groundslabpreprocessorcore", 255, 182, 50, 1, true},
      {"Boundary_Groundslabpreprocessorperimeter", 255, 178, 101, 1, true},
      {"Boundary_Groundbasementpreprocessoraveragewall", 204, 51, 0, 1, true},
      {"Boundary_Groundbasementpreprocessoraveragefloor", 204, 81, 40, 1, true},
      {"Boundary_Groundbasementpreprocessorupperwall", 204, 112, 81, 1, true},
      {"Boundary_Groundbasementpreprocessorlowerwall", 204, 173, 163, 1, true},
      {"Boundary_Othersidecoefficients", 63, 63, 63, 1, true},
      {"Boundary_Othersideconditionsmodel", 153, 0, 76, 1, true},

      // special rendering materials
      {"SpaceType_Plenum", 192, 192, 192, 0.1, true},
      {"ThermalZone_Plenum", 192, 192, 192, 0.1, true},

      // special rendering materials, these are components or textures in SketchUp
      {"DaylightingControl", 102, 178, 204, 0.1, true},
      {"AirWall", 102, 178, 204, 0.1, true},
      {"SolarCollector", 255, 255, 255, 1, true},
      {"Photovoltaic", 255, 255, 255, 0.1, true},
    };

    return result;
  }

  // Adds all pre-defined materials to GLTF Model materials
  // param : materialList
  // param : _materials
  void addGLTFMaterials(std::map<std::string, int>& materialList, std::vector<GLTF::Material>& _materials) {
    std::vector<MaterialData> result = getAllMaterials();
    for (auto& m : result) {
      if (materialList.find(m.matName) != materialList.end()) {
        LOG_FREE(Debug, "GltfForwardTranslator", "addGLTFMaterials, Key " << m.matName << " found in materialList");
      } else {
        materialList[m.matName] = createMaterial(_materials, m);
      }
    }
  }

  // Gets GLTF Material name on the basis of idd Object Type and Name
  // param : iddObjectType
  // param : name
  // returns : name of GLTF Material Name
  std::string getObjectGLTFMaterialName(const std::string& iddObjectType, const std::string& name) {
    std::string result;
    if (istringEqual(iddObjectType, "OS:Construction")) {
      result = "Construction_" + name;
    } else if (istringEqual(iddObjectType, "OS:ThermalZone")) {
      result = "ThermalZone_" + name;
    } else if (istringEqual(iddObjectType, "OS:AirLoopHVAC")) {
      result = "AirLoopHVAC_" + name;
    } else if (istringEqual(iddObjectType, "OS:SpaceType")) {
      result = "SpaceType_" + name;
    } else if (istringEqual(iddObjectType, "OS:BuildingStory")) {
      result = "BuildingStory_" + name;
    } else if (istringEqual(iddObjectType, "OS:BuildingUnit")) {
      result = "BuildingUnit_" + name;
    } else if (istringEqual(iddObjectType, "OS:Construction:AirBoundary")) {
      // This shouldn't happen
      LOG_FREE(Error, "getObjectGLTFMaterialName", "Didn't expect it would be called for '" << iddObjectType << "' (name = '" << name << "')");
      result = "ConstructionAirBoundary_" + name;
    } else {
      LOG_FREE(Error, "getObjectGLTFMaterialName", "Unknown iddObjectType '" << iddObjectType << "'");
    }
    return result;
  }

  // Gets GLTF Material Name on the basis of Model Object
  // param : object
  // returns : name of GLTF material Name
  std::string getObjectGLTFMaterialName(const ModelObject& object) {
    return getObjectGLTFMaterialName(object.iddObjectType().valueDescription(), object.nameString());
  }

  // Adds Model Specific GLTF Material to all Materials Collection
  // param : _materials
  // param : color
  // param : materialName
  // param : isDoubleSided
  // param : allMaterials
  void addModelSpecificMaterialToCollection(std::vector<GLTF::Material>& _materials, const RenderingColor& color, const std::string& materialName,
                                            bool isDoubleSided, std::vector<MaterialData>& allMaterials) {

    allMaterials.emplace_back(MaterialData{materialName, color.renderingRedValue(), color.renderingGreenValue(), color.renderingBlueValue(),
                                           color.renderingAlphaValue() / 255.0, true});
  }

  // Gets GLTF Material name on the basis of Suface Type
  // param : surfaceType
  // returns : surface Type
  std::string getSurfaceTypeGLTFMaterialName(const std::string& surfaceType) {
    if (istringEqual(surfaceType, "FixedWindow") || istringEqual(surfaceType, "OperableWindow") || istringEqual(surfaceType, "GlassDoor")
        || istringEqual(surfaceType, "Skylight") || istringEqual(surfaceType, "TubularDaylightDome")
        || istringEqual(surfaceType, "TubularDaylightDiffuser")) {
      return "Window";
    } else if (istringEqual(surfaceType, "Door") || istringEqual(surfaceType, "OverheadDoor")) {
      return "Door";
    }
    return surfaceType;
  }

  // Finds / Create GLTF Material depending upon MaterialName
  // param : materialName
  // param : materialList
  // param : allMaterials
  // param : _materials
  // returns : index of Material
  int getGLTFMaterialIndex(const std::string& materialName, std::map<std::string, int>& materialList, std::vector<MaterialData>& allMaterials,
                           std::vector<GLTF::Material>& _materials) {
    std::map<std::string, int>::const_iterator it = materialList.find(materialName);
    if (it != materialList.end()) {
      return it->second;
    } else {
      for (size_t i = 0; i < allMaterials.size(); ++i) {
        if (allMaterials[i].matName == materialName) {
          // Create Material and Add to _materials
          int newMatIndex = createMaterial(_materials, allMaterials[i]);
          // Add to map list
          materialList[allMaterials[i].matName] = newMatIndex;
          // send back index
          // return newMatIndex;
          break;
        }
      }
      std::map<std::string, int>::const_iterator it = materialList.find(materialName);
      if (it != materialList.end()) {
        return it->second;
      }
      it = materialList.find("Undefined");
      if (it != materialList.end()) {
        return it->second;
      }
    }
    return 0;
  }

  // Now: Instead of creating materials for all the objects in Model
  // we're adding them to the all materials collection and create them only if required
  // while processing the surface i.e, in MAIN LOOP
  void buildMaterials(const Model& model, std::vector<GLTF::Material>& _materials, std::map<std::string, int>& materialList,
                      std::vector<MaterialData>& allMaterials) {
    std::string materialName;

    auto getOrCreateRenderingColor = [&model](auto& object) {
      boost::optional<RenderingColor> color_ = object.renderingColor();
      if (!color_) {
        color_ = RenderingColor(model);
        object.setRenderingColor(color_.get());
      }
      return color_.get();
    };

    auto getOrCreateMaterial = [&getOrCreateRenderingColor, &_materials, &allMaterials, &materialList](auto& object) {
      std::string materialName = getObjectGLTFMaterialName(object);
      if (materialList.find(materialName) != materialList.end()) {
        LOG_FREE(Debug, "GltfForwardTranslator", "buildMaterials, Key " << materialName << " found in materialList");
      } else {
        // instead of creating the material whose node might get not used inshort to avoid
        // warning "UNUSED_OBJECT". we'll add the material to our allMaterial collection
        // and whie processing the surface if required we'll create and the node.
        // createMaterial(_materials, color, materialName, true);
        auto color = getOrCreateRenderingColor(object);
        addModelSpecificMaterialToCollection(_materials, color, materialName, true, allMaterials);
      }
    };

    // make construction materials
    for (auto& construction : model.getModelObjects<ConstructionBase>()) {
      // If it's ConstructionAirBoundary, we'll later use the standard material "AirWall"
      if (!construction.optionalCast<ConstructionAirBoundary>()) {
        getOrCreateMaterial(construction);
      }
    }

    // make thermal zone materials
    for (auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()) {
      getOrCreateMaterial(thermalZone);
    }

    // make space type materials
    for (auto& spaceType : model.getConcreteModelObjects<SpaceType>()) {
      getOrCreateMaterial(spaceType);
    }

    // make building story materials
    for (auto& buildingStory : model.getConcreteModelObjects<BuildingStory>()) {
      getOrCreateMaterial(buildingStory);
    }

    // make building unit materials
    for (auto& buildingUnit : model.getConcreteModelObjects<BuildingUnit>()) {
      getOrCreateMaterial(buildingUnit);
    }

    // make air loop HVAC materials
    for (auto& airLoopHVAC : model.getConcreteModelObjects<AirLoopHVAC>()) {
      materialName = getObjectGLTFMaterialName(airLoopHVAC);
      if (materialList.find(materialName) != materialList.end()) {
        LOG_FREE(Debug, "GltfForwardTranslator", "buildMaterials, Key " << materialName << " found in materialList");
      } else {
        RenderingColor color = RenderingColor(model);
        addModelSpecificMaterialToCollection(_materials, color, materialName, true, allMaterials);
      }
    }
  }

  // Finds the vertex Index from all Vertices of a planar Surface
  // param : point3d
  // param : allPoints
  // param : tol
  // returns :  index of the Vertex
  size_t getVertexIndexT(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol = 0.001) {
    size_t n = allPoints.size();
    for (size_t i = 0; i < n; ++i) {
      if (openstudio::getDistance(point3d, allPoints[i]) < tol) {
        return i;
      }
    }
    allPoints.push_back(point3d);
    return (allPoints.size() - 1);
  }

  // Get BYtes from an integer
  // param : paramInt
  // returns : collection of Bytes
  std::vector<unsigned char> intToBytes(int paramInt) {
    std::vector<unsigned char> arrayOfByte(4);
    for (int i = 0; i < 4; ++i)
      arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
  }

  // Get Bytes from a value
  // typeparam name :"T"
  // param : value
  // returns : collection of bytes
  template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  std::vector<uint8_t> splitValueToBytes(T const& value) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < sizeof(value); ++i) {
      uint8_t byte = value >> (i * 8);
      bytes.insert(bytes.begin(), byte);
    }
    return bytes;
  }

  // Adds & Creates Face Indices buffers and Accessors
  // param : faceIndices
  // param : _indicesBuffer
  // param : _accessors
  // returns : index of the Face Indices
  int addIndices(std::vector<size_t>& faceIndices, std::vector<unsigned char>& _indicesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    auto [min, max] = std::minmax_element(std::cbegin(faceIndices), std::cend(faceIndices));

    // This but seems to be just figuring out the best way to back the indices in
    // depenbding on the range of numbers
    int ct = TINYGLTF_COMPONENT_TYPE_BYTE;
    auto size = 0;

    if (*max <= static_cast<size_t>(std::pow(2, 8))) {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
      size = sizeof(std::byte);
    } else if (*max <= static_cast<size_t>(std::pow(2, 16))) {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
      size = sizeof(short);
    } else {
      ct = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
      size = sizeof(int);
    }
    // the offset position needs to be a multiple of the size
    // (this is from a warning we received in beta testing)
    // so we inject some padding when needed
    auto padding = _indicesBuffer.size() % size;
    for (size_t i = 0; i < padding; ++i) {
      _indicesBuffer.push_back(0x00);
    }

    // To avoid Accessor offset Validation Issue
    auto _padding = _indicesBuffer.size() % 4;
    for (size_t i = 0; i < _padding; ++i) {
      _indicesBuffer.push_back(0x00);
    }

    GLTF::Accessor& indAccessor = _accessors.emplace_back();
    indAccessor.bufferView = 0;
    indAccessor.componentType = ct;
    indAccessor.byteOffset = _indicesBuffer.size();
    indAccessor.normalized = false;
    indAccessor.type = TINYGLTF_TYPE_SCALAR;
    indAccessor.count = faceIndices.size();
    indAccessor.minValues = {static_cast<double>(*min)};
    indAccessor.maxValues = {static_cast<double>(*max)};

    std::vector<unsigned char> indicesBufferData;
    for (auto index : faceIndices) {
      std::vector<unsigned char> arrayOfByte;
      if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
        arrayOfByte.push_back((unsigned char)index);
      } else if (ct == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        arrayOfByte = splitValueToBytes(index);
      } else {
        arrayOfByte = splitValueToBytes(index);
      }
      indicesBufferData.insert(indicesBufferData.end(), arrayOfByte.begin(), arrayOfByte.end());
    }

    if (indicesBufferData.size() < 4) {
      indicesBufferData.push_back(0x00);
    }
    _indicesBuffer.insert(_indicesBuffer.end(), indicesBufferData.begin(), indicesBufferData.end());

    const auto thisIndex = _accessors.size();
    return thisIndex;
  }

  // Creates Coordinates / Normal Buffers and Accessors.
  // param : values
  // param : _coordinatesBuffer
  // param : _accessors
  // returns :
  int createBuffers(std::vector<float>& values, std::vector<unsigned char>& _coordinatesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    // Fixes ACCESSOR_TOTAL_OFFSET_ALIGNMENT
    // Accessor's total byteOffset XXXX isn't a multiple of componentType length 4.
    auto _padding = _coordinatesBuffer.size() % 4;
    for (size_t i = 0; i < _padding; ++i) {
      _coordinatesBuffer.push_back((unsigned)0);
    }
    int startingBufferPosition = _coordinatesBuffer.size();
    std::vector<float> min = {FLT_MAX, FLT_MAX, FLT_MAX};
    std::vector<float> max = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    int i = 0;
    for (auto& value : values) {
      min[i] = std::min(value, min[i]);
      max[i] = std::max(value, max[i]);
      ++i;
      if (i > 2) i = 0;
      std::vector<unsigned char> v;
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
      for (size_t i = 0; i < sizeof(float); ++i) {
        v.push_back(ptr[i]);
      }
      _coordinatesBuffer.insert(_coordinatesBuffer.end(), v.begin(), v.end());
    }
    // To Fix : offset 18 is not a multiple of Comonent Type lenght 4
    auto padding = _coordinatesBuffer.size() % 4;
    for (size_t i = 0; i < padding; ++i) {
      _coordinatesBuffer.push_back((unsigned)0);
    }
    // convert min and max to double
    std::vector<double> min_d;
    std::vector<double> max_d;
    for (auto mn : min) {
      min_d.push_back((double)mn);
    }
    for (auto mx : max) {
      max_d.push_back((double)mx);
    }
    GLTF::Accessor coordAccessor;
    coordAccessor.bufferView = 1;
    coordAccessor.byteOffset = startingBufferPosition;
    coordAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    coordAccessor.normalized = false;
    coordAccessor.count = values.size() / 3;
    coordAccessor.type = TINYGLTF_TYPE_VEC3;
    coordAccessor.minValues = min_d;
    coordAccessor.maxValues = max_d;

    auto ret = _accessors.size();
    _accessors.push_back(coordAccessor);
    return ret;
  }

  // Adds Coordinate Buffers for all vertices of the surface
  // param : allVertices
  // param : _coordinatesBuffer
  // param : _accessors
  // returns : index for the Coordinates Buffer
  int addCoordinates(Point3dVector& allVertices, std::vector<unsigned char>& _coordinatesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    std::vector<float> values(3 * allVertices.size());
    size_t i = 0;
    for (const auto& point : allVertices) {
      values[i++] = static_cast<float>(point.x());
      values[i++] = static_cast<float>(point.y());
      values[i++] = static_cast<float>(point.z());
    }

    return createBuffers(values, _coordinatesBuffer, _accessors);
  }

  // Adds Normal Buffers for all normal Vectors
  // param : normalVectors
  // param : _coordinatesBuffer
  // param : _accessors
  // returns : index for the Normals Buffer
  int addNormals(Vector3dVector& normalVectors, std::vector<unsigned char>& _coordinatesBuffer, std::vector<GLTF::Accessor>& _accessors) {
    std::vector<float> values(3 * normalVectors.size());
    size_t i = 0;
    for (const auto& vec : normalVectors) {
      values[i++] = static_cast<float>(vec.x());
      values[i++] = static_cast<float>(vec.y());
      values[i++] = static_cast<float>(vec.z());
    }

    return createBuffers(values, _coordinatesBuffer, _accessors);
  }

  // Returns material index depending upon the surface type.
  // param : planarSurface
  // param : _materials
  // param : materialList
  // param : allMaterials
  // returns : index of the Material
  int getMaterialIndex(const PlanarSurface& planarSurface, std::vector<GLTF::Material>& _materials, std::map<std::string, int>& materialList,
                       std::vector<MaterialData>& allMaterials) {
    std::string surfaceType;
    std::string surfaceTypeGLTFMaterialName;
    if (auto surface = planarSurface.optionalCast<Surface>()) {
      surfaceType = surface->surfaceType();
    } else if (planarSurface.optionalCast<SubSurface>()) {
      surfaceType = planarSurface.cast<SubSurface>().subSurfaceType();
    } else if (planarSurface.optionalCast<InteriorPartitionSurface>()) {
      surfaceType = "InteriorPartitionSurface";
    } else if (planarSurface.optionalCast<ShadingSurface>()) {
      boost::optional<ShadingSurface> shadingSurface = planarSurface.optionalCast<ShadingSurface>();
      std::string shadingSurfaceType = "Building";
      if (shadingSurface->shadingSurfaceGroup()) {
        shadingSurfaceType = shadingSurface->shadingSurfaceGroup()->shadingSurfaceType();
      }
      surfaceType = shadingSurfaceType + "Shading";
    }
    surfaceTypeGLTFMaterialName = getSurfaceTypeGLTFMaterialName(surfaceType);
    return getGLTFMaterialIndex(surfaceTypeGLTFMaterialName, materialList, allMaterials, _materials);
  }

  // Exports a gltf against a Model
  // param : model
  // param : outputPath
  // returns : exports a GLTF file against a Model
  bool GltfForwardTranslator::modelToGLTF(const Model& model, const path& outputPath) {
    return modelToGLTF(
      model, [](double percentage) {}, outputPath);
  }

  // MAIN PIPELINE TO TRANSLATE OPENSTUDIO MODEL -> GLTF MODEL
  // param : model
  // param : updatePercentage
  // param : outputPath
  // returns : exports GLTF file aginst the Model
  bool GltfForwardTranslator::modelToGLTF(const Model& model, std::function<void(double)> updatePercentage, const path& outputPath) {
    m_logSink.setThreadId(std::this_thread::get_id());
    m_logSink.resetStringStream();

    bool triangulateSurfaces = true;  //we're always triangulating the surfaces to get the best possible output.
    bool BufferInBase64 = false;      //no *.bin file is involed | everything is integrated in the mail output gltf file only.

    GLTF::TinyGLTF loader;
    GLTF::Model gltf_Model;
    std::string err;
    std::string warning;
    path p;

    std::vector<GLTF::Accessor>& _accessors = gltf_Model.accessors;
    std::vector<GLTF::Material>& _materials = gltf_Model.materials;
    std::vector<GLTF::Mesh>& _meshes = gltf_Model.meshes;
    std::vector<GLTF::Node>& _nodes = gltf_Model.nodes;
    std::vector<GLTF::BufferView>& _bufferViews = gltf_Model.bufferViews;

    GLTF::Scene& _scene = gltf_Model.scenes.emplace_back();
    GLTF::Buffer& _buffer = gltf_Model.buffers.emplace_back();

    GLTF::BufferView _indicesBv;
    GLTF::BufferView _coordinatesBv;
    std::vector<unsigned char> _indicesBuffer;
    std::vector<unsigned char> _coordinatesBuffer;

    GLTF::Asset& _asset = gltf_Model.asset;

    // Start Region INIT

    // Start Region MATERIALS

    //initMaterials(_materials);

    // End Region MATERIALS

    // Start Region SCENE

    initScene(_scene, _nodes);

    // End Region SCENE

    // Start Region BUFFERVIEWS

    // TODO: GLTF::BufferView will have to get copied around here...
    // TODO: _bufferViews is unused anyways
    initBufferViews(_indicesBv, _coordinatesBv, _bufferViews);

    // End Region BUFFERVIEWS

    // End Region INIT

    std::map<std::string, int> materialList;
    std::map<std::string, int> materialIndicesUsed;
    std::vector<MaterialData> allMaterials = getAllMaterials();

    /*std::vector<GLTF::Scene> scenes;
    std::vector<GLTF::Buffer> buffers;
    std::vector<GLTF::Animation> animations;
    std::vector<GLTF::Texture> textures;
    std::vector<GLTF::Image> images;
    std::vector<GLTF::Skin> skins;
    std::vector<GLTF::Sampler> samplers;
    std::vector<GLTF::Camera> cameras;
    std::vector<GLTF::Light> lights;*/

    // get number of things to translate
    std::vector<PlanarSurface> planarSurfaces = model.getModelObjects<PlanarSurface>();
    std::vector<PlanarSurfaceGroup> planarSurfaceGroups = model.getModelObjects<PlanarSurfaceGroup>();
    std::vector<BuildingStory> buildingStories = model.getConcreteModelObjects<BuildingStory>();
    std::vector<BuildingUnit> buildingUnits = model.getConcreteModelObjects<BuildingUnit>();
    std::vector<ThermalZone> thermalZones = model.getConcreteModelObjects<ThermalZone>();
    std::vector<AirLoopHVAC> airLoopHVACs = model.getConcreteModelObjects<AirLoopHVAC>();
    std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
    std::vector<DefaultConstructionSet> defaultConstructionSets = model.getConcreteModelObjects<DefaultConstructionSet>();
    double n = 0;
    //std::vector<PlanarSurface>::size_type _N = planarSurfaces.size() + planarSurfaceGroups.size() + buildingStories.size() + buildingUnits.size()
    //                                           + thermalZones.size() + spaceTypes.size() + defaultConstructionSets.size() + airLoopHVACs.size() + 1;

    std::vector<PlanarSurface>::size_type N = planarSurfaces.size() + 1;

    updatePercentage(0.0);

    // Start Region CREATE MATERIALS
    // add model specific materials
    buildMaterials(model, _materials, materialList, allMaterials);

    // End Region CREATE MATERIALS

    TransformationVector transformStack{Transformation{}};

    for (const auto& planarSurface : planarSurfaces) {
      // Start Region MAIN LOOP
      int materialIndex = 0;
      std::string elementName = planarSurface.nameString();
      // Construct in place
      GLTF::Node& node = _nodes.emplace_back();
      node.name = elementName;

      // Now the geometry
      // get the transformation to site coordinates
      Transformation buildingTransformation;
      if (boost::optional<PlanarSurfaceGroup> planarSurfaceGroup_ = planarSurface.planarSurfaceGroup()) {
        buildingTransformation = planarSurfaceGroup_->buildingTransformation();
      }
      // get the vertices
      Point3dVector vertices = planarSurface.vertices();
      Transformation& t = transformStack.emplace_back(Transformation::alignFace(vertices));
      Transformation tInv = t.inverse();
      Point3dVector faceVertices = reverse(tInv * vertices);
      std::vector<double> matrix = openstudio::toStandardVector(buildingTransformation.vector());

      // Adding a check to avoid warning "NODE_MATRIX_DEFAULT"  <Do not specify default transform matrix>.
      // This is the identity_matrix<4>
      std::vector<double> matrixDefaultTransformation{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
      if (matrixDefaultTransformation != matrix) {
        node.matrix = matrix;
      } else {
        node.matrix = {};
      }
      node.mesh = _meshes.size();

      GLTF::Mesh& targetMesh = _meshes.emplace_back();
      targetMesh.name = elementName;

      materialIndex = getMaterialIndex(planarSurface, _materials, materialList, allMaterials);

      // get vertices of all sub surfaces
      Point3dVectorVector faceSubVertices;
      boost::optional<Surface> surface = planarSurface.optionalCast<Surface>();
      if (surface) {
        for (const auto& subSurface : surface->subSurfaces()) {
          faceSubVertices.push_back(reverse(tInv * subSurface.vertices()));
        }
      }

      Point3dVectorVector finalFaceVertices;
      if (triangulateSurfaces) {
        finalFaceVertices = computeTriangulation(faceVertices, faceSubVertices);
        if (finalFaceVertices.empty()) {
          LOG_FREE(Error, "modelToGLTF", "Failed to triangulate surface " << elementName << " with " << faceSubVertices.size() << " sub surfaces");
          //return;
        }
      } else {
        finalFaceVertices.push_back(faceVertices);
      }

      Point3dVector allVertices;
      Point3dVector triangleVertices;
      std::vector<size_t> faceIndices;
      for (const auto& finalFaceVerts : finalFaceVertices) {
        Point3dVector finalVerts = t * finalFaceVerts;
        triangleVertices.insert(triangleVertices.end(), finalVerts.begin(), finalVerts.end());

        Point3dVector::reverse_iterator it = finalVerts.rbegin();
        Point3dVector::reverse_iterator itend = finalVerts.rend();
        for (; it != itend; ++it) {
          faceIndices.push_back(getVertexIndexT(*it, allVertices));
        }
      }

      Vector3d outwardNormal = planarSurface.outwardNormal();
      Vector3dVector normalVectors;
      for (n = 0; n < allVertices.size(); n++) {
        normalVectors.push_back(outwardNormal);
      }

      ShapeComponentIds shapeComponentIds{
        .IndicesAccessorId = addIndices(faceIndices, _indicesBuffer, _accessors),           // IndicesAccessorId
        .VerticesAccessorId = addCoordinates(allVertices, _coordinatesBuffer, _accessors),  // VerticesAccessorId
        .NormalsAccessorId = addNormals(normalVectors, _coordinatesBuffer, _accessors)      // NormalsAccessorId
      };

      GLTF::Primitive& thisPrimitive = targetMesh.primitives.emplace_back();
      thisPrimitive.attributes["NORMAL"] = shapeComponentIds.NormalsAccessorId;
      thisPrimitive.attributes["POSITION"] = shapeComponentIds.VerticesAccessorId;
      thisPrimitive.indices = shapeComponentIds.IndicesAccessorId;
      thisPrimitive.material = materialIndex;
      thisPrimitive.mode = TINYGLTF_MODE_TRIANGLES;

      n += 1;
      updatePercentage(100.0 * n / N);
      // End Region MAIN
    }
    // Start Region BUILD SCENE | ELEMENT

    if (_coordinatesBuffer.size() == 0) {
      return false;
    }

    _indicesBv.byteLength = _indicesBuffer.size();
    _indicesBv.byteOffset = 0;

    _coordinatesBv.byteLength = _coordinatesBuffer.size();
    _coordinatesBv.byteOffset = _indicesBuffer.size();

    // TODO: we could do that directly at the start
    _bufferViews.emplace_back(std::move(_indicesBv));
    _bufferViews.emplace_back(std::move(_coordinatesBv));

    if (!BufferInBase64) {
      // BPS:having a separate input file for the GLTF is old now everything resides
      // in the main GLTF file only..as a binary buffer data.
      auto padding = _indicesBuffer.size() % 4;
      for (size_t i = 0; i < padding; ++i) {
        _indicesBuffer.push_back(0x00);  // padding bytes
      }
      _buffer.data = std::move(_indicesBuffer);
      _buffer.data.insert(_buffer.data.end(), _coordinatesBuffer.begin(), _coordinatesBuffer.end());
    }

    // End Region BUILD SCENE | ELEMENT

    // Other tie ups
    // Define the asset. The version is required
    _asset.version = "2.0";
    _asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.

    GLTF::Node& _topNode = _nodes.front();
    std::vector<int> children_nodes(_nodes.size() - 1);                  // Construct a vector of a specific size
    std::iota(std::begin(children_nodes), std::end(children_nodes), 1);  // Fill it with [1..(size)]
    _topNode.children = std::move(children_nodes);

    // Save it to a file
    bool ret = loader.WriteGltfSceneToFile(&gltf_Model, toString(outputPath),
                                           true,    // embedImages
                                           true,    // embedBuffers
                                           true,    // pretty print
                                           false);  // write binary

    updatePercentage(100.0);

    return ret;
  }

  // To export a Minimal GLTF file (Triangle with 3 Points) using raw buffer data.
  // param : outputPath
  // returns : exports triangle.gltf
  bool GltfForwardTranslator::createTriangleGLTF(const path& outputPath) {
    GLTF::Model m;

    GLTF::Scene& scene = m.scenes.emplace_back();
    GLTF::Mesh& mesh = m.meshes.emplace_back();
    GLTF::Node& node = m.nodes.emplace_back();
    GLTF::Buffer& buffer = m.buffers.emplace_back();
    GLTF::BufferView& bufferView1 = m.bufferViews.emplace_back();
    GLTF::BufferView& bufferView2 = m.bufferViews.emplace_back();
    GLTF::Asset& asset = m.asset;

    // This is the raw data buffer.
    buffer.data = {// 6 bytes of indices and two bytes of padding
                   // 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00,
                   0x00, 0x01, 0x02, 0x00,
                   // 36 bytes of floating point numbers
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00};

    // Create an array of size equivalent to vector
    // unsigned char arr[42]

    // The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.byteOffset = 0;
    bufferView1.byteLength = 3;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    bufferView2.byteOffset = 4;
    bufferView2.byteLength = 36;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    // Describe the layout of bufferView1, the indices of the vertices
    GLTF::Accessor& accessor1 = m.accessors.emplace_back();
    accessor1.bufferView = 0;
    accessor1.byteOffset = 0;
    accessor1.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;  // TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
    accessor1.count = 3;
    accessor1.type = TINYGLTF_TYPE_SCALAR;
    accessor1.maxValues.push_back(2);
    accessor1.minValues.push_back(0);

    // Describe the layout of bufferView2, the vertices themself
    GLTF::Accessor& accessor2 = m.accessors.emplace_back();
    accessor2.bufferView = 1;
    accessor2.byteOffset = 0;
    accessor2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor2.count = 3;
    accessor2.type = TINYGLTF_TYPE_VEC3;
    accessor2.maxValues = {1.0, 1.0, 0.0};
    accessor2.minValues = {0.0, 0.0, 0.0};

    // Build the mesh primitive and add it to the mesh
    GLTF::Primitive& primitive = mesh.primitives.emplace_back();
    primitive.indices = 0;                 // The index of the accessor for the vertex indices
    primitive.attributes["POSITION"] = 1;  // The index of the accessor for positions
    primitive.material = 0;
    // primitive.mode = TINYGLTF_MODE_POINTS;
    // primitive.mode = TINYGLTF_MODE_LINE;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;

    // Other tie ups
    node.mesh = 0;
    scene.nodes.push_back(0);  // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    // Note: everything is emplace_back'ed above, so there should be nothing loose anymore

    // Create a simple material
    GLTF::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.metallicFactor = 0.8;
    mat.pbrMetallicRoughness.roughnessFactor = 0.5;
    mat.doubleSided = true;
    m.materials.push_back(mat);

    // Save it to a file
    GLTF::TinyGLTF gltf;
    bool ret = gltf.WriteGltfSceneToFile(&m, toString(outputPath),
                                         true,    // embedImages
                                         true,    // embedBuffers
                                         true,    // pretty print
                                         false);  // write binary
    return ret;
  }

  // To export a Minimal GLTF file (Triangle with 3 Points) using generated
  // raw data from Point3DVector
  // param : outputPath
  // returns : exports triangle_2.gltf
  bool GltfForwardTranslator::createTriangleGLTFFromPoint3DVector(const path& outputPath) {
    GLTF::Model m;
    GLTF::Scene& scene = m.scenes.emplace_back();
    GLTF::Mesh& mesh = m.meshes.emplace_back();
    GLTF::Node& node = m.nodes.emplace_back();
    GLTF::Buffer& buffer = m.buffers.emplace_back();
    GLTF::BufferView& bufferView1 = m.bufferViews.emplace_back();
    GLTF::BufferView& bufferView2 = m.bufferViews.emplace_back();
    GLTF::Asset& asset = m.asset;

    // Create an array of size equivalent to vector
    // unsigned char arr[42];
    // The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
    // start of the buffer.
    bufferView1.buffer = 0;
    bufferView1.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
    // at position 8 in the buffer and are of type ARRAY_BUFFER
    bufferView2.buffer = 0;
    bufferView2.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    std::vector<size_t> faceIndices{0, 1, 2};
    std::vector<unsigned char> _indicesBuffer;
    std::vector<unsigned char> _coordinatesBuffer;
    Point3dVector allVertices{
      {0, 0, 0},
      {1, 0, 0},
      {0, 1, 0},
    };

    ShapeComponentIds shapeComponentIds{
      .IndicesAccessorId = addIndices(faceIndices, _indicesBuffer, m.accessors),           //IndicesAccessorId
      .VerticesAccessorId = addCoordinates(allVertices, _coordinatesBuffer, m.accessors),  //VerticesAccessorId
      .NormalsAccessorId = 0                                                               //NormalsAccessorId
    };

    // Build the mesh primitive and add it to the mesh
    // The index of the accessor for the vertex indices
    GLTF::Primitive& primitive = mesh.primitives.emplace_back();
    primitive.indices = 0;
    // The index of the accessor for positions
    primitive.attributes["POSITION"] = shapeComponentIds.VerticesAccessorId;
    primitive.material = 0;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;

    bufferView1.byteOffset = 0;
    bufferView1.byteLength = _indicesBuffer.size();
    bufferView2.byteOffset = _indicesBuffer.size();
    bufferView2.byteLength = _coordinatesBuffer.size();

    // This is the raw data buffer.
    buffer.data = std::move(_indicesBuffer);
    buffer.data.insert(buffer.data.end(), _coordinatesBuffer.begin(), _coordinatesBuffer.end());

    // Other tie ups
    node.mesh = 0;
    scene.nodes.push_back(0);  // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "OpenStudio";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    // Note: eveyrthing is emplace_back'ed directly or passed by ref, so there's nothing loose anymore

    // Create a simple material
    GLTF::Material& mat = m.materials.emplace_back();
    mat.pbrMetallicRoughness.baseColorFactor = {1.0f, 0.9f, 0.9f, 1.0f};
    mat.pbrMetallicRoughness.metallicFactor = 1.0;
    mat.pbrMetallicRoughness.roughnessFactor = 0.5;
    mat.doubleSided = true;

    // Save it to a file
    GLTF::TinyGLTF gltf;
    bool ret = gltf.WriteGltfSceneToFile(&m, toString(outputPath),
                                         true,    // embedImages
                                         true,    // embedBuffers
                                         true,    // pretty print
                                         false);  // write binary
    return ret;
  }

  // To populate a GLTF Model from an existing GLTF file.
  // also exports a gltf file with a .bin file (non embeded version).
  // param : inputPath
  // param : inputNonEmbededPath
  // returns :
  bool GltfForwardTranslator::loadGLTF(const path& inputPath, const path& inputNonEmbededPath) {
    GLTF::TinyGLTF loader;
    GLTF::Model gltf_Model;
    std::string err;
    std::string warning;
    std::string fileName = toString(inputPath);
    bool ret = loader.LoadASCIIFromFile(&gltf_Model, &err, &warning, fileName);
    if (err.size() != 0) {
      LOG(Error, "Error loading GLTF " << err);
      //ret = false;
    }
    if (!warning.empty()) {
      LOG(Warn, "Error loading GLTF " << warning);
    }

    if (!ret) {
      LOG(Error, "Failed to parse glTF");
    } else {
      std::string output_filename = toString(inputNonEmbededPath);
      std::string embedded_filename = output_filename.substr(0, output_filename.size() - 5) + "-Embedded.gltf";
      bool a = loader.WriteGltfSceneToFile(&gltf_Model, output_filename);
      OS_ASSERT(a);

      // Embedd buffers and (images if present)
#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
      bool b = loader.WriteGltfSceneToFile(&gltf_Model, embedded_filename, true, true);
      OS_ASSERT(b);
#endif
    }
    return ret;
  }

  GltfForwardTranslator::GltfForwardTranslator() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> GltfForwardTranslator::warnings() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

  std::vector<LogMessage> GltfForwardTranslator::errors() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

}  // namespace model
}  // namespace openstudio
