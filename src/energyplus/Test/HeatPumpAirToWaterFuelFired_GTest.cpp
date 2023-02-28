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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveQuadratic.hpp"

#include <utilities/idd/HeatPump_AirToWater_FuelFired_Heating_FieldEnums.hxx>
#include <utilities/idd/HeatPump_AirToWater_FuelFired_Cooling_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatPumpAirToWaterFuelFired) {
  Model m;

  PlantLoop plant_loop_ffhp_htg(m);
  PlantLoop plant_loop_ffhp_clg(m);

  HeatPumpAirToWaterFuelFiredHeating ffhp_htg(m);

  EXPECT_TRUE(ffhp_htg.setFuelType("Propane"));
  EXPECT_TRUE(ffhp_htg.setEndUseSubcategory("AnyText"));
  EXPECT_TRUE(ffhp_htg.setNominalHeatingCapacity(1.0));
  EXPECT_TRUE(ffhp_htg.setNominalCOP(2.0));
  EXPECT_TRUE(ffhp_htg.setDesignFlowRate(3.0));
  EXPECT_TRUE(ffhp_htg.setDesignSupplyTemperature(4.0));
  EXPECT_TRUE(ffhp_htg.setDesignTemperatureLift(5.0));
  EXPECT_TRUE(ffhp_htg.setSizingFactor(6.0));
  EXPECT_TRUE(ffhp_htg.setFlowMode("ConstantFlow"));
  EXPECT_TRUE(ffhp_htg.setOutdoorAirTemperatureCurveInputVariable("WetBulb"));
  EXPECT_TRUE(ffhp_htg.setWaterTemperatureCurveInputVariable("LeavingCondenser"));
  CurveBiquadratic curve1(m);
  EXPECT_TRUE(ffhp_htg.setNormalizedCapacityFunctionofTemperatureCurve(curve1));
  CurveBiquadratic curve2(m);
  EXPECT_TRUE(ffhp_htg.setFuelEnergyInputRatioFunctionofTemperatureCurve(curve2));
  CurveQuadratic curve3(m);
  EXPECT_TRUE(ffhp_htg.setFuelEnergyInputRatioFunctionofPLRCurve(curve3));
  EXPECT_TRUE(ffhp_htg.setMinimumPartLoadRatio(0.5));
  EXPECT_TRUE(ffhp_htg.setMaximumPartLoadRatio(0.75));
  EXPECT_TRUE(ffhp_htg.setDefrostControlType("OnDemand"));
  EXPECT_TRUE(ffhp_htg.setDefrostOperationTimeFraction(0.8));
  CurveQuadratic curve4(m);
  EXPECT_TRUE(ffhp_htg.setFuelEnergyInputRatioDefrostAdjustmentCurve(curve4));
  EXPECT_TRUE(ffhp_htg.setResistiveDefrostHeaterCapacity(10.0));
  EXPECT_TRUE(ffhp_htg.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(9.9));
  CurveQuadratic curve5(m);
  EXPECT_TRUE(ffhp_htg.setCyclingRatioFactorCurve(curve5));
  EXPECT_TRUE(ffhp_htg.setNominalAuxiliaryElectricPower(12.0));
  CurveBiquadratic curve6(m);
  EXPECT_TRUE(ffhp_htg.setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(curve6));
  CurveQuadratic curve7(m);
  EXPECT_TRUE(ffhp_htg.setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(curve7));
  EXPECT_TRUE(ffhp_htg.setStandbyElectricPower(13.0));

  HeatPumpAirToWaterFuelFiredCooling ffhp_clg(m);

  EXPECT_TRUE(ffhp_clg.setFuelType("Propane"));
  EXPECT_TRUE(ffhp_clg.setEndUseSubcategory("AnyText"));
  EXPECT_TRUE(ffhp_clg.setNominalCoolingCapacity(1.0));
  EXPECT_TRUE(ffhp_clg.setNominalCOP(2.0));
  EXPECT_TRUE(ffhp_clg.setDesignFlowRate(3.0));
  EXPECT_TRUE(ffhp_clg.setDesignSupplyTemperature(4.0));
  EXPECT_TRUE(ffhp_clg.setDesignTemperatureLift(5.0));
  EXPECT_TRUE(ffhp_clg.setSizingFactor(6.0));
  EXPECT_TRUE(ffhp_clg.setFlowMode("ConstantFlow"));
  EXPECT_TRUE(ffhp_clg.setOutdoorAirTemperatureCurveInputVariable("WetBulb"));
  EXPECT_TRUE(ffhp_clg.setWaterTemperatureCurveInputVariable("LeavingEvaporator"));
  CurveBiquadratic curve8(m);
  EXPECT_TRUE(ffhp_clg.setNormalizedCapacityFunctionofTemperatureCurve(curve8));
  CurveBiquadratic curve9(m);
  EXPECT_TRUE(ffhp_clg.setFuelEnergyInputRatioFunctionofTemperatureCurve(curve9));
  CurveQuadratic curve10(m);
  EXPECT_TRUE(ffhp_clg.setFuelEnergyInputRatioFunctionofPLRCurve(curve10));
  EXPECT_TRUE(ffhp_clg.setMinimumPartLoadRatio(0.5));
  EXPECT_TRUE(ffhp_clg.setMaximumPartLoadRatio(0.75));
  CurveQuadratic curve11(m);
  EXPECT_TRUE(ffhp_clg.setCyclingRatioFactorCurve(curve11));
  EXPECT_TRUE(ffhp_clg.setNominalAuxiliaryElectricPower(12.0));
  CurveBiquadratic curve12(m);
  EXPECT_TRUE(ffhp_clg.setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(curve12));
  CurveQuadratic curve13(m);
  EXPECT_TRUE(ffhp_clg.setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(curve13));
  EXPECT_TRUE(ffhp_clg.setStandbyElectricPower(13.0));

  EXPECT_TRUE(ffhp_htg.setCompanionCoolingHeatPump(ffhp_clg));
  EXPECT_TRUE(ffhp_clg.setCompanionHeatingHeatPump(ffhp_htg));

  EXPECT_TRUE(plant_loop_ffhp_htg.addSupplyBranchForComponent(ffhp_htg));
  EXPECT_TRUE(plant_loop_ffhp_clg.addSupplyBranchForComponent(ffhp_clg));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::HeatPump_AirToWater_FuelFired_Heating).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::HeatPump_AirToWater_FuelFired_Cooling).size());

  IdfObject idf_ffhp_htg = w.getObjectsByType(IddObjectType::HeatPump_AirToWater_FuelFired_Heating)[0];

  EXPECT_EQ(ffhp_htg.nameString(), idf_ffhp_htg.getString(HeatPump_AirToWater_FuelFired_HeatingFields::Name, false).get());

  IdfObject idf_ffhp_clg = w.getObjectsByType(IddObjectType::HeatPump_AirToWater_FuelFired_Cooling)[0];

  EXPECT_EQ(ffhp_clg.nameString(), idf_ffhp_clg.getString(HeatPump_AirToWater_FuelFired_CoolingFields::Name, false).get());
}
