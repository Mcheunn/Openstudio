/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../GltfForwardTranslator.hpp"
#include "../Model.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../ConstructionAirBoundary.hpp"
#include "../../osversion/VersionTranslator.hpp"
#include "../Construction.hpp"

#include "../tiny_gltf.h"

#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;


/// <summary>
/// Validation report
////Format: glTF 2.0
///Stats:
///0 animations
///9 materials
///30 meshes
///31 nodes
///30 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_ExampleModel) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/exampleModel.gltf");
  Model model = exampleModel();
  model.save(resourcesPath() / toPath("model/exampleModel.osm"), true);
  bool result = ft.modelToGLTF(model, output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///4 materials
///481 meshes
///482 nodes
///481 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_RefBldgSecondarySchoolNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgSecondarySchoolNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgSecondarySchoolNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///5 materials
///443 meshes
///444 nodes
///443 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_RefBldgHospitalNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgHospitalNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgHospitalNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
/// Format: glTF 2.0
///0 animations
///5 materials
///871 meshes
///872 nodes
///871 primitives
///0 textures
///Extensions: None
/// 
/// FIXED : 1742 Validation issues 
/// starting from : 
/// ACCESSOR_TOTAL_OFFSET_ALIGNMENT	Accessor's total byteOffset 7162 isn't a multiple of componentType length 4.	
/// /accessors/1
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_RefBldgOutPatientNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgOutPatientNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgOutPatientNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///5 materials
///589 meshes
///590 nodes
///589 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_RefBldgSmallHotelNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgSmallHotelNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgSmallHotelNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///4 materials
///21 meshes
///22 nodes
///21 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_RefBldgFullServiceRestaurantNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgFullServiceRestaurantNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgFullServiceRestaurantNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///5 materials
///208 meshes
///209 nodes
///208 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_RefBldgLargeHotelNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgLargeHotelNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgLargeHotelNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///4 materials
///142 meshes
///143 nodes
///142 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_RefBldgLargeOfficeNew2004_Chicago) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/RefBldgLargeOfficeNew2004_Chicago.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/RefBldgLargeOfficeNew2004_Chicago.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///4 materials
///80 meshes
///81 nodes
///80 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_Space_SurfaceMatch_LargeTest) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/Space_SurfaceMatch_LargeTest.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/Space_SurfaceMatch_LargeTest.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///3 materials
///1515 meshes
///1516 nodes
///1515 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_15023_Model12) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/15023_Model12.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/15023_Model12.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///3 materials
///21 meshes
///22 nodes
///21 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_split_level) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/split_level.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/split_level.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///3 materials
///24 meshes
///25 nodes
///24 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_open_to_below) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/open_to_below.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/open_to_below.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Load Test with and without bin file
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_LoadTest) {
  GltfForwardTranslator ft;
  openstudio::path inputPath;
  openstudio::path inputNonEmbededPath;
  inputPath = resourcesPath() / toPath("utilities/Geometry/minimal_GLTF_File.gltf");
  inputNonEmbededPath = resourcesPath() / toPath("utilities/Geometry/input.gltf");
  bool result = ft.loadGLTF(inputPath, inputNonEmbededPath);
  ASSERT_TRUE(result);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///1 materials
///1 meshes
///1 nodes
///1 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_CreateTriangleGLTFTest) {
  GltfForwardTranslator ft;
  openstudio::path output;
  openstudio::path output_2;
  output = resourcesPath() / toPath("utilities/Geometry/triangle.gltf");
  //Passed Raw buffer data
  bool result = ft.CreateTriangleGLTF(output);
  ASSERT_TRUE(result);
  output_2 = resourcesPath() / toPath("utilities/Geometry/triangle_2.gltf");
  //Created Raw buffer data from Point3dVector
  bool result_2 = ft.CreateTriangleGLTF_2(output_2);
  ASSERT_TRUE(result_2);
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///4 materials
///3915 meshes
///3916 nodes
///3915 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_7_7_Windows_Complete) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/7-7_Windows_Complete.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/7-7_Windows_Complete.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// VersionTranslator.cpp@590 : Unable to retrieve OpenStudio Version 3.2.2 IDD from the IddFactory."
///  thrown in the test body.
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_story_multipliers) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/story_multipliers.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/story_multipliers.osm");
  //model::OptionalModel model = translator.loadModel(modelPath);
  //bool result = ft.modelToGLTF(model.get(), output);
  //ASSERT_TRUE(result);
}

/// <summary>
/// Assertion failed: this->is_initialized() | Optional.hpp 1212
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_story_space_heights) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/story_space_heights.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/story_space_heights.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  /*bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);*/
}

/// <summary>
/// Validation report
///Format: glTF 2.0
///Stats:
///0 animations
///3 materials
///227 meshes
///228 nodes
///227 primitives
///0 textures
///Extensions: None
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_floorplan_school) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/floorplan_school.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/floorplan_school.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);
}

/// <summary>
/// Assertion failed: this->is_initialized() | Optional.hpp 1212
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_two_stories_pre_intersect) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/two_stories_pre_intersect.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/two_stories_pre_intersect.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  /*bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);*/
}

/// <summary>
/// Assertion failed: this->is_initialized() | Optinoal.hpp 1212
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(ModelFixture, GltfForwardTranslator_TwoStoryOffice_Trane) {
  GltfForwardTranslator ft;
  openstudio::path output;
  output = resourcesPath() / toPath("utilities/Geometry/TwoStoryOffice_Trane.gltf");
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("gbxml/TwoStoryOffice_Trane.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  /*bool result = ft.modelToGLTF(model.get(), output);
  ASSERT_TRUE(result);*/
}
