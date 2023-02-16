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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputSQLite.hpp"
#include "../../model/OutputSQLite_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Output_SQLite_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputSQLite) {

  // Not there, but workspace should have it
  {
    Model m;
    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_SQLite);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_sqlite(idfObjs[0]);

    EXPECT_EQ("SimpleAndTabular", idf_sqlite.getString(Output_SQLiteFields::OptionType, false).get());
    EXPECT_TRUE(idf_sqlite.isEmpty(Output_SQLiteFields::UnitConversionforTabularData));
  }

  // Not there, m_forwardTranslatorOptions.excludeSQliteOutputReport() = True
  {
    Model m;
    ForwardTranslator ft;
    ft.setExcludeSQliteOutputReport(true);
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_SQLite);
    ASSERT_EQ(0u, idfObjs.size());
  }

  // It's already in the model
  {
    Model m;
    ForwardTranslator ft;

    OutputSQLite outputSQLite = m.getUniqueModelObject<OutputSQLite>();

    EXPECT_TRUE(outputSQLite.setOptionType("Simple"));
    EXPECT_TRUE(outputSQLite.setUnitConversionforTabularData("None"));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_SQLite);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_sqlite(idfObjs[0]);

    EXPECT_EQ("Simple", idf_sqlite.getString(Output_SQLiteFields::OptionType, false).get());
    EXPECT_EQ("None", idf_sqlite.getString(Output_SQLiteFields::UnitConversionforTabularData, false).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputSQLite) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m;
  m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputSQLite>());

  OptionalWorkspaceObject _i_outputSQLite = w.addObject(IdfObject(IddObjectType::Output_SQLite));
  ASSERT_TRUE(_i_outputSQLite);

  EXPECT_TRUE(_i_outputSQLite->setString(Output_SQLiteFields::OptionType, "Simple"));
  EXPECT_TRUE(_i_outputSQLite->setString(Output_SQLiteFields::UnitConversionforTabularData, "InchPound"));

  m = rt.translateWorkspace(w);

  // Get the unique object
  OutputSQLite outputSQLite = m.getUniqueModelObject<OutputSQLite>();
  EXPECT_EQ("Simple", outputSQLite.optionType());
  EXPECT_EQ("InchPound", outputSQLite.unitConversionforTabularData());
}
