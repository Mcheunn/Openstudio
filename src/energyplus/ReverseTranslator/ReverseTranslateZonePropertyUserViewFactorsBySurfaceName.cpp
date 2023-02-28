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

#include "../../model/ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/ZoneProperty_UserViewFactors_BySurfaceName_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/Space_FieldEnums.hxx>
#include <utilities/idd/SpaceList_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZonePropertyUserViewFactorsBySurfaceName(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneProperty_UserViewFactors_BySurfaceName) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZonePropertyUserViewFactorsBySurfaceName");
      return boost::none;
    }

    boost::optional<WorkspaceObject> target =
      workspaceObject.getTarget(ZoneProperty_UserViewFactors_BySurfaceNameFields::ZoneorZoneListorSpaceorSpaceListName);

    std::vector<ThermalZone> thermalZones;
    if (target) {

      std::vector<WorkspaceObject> i_zones;
      if (target->iddObject().type() == IddObjectType::Zone) {

        i_zones.push_back(target.get());

      } else if (target->iddObject().type() == IddObjectType::ZoneList) {

        for (const IdfExtensibleGroup& zeg : target->extensibleGroups()) {
          OptionalWorkspaceObject target_zone = zeg.cast<WorkspaceExtensibleGroup>().getTarget(ZoneListExtensibleFields::ZoneName);
          if (target_zone) {
            i_zones.push_back(target_zone.get());
          }
        }

      } else if (target->iddObject().type() == IddObjectType::Space) {

        boost::optional<WorkspaceObject> target_zone = target->getTarget(SpaceFields::ZoneName);
        if (target_zone) {
          i_zones.push_back(target_zone.get());
        }

      } else if (target->iddObject().type() == IddObjectType::SpaceList) {

        for (const IdfExtensibleGroup& seg : target->extensibleGroups()) {
          OptionalWorkspaceObject target_space = seg.cast<WorkspaceExtensibleGroup>().getTarget(SpaceListExtensibleFields::SpaceName);
          if (target_space) {
            boost::optional<WorkspaceObject> target_zone = target_space->getTarget(SpaceFields::ZoneName);
            if (target_zone) {
              i_zones.push_back(target_zone.get());
            }
          }
        }

      } else {
        LOG(Error,
            "For ZonePropertyUserViewFactorsBySurfaceName, an extensible group entry is neither a Zone or ZoneList or Space or SpaceList, skipping.");
        return boost::none;
      }

      for (const WorkspaceObject& i_zone : i_zones) {
        // Zone is translated, and a Space is returned instead
        OptionalModelObject _mo = translateAndMapWorkspaceObject(i_zone);
        if (_mo) {
          if (boost::optional<Space> _space = _mo->optionalCast<Space>()) {
            if (auto _zone = _space->thermalZone()) {
              thermalZones.push_back(_zone.get());
            }
          }
        }
      }
    }

    if (thermalZones.empty()) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find associated ThermalZone.");

      return boost::none;
    }

    boost::optional<ModelObject> result;
    for (const ThermalZone& thermalZone : thermalZones) {

      openstudio::model::ZonePropertyUserViewFactorsBySurfaceName zoneProp = thermalZone.getZonePropertyUserViewFactorsBySurfaceName();

      if (!result) {
        result = zoneProp;
      }

      for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
        auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();

        boost::optional<WorkspaceObject> fromTarget =
          workspaceGroup.getTarget(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurface);
        boost::optional<WorkspaceObject> toTarget = workspaceGroup.getTarget(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurface);

        boost::optional<ModelObject> toModelObject;
        boost::optional<ModelObject> fromModelObject = translateAndMapWorkspaceObject(*fromTarget);
        if (fromTarget != toTarget) {
          toModelObject = translateAndMapWorkspaceObject(*toTarget);
        } else {
          toModelObject = fromModelObject;
        }

        OptionalDouble _viewFactor = workspaceGroup.getDouble(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor);

        // If the objects translated ok, and the viewFactor double value exists
        if (fromModelObject && toModelObject && _viewFactor) {
          // try to create a viewFactor, which will throw in case wrong type, or wrong double value. Here we let it slide though
          try {
            ViewFactor viewFactor(fromModelObject.get(), toModelObject.get(), _viewFactor.get());
            // add the view factor
            if (!zoneProp.addViewFactor(viewFactor)) {
              LOG(Warn, "Adding ViewFactor in ThermalZone " << thermalZone.nameString() << " failed for viewFactor=" << viewFactor << ".");
            }
          } catch (...) {
            // The ViewFactor Ctor threw, so there's a wrong type, or a wrong double
            LOG(Error, "Could not create ViewFactor in ThermalZone "
                         << thermalZone.nameString() << " for fromModelObject=(" << fromModelObject->briefDescription() << "), toModelObject=("
                         << toModelObject->briefDescription() << ") and viewFactor=" << _viewFactor.get() << ".");
          }
        }
      }
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
