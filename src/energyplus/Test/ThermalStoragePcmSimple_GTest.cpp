/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/ThermalStoragePcmSimple.hpp"
#include "../../model/ThermalStoragePcmSimple_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/ThermalStorage_Pcm_Simple_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalStoragePcmSimple) {
  Model m;

  ThermalStoragePcmSimple t(m);

  t.setIceStorageType("IceOnCoilExternal");
  t.setCapacity(0.8);

  PlantLoop p(m);
  ASSERT_TRUE(p.addSupplyBranchForComponent(t));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::PlantLoop).size());

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::ThermalStorage_Pcm_Simple));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_t(idfObjs[0]);

  EXPECT_NE("", idf_t.getString(ThermalStorage_Pcm_SimpleFields::InletNodeName, false).get());
  EXPECT_NE("", idf_t.getString(ThermalStorage_Pcm_SimpleFields::OutletNodeName, false).get());
  EXPECT_EQ("IceOnCoilExternal", idf_t.getString(ThermalStorage_Pcm_SimpleFields::IceStorageType, false).get());
  EXPECT_EQ(0.8, idf_t.getDouble(ThermalStorage_Pcm_SimpleFields::Capacity, false).get());
  EXPECT_EQ(5.5, idf_t.getDouble(ThermalStorage_Pcm_SimpleFields::OnsetTemperatureOfPhaseChange, false).get());
  EXPECT_EQ(7, idf_t.getDouble(ThermalStorage_Pcm_SimpleFields::FinishTemperatureOfPhaseChange, false).get());
  EXPECT_EQ(20000, idf_t.getDouble(ThermalStorage_Pcm_SimpleFields::UAAtSolidPhaseOfPhaseChangeMaterial, false).get());
  EXPECT_EQ(20000, idf_t.getDouble(ThermalStorage_Pcm_SimpleFields::UAAtLiquidPhaseOfPhaseChangeMaterial, false).get());
}
