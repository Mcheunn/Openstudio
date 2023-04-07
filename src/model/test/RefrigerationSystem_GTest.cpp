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

#include "../RefrigerationSystem.hpp"
#include "../RefrigerationSystem_Impl.hpp"
#include "../RefrigerationAirChiller.hpp"
#include "../RefrigerationAirChiller_Impl.hpp"
#include "../RefrigerationCompressor.hpp"
#include "../RefrigerationCompressor_Impl.hpp"
#include "../RefrigerationCondenserAirCooled.hpp"
#include "../RefrigerationCondenserAirCooled_Impl.hpp"
#include "../RefrigerationCondenserEvaporativeCooled.hpp"
#include "../RefrigerationCondenserEvaporativeCooled_Impl.hpp"
#include "../RefrigerationCondenserWaterCooled.hpp"
#include "../RefrigerationCondenserWaterCooled_Impl.hpp"
#include "../RefrigerationCondenserCascade.hpp"
#include "../RefrigerationCondenserCascade_Impl.hpp"
#include "../RefrigerationCase.hpp"
#include "../RefrigerationCase_Impl.hpp"
#include "../RefrigerationWalkIn.hpp"
#include "../RefrigerationWalkIn_Impl.hpp"
#include "../RefrigerationSecondarySystem.hpp"
#include "../RefrigerationSecondarySystem_Impl.hpp"
#include "../RefrigerationSubcoolerLiquidSuction.hpp"
#include "../RefrigerationSubcoolerLiquidSuction_Impl.hpp"
#include "../RefrigerationSubcoolerMechanical.hpp"
#include "../RefrigerationSubcoolerMechanical_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationSystem_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      RefrigerationSystem testObject = RefrigerationSystem(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationSystem_Remove) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
  RefrigerationSubcoolerMechanical mechSubcooler = RefrigerationSubcoolerMechanical(model);
  RefrigerationSubcoolerLiquidSuction liqSuctionSubcool = RefrigerationSubcoolerLiquidSuction(model);

  testObject.setRefrigerationCondenser(condenser);
  testObject.setMechanicalSubcooler(mechSubcooler);
  testObject.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubcool);

  std::vector<RefrigerationSystem> refrigerationSystems = model.getConcreteModelObjects<RefrigerationSystem>();
  EXPECT_EQ(1, refrigerationSystems.size());

  std::vector<ModelObjectList> refrigerationModelObjectLists = model.getConcreteModelObjects<ModelObjectList>();
  EXPECT_EQ(4, refrigerationModelObjectLists.size());

  std::vector<RefrigerationCondenserAirCooled> refrigerationCondensers = model.getConcreteModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondensers.size());

  std::vector<RefrigerationSubcoolerMechanical> refrigerationSubcoolerMechanicals = model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, refrigerationSubcoolerMechanicals.size());

  std::vector<RefrigerationSubcoolerLiquidSuction> refrigerationSubcoolerLiquidSuctions =
    model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, refrigerationSubcoolerLiquidSuctions.size());

  testObject.remove();

  refrigerationSystems = model.getConcreteModelObjects<RefrigerationSystem>();
  EXPECT_EQ(0, refrigerationSystems.size());

  refrigerationModelObjectLists = model.getConcreteModelObjects<ModelObjectList>();
  EXPECT_EQ(0, refrigerationModelObjectLists.size());

  refrigerationCondensers = model.getConcreteModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(0, refrigerationCondensers.size());

  refrigerationSubcoolerMechanicals = model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(0, refrigerationSubcoolerMechanicals.size());

  refrigerationSubcoolerLiquidSuctions = model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(0, refrigerationSubcoolerLiquidSuctions.size());
}

TEST_F(ModelFixture, RefrigerationSystem_CloneOneModelWithDefaultData) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  auto testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_EQ("R404a", testObjectClone.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("ConstantSuctionTemperature", testObjectClone.suctionTemperatureControlType());
  EXPECT_DOUBLE_EQ(21.0, testObjectClone.minimumCondensingTemperature());
  EXPECT_TRUE(testObjectClone.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.sumUASuctionPiping());
}

TEST_F(ModelFixture, RefrigerationSystem_CloneOneModelWithCustomData) {
  Model model;
  ThermalZone thermalZone(model);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationCompressor compressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor highStageCompressor1 = RefrigerationCompressor(model);
  RefrigerationCondenserCascade condenser1 = RefrigerationCondenserCascade(model);
  RefrigerationSubcoolerMechanical mechSubcooler = RefrigerationSubcoolerMechanical(model);
  RefrigerationSubcoolerLiquidSuction liqSuctionSubcool = RefrigerationSubcoolerLiquidSuction(model);

  RefrigerationSystem testObject = RefrigerationSystem(model);
  testObject.setMinimumCondensingTemperature(999.0);
  testObject.setRefrigerationSystemWorkingFluidType("R410a");
  testObject.setSuctionTemperatureControlType("FloatSuctionTemperature");
  testObject.setSumUASuctionPiping(999.0);
  testObject.setSuctionPipingZone(thermalZone);
  testObject.addCase(case1);
  testObject.addWalkin(walkin1);
  testObject.addCompressor(compressor1);
  testObject.addHighStageCompressor(highStageCompressor1);
  testObject.setRefrigerationCondenser(condenser1);
  testObject.setMechanicalSubcooler(mechSubcooler);
  testObject.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubcool);

  std::vector<RefrigerationCase> _cases = testObject.cases();
  std::vector<RefrigerationWalkIn> _walkins = testObject.walkins();
  std::vector<RefrigerationCompressor> _compressors = testObject.compressors();
  std::vector<RefrigerationCompressor> _highStageCompressors = testObject.highStageCompressors();

  auto testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.minimumCondensingTemperature());
  EXPECT_FALSE(testObjectClone.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.sumUASuctionPiping());
  EXPECT_EQ("R410a", testObjectClone.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("FloatSuctionTemperature", testObjectClone.suctionTemperatureControlType());
  EXPECT_FALSE(testObjectClone.suctionPipingZone());
  std::vector<RefrigerationCase> casesClone = testObjectClone.cases();
  EXPECT_EQ(1, casesClone.size());
  EXPECT_NE(casesClone[0].handle(), _cases[0].handle());
  std::vector<RefrigerationWalkIn> walkinsClone = testObjectClone.walkins();
  EXPECT_EQ(1, walkinsClone.size());
  EXPECT_NE(walkinsClone[0].handle(), _walkins[0].handle());
  std::vector<RefrigerationCompressor> compressorsClone = testObjectClone.compressors();
  EXPECT_EQ(1, compressorsClone.size());
  EXPECT_NE(compressorsClone[0].handle(), _compressors[0].handle());
  std::vector<RefrigerationCompressor> highStageCompressorsClone = testObjectClone.highStageCompressors();
  EXPECT_EQ(1, highStageCompressorsClone.size());
  EXPECT_NE(highStageCompressorsClone[0].handle(), _highStageCompressors[0].handle());
  EXPECT_NE(testObjectClone.refrigerationCondenser().get().handle(), condenser1.handle());
  EXPECT_NE(testObjectClone.mechanicalSubcooler().get().handle(), mechSubcooler.handle());
  EXPECT_NE(testObjectClone.liquidSuctionHeatExchangerSubcooler().get().handle(), liqSuctionSubcool.handle());
}

TEST_F(ModelFixture, RefrigerationSystem_CloneTwoModelsWithDefaultData) {
  Model model;
  ThermalZone thermalZone(model);
  RefrigerationSystem testObject = RefrigerationSystem(model);

  auto testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();

  Model model2;

  auto testObjectClone2 = testObject.clone(model2).cast<RefrigerationSystem>();
  EXPECT_EQ("R404a", testObjectClone2.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("ConstantSuctionTemperature", testObjectClone2.suctionTemperatureControlType());
  EXPECT_DOUBLE_EQ(21.0, testObjectClone2.minimumCondensingTemperature());
  EXPECT_TRUE(testObjectClone2.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.sumUASuctionPiping());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture, RefrigerationSystem_CloneTwoModelWithCustomData) {
  Model model;
  ThermalZone thermalZone(model);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationCompressor compressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor highStageCompressor1 = RefrigerationCompressor(model);
  RefrigerationCondenserCascade condenser1 = RefrigerationCondenserCascade(model);
  RefrigerationSubcoolerMechanical mechSubcooler = RefrigerationSubcoolerMechanical(model);
  RefrigerationSubcoolerLiquidSuction liqSuctionSubcooler = RefrigerationSubcoolerLiquidSuction(model);

  RefrigerationSystem testObject = RefrigerationSystem(model);
  testObject.setMinimumCondensingTemperature(999.0);
  testObject.setRefrigerationSystemWorkingFluidType("R410a");
  testObject.setSuctionTemperatureControlType("FloatSuctionTemperature");
  testObject.setSumUASuctionPiping(999.0);
  testObject.setSuctionPipingZone(thermalZone);
  testObject.addCase(case1);
  testObject.addWalkin(walkin1);
  testObject.addCompressor(compressor1);
  testObject.addHighStageCompressor(highStageCompressor1);
  testObject.setRefrigerationCondenser(condenser1);
  testObject.setMechanicalSubcooler(mechSubcooler);
  testObject.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubcooler);

  std::vector<RefrigerationCase> _cases = testObject.cases();
  std::vector<RefrigerationWalkIn> _walkins = testObject.walkins();
  std::vector<RefrigerationCompressor> _compressors = testObject.compressors();
  std::vector<RefrigerationCompressor> _highStageCompressors = testObject.highStageCompressors();

  auto testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();

  Model model2;
  auto testObjectClone2 = testObject.clone(model2).cast<RefrigerationSystem>();

  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.minimumCondensingTemperature());
  EXPECT_FALSE(testObjectClone2.isSumUASuctionPipingDefaulted());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.sumUASuctionPiping());
  EXPECT_EQ("R410a", testObjectClone2.refrigerationSystemWorkingFluidType());
  EXPECT_EQ("FloatSuctionTemperature", testObjectClone2.suctionTemperatureControlType());
  EXPECT_FALSE(testObjectClone2.suctionPipingZone());
  std::vector<RefrigerationCase> casesClone = testObjectClone2.cases();
  EXPECT_EQ(1, casesClone.size());
  EXPECT_NE(casesClone[0].handle(), _cases[0].handle());
  std::vector<RefrigerationWalkIn> walkinsClone = testObjectClone2.walkins();
  EXPECT_EQ(1, walkinsClone.size());
  EXPECT_NE(walkinsClone[0].handle(), _walkins[0].handle());
  std::vector<RefrigerationCompressor> compressorsClone = testObjectClone2.compressors();
  EXPECT_EQ(1, compressorsClone.size());
  EXPECT_NE(compressorsClone[0].handle(), _compressors[0].handle());
  std::vector<RefrigerationCompressor> highStageCompressorsClone = testObjectClone2.highStageCompressors();
  EXPECT_EQ(1, highStageCompressorsClone.size());
  EXPECT_NE(highStageCompressorsClone[0].handle(), _highStageCompressors[0].handle());
  EXPECT_NE(testObjectClone2.refrigerationCondenser().get().handle(), condenser1.handle());
  EXPECT_NE(testObjectClone2.mechanicalSubcooler().get().handle(), mechSubcooler.handle());
  EXPECT_NE(testObjectClone2.liquidSuctionHeatExchangerSubcooler().get().handle(), liqSuctionSubcooler.handle());
}

TEST_F(ModelFixture, RefrigerationSystem_MechanicalSubcooler) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationSubcoolerMechanical mechSubcooler = RefrigerationSubcoolerMechanical(model);

  EXPECT_FALSE(testObject.mechanicalSubcooler());
  EXPECT_TRUE(testObject.setMechanicalSubcooler(mechSubcooler));
  EXPECT_TRUE(testObject.mechanicalSubcooler());

  std::vector<RefrigerationSubcoolerMechanical> testMechanicalSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, testMechanicalSubcoolers.size());

  auto testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_NE(testObjectClone.mechanicalSubcooler().get().handle(), mechSubcooler.handle());

  testMechanicalSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(2, testMechanicalSubcoolers.size());

  Model model2;
  auto testObjectClone2 = testObject.clone(model2).cast<RefrigerationSystem>();

  testMechanicalSubcoolers = model2.getConcreteModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, testMechanicalSubcoolers.size());

  EXPECT_NE(testObjectClone2.mechanicalSubcooler().get().handle(), mechSubcooler.handle());

  mechSubcooler.remove();

  EXPECT_FALSE(testObject.mechanicalSubcooler());
  EXPECT_TRUE(testObjectClone.mechanicalSubcooler());
  EXPECT_TRUE(testObjectClone2.mechanicalSubcooler());

  testMechanicalSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, testMechanicalSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSystem_RefrigerationSubcoolerLiquidSuction) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationSubcoolerLiquidSuction liqSuctionSubcooler = RefrigerationSubcoolerLiquidSuction(model);

  EXPECT_FALSE(testObject.liquidSuctionHeatExchangerSubcooler());
  EXPECT_TRUE(testObject.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubcooler));
  EXPECT_TRUE(testObject.liquidSuctionHeatExchangerSubcooler());

  std::vector<RefrigerationSubcoolerLiquidSuction> testLiqSuctionSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, testLiqSuctionSubcoolers.size());

  auto testObjectClone = testObject.clone(model).cast<RefrigerationSystem>();
  EXPECT_NE(testObjectClone.liquidSuctionHeatExchangerSubcooler().get().handle(), liqSuctionSubcooler.handle());

  testLiqSuctionSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(2, testLiqSuctionSubcoolers.size());

  Model model2;
  auto testObjectClone2 = testObject.clone(model2).cast<RefrigerationSystem>();

  testLiqSuctionSubcoolers = model2.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, testLiqSuctionSubcoolers.size());

  EXPECT_NE(testObjectClone2.liquidSuctionHeatExchangerSubcooler().get().handle(), liqSuctionSubcooler.handle());

  liqSuctionSubcooler.remove();

  EXPECT_FALSE(testObject.liquidSuctionHeatExchangerSubcooler());
  EXPECT_TRUE(testObjectClone.liquidSuctionHeatExchangerSubcooler());
  EXPECT_TRUE(testObjectClone2.liquidSuctionHeatExchangerSubcooler());

  testLiqSuctionSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>();
  EXPECT_EQ(1, testLiqSuctionSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSystem_Compressors) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  std::vector<RefrigerationCompressor> compressors = testObject.compressors();
  EXPECT_TRUE(compressors.empty());
}

TEST_F(ModelFixture, RefrigerationSystem_AddCompressor) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor = RefrigerationCompressor(model);

  EXPECT_TRUE(testObject.addCompressor(testCompressor));

  std::vector<RefrigerationCompressor> compressors = testObject.compressors();
  EXPECT_EQ(1, compressors.size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCompressor) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addCompressor(testCompressor1);
  testObject.addCompressor(testCompressor2);
  boost::optional<ModelObjectList> compressorList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->compressorList();
  ASSERT_TRUE(compressorList);
  EXPECT_EQ(2u, compressorList->size());

  testObject.removeCompressor(testCompressor1);
  EXPECT_EQ(1u, compressorList->size());

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getConcreteModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> compressors = testObject.compressors();
  EXPECT_EQ(1, compressors.size());
  EXPECT_EQ(testCompressor2.handle(), compressors[0].handle());

  testObject.removeCompressor(testCompressor2);
  testRefrigerationCompressors = model.getConcreteModelObjects<RefrigerationCompressor>();
  compressors = testObject.compressors();
  EXPECT_EQ(2, testRefrigerationCompressors.size());
  EXPECT_TRUE(compressors.empty());
  EXPECT_EQ(0u, compressorList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllCompressors) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addCompressor(testCompressor1);
  testObject.addCompressor(testCompressor2);
  boost::optional<ModelObjectList> compressorList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->compressorList();
  ASSERT_TRUE(compressorList);
  EXPECT_EQ(2u, compressorList->size());

  testObject.removeAllCompressors();

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getConcreteModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> compressors = testObject.compressors();
  EXPECT_TRUE(compressors.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->compressorList());
  EXPECT_EQ(0u, compressorList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCompressor_Child) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);

  testObject.addCompressor(testCompressor1);
  boost::optional<ModelObjectList> compressorList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->compressorList();
  ASSERT_TRUE(compressorList);
  EXPECT_EQ(1u, compressorList->size());

  // Remove child directly, not calling testObject.removeCompressor
  testCompressor1.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<RefrigerationCompressor>().size());
  EXPECT_TRUE(testObject.compressors().empty());
  // ModelObjectList should not have extensible groups anymore
  EXPECT_EQ(0u, compressorList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_HighStageCompressors) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  std::vector<RefrigerationCompressor> highStageCompressors = testObject.highStageCompressors();
  EXPECT_TRUE(highStageCompressors.empty());
}

TEST_F(ModelFixture, RefrigerationSystem_AddHighStageCompressor) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor = RefrigerationCompressor(model);

  EXPECT_TRUE(testObject.addHighStageCompressor(testCompressor));

  std::vector<RefrigerationCompressor> highStageCompressors = testObject.highStageCompressors();
  EXPECT_EQ(1, highStageCompressors.size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveHighStageCompressor) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addHighStageCompressor(testCompressor1);
  testObject.addHighStageCompressor(testCompressor2);
  boost::optional<ModelObjectList> highStageCompressorList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->highStageCompressorList();
  ASSERT_TRUE(highStageCompressorList);
  EXPECT_EQ(2u, highStageCompressorList->size());

  testObject.removeHighStageCompressor(testCompressor1);
  EXPECT_EQ(1u, highStageCompressorList->size());

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getConcreteModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> highStageCompressors = testObject.highStageCompressors();
  EXPECT_EQ(1, highStageCompressors.size());
  EXPECT_EQ(testCompressor2.handle(), highStageCompressors[0].handle());

  testObject.removeHighStageCompressor(testCompressor2);
  testRefrigerationCompressors = model.getConcreteModelObjects<RefrigerationCompressor>();
  highStageCompressors = testObject.highStageCompressors();
  EXPECT_EQ(2, testRefrigerationCompressors.size());
  EXPECT_TRUE(highStageCompressors.empty());
  EXPECT_EQ(0u, highStageCompressorList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllHighStageCompressors) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);
  RefrigerationCompressor testCompressor2 = RefrigerationCompressor(model);

  testObject.addHighStageCompressor(testCompressor1);
  testObject.addHighStageCompressor(testCompressor2);
  boost::optional<ModelObjectList> highStageCompressorList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->highStageCompressorList();
  ASSERT_TRUE(highStageCompressorList);
  EXPECT_EQ(2u, highStageCompressorList->size());

  testObject.removeAllHighStageCompressors();
  EXPECT_EQ(0u, highStageCompressorList->size());

  std::vector<RefrigerationCompressor> testRefrigerationCompressors = model.getConcreteModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(2, testRefrigerationCompressors.size());

  std::vector<RefrigerationCompressor> highStageCompressors = testObject.highStageCompressors();
  EXPECT_TRUE(highStageCompressors.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->highStageCompressorList());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveHighStageCompressor_Child) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCompressor testCompressor1 = RefrigerationCompressor(model);

  testObject.addHighStageCompressor(testCompressor1);
  boost::optional<ModelObjectList> highStageCompressorList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->highStageCompressorList();
  ASSERT_TRUE(highStageCompressorList);
  EXPECT_EQ(1u, highStageCompressorList->size());

  // Remove child directly, not calling testObject.removeHighStageCompressor
  testCompressor1.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<RefrigerationCompressor>().size());
  EXPECT_TRUE(testObject.highStageCompressors().empty());
  // ModelObjectList should not have extensible groups anymore
  EXPECT_EQ(0u, highStageCompressorList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_Cases) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addCase(case1);
  cases = testObject.cases();
  EXPECT_EQ(1, cases.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddCase) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  EXPECT_TRUE(testObject.addCase(case1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCase) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addCase(case1);
  testObject.addCase(case2);
  boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedCaseAndWalkInList);
  EXPECT_EQ(2u, refrigeratedCaseAndWalkInList->size());

  testObject.removeCase(case1);
  EXPECT_EQ(1u, refrigeratedCaseAndWalkInList->size());

  std::vector<RefrigerationCase> testRefrigerationCases = model.getConcreteModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_EQ(1, cases.size());
  EXPECT_EQ(case2.handle(), cases[0].handle());

  testObject.removeCase(case2);
  testRefrigerationCases = model.getConcreteModelObjects<RefrigerationCase>();
  cases = testObject.cases();
  EXPECT_EQ(2, testRefrigerationCases.size());
  EXPECT_TRUE(cases.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllCases) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addCase(case1);
  testObject.addCase(case2);
  boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedCaseAndWalkInList);
  EXPECT_EQ(2u, refrigeratedCaseAndWalkInList->size());

  testObject.removeAllCases();

  std::vector<RefrigerationCase> testRefrigerationCases = model.getConcreteModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCase_Child) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);

  testObject.addCase(case1);
  boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedCaseAndWalkInList);
  EXPECT_EQ(1u, refrigeratedCaseAndWalkInList->size());

  // Remove child directly, not calling testObject.removeCase
  case1.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<RefrigerationCase>().size());
  EXPECT_TRUE(testObject.cases().empty());
  // ModelObjectList should not have extensible groups anymore
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_AirChillers) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);

  std::vector<RefrigerationAirChiller> airChillers = testObject.airChillers();
  EXPECT_TRUE(airChillers.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addAirChiller(airChiller1);
  airChillers = testObject.airChillers();
  EXPECT_EQ(1, airChillers.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddAirChiller) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);

  EXPECT_TRUE(testObject.addAirChiller(airChiller1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAirChiller) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);
  RefrigerationAirChiller airChiller2 = RefrigerationAirChiller(model, s1);

  testObject.addAirChiller(airChiller1);
  testObject.addAirChiller(airChiller2);
  boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedCaseAndWalkInList);
  EXPECT_EQ(2u, refrigeratedCaseAndWalkInList->size());

  testObject.removeAirChiller(airChiller1);
  EXPECT_EQ(1u, refrigeratedCaseAndWalkInList->size());

  std::vector<RefrigerationAirChiller> testRefrigerationAirChillers = model.getConcreteModelObjects<RefrigerationAirChiller>();
  EXPECT_EQ(2, testRefrigerationAirChillers.size());

  std::vector<RefrigerationAirChiller> airChillers = testObject.airChillers();
  EXPECT_EQ(1, airChillers.size());
  EXPECT_EQ(airChiller2.handle(), airChillers[0].handle());

  testObject.removeAirChiller(airChiller2);
  testRefrigerationAirChillers = model.getConcreteModelObjects<RefrigerationAirChiller>();
  airChillers = testObject.airChillers();
  EXPECT_EQ(2, testRefrigerationAirChillers.size());
  EXPECT_TRUE(airChillers.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllAirChillers) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);
  RefrigerationAirChiller airChiller2 = RefrigerationAirChiller(model, s1);

  testObject.addAirChiller(airChiller1);
  testObject.addAirChiller(airChiller2);
  boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedCaseAndWalkInList);
  EXPECT_EQ(2u, refrigeratedCaseAndWalkInList->size());

  testObject.removeAllAirChillers();

  std::vector<RefrigerationAirChiller> testRefrigerationAirChillers = model.getConcreteModelObjects<RefrigerationAirChiller>();
  EXPECT_EQ(2, testRefrigerationAirChillers.size());

  std::vector<RefrigerationAirChiller> airChillers = testObject.airChillers();
  EXPECT_TRUE(airChillers.empty());
  EXPECT_NO_THROW(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAirChiller_Child) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller1 = RefrigerationAirChiller(model, s1);

  testObject.addAirChiller(airChiller1);
  boost::optional<ModelObjectList> refrigeratedAirChillerAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedAirChillerAndWalkInList);
  EXPECT_EQ(1u, refrigeratedAirChillerAndWalkInList->size());

  // Remove child directly, not calling testObject.removeAirChiller
  airChiller1.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<RefrigerationAirChiller>().size());
  EXPECT_TRUE(testObject.airChillers().empty());
  // ModelObjectList should not have extensible groups anymore
  EXPECT_EQ(0u, refrigeratedAirChillerAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_Walkins) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addWalkin(walkin1);
  walkins = testObject.walkins();
  EXPECT_EQ(1, walkins.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddWalkIn) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  EXPECT_TRUE(testObject.addWalkin(walkin1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveWalkIn) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addWalkin(walkin1);
  testObject.addWalkin(walkin2);
  boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedCaseAndWalkInList);
  EXPECT_EQ(2u, refrigeratedCaseAndWalkInList->size());

  testObject.removeWalkin(walkin1);
  EXPECT_EQ(1u, refrigeratedCaseAndWalkInList->size());

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getConcreteModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_EQ(1, walkins.size());
  EXPECT_EQ(walkin2.handle(), walkins[0].handle());

  testObject.removeWalkin(walkin2);
  testRefrigerationWalkIns = model.getConcreteModelObjects<RefrigerationWalkIn>();
  walkins = testObject.walkins();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  EXPECT_TRUE(walkins.empty());
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllWalkIns) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addWalkin(walkin1);
  testObject.addWalkin(walkin2);
  testObject.removeAllWalkins();

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getConcreteModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedCaseAndWalkInList);
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveWalkIn_Child) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  testObject.addWalkin(walkin1);
  boost::optional<ModelObjectList> refrigeratedWalkInAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedWalkInAndWalkInList);
  EXPECT_EQ(1u, refrigeratedWalkInAndWalkInList->size());

  // Remove child directly, not calling testObject.removeWalkIn
  walkin1.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<RefrigerationWalkIn>().size());
  EXPECT_TRUE(testObject.cases().empty());
  // ModelObjectList should not have extensible groups anymore
  EXPECT_EQ(0u, refrigeratedWalkInAndWalkInList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_CasesAndWalkins) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());

  testObject.addWalkin(walkin1);
  walkins = testObject.walkins();
  testObject.addCase(case1);
  cases = testObject.cases();
  EXPECT_EQ(1, walkins.size());
  EXPECT_EQ(1, cases.size());
  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCaseAndWalkIn) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);
  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);

  testObject.addWalkin(walkin1);
  testObject.addWalkin(walkin2);
  testObject.removeWalkin(walkin1);

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getConcreteModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_EQ(1, walkins.size());
  EXPECT_EQ(walkin2.handle(), walkins[0].handle());

  testObject.addCase(case1);
  testObject.addCase(case2);
  testObject.removeCase(case1);

  std::vector<RefrigerationCase> testRefrigerationCases = model.getConcreteModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_EQ(1, cases.size());
  EXPECT_EQ(case2.handle(), cases[0].handle());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());

  testObject.removeWalkin(walkin2);
  testRefrigerationWalkIns = model.getConcreteModelObjects<RefrigerationWalkIn>();
  walkins = testObject.walkins();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  EXPECT_TRUE(walkins.empty());

  testObject.removeCase(case2);
  testRefrigerationCases = model.getConcreteModelObjects<RefrigerationCase>();
  cases = testObject.cases();
  EXPECT_EQ(2, testRefrigerationCases.size());
  EXPECT_TRUE(cases.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
  EXPECT_EQ(0u, modelObjectList.size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllCasesAndWalkIns) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1 = RefrigerationCase(model, s1);
  RefrigerationCase case2 = RefrigerationCase(model, s1);
  ScheduleCompact wds(model);
  RefrigerationWalkIn walkin1 = RefrigerationWalkIn(model, wds);
  RefrigerationWalkIn walkin2 = RefrigerationWalkIn(model, wds);

  testObject.addWalkin(walkin1);
  testObject.addWalkin(walkin2);
  testObject.addCase(case1);
  testObject.addCase(case2);

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(4, modelObjectList.modelObjects().size());

  testObject.removeAllCases();
  testObject.removeAllWalkins();

  std::vector<RefrigerationWalkIn> testRefrigerationWalkIns = model.getConcreteModelObjects<RefrigerationWalkIn>();
  EXPECT_EQ(2, testRefrigerationWalkIns.size());
  std::vector<RefrigerationCase> testRefrigerationCases = model.getConcreteModelObjects<RefrigerationCase>();
  EXPECT_EQ(2, testRefrigerationCases.size());

  std::vector<RefrigerationWalkIn> walkins = testObject.walkins();
  EXPECT_TRUE(walkins.empty());
  std::vector<RefrigerationCase> cases = testObject.cases();
  EXPECT_TRUE(cases.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
  EXPECT_EQ(0u, modelObjectList.size());
}

TEST_F(ModelFixture, RefrigerationSystem_SecondarySystemLoads) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_TRUE(secondarySystemLoads.empty());

  boost::optional<ModelObjectList> transferLoadList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
  ASSERT_TRUE(transferLoadList);
  EXPECT_EQ(0u, transferLoadList->size());

  testObject.addSecondarySystemLoad(secondarySystem1);
  secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(1, secondarySystemLoads.size());
  EXPECT_EQ(1u, transferLoadList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_AddSecondarySystemLoad) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);

  EXPECT_TRUE(testObject.addSecondarySystemLoad(secondarySystem1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveSecondarySystemLoad) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);
  RefrigerationSecondarySystem secondarySystem2 = RefrigerationSecondarySystem(model);

  testObject.addSecondarySystemLoad(secondarySystem1);
  testObject.addSecondarySystemLoad(secondarySystem2);
  boost::optional<ModelObjectList> transferLoadList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
  ASSERT_TRUE(transferLoadList);
  EXPECT_EQ(2u, transferLoadList->size());

  testObject.removeSecondarySystemLoad(secondarySystem1);
  EXPECT_EQ(1u, transferLoadList->size());

  std::vector<RefrigerationSecondarySystem> testRefrigerationSecondarySystems = model.getConcreteModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(1, secondarySystemLoads.size());
  EXPECT_EQ(secondarySystem2.handle(), secondarySystemLoads[0].handle());

  testObject.removeSecondarySystemLoad(secondarySystem2);
  testRefrigerationSecondarySystems = model.getConcreteModelObjects<RefrigerationSecondarySystem>();
  secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());
  EXPECT_TRUE(secondarySystemLoads.empty());
  EXPECT_EQ(0u, transferLoadList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllSecondarySystemLoads) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);
  RefrigerationSecondarySystem secondarySystem2 = RefrigerationSecondarySystem(model);

  testObject.addSecondarySystemLoad(secondarySystem1);
  testObject.addSecondarySystemLoad(secondarySystem2);
  boost::optional<ModelObjectList> transferLoadList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
  ASSERT_TRUE(transferLoadList);
  EXPECT_EQ(2u, transferLoadList->size());

  testObject.removeAllSecondarySystemLoads();

  std::vector<RefrigerationSecondarySystem> testRefrigerationSecondarySystems = model.getConcreteModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_TRUE(secondarySystemLoads.empty());
  EXPECT_EQ(0u, transferLoadList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveSecondarySystemLoad_Child) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);

  testObject.addSecondarySystemLoad(secondarySystem1);
  boost::optional<ModelObjectList> transferLoadList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
  ASSERT_TRUE(transferLoadList);
  EXPECT_EQ(1u, transferLoadList->size());

  // Remove child directly, not calling testObject.removeSecondarySystemLoad
  secondarySystem1.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<RefrigerationSecondarySystem>().size());
  EXPECT_TRUE(testObject.secondarySystemLoads().empty());
  // ModelObjectList should not have extensible groups anymore
  EXPECT_EQ(0u, transferLoadList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_CascadeCondenserLoads) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());

  testObject.addCascadeCondenserLoad(condenserCascade1);
  cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(1, cascadeCondenserLoads.size());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
}

TEST_F(ModelFixture, RefrigerationSystem_AddCascadeCondenserLoad) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);

  EXPECT_TRUE(testObject.addCascadeCondenserLoad(condenserCascade1));
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveCascadeCondenserLoad) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationCondenserCascade condenserCascade2 = RefrigerationCondenserCascade(model);

  testObject.addCascadeCondenserLoad(condenserCascade1);
  testObject.addCascadeCondenserLoad(condenserCascade2);
  boost::optional<ModelObjectList> transferLoadList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
  ASSERT_TRUE(transferLoadList);
  EXPECT_EQ(2u, transferLoadList->size());

  testObject.removeCascadeCondenserLoad(condenserCascade1);
  EXPECT_EQ(1u, transferLoadList->size());

  std::vector<RefrigerationCondenserCascade> testCascadeCondenserLoads = model.getConcreteModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(1, cascadeCondenserLoads.size());
  EXPECT_EQ(condenserCascade2.handle(), cascadeCondenserLoads[0].handle());

  testObject.removeCascadeCondenserLoad(condenserCascade2);
  testCascadeCondenserLoads = model.getConcreteModelObjects<RefrigerationCondenserCascade>();
  cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  EXPECT_EQ(0u, transferLoadList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllCascadeCondenserLoads) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationCondenserCascade condenserCascade2 = RefrigerationCondenserCascade(model);

  testObject.addCascadeCondenserLoad(condenserCascade1);
  testObject.addCascadeCondenserLoad(condenserCascade2);
  boost::optional<ModelObjectList> transferLoadList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
  ASSERT_TRUE(transferLoadList);
  EXPECT_EQ(2u, transferLoadList->size());

  testObject.removeAllCascadeCondenserLoads();

  std::vector<RefrigerationCondenserCascade> testCascadeCondenserLoads = model.getConcreteModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  EXPECT_EQ(0u, transferLoadList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_CascadeCondenserLoad_Child) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);
  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);

  testObject.addCascadeCondenserLoad(condenserCascade1);
  boost::optional<ModelObjectList> transferLoadList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
  ASSERT_TRUE(transferLoadList);
  EXPECT_EQ(1u, transferLoadList->size());

  // Remove child directly, not calling testObject.removeSecondarySystemLoad
  condenserCascade1.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<RefrigerationSecondarySystem>().size());
  EXPECT_TRUE(testObject.secondarySystemLoads().empty());
  // ModelObjectList should not have extensible groups anymore
  EXPECT_EQ(0u, transferLoadList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_TransferLoads) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_TRUE(secondarySystemLoads.empty());
  EXPECT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());

  testObject.addCascadeCondenserLoad(condenserCascade1);
  cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  testObject.addSecondarySystemLoad(secondarySystem1);
  secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(1, cascadeCondenserLoads.size());
  EXPECT_EQ(1, secondarySystemLoads.size());
  boost::optional<ModelObjectList> transferLoadList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
  ASSERT_TRUE(transferLoadList);
  EXPECT_EQ(2u, transferLoadList->size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveTransferLoads) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationCondenserCascade condenserCascade2 = RefrigerationCondenserCascade(model);
  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);
  RefrigerationSecondarySystem secondarySystem2 = RefrigerationSecondarySystem(model);

  testObject.addSecondarySystemLoad(secondarySystem1);
  testObject.addSecondarySystemLoad(secondarySystem2);
  testObject.removeSecondarySystemLoad(secondarySystem1);

  std::vector<RefrigerationSecondarySystem> testRefrigerationSecondarySystems = model.getConcreteModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());

  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(1, secondarySystemLoads.size());
  EXPECT_EQ(secondarySystem2.handle(), secondarySystemLoads[0].handle());

  testObject.addCascadeCondenserLoad(condenserCascade1);
  testObject.addCascadeCondenserLoad(condenserCascade2);
  testObject.removeCascadeCondenserLoad(condenserCascade1);

  std::vector<RefrigerationCondenserCascade> testCascadeCondenserLoads = model.getConcreteModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(1, cascadeCondenserLoads.size());
  EXPECT_EQ(condenserCascade2.handle(), cascadeCondenserLoads[0].handle());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(2, modelObjectList.modelObjects().size());

  testObject.removeSecondarySystemLoad(secondarySystem2);
  testRefrigerationSecondarySystems = model.getConcreteModelObjects<RefrigerationSecondarySystem>();
  secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_EQ(2, testRefrigerationSecondarySystems.size());
  EXPECT_TRUE(secondarySystemLoads.empty());

  testObject.removeCascadeCondenserLoad(condenserCascade2);
  testCascadeCondenserLoads = model.getConcreteModelObjects<RefrigerationCondenserCascade>();
  cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());
  EXPECT_TRUE(cascadeCondenserLoads.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_RemoveAllTransferLoads) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade1 = RefrigerationCondenserCascade(model);
  RefrigerationCondenserCascade condenserCascade2 = RefrigerationCondenserCascade(model);
  RefrigerationSecondarySystem secondarySystem1 = RefrigerationSecondarySystem(model);
  RefrigerationSecondarySystem secondarySystem2 = RefrigerationSecondarySystem(model);

  testObject.addCascadeCondenserLoad(condenserCascade1);
  testObject.addCascadeCondenserLoad(condenserCascade2);
  testObject.addSecondarySystemLoad(secondarySystem1);
  testObject.addSecondarySystemLoad(secondarySystem2);

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  ModelObjectList modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(4, modelObjectList.modelObjects().size());

  testObject.removeAllCascadeCondenserLoads();
  testObject.removeAllSecondarySystemLoads();

  std::vector<RefrigerationCondenserCascade> testCascadeCondenserLoads = model.getConcreteModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(2, testCascadeCondenserLoads.size());
  std::vector<RefrigerationSecondarySystem> testSecondarySystemLoads = model.getConcreteModelObjects<RefrigerationSecondarySystem>();
  EXPECT_EQ(2, testSecondarySystemLoads.size());

  std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads = testObject.cascadeCondenserLoads();
  EXPECT_TRUE(cascadeCondenserLoads.empty());
  std::vector<RefrigerationSecondarySystem> secondarySystemLoads = testObject.secondarySystemLoads();
  EXPECT_TRUE(secondarySystemLoads.empty());

  ASSERT_TRUE(testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList());
  modelObjectList = testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList().get();
  EXPECT_EQ(0, modelObjectList.modelObjects().size());
}

TEST_F(ModelFixture, RefrigerationSystem_RefrigerationSystemWorkingFluidType) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  EXPECT_EQ("R404a", testObject.refrigerationSystemWorkingFluidType());
  EXPECT_TRUE(testObject.setRefrigerationSystemWorkingFluidType("R410a"));
  EXPECT_TRUE(testObject.setRefrigerationSystemWorkingFluidType("R22"));
  EXPECT_FALSE(testObject.setRefrigerationSystemWorkingFluidType("Not Valid"));
}

TEST_F(ModelFixture, RefrigerationSystem_RefrigerationCondenser) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  RefrigerationCondenserAirCooled condenserAirCooled = RefrigerationCondenserAirCooled(model);
  RefrigerationCondenserEvaporativeCooled condenserEvaporativeCooled = RefrigerationCondenserEvaporativeCooled(model);
  RefrigerationCondenserCascade condenserCascade = RefrigerationCondenserCascade(model);
  RefrigerationCondenserWaterCooled condenserWaterCooled = RefrigerationCondenserWaterCooled(model);

  EXPECT_TRUE(testObject.setRefrigerationCondenser(condenserAirCooled));
  EXPECT_EQ(testObject.refrigerationCondenser().get().handle(), condenserAirCooled.handle());

  EXPECT_TRUE(testObject.setRefrigerationCondenser(condenserEvaporativeCooled));
  EXPECT_EQ(testObject.refrigerationCondenser().get().handle(), condenserEvaporativeCooled.handle());

  EXPECT_TRUE(testObject.setRefrigerationCondenser(condenserCascade));
  EXPECT_EQ(testObject.refrigerationCondenser().get().handle(), condenserCascade.handle());

  EXPECT_TRUE(testObject.setRefrigerationCondenser(condenserWaterCooled));
  EXPECT_EQ(testObject.refrigerationCondenser().get().handle(), condenserWaterCooled.handle());

  RefrigerationCompressor testCompressor = RefrigerationCompressor(model);
  EXPECT_FALSE(testObject.setRefrigerationCondenser(testCompressor));
}

// Test for #3922
TEST_F(ModelFixture, RefrigerationSystem_CasesAndWalkinsList_Unicity) {
  Model model;
  RefrigerationSystem testObject = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase case1(model, s1);
  RefrigerationWalkIn walkin1(model, s1);
  RefrigerationAirChiller airChiller1(model, s1);

  boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList =
    testObject.getImpl<openstudio::model::detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
  ASSERT_TRUE(refrigeratedCaseAndWalkInList);

  EXPECT_TRUE(testObject.walkins().empty());
  EXPECT_TRUE(testObject.cases().empty());
  EXPECT_TRUE(testObject.airChillers().empty());
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());

  // Add the Walkin
  EXPECT_TRUE(testObject.addWalkin(walkin1));
  EXPECT_EQ(1u, testObject.walkins().size());
  EXPECT_EQ(0u, testObject.cases().size());
  EXPECT_EQ(0u, testObject.airChillers().size());
  EXPECT_EQ(1u, refrigeratedCaseAndWalkInList->size());

  // Add the Case: OK, not mixed
  EXPECT_TRUE(testObject.addCase(case1));
  EXPECT_EQ(1u, testObject.walkins().size());
  EXPECT_EQ(1u, testObject.cases().size());
  EXPECT_EQ(0u, testObject.airChillers().size());
  EXPECT_EQ(2u, refrigeratedCaseAndWalkInList->size());

  // Add the AirChiller: NOT OK, mixed
  EXPECT_FALSE(testObject.addAirChiller(airChiller1));
  EXPECT_EQ(1u, testObject.walkins().size());
  EXPECT_EQ(1u, testObject.cases().size());
  EXPECT_EQ(0u, testObject.airChillers().size());
  EXPECT_EQ(2u, refrigeratedCaseAndWalkInList->size());

  testObject.removeAllCases();
  EXPECT_EQ(1u, testObject.walkins().size());
  EXPECT_EQ(0u, testObject.cases().size());
  EXPECT_EQ(0u, testObject.airChillers().size());
  EXPECT_EQ(1u, refrigeratedCaseAndWalkInList->size());

  testObject.removeAllWalkins();
  EXPECT_EQ(0u, testObject.walkins().size());
  EXPECT_EQ(0u, testObject.cases().size());
  EXPECT_EQ(0u, testObject.airChillers().size());
  EXPECT_EQ(0u, refrigeratedCaseAndWalkInList->size());

  // Add the AirChiller: OK, can't be mixed
  EXPECT_TRUE(testObject.addAirChiller(airChiller1));
  EXPECT_EQ(0u, testObject.walkins().size());
  EXPECT_EQ(0u, testObject.cases().size());
  EXPECT_EQ(1u, testObject.airChillers().size());
  EXPECT_EQ(1u, refrigeratedCaseAndWalkInList->size());

  // Add the case/walkin: not ok
  EXPECT_FALSE(testObject.addWalkin(walkin1));
  EXPECT_FALSE(testObject.addCase(case1));
  EXPECT_EQ(0u, testObject.walkins().size());
  EXPECT_EQ(0u, testObject.cases().size());
  EXPECT_EQ(1u, testObject.airChillers().size());
  EXPECT_EQ(1u, refrigeratedCaseAndWalkInList->size());

  // A second one: OK
  RefrigerationAirChiller airChiller2(model, s1);
  // Add the AirChiller: OK, can't be mixed
  EXPECT_TRUE(testObject.addAirChiller(airChiller2));
  EXPECT_EQ(0u, testObject.walkins().size());
  EXPECT_EQ(0u, testObject.cases().size());
  EXPECT_EQ(2u, testObject.airChillers().size());
  EXPECT_EQ(2u, refrigeratedCaseAndWalkInList->size());
}

// Test for #3921
TEST_F(ModelFixture, RefrigerationSystem_Case_AddMultipleTimes) {
  Model model;
  RefrigerationSystem system = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationCase c(model, s1);

  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCase>().size());

  EXPECT_TRUE(system.addCase(c));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCase>().size());
  ASSERT_EQ(1, system.cases().size());
  EXPECT_EQ(c, system.cases()[0]);
  EXPECT_TRUE(system.addCase(c));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCase>().size());
  ASSERT_EQ(1, system.cases().size());
  EXPECT_EQ(c, system.cases()[0]);

  // Shouldn't throw, and should remove the Case too
  EXPECT_NO_THROW(system.remove());
  EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationCase>().size());
}

// Test for #3921
TEST_F(ModelFixture, RefrigerationSystem_WalkIn_AddMultipleTimes) {
  Model model;
  RefrigerationSystem system = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationWalkIn walkIn(model, s1);

  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationWalkIn>().size());

  EXPECT_TRUE(system.addWalkin(walkIn));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationWalkIn>().size());
  ASSERT_EQ(1, system.walkins().size());
  EXPECT_EQ(walkIn, system.walkins()[0]);
  EXPECT_TRUE(system.addWalkin(walkIn));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationWalkIn>().size());
  ASSERT_EQ(1, system.walkins().size());
  EXPECT_EQ(walkIn, system.walkins()[0]);

  // Shouldn't throw, and should remove the WalkIn too
  EXPECT_NO_THROW(system.remove());
  EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationWalkIn>().size());
}

// Test for #3921
TEST_F(ModelFixture, RefrigerationSystem_Compressor_AddMultipleTimes) {
  Model model;
  RefrigerationSystem system = RefrigerationSystem(model);

  RefrigerationCompressor compressor(model);

  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCompressor>().size());

  EXPECT_TRUE(system.addCompressor(compressor));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCompressor>().size());
  ASSERT_EQ(1, system.compressors().size());
  EXPECT_EQ(compressor, system.compressors()[0]);
  EXPECT_EQ(0, system.highStageCompressors().size());
  EXPECT_TRUE(system.addCompressor(compressor));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCompressor>().size());
  ASSERT_EQ(1, system.compressors().size());
  EXPECT_EQ(compressor, system.compressors()[0]);
  EXPECT_EQ(0, system.highStageCompressors().size());

  // Test on the High Stage too
  EXPECT_TRUE(system.addHighStageCompressor(compressor));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCompressor>().size());
  EXPECT_EQ(0, system.compressors().size());
  ASSERT_EQ(1, system.highStageCompressors().size());
  EXPECT_EQ(compressor, system.highStageCompressors()[0]);

  // Shouldn't throw, and should remove the Compressor too
  system.remove();
  EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationCompressor>().size());
}

// Test for #3921
TEST_F(ModelFixture, RefrigerationSystem_SecondarySystemLoad_AddMultipleTimes) {
  Model model;
  RefrigerationSystem system = RefrigerationSystem(model);

  RefrigerationSecondarySystem secondarySystem(model);

  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSecondarySystem>().size());

  EXPECT_TRUE(system.addSecondarySystemLoad(secondarySystem));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSecondarySystem>().size());
  ASSERT_EQ(1, system.secondarySystemLoads().size());
  EXPECT_EQ(secondarySystem, system.secondarySystemLoads()[0]);
  EXPECT_TRUE(system.addSecondarySystemLoad(secondarySystem));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSecondarySystem>().size());
  ASSERT_EQ(1, system.secondarySystemLoads().size());
  EXPECT_EQ(secondarySystem, system.secondarySystemLoads()[0]);

  // Shouldn't throw, and should remove the SecondarySystem too
  EXPECT_NO_THROW(system.remove());
  EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationSecondarySystem>().size());
}

// Test for #3921
TEST_F(ModelFixture, RefrigerationSystem_CondenserCascadeLoad_AddMultipleTimes) {
  Model model;
  RefrigerationSystem system = RefrigerationSystem(model);

  RefrigerationCondenserCascade condenserCascade(model);

  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserCascade>().size());

  EXPECT_TRUE(system.addCascadeCondenserLoad(condenserCascade));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserCascade>().size());
  ASSERT_EQ(1, system.cascadeCondenserLoads().size());
  EXPECT_EQ(condenserCascade, system.cascadeCondenserLoads()[0]);
  EXPECT_TRUE(system.addCascadeCondenserLoad(condenserCascade));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserCascade>().size());
  ASSERT_EQ(1, system.cascadeCondenserLoads().size());
  EXPECT_EQ(condenserCascade, system.cascadeCondenserLoads()[0]);

  // Shouldn't throw, and should remove the CondenserCascade too
  EXPECT_NO_THROW(system.remove());
  EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationCondenserCascade>().size());
}

// Test for #3921
TEST_F(ModelFixture, RefrigerationSystem_AirChiller_AddMultipleTimes) {
  Model model;
  RefrigerationSystem system = RefrigerationSystem(model);

  ScheduleCompact s1(model);
  RefrigerationAirChiller airChiller(model, s1);

  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationAirChiller>().size());

  EXPECT_TRUE(system.addAirChiller(airChiller));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationAirChiller>().size());
  ASSERT_EQ(1, system.airChillers().size());
  EXPECT_EQ(airChiller, system.airChillers()[0]);
  EXPECT_TRUE(system.addAirChiller(airChiller));
  EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationAirChiller>().size());
  ASSERT_EQ(1, system.airChillers().size());
  EXPECT_EQ(airChiller, system.airChillers()[0]);

  // Shouldn't throw, and should remove the AirChiller too
  EXPECT_NO_THROW(system.remove());
  EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationAirChiller>().size());
}

// Test for #3921
TEST_F(ModelFixture, RefrigerationSystem_RefrigerationCondenser_Unicity) {
  Model model;

  {
    RefrigerationSystem system = RefrigerationSystem(model);
    RefrigerationSystem system2 = RefrigerationSystem(model);

    RefrigerationCondenserAirCooled condenser(model);
    EXPECT_FALSE(condenser.system());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserAirCooled>().size());

    EXPECT_TRUE(system.setRefrigerationCondenser(condenser));
    ASSERT_TRUE(system.refrigerationCondenser());
    EXPECT_EQ(condenser, system.refrigerationCondenser().get());
    EXPECT_FALSE(system2.refrigerationCondenser());
    ASSERT_TRUE(condenser.system());
    EXPECT_EQ(system, condenser.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserAirCooled>().size());

    // Adding it to another one? It should remove it from the first
    EXPECT_TRUE(system2.setRefrigerationCondenser(condenser));
    ASSERT_TRUE(system2.refrigerationCondenser());
    EXPECT_EQ(condenser, system2.refrigerationCondenser().get());
    EXPECT_FALSE(system.refrigerationCondenser());
    ASSERT_TRUE(condenser.system());
    EXPECT_EQ(system2, condenser.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserAirCooled>().size());

    system2.remove();
    EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationCondenserAirCooled>().size());
  }

  {
    RefrigerationSystem system = RefrigerationSystem(model);
    RefrigerationSystem system2 = RefrigerationSystem(model);

    RefrigerationCondenserWaterCooled condenser(model);
    EXPECT_FALSE(condenser.system());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserWaterCooled>().size());

    EXPECT_TRUE(system.setRefrigerationCondenser(condenser));
    ASSERT_TRUE(system.refrigerationCondenser());
    EXPECT_EQ(condenser, system.refrigerationCondenser().get());
    EXPECT_FALSE(system2.refrigerationCondenser());
    ASSERT_TRUE(condenser.system());
    EXPECT_EQ(system, condenser.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserWaterCooled>().size());

    // Adding it to another one? It should remove it from the first
    EXPECT_TRUE(system2.setRefrigerationCondenser(condenser));
    ASSERT_TRUE(system2.refrigerationCondenser());
    EXPECT_EQ(condenser, system2.refrigerationCondenser().get());
    EXPECT_FALSE(system.refrigerationCondenser());
    ASSERT_TRUE(condenser.system());
    EXPECT_EQ(system2, condenser.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserWaterCooled>().size());

    system2.remove();
    EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationCondenserWaterCooled>().size());
  }

  {
    RefrigerationSystem system = RefrigerationSystem(model);
    RefrigerationSystem system2 = RefrigerationSystem(model);

    RefrigerationCondenserEvaporativeCooled condenser(model);
    EXPECT_FALSE(condenser.system());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserEvaporativeCooled>().size());

    EXPECT_TRUE(system.setRefrigerationCondenser(condenser));
    ASSERT_TRUE(system.refrigerationCondenser());
    EXPECT_EQ(condenser, system.refrigerationCondenser().get());
    EXPECT_FALSE(system2.refrigerationCondenser());
    ASSERT_TRUE(condenser.system());
    EXPECT_EQ(system, condenser.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserEvaporativeCooled>().size());

    // Adding it to another one? It should remove it from the first
    EXPECT_TRUE(system2.setRefrigerationCondenser(condenser));
    ASSERT_TRUE(system2.refrigerationCondenser());
    EXPECT_EQ(condenser, system2.refrigerationCondenser().get());
    EXPECT_FALSE(system.refrigerationCondenser());
    ASSERT_TRUE(condenser.system());
    EXPECT_EQ(system2, condenser.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserEvaporativeCooled>().size());

    system2.remove();
    EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationCondenserEvaporativeCooled>().size());
  }

  {
    RefrigerationSystem system = RefrigerationSystem(model);
    RefrigerationSystem system2 = RefrigerationSystem(model);

    RefrigerationCondenserCascade condenser(model);
    EXPECT_FALSE(condenser.system());
    EXPECT_FALSE(condenser.heatRejectingSystem());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserCascade>().size());

    EXPECT_TRUE(system.setRefrigerationCondenser(condenser));
    ASSERT_TRUE(system.refrigerationCondenser());
    EXPECT_EQ(condenser, system.refrigerationCondenser().get());
    EXPECT_FALSE(system2.refrigerationCondenser());
    EXPECT_FALSE(condenser.system());
    ASSERT_TRUE(condenser.heatRejectingSystem());
    EXPECT_EQ(system, condenser.heatRejectingSystem().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserCascade>().size());

    // Adding it to another one? It should remove it from the first
    EXPECT_TRUE(system2.setRefrigerationCondenser(condenser));
    ASSERT_TRUE(system2.refrigerationCondenser());
    EXPECT_EQ(condenser, system2.refrigerationCondenser().get());
    EXPECT_FALSE(system.refrigerationCondenser());
    EXPECT_FALSE(condenser.system());
    ASSERT_TRUE(condenser.heatRejectingSystem());
    EXPECT_EQ(system2, condenser.heatRejectingSystem().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationCondenserCascade>().size());

    system2.remove();
    EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationCondenserCascade>().size());
  }
}

// Test for #3921
TEST_F(ModelFixture, RefrigerationSystem_Subcoolers_Unicity) {
  Model model;

  {
    RefrigerationSystem system = RefrigerationSystem(model);
    RefrigerationSystem system2 = RefrigerationSystem(model);

    RefrigerationSubcoolerMechanical subcooler(model);
    EXPECT_FALSE(subcooler.system());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>().size());

    EXPECT_TRUE(system.setMechanicalSubcooler(subcooler));
    ASSERT_TRUE(system.mechanicalSubcooler());
    EXPECT_EQ(subcooler, system.mechanicalSubcooler().get());
    EXPECT_FALSE(system2.mechanicalSubcooler());
    ASSERT_TRUE(subcooler.system());
    EXPECT_EQ(system, subcooler.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>().size());

    // Adding it to another one? It should remove it from the first
    EXPECT_TRUE(system2.setMechanicalSubcooler(subcooler));
    ASSERT_TRUE(system2.mechanicalSubcooler());
    EXPECT_EQ(subcooler, system2.mechanicalSubcooler().get());
    EXPECT_FALSE(system.mechanicalSubcooler());
    ASSERT_TRUE(subcooler.system());
    EXPECT_EQ(system2, subcooler.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>().size());

    system2.remove();
    EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>().size());
  }

  {
    RefrigerationSystem system = RefrigerationSystem(model);
    RefrigerationSystem system2 = RefrigerationSystem(model);

    RefrigerationSubcoolerLiquidSuction subcooler(model);
    EXPECT_FALSE(subcooler.system());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>().size());

    EXPECT_TRUE(system.setLiquidSuctionHeatExchangerSubcooler(subcooler));
    ASSERT_TRUE(system.liquidSuctionHeatExchangerSubcooler());
    EXPECT_EQ(subcooler, system.liquidSuctionHeatExchangerSubcooler().get());
    EXPECT_FALSE(system2.liquidSuctionHeatExchangerSubcooler());
    ASSERT_TRUE(subcooler.system());
    EXPECT_EQ(system, subcooler.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>().size());

    // Adding it to another one? It should remove it from the first
    EXPECT_TRUE(system2.setLiquidSuctionHeatExchangerSubcooler(subcooler));
    ASSERT_TRUE(system2.liquidSuctionHeatExchangerSubcooler());
    EXPECT_EQ(subcooler, system2.liquidSuctionHeatExchangerSubcooler().get());
    EXPECT_FALSE(system.liquidSuctionHeatExchangerSubcooler());
    ASSERT_TRUE(subcooler.system());
    EXPECT_EQ(system2, subcooler.system().get());
    EXPECT_EQ(1, model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>().size());

    system2.remove();
    EXPECT_EQ(0, model.getConcreteModelObjects<RefrigerationSubcoolerLiquidSuction>().size());
  }
}
