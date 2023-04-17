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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"

#include <utilities/idd/IddObject.hpp>
#include <utilities/idd/AirTerminal_SingleDuct_VAV_NoReheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctVAVNoReheat) {
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  DesignSpecificationOutdoorAir dsoa(m);
  EXPECT_TRUE(s.setDesignSpecificationOutdoorAir(dsoa));

  Schedule sch = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctVAVNoReheat atu(m, sch);
  atu.setName("ATU SingleDuct VAV No Reheat");
  atu.setAvailabilitySchedule(sch);
  atu.setMaximumAirFlowRate(0.1);
  atu.setZoneMinimumAirFlowInputMethod("FixedFlowRate");
  atu.setConstantMinimumAirFlowFraction(0.2);
  atu.setFixedMinimumAirFlowRate(0.3);
  atu.setMinimumAirFlowFractionSchedule(sch);
  atu.setMinimumAirFlowTurndownSchedule(sch);
  atu.setControlForOutdoorAir(true);

  AirLoopHVAC a(m);
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_VAV_NoReheat));
  ASSERT_EQ(1u, idf_atus.size());
  WorkspaceObject idf_atu(idf_atus[0]);

  EXPECT_EQ("ATU SingleDuct VAV No Reheat", idf_atu.nameString());

  boost::optional<WorkspaceObject> woAvailabilitySchedule(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName));
  EXPECT_TRUE(woAvailabilitySchedule);
  EXPECT_EQ(woAvailabilitySchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woAvailabilitySchedule->nameString());

  EXPECT_EQ(atu.outletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::AirOutletNodeName).get());
  EXPECT_EQ(atu.inletModelObject()->nameString(), idf_atu.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::AirInletNodeName).get());

  EXPECT_EQ(0.1, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate).get());
  EXPECT_EQ("FixedFlowRate", idf_atu.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::ZoneMinimumAirFlowInputMethod).get());
  EXPECT_EQ(0.2, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction).get());
  EXPECT_EQ(0.3, idf_atu.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate).get());

  boost::optional<WorkspaceObject> woFractionSchedule(
    idf_atu.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woFractionSchedule);
  EXPECT_EQ(woFractionSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woFractionSchedule->nameString());

  boost::optional<WorkspaceObject> woTurndownSchedule(
    idf_atu.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woTurndownSchedule);
  EXPECT_EQ(woTurndownSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ("Always On Discrete", woTurndownSchedule->nameString());

  boost::optional<WorkspaceObject> idf_dsoa(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::DesignSpecificationOutdoorAirObjectName));
  EXPECT_TRUE(idf_dsoa);
  EXPECT_EQ(idf_dsoa->iddObject().type(), IddObjectType::DesignSpecification_OutdoorAir);
}
