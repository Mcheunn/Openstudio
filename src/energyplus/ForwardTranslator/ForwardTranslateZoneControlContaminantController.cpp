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

#include "../ForwardTranslator.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneControlContaminantController.hpp"
#include "../../model/ZoneControlContaminantController_Impl.hpp"
#include <utilities/idd/ZoneControl_ContaminantController_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneControlContaminantController(ZoneControlContaminantController& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::ZoneControl_ContaminantController);

    m_idfObjects.push_back(idfObject);

    {
      auto s = modelObject.name();
      idfObject.setName(s.get());
    }

    if (auto tz = modelObject.getImpl<model::detail::ZoneControlContaminantController_Impl>()->controlledZone()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::ZoneName, tz->name().get());
    }

    if (auto s = modelObject.carbonDioxideControlAvailabilitySchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::CarbonDioxideControlAvailabilityScheduleName, s->name().get());
    }

    if (auto s = modelObject.carbonDioxideSetpointSchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::CarbonDioxideSetpointScheduleName, s->name().get());
    }

    if (auto s = modelObject.minimumCarbonDioxideConcentrationSchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::MinimumCarbonDioxideConcentrationScheduleName, s->name().get());
    }

    if (auto s = modelObject.genericContaminantControlAvailabilitySchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::GenericContaminantControlAvailabilityScheduleName, s->name().get());
    }

    if (auto s = modelObject.genericContaminantSetpointSchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::GenericContaminantSetpointScheduleName, s->name().get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
