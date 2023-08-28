/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/DistrictHeating.hpp"
#include "../../model/DistrictHeating_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/DistrictHeating_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDistrictHeating(DistrictHeating& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::DistrictHeating_Water);

    m_idfObjects.push_back(idfObject);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Name ////////////////////////////////////////////////////////////
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Inlet Node Name ////////////////////////////////////////////////////
    temp = modelObject.inletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::DistrictHeating_WaterFields::HotWaterInletNodeName, *s);
      }
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Outlet Node Name ///////////////////////////////////////////////////
    temp = modelObject.outletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(openstudio::DistrictHeating_WaterFields::HotWaterOutletNodeName, *s);
      }
    }
    ///
    ////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    //Nominal Capacity ///////////////////////////////////////////////////
    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(DistrictHeating_WaterFields::NominalCapacity, "Autosize");
    } else if ((value = modelObject.nominalCapacity())) {
      idfObject.setDouble(DistrictHeating_WaterFields::NominalCapacity, value.get());
    }
    //
    ////////////////////////////////////////////////////////////////////////

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
