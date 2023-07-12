/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/RunPeriodControlSpecialDays.hpp"
#include "../../model/RunPeriodControlSpecialDays_Impl.hpp"

#include "../../utilities/time/Date.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OS_RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include <utilities/idd/RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriodControlSpecialDays) {
  ForwardTranslator ft;

  // Create a model
  Model m;

  RunPeriodControlSpecialDays holiday(openstudio::MonthOfYear::Jul, 4, m);
  holiday.setName("4th of July");
  holiday.setSpecialDayType("Holiday");
  holiday.setDuration(1);

  {
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays).size());
    WorkspaceObject idf_holiday = w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays)[0];
    EXPECT_EQ("4th of July", idf_holiday.nameString(false));
    EXPECT_EQ("7/4", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::StartDate).get());
    EXPECT_EQ(1, idf_holiday.getInt(RunPeriodControl_SpecialDaysFields::Duration).get());
    EXPECT_EQ("Holiday", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::SpecialDayType).get());
  }

  holiday.setName("A week off");
  EXPECT_TRUE(holiday.setStartDate(openstudio::NthDayOfWeekInMonth::first, openstudio::DayOfWeek::Monday, openstudio::MonthOfYear::Mar));
  EXPECT_TRUE(holiday.setDuration(5));
  EXPECT_TRUE(holiday.setSpecialDayType("CustomDay1"));

  {
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays).size());
    WorkspaceObject idf_holiday = w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays)[0];
    EXPECT_EQ("A week off", idf_holiday.nameString(false));
    EXPECT_EQ("1st Monday in March", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::StartDate).get());
    EXPECT_EQ(5, idf_holiday.getInt(RunPeriodControl_SpecialDaysFields::Duration).get());
    EXPECT_EQ("CustomDay1", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::SpecialDayType).get());
  }

  // 5th replaced by Last
  EXPECT_TRUE(holiday.setStartDate(openstudio::NthDayOfWeekInMonth::fifth, openstudio::DayOfWeek::Tuesday, openstudio::MonthOfYear::Aug));
  {
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays).size());
    WorkspaceObject idf_holiday = w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays)[0];
    EXPECT_EQ("Last Tuesday in August", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::StartDate).get());
  }
}

// TODO: RT of RunPeriodControlSpecialDays is disabled
TEST_F(EnergyPlusFixture, DISABLED_ReverseTranslator_RunPeriodControlSpecialDays) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_EQ(0u, m.getConcreteModelObjects<RunPeriodControlSpecialDays>().size());
  }

  OptionalWorkspaceObject _i_specialDay = w.addObject(IdfObject(IddObjectType::RunPeriodControl_SpecialDays));
  ASSERT_TRUE(_i_specialDay);

  EXPECT_TRUE(_i_specialDay->setName("Memorial Day"));
  EXPECT_TRUE(_i_specialDay->setString(RunPeriodControl_SpecialDaysFields::StartDate, "last MoNday    in Aug"));
  EXPECT_TRUE(_i_specialDay->setInt(RunPeriodControl_SpecialDaysFields::Duration, 1));
  EXPECT_TRUE(_i_specialDay->setString(RunPeriodControl_SpecialDaysFields::SpecialDayType, "Holiday"));

  {
    Model m = rt.translateWorkspace(w);

    std::vector<openstudio::model::RunPeriodControlSpecialDays> specialDays =
      m.getConcreteModelObjects<openstudio::model::RunPeriodControlSpecialDays>();
    ASSERT_EQ(1u, specialDays.size());
    RunPeriodControlSpecialDays specialDay = specialDays[0];

    EXPECT_EQ("Memorial Day", specialDay.nameString());
    EXPECT_EQ("last MoNday    in Aug", specialDay.getString(OS_RunPeriodControl_SpecialDaysFields::StartDate).get());
    ASSERT_NO_THROW(specialDay.startDate());
    // Assumed base year is 2009, so last one falls on the 31th
    EXPECT_EQ(31u, specialDay.startDate().dayOfMonth());
    EXPECT_EQ(MonthOfYear::Aug, specialDay.startDate().monthOfYear().value());
    EXPECT_EQ(DayOfWeek::Monday, specialDay.startDate().dayOfWeek().value());
    EXPECT_EQ(1u, specialDay.duration());
    EXPECT_EQ("Holiday", specialDay.specialDayType());
  }

  EXPECT_TRUE(_i_specialDay->setString(RunPeriodControl_SpecialDaysFields::StartDate, "7/1"));
  EXPECT_TRUE(_i_specialDay->setInt(RunPeriodControl_SpecialDaysFields::Duration, 15));
  EXPECT_TRUE(_i_specialDay->setString(RunPeriodControl_SpecialDaysFields::SpecialDayType, "CustomDay1"));

  {
    Model m = rt.translateWorkspace(w);

    std::vector<openstudio::model::RunPeriodControlSpecialDays> specialDays =
      m.getConcreteModelObjects<openstudio::model::RunPeriodControlSpecialDays>();
    ASSERT_EQ(1u, specialDays.size());
    RunPeriodControlSpecialDays specialDay = specialDays[0];

    EXPECT_EQ("7/1", specialDay.getString(OS_RunPeriodControl_SpecialDaysFields::StartDate).get());
    ASSERT_NO_THROW(specialDay.startDate());
    // Assumed base year is 2009, so last one falls on the 31th
    EXPECT_EQ(1u, specialDay.startDate().dayOfMonth());
    EXPECT_EQ(MonthOfYear::Jul, specialDay.startDate().monthOfYear().value());
    EXPECT_EQ(15u, specialDay.duration());
    EXPECT_EQ("CustomDay1", specialDay.specialDayType());
  }
}
