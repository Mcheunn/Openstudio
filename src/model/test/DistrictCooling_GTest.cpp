/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../DistrictCooling.hpp"
#include "../DistrictCooling_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

//test construction of the object
TEST_F(ModelFixture, DistrictCooling_GettersSetters) {

  Model m;
  DistrictCooling districtCooling(m);

  // Default to autosize
  EXPECT_FALSE(districtCooling.nominalCapacity());
  EXPECT_TRUE(districtCooling.isNominalCapacityAutosized());
  // Set it
  districtCooling.setNominalCapacity(1000.0);
  ASSERT_TRUE(districtCooling.nominalCapacity());
  EXPECT_EQ(1000.0, districtCooling.nominalCapacity().get());
  EXPECT_FALSE(districtCooling.isNominalCapacityAutosized());
  // autosize
  districtCooling.autosizeNominalCapacity();
  EXPECT_TRUE(districtCooling.isNominalCapacityAutosized());
  EXPECT_FALSE(districtCooling.nominalCapacity());

  EXPECT_EQ(m.alwaysOnContinuousSchedule(), districtCooling.capacityFractionSchedule());
  ScheduleConstant scheduleConstant(m);
  EXPECT_TRUE(districtCooling.setCapacityFractionSchedule(scheduleConstant));
  EXPECT_EQ(scheduleConstant, districtCooling.capacityFractionSchedule());
}

//test connecting the object to a loop and get the inlet node and the outlet node
TEST_F(ModelFixture, DistrictCooling_connections) {
  Model m;

  //make a plant loop
  PlantLoop plantLoop(m);

  //make a DistrictCooling object
  DistrictCooling districtCooling(m);

  //get the supply outlet node of the plant loop
  Node plantOutletNode = plantLoop.supplyOutletNode();

  //hook the DistrictCooling object to the supply outlet node
  ASSERT_TRUE(districtCooling.addToNode(plantOutletNode));

  //it should now be on a loop and have inlet and outlet objects
  ASSERT_TRUE(districtCooling.loop());
  ASSERT_TRUE(districtCooling.inletModelObject());
  ASSERT_TRUE(districtCooling.outletModelObject());

  //it should be removable from the loop
  ASSERT_TRUE(districtCooling.isRemovable());

  //now, disconnect the object
  districtCooling.disconnect();

  //it should no longer have a loop or inlet/outlet objects
  ASSERT_FALSE(districtCooling.loop());
  ASSERT_FALSE(districtCooling.inletModelObject());
  ASSERT_FALSE(districtCooling.outletModelObject());

  //make an airloop
  AirLoopHVAC airLoop(m);

  //get the supply outlet node of the airloop
  Node airOutletNode = airLoop.supplyOutletNode();

  //it should not connect to an airloop
  ASSERT_FALSE(districtCooling.addToNode(airOutletNode));
}

//test cloning the object
TEST_F(ModelFixture, DistrictCooling_Clone) {
  Model m;
  //make an object to clone, and edit some property to make sure the clone worked
  DistrictCooling districtCooling(m);

  districtCooling.setNominalCapacity(1234);

  //clone into the same model
  auto districtCoolingClone = districtCooling.clone(m).cast<DistrictCooling>();
  auto capacity = districtCoolingClone.nominalCapacity();
  ASSERT_TRUE(capacity);
  ASSERT_EQ(1234, capacity.get());

  //clone into another model
  Model m2;
  auto districtCoolingClone2 = districtCooling.clone(m2).cast<DistrictCooling>();

  capacity = districtCoolingClone2.nominalCapacity();
  ASSERT_TRUE(capacity);
  ASSERT_EQ(1234, capacity.get());
}

TEST_F(ModelFixture, DistrictCooling_addToNode) {
  Model m;
  DistrictCooling testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<DistrictCooling>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
}
