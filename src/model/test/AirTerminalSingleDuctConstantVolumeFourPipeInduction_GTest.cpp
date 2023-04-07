/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "../AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../PortList.hpp"
#include "../PortList_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../ModelExtensibleGroup.hpp"
#include <utilities/idd/OS_PortList_FieldEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_AirTerminalSingleDuctConstantVolumeFourPipeInduction) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule s = m.alwaysOnDiscreteSchedule();
      CoilHeatingWater heatingCoil(m, s);
      AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m, heatingCoil);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m, heatingCoil);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_remove) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingWater coolingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m, heatingCoil);

  testObject.setCoolingCoil(coolingCoil);

  AirLoopHVAC airLoop(m);
  ThermalZone thermalZone(m);
  PlantLoop plantLoop(m);

  // KSB: I don't think it is the greatest idea to test these private methods,
  // but this area has resulted in a simulation error so it needs to be tested
  // (Note: JM 2021-03-02 These aren't private anymore)
  EXPECT_EQ(0u, thermalZone.exhaustPortList().numExtensibleGroups());
  EXPECT_EQ(0u, thermalZone.inletPortList().numExtensibleGroups());

  airLoop.addBranchForZone(thermalZone, testObject);
  plantLoop.addDemandBranchForComponent(heatingCoil);
  plantLoop.addDemandBranchForComponent(coolingCoil);

  ASSERT_EQ(1u, thermalZone.exhaustPortList().numExtensibleGroups());
  EXPECT_TRUE(thermalZone.exhaustPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));
  ASSERT_EQ(1u, thermalZone.inletPortList().numExtensibleGroups());
  EXPECT_TRUE(thermalZone.inletPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));

  EXPECT_EQ((unsigned)10, plantLoop.demandComponents().size());
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  testObject.remove();

  EXPECT_EQ(0u, thermalZone.exhaustPortList().numExtensibleGroups());
  ASSERT_EQ(1u, thermalZone.inletPortList().numExtensibleGroups());
  EXPECT_TRUE(thermalZone.inletPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));

  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_clone) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingWater coolingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m, heatingCoil);

  testObject.setCoolingCoil(coolingCoil);

  AirLoopHVAC airLoop(m);
  ThermalZone thermalZone(m);
  PlantLoop plantLoop(m);

  airLoop.addBranchForZone(thermalZone, testObject);
  plantLoop.addDemandBranchForComponent(heatingCoil);
  plantLoop.addDemandBranchForComponent(coolingCoil);

  auto testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  ASSERT_NO_THROW(testObjectClone.heatingCoil());
  ASSERT_TRUE(testObjectClone.coolingCoil());
  EXPECT_FALSE(testObjectClone.airLoopHVAC());

  EXPECT_NE(testObjectClone.heatingCoil(), testObject.heatingCoil());
  EXPECT_NE(testObjectClone.coolingCoil().get(), testObject.coolingCoil().get());

  Model m2;

  auto testObjectClone2 = testObject.clone(m2).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  ASSERT_NO_THROW(testObjectClone2.heatingCoil());
  ASSERT_TRUE(testObjectClone2.coolingCoil());
  EXPECT_FALSE(testObjectClone.airLoopHVAC());

  EXPECT_NE(testObjectClone2.heatingCoil(), testObject.heatingCoil());
  EXPECT_NE(testObjectClone2.coolingCoil().get(), testObject.coolingCoil().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_connectSecondaryAirInlet_regularCase_2033) {

  // Test for #2033
  // Base case: works fine
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction atu(m, heatingCoil);

  ThermalZone zone(m);
  AirLoopHVAC airLoopHVAC(m);

  EXPECT_FALSE(atu.inducedAirInletNode());
  EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

  // Connect simulateanously the branch and atu
  EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone, atu));

  ASSERT_TRUE(atu.inducedAirInletNode());
  ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
  EXPECT_EQ(atu.inducedAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_connectSecondaryAirInlet_atuFirst_2033) {

  // Test for #2033: When you connect the atu first, then add a zone it should work as well.
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction atu(m, heatingCoil);
  AirLoopHVAC airLoopHVAC(m);

  // Connect atu only first
  airLoopHVAC.addBranchForHVACComponent(atu);
  EXPECT_FALSE(atu.inducedAirInletNode());

  // First zone: this is the problematic case
  {
    ThermalZone zone(m);
    EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

    // Now add zone (this was the problematic case)
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone));
    ASSERT_TRUE(atu.inducedAirInletNode());  // <===== Actual test is here
    ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
    EXPECT_EQ(atu.inducedAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
  }

  // Should work for any zone added after that too
  {
    ThermalZone zone(m);
    EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

    // Now add zone (this was the problematic case)
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone));

    // Get the cloned ATU
    ASSERT_EQ(1u, zone.equipment().size());
    auto _atu = zone.equipment()[0].optionalCast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
    ASSERT_TRUE(_atu);

    ASSERT_TRUE(_atu->inducedAirInletNode());
    ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
    EXPECT_EQ(_atu->inducedAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
  }
}
