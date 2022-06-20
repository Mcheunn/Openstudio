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

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../MaterialPropertyPhaseChange.hpp"
#include "../MaterialPropertyPhaseChange_Impl.hpp"

#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, MaterialPropertyPhaseChange_MaterialPropertyPhaseChange) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a material object to use
      StandardOpaqueMaterial material(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create a material object to use
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(1, model.modelObjects().size());

  // new material does not have material property phase change yet
  EXPECT_TRUE(!material.materialPropertyPhaseChange());

  // create a material property phase change object to use
  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange();
  ASSERT_TRUE(optphaseChange);
  EXPECT_EQ(2, model.modelObjects().size());

  // now the material has material property phase change
  EXPECT_TRUE(material.materialPropertyPhaseChange());

  // check to make sure the temperature coefficient for thermal conductivity field is defaulted as expected
  auto phaseChange = optphaseChange.get();
  EXPECT_TRUE(phaseChange.isTemperatureCoefficientforThermalConductivityDefaulted());

  // check that creating the material property phase change when they already exists does nothing and returns nil
  boost::optional<MaterialPropertyPhaseChange> optphaseChange2 = material.createMaterialPropertyPhaseChange();
  ASSERT_FALSE(optphaseChange2);
  EXPECT_EQ(2, model.modelObjects().size());
}

// test setting and getting
TEST_F(ModelFixture, MaterialPropertyPhaseChange_SetGetFields) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange();
  auto phaseChange = optphaseChange.get();

  // check that the properties were set properly
  EXPECT_EQ(0, phaseChange.temperatureCoefficientforThermalConductivity());
  EXPECT_EQ(0, phaseChange.temperatureEnthalpys().size());
  EXPECT_EQ(0, phaseChange.numberofTemperatureEnthalpys());

  // now override the defaults with explicit values
  phaseChange.setTemperatureCoefficientforThermalConductivity(1);

  EXPECT_EQ(1, phaseChange.temperatureCoefficientforThermalConductivity());
  EXPECT_FALSE(phaseChange.isTemperatureCoefficientforThermalConductivityDefaulted());

  // test setting back to defaults
  phaseChange.resetTemperatureCoefficientforThermalConductivity();
  EXPECT_TRUE(phaseChange.isTemperatureCoefficientforThermalConductivityDefaulted());
}

// check temperature-enthalpy
TEST_F(ModelFixture, MaterialPropertyPhaseChange_TemperatureEnthalpy) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange();
  auto phaseChange = optphaseChange.get();

  phaseChange.removeAllTemperatureEnthalpys();

  EXPECT_EQ(0, phaseChange.numberofTemperatureEnthalpys());
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpy(10, 20));
  EXPECT_EQ(1, phaseChange.numberofTemperatureEnthalpys());
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpy(25, 30));
  EXPECT_EQ(2, phaseChange.numberofTemperatureEnthalpys());
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpy(1, 2));
  EXPECT_EQ(3, phaseChange.numberofTemperatureEnthalpys());

  phaseChange.removeTemperatureEnthalpy(1);
  EXPECT_EQ(2, phaseChange.numberofTemperatureEnthalpys());

  // check that remaining temperature enthalpys moved correctly
  std::vector<TemperatureEnthalpy> temperatureEnthalpys = phaseChange.temperatureEnthalpys();
  EXPECT_EQ(10, temperatureEnthalpys[0].temperature());
  EXPECT_EQ(20, temperatureEnthalpys[0].enthalpy());
  EXPECT_EQ(1, temperatureEnthalpys[1].temperature());
  EXPECT_EQ(2, temperatureEnthalpys[1].enthalpy());

  // more remove checking
  phaseChange.removeAllTemperatureEnthalpys();
  EXPECT_EQ(0, phaseChange.numberofTemperatureEnthalpys());
  phaseChange.removeTemperatureEnthalpy(0);
  EXPECT_EQ(0, phaseChange.numberofTemperatureEnthalpys());

  // check adding temperature enthalpy non-conveniently
  TemperatureEnthalpy temperatureEnthalpy(50, 100);
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpy(temperatureEnthalpy));
  EXPECT_EQ(1, phaseChange.numberofTemperatureEnthalpys());

  // check buil-adding temperature enthalpys
  std::vector<TemperatureEnthalpy> temperatureEnthalpysToAdd;
  TemperatureEnthalpy temperatureEnthalpy1(3, 4);
  temperatureEnthalpysToAdd.push_back(temperatureEnthalpy1);
  TemperatureEnthalpy temperatureEnthalpy2(5, 6);
  temperatureEnthalpysToAdd.push_back(temperatureEnthalpy2);
  ASSERT_TRUE(phaseChange.addTemperatureEnthalpys(temperatureEnthalpysToAdd));
  EXPECT_EQ(3, phaseChange.numberofTemperatureEnthalpys());
  EXPECT_EQ(3, phaseChange.temperatureEnthalpys().size());
}
