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
#include "../../model/ThermalStorageHeatingPair.hpp"
#include "../../model/ThermalStorageHeatingPair_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/ThermalStorage_Heating_Pair_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateThermalStorageHeatingPair(ThermalStorageHeatingPair& modelObject) {
    IdfObject idfObject(IddObjectType::ThermalStorage_Heating_Pair);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // Heating Coil
    {
      auto mo = modelObject.heatingCoil();
      if (auto idf = translateAndMapModelObject(mo)) {
        idfObject.setString(ThermalStorage_Heating_PairFields::HeatingCoilObjectType, idf->iddObject().name());
        idfObject.setString(ThermalStorage_Heating_PairFields::HeatingCoilName, idf->name().get());
      }
    }

    // Tank
    {
      auto mo = modelObject.tank();
      if (auto idf = translateAndMapModelObject(mo)) {
        idfObject.setString(ThermalStorage_Heating_PairFields::TankObjectType, idf->iddObject().name());
        idfObject.setString(ThermalStorage_Heating_PairFields::TankName, idf->name().get());
      }
    }

    // Maximum Peak Operation Hours
    {
      auto value = modelObject.maximumPeakOperationHours();
      idfObject.setDouble(ThermalStorage_Heating_PairFields::MaximumPeakOperationHours, value);
    }

    // Temperature Change In Tank Through Operation
    {
      auto value = modelObject.temperatureChangeInTankThroughOperation();
      idfObject.setDouble(ThermalStorage_Heating_PairFields::TemperatureChangeInTankThroughOperation, value);
    }

    // Recovery Unit
    {
      auto mo = modelObject.recoveryUnit();
      if (auto idf = translateAndMapModelObject(mo)) {
        idfObject.setString(ThermalStorage_Heating_PairFields::RecoveryUnitType, idf->iddObject().name());
        idfObject.setString(ThermalStorage_Heating_PairFields::RecoveryUnitName, idf->name().get());
      }
    }

    boost::optional<double> value;

    // Capacity Ratio Of Recovery Unit To Main Cooling Coil
    if ((value = modelObject.capacityRatioOfRecoveryUnitToMainCoolingCoil())) {
      idfObject.setDouble(ThermalStorage_Heating_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
