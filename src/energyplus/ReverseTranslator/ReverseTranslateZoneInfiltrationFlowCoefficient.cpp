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

#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_FlowCoefficient_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneInfiltrationFlowCoefficient(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneInfiltration_FlowCoefficient) {
      LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription() << " is not IddObjectType: SpaceInfiltration_FlowCoefficient");
      return boost::none;
    }

    OptionalWorkspaceObject zoneTarget = workspaceObject.getTarget(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName);
    if (!zoneTarget) {
      LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription() << " has no Zone Name assigned, not ReverseTranslating it.");
      return boost::none;
    }
    OptionalModelObject zoneModelObject = translateAndMapWorkspaceObject(*zoneTarget);
    if (!zoneModelObject) {
      LOG(Error, "For WorkspaceObject " << workspaceObject.briefDescription()
                                        << ", failed to Reverse Translate the ZoneName object, so not ReverseTranslating it.");
      return boost::none;
    }

    // create the instance
    SpaceInfiltrationFlowCoefficient spaceInfiltrationFlowCoefficient(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      spaceInfiltrationFlowCoefficient.setName(*s);
    }

    if (zoneModelObject->optionalCast<Space>()) {
      spaceInfiltrationFlowCoefficient.setSpace(zoneModelObject->cast<Space>());
    } else if (zoneModelObject->optionalCast<SpaceType>()) {
      spaceInfiltrationFlowCoefficient.setSpaceType(zoneModelObject->cast<SpaceType>());
    }

    // This is a required field technically, but it's ok if if it's missing here since not required in model API.
    if (auto target = workspaceObject.getTarget(ZoneInfiltration_FlowCoefficientFields::ScheduleName)) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          spaceInfiltrationFlowCoefficient.setSchedule(schedule);
        }
      }
    }

    boost::optional<double> value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient);
    if (value) {
      spaceInfiltrationFlowCoefficient.setFlowCoefficient(value.get());
    }

    value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient);
    if (value) {
      spaceInfiltrationFlowCoefficient.setStackCoefficient(value.get());
    }

    value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent);
    if (value) {
      spaceInfiltrationFlowCoefficient.setPressureExponent(value.get());
    }

    value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient);
    if (value) {
      spaceInfiltrationFlowCoefficient.setWindCoefficient(value.get());
    }

    value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor);
    if (value) {
      spaceInfiltrationFlowCoefficient.setShelterFactor(value.get());
    }

    return spaceInfiltrationFlowCoefficient;
  }

}  // namespace energyplus

}  // namespace openstudio
