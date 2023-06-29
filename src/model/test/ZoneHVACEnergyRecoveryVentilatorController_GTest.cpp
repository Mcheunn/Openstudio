/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../ZoneHVACEnergyRecoveryVentilatorController.hpp"
#include "../ZoneHVACEnergyRecoveryVentilatorController_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACEnergyRecoveryVentilatorController) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ZoneHVACEnergyRecoveryVentilatorController zonehvac(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}
