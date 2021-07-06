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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/WaterHeaterHeatPump.hpp"
#include "../../model/ThermalZone.hpp"

#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilWaterHeatingAirToWaterHeatPumpVariableSpeed) {
  Model m;

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);

  WaterHeaterHeatPump hpwh(m);
  CurveQuadratic curve(m);
  ScheduleConstant schedule(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData speed(m);
  ThermalZone tz(m);

  hpwh.setDXCoil(coil);
  hpwh.addToThermalZone(tz);
  coil.setPartLoadFractionCorrelationCurve(curve);
  coil.setGridSignalSchedule(schedule);
  coil.addSpeed(speed);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_HeatPump_PumpedCondenser).size());
  EXPECT_EQ(6u, w.getObjectsByType(IddObjectType::Curve_Quadratic).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Schedule_Constant).size());

  WorkspaceObjectVector idf_coils(w.getObjectsByType(IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed));
  EXPECT_EQ(1u, idf_coils.size());
  WorkspaceObject idf_coil(idf_coils[0]);

  EXPECT_EQ(1, idf_coil.getInt(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::NominalSpeedLevel, false).get());
  EXPECT_EQ(4000.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedWaterHeatingCapacity, false).get());
  EXPECT_EQ(29.44,
            idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirDryBulbTemperature, false).get());
  EXPECT_EQ(22.22,
            idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirWetBulbTemperature, false).get());
  EXPECT_EQ(55.72, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserInletWaterTemperature, false).get());
  EXPECT_EQ("autocalculate", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, false).get());
  EXPECT_EQ("autocalculate", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterFlowRate, false).get());
  EXPECT_EQ("Yes", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorFanPowerIncludedinRatedCOP, false).get());
  EXPECT_EQ("No", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpPowerIncludedinRatedCOP, false).get());
  EXPECT_EQ(
    "No",
    idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP, false)
      .get());
  EXPECT_EQ(0.2, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::FractionofCondenserPumpHeattoWater, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirInletNodeName, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirOutletNodeName, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserWaterInletNodeName, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserWaterOutletNodeName, false).get());
  EXPECT_EQ(0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacity, false).get());
  EXPECT_EQ(
    10,
    idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation, false).get());
  EXPECT_EQ("WetBulbTemperature",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirTemperatureTypeforCurveObjects, false).get());
  boost::optional<WorkspaceObject> idf_curve(
    idf_coil.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::PartLoadFractionCorrelationCurveName));
  ASSERT_TRUE(idf_curve);
  EXPECT_EQ(idf_curve->iddObject().type(), IddObjectType::Curve_Quadratic);
  boost::optional<WorkspaceObject> idf_sch(idf_coil.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::GridSignalScheduleName));
  ASSERT_TRUE(idf_sch);
  EXPECT_EQ(idf_sch->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ(100.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::LowerBoundtoApplyGridResponsiveControl, false).get());
  EXPECT_EQ(-100.0,
            idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::UpperBoundtoApplyGridResponsiveControl, false).get());
  EXPECT_EQ(10, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::MaxSpeedLevelDuringGridResponsiveControl, false).get());
  EXPECT_EQ(1, idf_coil.getInt(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::NumberofSpeeds, false).get());

  EXPECT_EQ(1u, idf_coil.numExtensibleGroups());

  WorkspaceExtensibleGroup w_eg = idf_coil.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(400.0, w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedWaterHeatingCapacityatSpeed, false).get());
  EXPECT_EQ(5.0, w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedWaterHeatingCOPatSpeed, false).get());
  EXPECT_EQ(0.8, w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedSensibleHeatRatioatSpeed, false).get());
  EXPECT_EQ(0.02014,
            w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate, false).get());
  EXPECT_EQ(0.000018,
            w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedWaterFlowRate, false).get());
  EXPECT_EQ(
    10.0,
    w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitWaterPumpInputPowerAtRatedConditions, false)
      .get());
  boost::optional<WorkspaceObject> idf_curve1(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofTemperatureCurveName));
  ASSERT_TRUE(idf_curve1);
  EXPECT_EQ(idf_curve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
  boost::optional<WorkspaceObject> idf_curve2(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofAirFlowFractionCurveName));
  ASSERT_TRUE(idf_curve2);
  EXPECT_EQ(idf_curve2->iddObject().type(), IddObjectType::Curve_Cubic);
  boost::optional<WorkspaceObject> idf_curve3(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofWaterFlowFractionCurveName));
  ASSERT_TRUE(idf_curve3);
  EXPECT_EQ(idf_curve3->iddObject().type(), IddObjectType::Curve_Cubic);
  boost::optional<WorkspaceObject> idf_curve4(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofTemperatureCurveName));
  ASSERT_TRUE(idf_curve4);
  EXPECT_EQ(idf_curve4->iddObject().type(), IddObjectType::Curve_Biquadratic);
  boost::optional<WorkspaceObject> idf_curve5(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofAirFlowFractionCurveName));
  ASSERT_TRUE(idf_curve5);
  EXPECT_EQ(idf_curve5->iddObject().type(), IddObjectType::Curve_Cubic);
  boost::optional<WorkspaceObject> idf_curve6(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofWaterFlowFractionCurveName));
  ASSERT_TRUE(idf_curve6);
  EXPECT_EQ(idf_curve6->iddObject().type(), IddObjectType::Curve_Cubic);
}
