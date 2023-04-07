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
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PerformancePrecisionTradeoffs.hpp"
#include "../../model/PerformancePrecisionTradeoffs_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/PerformancePrecisionTradeoffs_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PerformancePrecisionTradeoffs) {

  ForwardTranslator ft;

  Model model;

  // Get the unique object
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();
  EXPECT_TRUE(performancePrecisionTradeoffs.isUseCoilDirectSolutionsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isOverrideModeDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxZoneTempDiffDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxAllowedDelTempDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isUseRepresentativeSurfacesforCalculationsDefaulted());

  Workspace workspace = ft.translateModel(model);

  WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::PerformancePrecisionTradeoffs);
  ASSERT_EQ(1u, idfObjs.size());

  WorkspaceObject idf_perf(idfObjs[0]);

  EXPECT_FALSE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions));
  EXPECT_EQ("No", idf_perf.getString(PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions).get());
  EXPECT_TRUE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm));
  EXPECT_TRUE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::OverrideMode));
  EXPECT_TRUE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::MaxZoneTempDiff));
  EXPECT_TRUE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::MaxAllowedDelTemp));
  EXPECT_FALSE(idf_perf.isEmpty(PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations));
}
