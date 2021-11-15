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
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../HumidifierSteamGas.hpp"
#include "../HumidifierSteamGas_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Curve.hpp"
#include "../CurveLinear.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, HumidifierSteamGas_HumidifierSteamGas) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HumidifierSteamGas humidifier(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HumidifierSteamGas humidifier(m);

  EXPECT_FALSE(humidifier.availabilitySchedule());
  EXPECT_FALSE(humidifier.ratedCapacity());
  EXPECT_TRUE(humidifier.isRatedCapacityAutosized());
  EXPECT_TRUE(humidifier.ratedGasUseRate());
  EXPECT_EQ(104000, humidifier.ratedGasUseRate().get());
  EXPECT_FALSE(humidifier.isRatedGasUseRateAutosized());
  EXPECT_EQ(0.8, humidifier.thermalEfficiency());
  EXPECT_TRUE(humidifier.isThermalEfficiencyDefaulted());
  EXPECT_FALSE(humidifier.thermalEfficiencyModifierCurve());
  EXPECT_FALSE(humidifier.ratedFanPower());
  EXPECT_EQ(0.0, humidifier.auxiliaryElectricPower());
  EXPECT_TRUE(humidifier.isAuxiliaryElectricPowerDefaulted());
  EXPECT_EQ("FixedInletWaterTemperature", humidifier.inletWaterTemperatureOption());
  EXPECT_TRUE(humidifier.isInletWaterTemperatureOptionDefaulted());

  ScheduleConstant schedule(m);
  EXPECT_TRUE(humidifier.setAvailabilitySchedule(schedule));
  EXPECT_TRUE(humidifier.availabilitySchedule());
  humidifier.resetAvailabilitySchedule();
  EXPECT_FALSE(humidifier.availabilitySchedule());

  EXPECT_TRUE(humidifier.setRatedCapacity(1000));
  EXPECT_TRUE(humidifier.ratedCapacity());
  EXPECT_EQ(1000, humidifier.ratedCapacity().get());
  EXPECT_FALSE(humidifier.isRatedCapacityAutosized());
  humidifier.autosizeRatedCapacity();
  EXPECT_TRUE(humidifier.isRatedCapacityAutosized());

  humidifier.autosizeRatedGasUseRate();
  EXPECT_TRUE(humidifier.isRatedGasUseRateAutosized());
  EXPECT_FALSE(humidifier.ratedGasUseRate());
  humidifier.resetRatedGasUseRate();
  EXPECT_FALSE(humidifier.ratedGasUseRate());
  EXPECT_FALSE(humidifier.isRatedGasUseRateAutosized());

  EXPECT_TRUE(humidifier.setThermalEfficiency(0.9));
  EXPECT_EQ(0.9, humidifier.thermalEfficiency());
  EXPECT_FALSE(humidifier.isThermalEfficiencyDefaulted());
  humidifier.resetThermalEfficiency();
  EXPECT_EQ(0.8, humidifier.thermalEfficiency());
  EXPECT_TRUE(humidifier.isThermalEfficiencyDefaulted());

  CurveLinear curve(m);
  EXPECT_TRUE(humidifier.setThermalEfficiencyModifierCurve(curve));
  EXPECT_TRUE(humidifier.thermalEfficiencyModifierCurve());
  humidifier.resetThermalEfficiencyModifierCurve();
  EXPECT_FALSE(humidifier.thermalEfficiencyModifierCurve());

  EXPECT_TRUE(humidifier.setRatedFanPower(10));
  EXPECT_TRUE(humidifier.ratedFanPower());
  humidifier.resetRatedFanPower();
  EXPECT_FALSE(humidifier.ratedFanPower());

  EXPECT_TRUE(humidifier.setAuxiliaryElectricPower(0.1));
  EXPECT_EQ(0.1, humidifier.auxiliaryElectricPower());
  EXPECT_FALSE(humidifier.isAuxiliaryElectricPowerDefaulted());
  humidifier.resetAuxiliaryElectricPower();
  EXPECT_EQ(0.0, humidifier.auxiliaryElectricPower());
  EXPECT_TRUE(humidifier.isAuxiliaryElectricPowerDefaulted());

  EXPECT_TRUE(humidifier.setInletWaterTemperatureOption("VariableInletWaterTemperature"));
  EXPECT_EQ("VariableInletWaterTemperature", humidifier.inletWaterTemperatureOption());
  EXPECT_FALSE(humidifier.isInletWaterTemperatureOptionDefaulted());
  humidifier.resetInletWaterTemperatureOption();
  EXPECT_EQ("FixedInletWaterTemperature", humidifier.inletWaterTemperatureOption());
  EXPECT_TRUE(humidifier.isInletWaterTemperatureOptionDefaulted());
}

TEST_F(ModelFixture, HumidifierSteamGas_addToNode) {
  Model m;
  HumidifierSteamGas testObject(m);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  HumidifierSteamGas testObject2(m);
  HumidifierSteamGas testObject3(m);

  if (boost::optional<Node> OANode = outdoorAirSystem.outboardOANode()) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.oaComponents().size());
  }

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  HumidifierSteamGas testObjectClone = testObject.clone(m).cast<HumidifierSteamGas>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
}