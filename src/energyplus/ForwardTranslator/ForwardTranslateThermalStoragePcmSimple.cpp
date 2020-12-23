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

#include "../ForwardTranslator.hpp"
#include "../../model/ThermalStoragePcmSimple.hpp"
#include "../../model/ThermalStoragePcmSimple_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/ThermalStorage_Pcm_Simple_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateThermalStoragePcmSimple(ThermalStoragePcmSimple& modelObject) {
    IdfObject idfObject(IddObjectType::ThermalStorage_Pcm_Simple);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // InletNodeName
    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(ThermalStorage_Pcm_SimpleFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(ThermalStorage_Pcm_SimpleFields::OutletNodeName, node->name().get());
      }
    }

    // IceStorageType
    {
      auto value = modelObject.iceStorageType();
      idfObject.setString(ThermalStorage_Pcm_SimpleFields::IceStorageType, value);
    }

    // Capacity
    {
      auto value = modelObject.capacity();
      idfObject.setDouble(ThermalStorage_Pcm_SimpleFields::Capacity, value);
    }

    boost::optional<double> value;

    // OnsetTemperatureOfPhaseChange
    if ((value = modelObject.onsetTemperatureOfPhaseChange())) {
      idfObject.setDouble(ThermalStorage_Pcm_SimpleFields::OnsetTemperatureOfPhaseChange, value.get());
    }

    // FinishTemperatureOfPhaseChange
    if ((value = modelObject.finishTemperatureOfPhaseChange())) {
      idfObject.setDouble(ThermalStorage_Pcm_SimpleFields::FinishTemperatureOfPhaseChange, value.get());
    }

    // UAAtSolidPhaseOfPhaseChangeMaterial
    if ((value = modelObject.uaAtSolidPhaseOfPhaseChangeMaterial())) {
      idfObject.setDouble(ThermalStorage_Pcm_SimpleFields::UAAtSolidPhaseOfPhaseChangeMaterial, value.get());
    }

    // UAAtLiquidPhaseOfPhaseChangeMaterial
    if ((value = modelObject.uaAtLiquidPhaseOfPhaseChangeMaterial())) {
      idfObject.setDouble(ThermalStorage_Pcm_SimpleFields::UAAtLiquidPhaseOfPhaseChangeMaterial, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
