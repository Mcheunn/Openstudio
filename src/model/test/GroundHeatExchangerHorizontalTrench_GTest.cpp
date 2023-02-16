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
#include "ModelFixture.hpp"
#include "../GroundHeatExchangerHorizontalTrench.hpp"
#include "../GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "../SiteGroundTemperatureUndisturbedKusudaAchenbach.hpp"
#include "../SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl.hpp"
#include "../SiteGroundTemperatureUndisturbedXing.hpp"
#include "../SiteGroundTemperatureUndisturbedXing_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GroundHeatExchangerHorizontalTrench_GroundHeatExchangerHorizontalTrench) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      GroundHeatExchangerHorizontalTrench gh(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    GroundHeatExchangerHorizontalTrench gh(m);

    EXPECT_EQ(0.004, gh.designFlowRate());
    EXPECT_EQ(75, gh.trenchLengthinPipeAxialDirection());
    EXPECT_EQ(2, gh.numberofTrenches());
    EXPECT_EQ(2.0, gh.horizontalSpacingBetweenPipes());
    EXPECT_EQ(0.016, gh.pipeInnerDiameter());
    EXPECT_EQ(0.02667, gh.pipeOuterDiameter());
    EXPECT_EQ(1.25, gh.burialDepth());
    EXPECT_EQ(1.08, gh.soilThermalConductivity());
    EXPECT_EQ(962, gh.soilDensity());
    EXPECT_EQ(2576, gh.soilSpecificHeat());
    EXPECT_EQ(0.3895, gh.pipeThermalConductivity());
    EXPECT_EQ(641, gh.pipeDensity());
    EXPECT_EQ(2405, gh.pipeSpecificHeat());
    EXPECT_EQ(30, gh.soilMoistureContentPercent());
    EXPECT_EQ(50, gh.soilMoistureContentPercentatSaturation());
    EXPECT_EQ("KusudaAchenbach", gh.groundTemperatureModel());
    EXPECT_FALSE(gh.isGroundTemperatureModelDefaulted());
    EXPECT_EQ(15.5, gh.kusudaAchenbachAverageSurfaceTemperature());
    EXPECT_EQ(12.8, gh.kusudaAchenbachAverageAmplitudeofSurfaceTemperature());
    EXPECT_EQ(17.3, gh.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature());
    EXPECT_EQ(0.408, gh.evapotranspirationGroundCoverParameter());
    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka);
  }

  {
    Model m;
    SiteGroundTemperatureUndisturbedKusudaAchenbach sgt(m);
    GroundHeatExchangerHorizontalTrench gh(m, sgt);

    EXPECT_EQ(0.004, gh.designFlowRate());
    EXPECT_EQ(75, gh.trenchLengthinPipeAxialDirection());
    EXPECT_EQ(2, gh.numberofTrenches());
    EXPECT_EQ(2.0, gh.horizontalSpacingBetweenPipes());
    EXPECT_EQ(0.016, gh.pipeInnerDiameter());
    EXPECT_EQ(0.02667, gh.pipeOuterDiameter());
    EXPECT_EQ(1.25, gh.burialDepth());
    EXPECT_EQ(1.08, gh.soilThermalConductivity());
    EXPECT_EQ(962, gh.soilDensity());
    EXPECT_EQ(2576, gh.soilSpecificHeat());
    EXPECT_EQ(0.3895, gh.pipeThermalConductivity());
    EXPECT_EQ(641, gh.pipeDensity());
    EXPECT_EQ(2405, gh.pipeSpecificHeat());
    EXPECT_EQ(30, gh.soilMoistureContentPercent());
    EXPECT_EQ(50, gh.soilMoistureContentPercentatSaturation());
    EXPECT_EQ("KusudaAchenbach", gh.groundTemperatureModel());
    EXPECT_FALSE(gh.isGroundTemperatureModelDefaulted());
    EXPECT_EQ(15.5, gh.kusudaAchenbachAverageSurfaceTemperature());
    EXPECT_EQ(12.8, gh.kusudaAchenbachAverageAmplitudeofSurfaceTemperature());
    EXPECT_EQ(17.3, gh.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature());
    EXPECT_EQ(0.408, gh.evapotranspirationGroundCoverParameter());
    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka);
    EXPECT_EQ(sgt, uka.get());
  }
}

TEST_F(ModelFixture, GroundHeatExchangerHorizontalTrench_SetGetFields) {
  Model m;
  GroundHeatExchangerHorizontalTrench gh(m);

  EXPECT_TRUE(gh.setDesignFlowRate(0.005));
  EXPECT_TRUE(gh.setTrenchLengthinPipeAxialDirection(80));
  EXPECT_TRUE(gh.setNumberofTrenches(3));
  EXPECT_TRUE(gh.setHorizontalSpacingBetweenPipes(3.0));
  EXPECT_TRUE(gh.setPipeInnerDiameter(0.02));
  EXPECT_TRUE(gh.setPipeOuterDiameter(0.03));
  EXPECT_TRUE(gh.setBurialDepth(1.3));
  EXPECT_TRUE(gh.setSoilThermalConductivity(1.1));
  EXPECT_TRUE(gh.setSoilDensity(965));
  EXPECT_TRUE(gh.setSoilSpecificHeat(2600));
  EXPECT_TRUE(gh.setPipeThermalConductivity(0.4));
  EXPECT_TRUE(gh.setPipeDensity(650));
  EXPECT_TRUE(gh.setPipeSpecificHeat(2500));
  EXPECT_TRUE(gh.setSoilMoistureContentPercent(40));
  EXPECT_TRUE(gh.setSoilMoistureContentPercentatSaturation(60));
  EXPECT_FALSE(gh.setGroundTemperatureModel("SiteGroundTemperature"));
  gh.resetGroundTemperatureModel();
  EXPECT_TRUE(gh.setKusudaAchenbachAverageSurfaceTemperature(16.0));
  EXPECT_TRUE(gh.setKusudaAchenbachAverageAmplitudeofSurfaceTemperature(13.0));
  EXPECT_TRUE(gh.setKusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(18.0));
  EXPECT_TRUE(gh.setEvapotranspirationGroundCoverParameter(0.5));

  SiteGroundTemperatureUndisturbedKusudaAchenbach sgt(m);
  EXPECT_TRUE(gh.setUndisturbedGroundTemperatureModel(sgt));

  EXPECT_EQ(0.005, gh.designFlowRate());
  EXPECT_EQ(80, gh.trenchLengthinPipeAxialDirection());
  EXPECT_EQ(3, gh.numberofTrenches());
  EXPECT_EQ(3.0, gh.horizontalSpacingBetweenPipes());
  EXPECT_EQ(0.02, gh.pipeInnerDiameter());
  EXPECT_EQ(0.03, gh.pipeOuterDiameter());
  EXPECT_EQ(1.3, gh.burialDepth());
  EXPECT_EQ(1.1, gh.soilThermalConductivity());
  EXPECT_EQ(965, gh.soilDensity());
  EXPECT_EQ(2600, gh.soilSpecificHeat());
  EXPECT_EQ(0.4, gh.pipeThermalConductivity());
  EXPECT_EQ(650, gh.pipeDensity());
  EXPECT_EQ(2500, gh.pipeSpecificHeat());
  EXPECT_EQ(40, gh.soilMoistureContentPercent());
  EXPECT_EQ(60, gh.soilMoistureContentPercentatSaturation());
  EXPECT_EQ("KusudaAchenbach", gh.groundTemperatureModel());
  EXPECT_FALSE(gh.isGroundTemperatureModelDefaulted());
  EXPECT_EQ(15.5, gh.kusudaAchenbachAverageSurfaceTemperature());
  EXPECT_EQ(12.8, gh.kusudaAchenbachAverageAmplitudeofSurfaceTemperature());
  EXPECT_EQ(17.3, gh.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature());
  EXPECT_EQ(0.5, gh.evapotranspirationGroundCoverParameter());
  ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
  boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
    undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
  ASSERT_TRUE(uka);
  EXPECT_EQ(sgt, uka.get());

  SiteGroundTemperatureUndisturbedXing sgt2(m);
  EXPECT_TRUE(gh.setUndisturbedGroundTemperatureModel(sgt2));

  EXPECT_EQ(0.005, gh.designFlowRate());
  EXPECT_EQ(80, gh.trenchLengthinPipeAxialDirection());
  EXPECT_EQ(3, gh.numberofTrenches());
  EXPECT_EQ(3.0, gh.horizontalSpacingBetweenPipes());
  EXPECT_EQ(0.02, gh.pipeInnerDiameter());
  EXPECT_EQ(0.03, gh.pipeOuterDiameter());
  EXPECT_EQ(1.3, gh.burialDepth());
  EXPECT_EQ(1.1, gh.soilThermalConductivity());
  EXPECT_EQ(965, gh.soilDensity());
  EXPECT_EQ(2600, gh.soilSpecificHeat());
  EXPECT_EQ(0.4, gh.pipeThermalConductivity());
  EXPECT_EQ(650, gh.pipeDensity());
  EXPECT_EQ(2500, gh.pipeSpecificHeat());
  EXPECT_EQ(40, gh.soilMoistureContentPercent());
  EXPECT_EQ(60, gh.soilMoistureContentPercentatSaturation());
  EXPECT_EQ("SiteGroundTemperature", gh.groundTemperatureModel());
  EXPECT_FALSE(gh.isGroundTemperatureModelDefaulted());
  EXPECT_THROW(gh.kusudaAchenbachAverageSurfaceTemperature(), openstudio::Exception);
  EXPECT_THROW(gh.kusudaAchenbachAverageAmplitudeofSurfaceTemperature(), openstudio::Exception);
  EXPECT_THROW(gh.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(), openstudio::Exception);
  EXPECT_EQ(0.5, gh.evapotranspirationGroundCoverParameter());
  ModelObject undisturbedGroundTemperatureModel2 = gh.undisturbedGroundTemperatureModel();
  boost::optional<SiteGroundTemperatureUndisturbedXing> ux = undisturbedGroundTemperatureModel2.optionalCast<SiteGroundTemperatureUndisturbedXing>();
  ASSERT_TRUE(ux);
  EXPECT_EQ(sgt2, ux.get());
}

TEST_F(ModelFixture, GroundHeatExchangerHorizontalTrench_Clone) {
  {
    Model m;
    SiteGroundTemperatureUndisturbedKusudaAchenbach sgt(m);
    GroundHeatExchangerHorizontalTrench gh(m, sgt);

    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

    auto ghClone = gh.clone(m).cast<GroundHeatExchangerHorizontalTrench>();
    EXPECT_EQ(2u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka);
    EXPECT_EQ(sgt, uka.get());

    ModelObject undisturbedGroundTemperatureModelClone = ghClone.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> ukaClone =
      undisturbedGroundTemperatureModelClone.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(ukaClone);
    EXPECT_EQ(sgt, ukaClone.get());

    gh.remove();
    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

    ghClone.remove();
    EXPECT_EQ(0u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());
  }

  {
    Model m;
    SiteGroundTemperatureUndisturbedXing sgt(m);
    GroundHeatExchangerHorizontalTrench gh(m, sgt);

    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

    auto ghClone = gh.clone(m).cast<GroundHeatExchangerHorizontalTrench>();
    EXPECT_EQ(2u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedXing> ux = undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedXing>();
    ASSERT_TRUE(ux);
    EXPECT_EQ(sgt, ux.get());

    ModelObject undisturbedGroundTemperatureModelClone = ghClone.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedXing> uxClone =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedXing>();
    ASSERT_TRUE(uxClone);
    EXPECT_EQ(sgt, uxClone.get());

    gh.remove();
    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

    ghClone.remove();
    EXPECT_EQ(0u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());
  }
}
