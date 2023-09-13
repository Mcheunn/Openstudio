/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/LoadProfilePlant.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/LoadProfile_Plant_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_LoadProfilePlant) {
  Model m;

  PlantLoop pl(m);
  LoadProfilePlant lpp(m);
  EXPECT_TRUE(pl.addDemandBranchForComponent(lpp));

  ScheduleConstant loadSchedule(m);
  loadSchedule.setName("Load Schedule");
  EXPECT_TRUE(lpp.setLoadSchedule(loadSchedule));

  EXPECT_TRUE(lpp.setPeakFlowRate(0.005));

  ScheduleConstant flowRateFractionSchedule(m);
  flowRateFractionSchedule.setName("Flow Rate Fraction Schedule");
  EXPECT_TRUE(lpp.setFlowRateFractionSchedule(flowRateFractionSchedule));

  EXPECT_TRUE(lpp.setPlantLoopFluidType("Steam"));
  EXPECT_TRUE(lpp.setDegreeofSubCooling(6.0));
  EXPECT_TRUE(lpp.setDegreeofLoopSubCooling(21.0));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::LoadProfile_Plant).size());

  IdfObject idf_lpp = w.getObjectsByType(IddObjectType::LoadProfile_Plant)[0];

  EXPECT_EQ(lpp.nameString(), idf_lpp.getString(LoadProfile_PlantFields::Name, false).get());
  EXPECT_FALSE(idf_lpp.isEmpty(LoadProfile_PlantFields::InletNodeName));
  EXPECT_FALSE(idf_lpp.isEmpty(LoadProfile_PlantFields::OutletNodeName));
  EXPECT_EQ(loadSchedule.nameString(), idf_lpp.getString(LoadProfile_PlantFields::LoadScheduleName, false).get());
  EXPECT_EQ(0.005, idf_lpp.getDouble(LoadProfile_PlantFields::PeakFlowRate, false).get());
  EXPECT_EQ(flowRateFractionSchedule.nameString(), idf_lpp.getString(LoadProfile_PlantFields::FlowRateFractionScheduleName, false).get());
  EXPECT_EQ("Steam", idf_lpp.getString(LoadProfile_PlantFields::PlantLoopFluidType, false).get());
  EXPECT_EQ(6.0, idf_lpp.getDouble(LoadProfile_PlantFields::DegreeofSubCooling, false).get());
  EXPECT_EQ(21.0, idf_lpp.getDouble(LoadProfile_PlantFields::DegreeofLoopSubCooling, false).get());
}
