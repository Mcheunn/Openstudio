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
#include "epJSONFixture.hpp"
#include "../epJSONTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../energyplus/ForwardTranslator.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/PathHelpers.hpp"

#include <utilities/idd/GroundHeatExchanger_ResponseFactors_FieldEnums.hxx>
#include <utilities/idd/UnitarySystemPerformance_Multispeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>
#include <json/json.h>
#include <resources.hxx>
#include <algorithm>

openstudio::path setupIdftoEPJSONTest(const openstudio::path& location) {
  const auto basename = openstudio::toPath(openstudio::filesystem::basename(location));
  const auto working_directory = openstudio::filesystem::complete(openstudio::toPath("epjson_tests") / basename);
  const auto idf_path = working_directory / openstudio::toPath("eplus.idf");
  openstudio::filesystem::create_directories(working_directory);
  openstudio::filesystem::copy_file(location, idf_path, openstudio::filesystem::copy_option::overwrite_if_exists);
  return idf_path;
}

Json::Value translateIdfToEPJSONWithEP(const openstudio::path& location) {

  [[maybe_unused]] auto result =
    std::system(fmt::format("{} --output-directory {} --convert-only {}", openstudio::toString(openstudio::getEnergyPlusExecutable().native()),
                            openstudio::toString(location.parent_path().native()), openstudio::toString(location.native()))
                  .c_str());

  const auto epJSONFile = openstudio::setFileExtension(location, "epJSON", true);
  const auto root = openstudio::epJSON::loadJSON(epJSONFile);

  const auto outputLocation = location.parent_path() / openstudio::toPath("eplus-rewritten.epJSON");
  std::ofstream ofs(openstudio::toString(outputLocation), std::ofstream::trunc);
  ofs << root.toStyledString() << '\n';

  return root;
}

openstudio::path completeIDFPath(const openstudio::path& idf) {
  return openstudio::getEnergyPlusDirectory() / openstudio::toPath("ExampleFiles") / idf;
}

Json::Value translateIdfToEPJSONWithOS(const openstudio::path& location) {
  auto idf = openstudio::IdfFile::load(location);

  if (!idf) {
    return Json::Value{};
  }

  auto result = openstudio::epJSON::toJSON(*idf);

  const auto outputLocation = location.parent_path() / openstudio::toPath("os.epJSON");
  std::ofstream ofs(openstudio::toString(outputLocation), std::ofstream::trunc);
  ofs << result.toStyledString() << '\n';
  return result;
}

void makeDoubles(Json::Value& value) {
  if (value.isNumeric()) {
    value = value.asDouble();
  } else {
    for (auto& child : value) {
      makeDoubles(child);
    }
  }
}

bool equal(const Json::Value& lhs, const Json::Value& rhs) {
  auto doubledLhs = lhs;
  makeDoubles(doubledLhs);

  auto doubledRhs = rhs;
  makeDoubles(doubledRhs);

  return doubledLhs == doubledRhs;
}

std::pair<Json::Value, Json::Value> doEPJSONTranslations(const std::string& idfname) {

  const auto setupIdf = setupIdftoEPJSONTest(completeIDFPath(openstudio::toString(idfname)));
  return {translateIdfToEPJSONWithEP(setupIdf), translateIdfToEPJSONWithOS(setupIdf)};
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgMediumOfficeNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgMediumOfficeNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_1ZoneEvapCooler) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("1ZoneEvapCooler.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_WCE_DoubleClear_BSDF) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("WCE_DoubleClear_BSDF.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_SupermarketSecondary) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("SupermarketSecondary.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_5ZoneWaterLoopHeatPump) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("5ZoneWaterLoopHeatPump.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ZoneCoupledGroundHTSlabInGrade) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ZoneCoupledGroundHTSlabInGrade.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_MeasuredDeflectionAndShading) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_MeasuredDeflectionAndShading.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_HospitalLowEnergy) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("HospitalLowEnergy.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_HospitalBaselineReheatReportEMS) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("HospitalBaselineReheatReportEMS.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_HospitalBaseline) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("HospitalBaseline.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_Daylighting_SouthVB45deg) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_Daylighting_SouthVB45deg.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_Daylighting_SouthVerticalVB45deg) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_Daylighting_SouthVerticalVB45deg.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_InShadeGasMix) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_InShadeGasMix.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_SchedSurfGains) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_SchedSurfGains.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_ApartmentHighRise_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_ApartmentHighRise_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_OutPatientHealthCare_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_OutPatientHealthCare_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgOutPatientNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgOutPatientNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_SchoolSecondary_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_SchoolSecondary_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_Hospital_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_Hospital_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_UnitaryHybridAC_DedicatedOutsideAir) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("UnitaryHybridAC_DedicatedOutsideAir.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgSmallHotelNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgSmallHotelNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgHospitalNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgHospitalNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgSecondarySchoolNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgSecondarySchoolNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_SchoolPrimary_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_SchoolPrimary_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_OfficeLarge_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_OfficeLarge_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_HotelLarge_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_HotelLarge_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_ApartmentMidRise_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_ApartmentMidRise_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgMidriseApartmentNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgMidriseApartmentNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefrigeratedWarehouse) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefrigeratedWarehouse.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_SmOff_IntExtShading) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_SmOff_IntExtShading.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgPrimarySchoolNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgPrimarySchoolNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_RetailStripmall_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_RetailStripmall_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgLargeHotelNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgLargeHotelNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_EMSDemandManager_LargeOffice) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("EMSDemandManager_LargeOffice.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, toStringUilityWorksAsExpected) {

  const auto location = completeIDFPath("RefBldgMediumOfficeNew2004_Chicago.idf");
  auto idf = openstudio::IdfFile::load(location);

  ASSERT_TRUE(idf);
  const auto str1 = openstudio::epJSON::toJSON(*idf).toStyledString();
  const auto str2 = openstudio::epJSON::toJSONString(*idf);

  EXPECT_EQ(str1, str2);
}

TEST_F(epJSONFixture, canTranslateWorkspaceToJSON) {
  auto m = openstudio::model::exampleModel();
  openstudio::energyplus::ForwardTranslator ft;
  openstudio::Workspace w = ft.translateModel(m);

  const auto str1 = openstudio::epJSON::toJSON(w).toStyledString();

  EXPECT_TRUE(str1.size() > 100);
}

TEST_F(epJSONFixture, CustomCases) {

  // Test for #4264, part 1
  openstudio::Workspace w(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::WorkspaceObject response_factor =
    w.addObject(openstudio::IdfObject(openstudio::IddObjectType::GroundHeatExchanger_ResponseFactors)).get();

  EXPECT_TRUE(response_factor.setInt(openstudio::GroundHeatExchanger_ResponseFactorsFields::NumberofBoreholes, 5));
  EXPECT_TRUE(response_factor.setDouble(openstudio::GroundHeatExchanger_ResponseFactorsFields::GFunctionReferenceRatio, 0.0005));

  auto eg = response_factor.pushExtensibleGroup();
  EXPECT_TRUE(eg.setDouble(openstudio::GroundHeatExchanger_ResponseFactorsExtensibleFields::gFunctionLn_T_Ts_Value, 0.0005));
  EXPECT_TRUE(eg.setDouble(openstudio::GroundHeatExchanger_ResponseFactorsExtensibleFields::gFunctiongValue, 2.679));

  auto json = openstudio::epJSON::toJSON(w);

  const Json::Value& json_rfs = json["GroundHeatExchanger:ResponseFactors"];
  ASSERT_TRUE(json_rfs);
  ASSERT_EQ(1u, json_rfs.getMemberNames().size());
  const Json::Value& json_rf = json_rfs[json_rfs.getMemberNames()[0]];
  ASSERT_EQ(1u, json_rf["g_functions"].size());
  const auto& g_func = json_rf["g_functions"][0];
  auto keys = g_func.getMemberNames();
  EXPECT_EQ(2u, keys.size());
  EXPECT_NE(keys.cend(), std::find(keys.cbegin(), keys.cend(), "g_function_ln_t_ts_value"))
    << fmt::format("Expected to find 'g_function_ln_t_ts_value' in keys, but found [{}]", fmt::join(keys, ", "));
}

TEST_F(epJSONFixture, Autosize_case_sensitiveness) {

  // Test for #4264, part 2
  openstudio::Workspace w(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::WorkspaceObject wo = w.addObject(openstudio::IdfObject(openstudio::IddObjectType::UnitarySystemPerformance_Multispeed)).get();

  wo.setName("Unitary Performance Multispeed");
  EXPECT_TRUE(wo.setInt(openstudio::UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, 0));
  EXPECT_TRUE(wo.setInt(openstudio::UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, 1));
  EXPECT_TRUE(wo.setString(openstudio::UnitarySystemPerformance_MultispeedFields::SingleModeOperation, "No"));
  EXPECT_TRUE(wo.setString(openstudio::UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio, ""));

  auto eg = wo.pushExtensibleGroup();
  EXPECT_TRUE(eg.setString(openstudio::UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, "autosize"));
  EXPECT_TRUE(eg.setDouble(openstudio::UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, 0.42));

  auto json = openstudio::epJSON::toJSON(w);

  const Json::Value& json_perfs = json["UnitarySystemPerformance:Multispeed"];
  ASSERT_TRUE(json_perfs);
  ASSERT_EQ(1u, json_perfs.getMemberNames().size());
  const Json::Value& json_perf = json_perfs[json_perfs.getMemberNames()[0]];
  ASSERT_EQ(1u, json_perf["flow_ratios"].size());
  const auto& flow_ratio = json_perf["flow_ratios"][0];
  EXPECT_EQ("Autosize", flow_ratio["heating_speed_supply_air_flow_ratio"].asString());
}
