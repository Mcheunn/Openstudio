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

#include <gtest/gtest.h>
#include "gbXMLFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/ModelMerger.hpp"
#include "../../model/AdditionalProperties.hpp"
#include "../../model/AdditionalProperties_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/BuildingStory.hpp"
#include "../../model/BuildingStory_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Facility.hpp"
#include "../../model/Facility_Impl.hpp"
#include "../../model/MasslessOpaqueMaterial.hpp"
#include "../../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"
#include "../../model/ScheduleWeek.hpp"
#include "../../model/ScheduleWeek_Impl.hpp"
#include "../../model/ScheduleYear.hpp"
#include "../../model/ScheduleYear_Impl.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/YearDescription.hpp"
#include "../../model/YearDescription_Impl.hpp"

#include "utilities/core/Compare.hpp"

#include <resources.hxx>

#include <sstream>
#include <iostream>
#include <algorithm>
#include <pugixml.hpp>

using namespace openstudio::model;
using namespace openstudio::gbxml;
using namespace openstudio;

TEST_F(gbXMLFixture, ForwardTranslator_exampleModel) {
  Model model = exampleModel();

  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);

  path p1 = resourcesPath() / openstudio::toPath("gbxml/exampleModel.osm");
  model.save(p1, true);

  path p2 = resourcesPath() / openstudio::toPath("gbxml/exampleModel2.osm");
  model2->save(p2, true);
}

TEST_F(gbXMLFixture, ForwardTranslator_AdiabaticSurface) {
  Model model = exampleModel();

  std::string surfname("Adiabatic_Surface");

  // Find a surface, make it adiabatic
  for (auto& surf : model.getModelObjects<Surface>()) {
    if (surf.outsideBoundaryCondition() == "Outdoors") {
      surf.setOutsideBoundaryCondition("Adiabatic");
      surf.setSunExposure("NoSun");
      surf.setWindExposure("NoWind");
      surf.setName(surfname);
      break;
    }
  }

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModelAdiabatic.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check the surface
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);

  auto osurf = model2->getModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Adiabatic", osurf->outsideBoundaryCondition());
  EXPECT_EQ("NoSun", osurf->sunExposure());
  EXPECT_EQ("NoWind", osurf->windExposure());
}

TEST_F(gbXMLFixture, ForwardTranslator_ConstructionLayers) {
  Model model;

  Construction construction(model);
  construction.setName("Construction1");

  MaterialVector layers;

  MasslessOpaqueMaterial material1(model);
  material1.setName("Material1");
  layers.push_back(material1);

  StandardOpaqueMaterial material2(model);
  material2.setName("Material2");
  layers.push_back(material2);

  MasslessOpaqueMaterial material3(model);
  material3.setName("Material3");
  layers.push_back(material3);

  StandardOpaqueMaterial material4(model);
  material4.setName("Material4");
  layers.push_back(material4);

  construction.setLayers(layers);

  Facility facility = model.getUniqueModelObject<Facility>();

  Building building = model.getUniqueModelObject<Building>();

  Space space(model);
  space.setName("Space1");

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 0, 1));

  //std::string surfname("Surface 1"); // DLM: note this will fail because "Surface 1" gets round tripped as "Surface_1"
  std::string surfname("Surface1");
  Surface surface(points, model);
  surface.setName(surfname);
  surface.setConstruction(construction);
  surface.setSpace(space);

  ThermalZone zone(model);
  space.setThermalZone(zone);

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_ConstructionLayers.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check the surface
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);
  //std::cout << *model2 << '\n';
  auto osurf = model2->getModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  auto ocons = osurf->construction();
  ASSERT_TRUE(ocons);
  auto olayeredcons = ocons->optionalCast<Construction>();
  ASSERT_TRUE(olayeredcons);
  ASSERT_EQ(4u, olayeredcons->layers().size());
  EXPECT_TRUE(olayeredcons->layers()[0].optionalCast<MasslessOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[1].optionalCast<StandardOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[2].optionalCast<MasslessOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[3].optionalCast<StandardOpaqueMaterial>());
}

TEST_F(gbXMLFixture, ForwardTranslator_NoFacility) {
  // Test for #3314: gbXML translation does not roundtrip unless Facility object present

  Model model;

  Construction construction(model);
  construction.setName("Construction1");

  MaterialVector layers;

  MasslessOpaqueMaterial material1(model);
  material1.setName("Material1");
  layers.push_back(material1);

  StandardOpaqueMaterial material2(model);
  material2.setName("Material2");
  layers.push_back(material2);

  MasslessOpaqueMaterial material3(model);
  material3.setName("Material3");
  layers.push_back(material3);

  StandardOpaqueMaterial material4(model);
  material4.setName("Material4");
  material4.setRoughness("MediumSmooth");
  layers.push_back(material4);

  construction.setLayers(layers);

  // Not instantiating facility nor building on purpose
  // Facility facility = model.getUniqueModelObject<Facility>();
  // Building building = model.getUniqueModelObject<Building>();

  Space space(model);
  space.setName("Space1");

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 0, 1));

  //std::string surfname("Surface 1"); // DLM: note this will fail because "Surface 1" gets round tripped as "Surface_1"
  std::string surfname("Surface1");
  Surface surface(points, model);
  surface.setName(surfname);
  surface.setConstruction(construction);
  surface.setSpace(space);

  ThermalZone zone(model);
  zone.setName("Zone1");
  space.setThermalZone(zone);

  // save model for diffing
  bool debug = false;
  if (debug) {
    path modelPath = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility_original.osm");
    model.save(modelPath, true);
  }

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check surface/space/zone were all translated
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);
  //std::cout << *model2 << '\n';
  auto osurf = model2->getModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  auto ospace = model2->getModelObjectByName<Space>(space.nameString());
  ASSERT_TRUE(ospace);
  auto ozone = model2->getModelObjectByName<ThermalZone>(zone.nameString());  // Dragostea Din Tei!
  ASSERT_TRUE(ozone);

  // This really tests a RT feature, but doesn't really matter. When diffing original & rountripped, I noticed a diff in Material:
  // the roundtripped model has Roughness missing
  auto omat = model2->getModelObjectByName<StandardOpaqueMaterial>("Material4");
  ASSERT_TRUE(omat);
  EXPECT_EQ("MediumSmooth", omat->roughness());

  if (debug) {
    path modelPath2 = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility_roundtripped.osm");
    model2->save(modelPath2, true);
  }
}

TEST_F(gbXMLFixture, ForwardTranslator_surfaceType_4001) {
  // Test for #4001 : surfaceType is written twice when SlabOnGrade
  Model model = exampleModel();

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  ASSERT_TRUE(openstudio::filesystem::exists(p));

  openstudio::filesystem::ifstream file(p, std::ios_base::binary);
  ASSERT_TRUE(file.is_open());
  pugi::xml_document doc;
  auto load_result = doc.load(file);
  file.close();
  ASSERT_TRUE(load_result) << "'" << p << "' Failed to load:\n"
                           << "Error description: " << load_result.description() << "\n"
                           << "Error offset: " << load_result.offset;

  // Now go select the Surface_1, which currently ends up with two surfaceType attributes before fix for #4001
  pugi::xpath_node surfaceXPath = doc.select_node("//Surface[@id='Surface_1']");
  ASSERT_TRUE(surfaceXPath);
  pugi::xml_node surfaceNode = surfaceXPath.node();
  EXPECT_EQ(1u, std::count_if(surfaceNode.attributes_begin(), surfaceNode.attributes_end(),
                              [](const auto& att) { return openstudio::istringEqual(att.name(), "surfaceType"); }));
  std::string surfaceType = surfaceNode.attribute("surfaceType").value();
  std::string expectedSurfaceType("SlabOnGrade");
  EXPECT_EQ(expectedSurfaceType, surfaceType);
}

TEST_F(gbXMLFixture, ForwardTranslator_exampleModel_State) {
  // Test for #4135: translating a model twice should produce the same result
  Model model = exampleModel();

  ForwardTranslator forwardTranslator;
  std::string gbXML_str1 = forwardTranslator.modelToGbXMLString(model);
  EXPECT_FALSE(gbXML_str1.empty());

  std::string gbXML_str2 = forwardTranslator.modelToGbXMLString(model);
  EXPECT_FALSE(gbXML_str2.empty());

  EXPECT_EQ(gbXML_str1.length(), gbXML_str2.length());
  EXPECT_GT(gbXML_str1.length(), 50000);
}

Model testModel() {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  BuildingStory buildingStory(model);
  buildingStory.setNominalZCoordinate(0);
  buildingStory.setNominalFloortoFloorHeight(3);

  ThermalZone thermalZone(model);

  std::vector<Point3d> floorPrint;
  floorPrint.push_back(Point3d(0, 0, 0));
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));

  // make spaces
  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space1);
  space1->setThermalZone(thermalZone);
  space1->setBuildingStory(buildingStory);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space2);
  space2->setXOrigin(10);
  space2->setThermalZone(thermalZone);
  space2->setBuildingStory(buildingStory);

  boost::optional<Space> space3 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space3);
  space3->setYOrigin(10);
  space3->setThermalZone(thermalZone);
  space3->setBuildingStory(buildingStory);

  boost::optional<Space> space4 = Space::fromFloorPrint(floorPrint, 3, model);
  OS_ASSERT(space4);
  space4->setXOrigin(10);
  space4->setYOrigin(10);
  space4->setThermalZone(thermalZone);
  space4->setBuildingStory(buildingStory);

  // add a door to south wall of space1
  std::vector<Point3d> doorPoints;
  doorPoints.push_back(Point3d(2, 0, 2));
  doorPoints.push_back(Point3d(2, 0, 0));
  doorPoints.push_back(Point3d(4, 0, 0));
  doorPoints.push_back(Point3d(4, 0, 2));

  // find south wall
  std::vector<Surface> searchResults;

  searchResults = space1->findSurfaces(180.0, 180.0, 90.0, 90.0);
  OS_ASSERT(searchResults.size() >= 1);

  // add door
  SubSurface door(doorPoints, model);
  door.setSurface(searchResults[0]);

  // add a window to east wall of space2
  std::vector<Point3d> windowPoints;
  windowPoints.push_back(Point3d(10, 2, 2));
  windowPoints.push_back(Point3d(10, 2, 1));
  windowPoints.push_back(Point3d(10, 8, 1));
  windowPoints.push_back(Point3d(10, 8, 2));

  // find east wall
  searchResults = space2->findSurfaces(90.0, 90.0, 90.0, 90.0);
  OS_ASSERT(searchResults.size() >= 1);

  // add window
  SubSurface window(windowPoints, model);
  window.setSurface(searchResults[0]);

  // match surfaces
  std::vector<Space> spaces = model.getConcreteModelObjects<Space>();
  matchSurfaces(spaces);

  return model;
}

// Create two gbxml models from two identical osm models should produce
// gbxml models with surfaces in the same order. issue 4438
TEST_F(gbXMLFixture, ForwardTranslator_4438_Deterministic) {
  Model model1 = testModel();

  path p1 = resourcesPath() / openstudio::toPath("gbxml/exampleModel1.xml");

  ForwardTranslator forwardTranslator;
  bool test1 = forwardTranslator.modelToGbXML(model1, p1);

  EXPECT_TRUE(test1);

  path ps1 = resourcesPath() / openstudio::toPath("gbxml/exampleModel1.osm");
  model1.save(ps1, true);

  Model model2 = testModel();

  path p2 = resourcesPath() / openstudio::toPath("gbxml/exampleModel2.xml");

  //ForwardTranslator forwardTranslator;
  bool test2 = forwardTranslator.modelToGbXML(model2, p2);

  EXPECT_TRUE(test2);

  path ps2 = resourcesPath() / openstudio::toPath("gbxml/exampleModel2.osm");
  model1.save(ps2, true);
}

TEST_F(gbXMLFixture, ForwardTranslator_IDs_Names) {
  // Test for #4457 - Support gbXML translation where user-input <Name> is different from the id
  Model model = exampleModel();

  model.save(resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames.osm"), true);

  ForwardTranslator forwardTranslator;
  ReverseTranslator reverseTranslator;

  // does not have additional properties (gbXMLId, displayName, CADObjectId)
  {
    path p = resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames_1.xml");
    bool test = forwardTranslator.modelToGbXML(model, p);
    EXPECT_TRUE(test);

    boost::optional<Model> model2 = reverseTranslator.loadModel(p);
    ASSERT_TRUE(model2);

    model2->save(resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames_1.osm"), true);

    {
      auto _obj = model2->getModelObjectByName<Building>("Building_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Building 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<BuildingStory>("Building_Story_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Building Story 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Space>("Space_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Space 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Surface>("Surface_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Surface 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<SubSurface>("Sub_Surface_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Sub Surface 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<ThermalZone>("Thermal_Zone_1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Thermal Zone 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Construction>("Exterior_Wall");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Exterior Wall", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Material>("I02_25mm_insulation_board");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("I02 25mm insulation board", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
    }
  }

  // does have additional properties (gbXMLId, displayName, CADObjectId)
  {
    auto _building = model.getModelObjectByName<Building>("Building 1");
    ASSERT_TRUE(_building);
    EXPECT_TRUE(_building->setGBXMLId("Building1"));
    EXPECT_TRUE(_building->setDisplayName("Building 1"));
    EXPECT_TRUE(_building->setCADObjectId("1234"));

    auto _buildingStory = model.getModelObjectByName<BuildingStory>("Building Story 1");
    ASSERT_TRUE(_buildingStory);
    EXPECT_TRUE(_buildingStory->setGBXMLId("BuildingStory1"));
    EXPECT_TRUE(_buildingStory->setDisplayName("Building Story 1"));
    EXPECT_TRUE(_buildingStory->setCADObjectId("1234"));

    auto _space = model.getModelObjectByName<Space>("Space 1");
    ASSERT_TRUE(_space);
    EXPECT_TRUE(_space->setGBXMLId("Space1"));
    EXPECT_TRUE(_space->setDisplayName("Space 1"));
    EXPECT_TRUE(_space->setCADObjectId("1234"));

    auto _surface = model.getModelObjectByName<Surface>("Surface 1");
    ASSERT_TRUE(_surface);
    EXPECT_TRUE(_surface->setGBXMLId("Surface1"));
    EXPECT_TRUE(_surface->setDisplayName("Surface 1"));
    EXPECT_TRUE(_surface->setCADObjectId("1234"));

    auto _subSurface = model.getModelObjectByName<SubSurface>("Sub Surface 1");
    ASSERT_TRUE(_subSurface);
    EXPECT_TRUE(_subSurface->setGBXMLId("SubSurface1"));
    EXPECT_TRUE(_subSurface->setDisplayName("Sub Surface 1"));
    EXPECT_TRUE(_subSurface->setCADObjectId("1234"));

    auto _zone = model.getModelObjectByName<ThermalZone>("Thermal Zone 1");
    ASSERT_TRUE(_zone);
    EXPECT_TRUE(_zone->setGBXMLId("ThermalZone1"));
    EXPECT_TRUE(_zone->setDisplayName("Thermal Zone 1"));
    EXPECT_TRUE(_zone->setCADObjectId("1234"));

    auto _construction = model.getModelObjectByName<Construction>("Exterior Wall");
    ASSERT_TRUE(_construction);
    EXPECT_TRUE(_construction->setGBXMLId("ExteriorWall"));
    EXPECT_TRUE(_construction->setDisplayName("Exterior Wall"));
    EXPECT_TRUE(_construction->setCADObjectId("1234"));

    auto _material = model.getModelObjectByName<Material>("I02 25mm insulation board");
    ASSERT_TRUE(_material);
    EXPECT_TRUE(_material->setGBXMLId("I0225mminsulationboard"));
    EXPECT_TRUE(_material->setDisplayName("I02 25mm insulation board"));
    EXPECT_TRUE(_material->setCADObjectId("1234"));

    path p = resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames_2.xml");
    bool test = forwardTranslator.modelToGbXML(model, p);
    EXPECT_TRUE(test);

    boost::optional<Model> model2 = reverseTranslator.loadModel(p);
    ASSERT_TRUE(model2);

    model2->save(resourcesPath() / openstudio::toPath("gbxml/exampleModelIDsNames_2.osm"), true);

    {
      auto _obj = model2->getModelObjectByName<Building>("Building_1");  // Building doesn't use gbXMLId
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));  // Building doesn't use CADObjectId
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Building 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("Building_1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<BuildingStory>("BuildingStory1");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));  // BuildingStory doesn't use CADObjectId
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Building Story 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("BuildingStory1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Space>("Space1");
      ASSERT_TRUE(_obj);
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_TRUE(_obj->cadObjectId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
      EXPECT_EQ("1234", _obj->cadObjectId().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Space 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("Space1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Surface>("Surface1");
      ASSERT_TRUE(_obj);
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_TRUE(_obj->cadObjectId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
      EXPECT_EQ("1234", _obj->cadObjectId().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Surface 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("Surface1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<SubSurface>("SubSurface1");
      ASSERT_TRUE(_obj);
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_TRUE(_obj->cadObjectId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
      EXPECT_EQ("1234", _obj->cadObjectId().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Sub Surface 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("SubSurface1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<ThermalZone>("ThermalZone1");
      ASSERT_TRUE(_obj);
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("CADObjectId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_TRUE(_obj->cadObjectId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("CADObjectId").get(), _obj->cadObjectId().get());
      EXPECT_EQ("1234", _obj->cadObjectId().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Thermal Zone 1", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("ThermalZone1", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Construction>("ExteriorWall");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));  // Construction doesn't use CADObjectId
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("Exterior Wall", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("ExteriorWall", _obj->gbXMLId().get());
    }

    {
      auto _obj = model2->getModelObjectByName<Material>("I0225mminsulationboard");
      ASSERT_TRUE(_obj);
      EXPECT_FALSE(_obj->additionalProperties().hasFeature("CADObjectId"));  // Material doesn't use CADObjectId
      ASSERT_FALSE(_obj->additionalProperties().getFeatureAsString("CADObjectId"));
      ASSERT_FALSE(_obj->cadObjectId());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("displayName"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("displayName"));
      ASSERT_TRUE(_obj->displayName());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("displayName").get(), _obj->displayName().get());
      EXPECT_EQ("I02 25mm insulation board", _obj->displayName().get());
      EXPECT_TRUE(_obj->additionalProperties().hasFeature("gbXMLId"));
      ASSERT_TRUE(_obj->additionalProperties().getFeatureAsString("gbXMLId"));
      ASSERT_TRUE(_obj->gbXMLId());
      EXPECT_EQ(_obj->additionalProperties().getFeatureAsString("gbXMLId").get(), _obj->gbXMLId().get());
      EXPECT_EQ("I0225mminsulationboard", _obj->gbXMLId().get());
    }
  }
}
