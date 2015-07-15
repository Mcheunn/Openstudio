/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_IMPL_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

class ThermalZone;
class Node;

namespace detail {

  /** SetpointManagerSingleZoneOneStageCooling_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerSingleZoneOneStageCooling.*/
  class MODEL_API SetpointManagerSingleZoneOneStageCooling_Impl : public SetpointManager_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SetpointManagerSingleZoneOneStageCooling_Impl(const IdfObject& idfObject,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    SetpointManagerSingleZoneOneStageCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    SetpointManagerSingleZoneOneStageCooling_Impl(const SetpointManagerSingleZoneOneStageCooling_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    virtual ~SetpointManagerSingleZoneOneStageCooling_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    virtual std::string controlVariable() const override;

    double coolingStageOnSupplyAirSetpointTemperature() const;

    double coolingStageOffSupplyAirSetpointTemperature() const;

    boost::optional<ThermalZone> controlZone() const;

    virtual boost::optional<Node> setpointNode() const override;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(const std::string& controlVariable);

    void setCoolingStageOnSupplyAirSetpointTemperature(double coolingStageOnSupplyAirSetpointTemperature);

    void setCoolingStageOffSupplyAirSetpointTemperature(double coolingStageOffSupplyAirSetpointTemperature);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();


    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    virtual bool setSetpointNode(const Node& node) override;

    virtual void resetSetpointNode() override;

    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneOneStageCooling");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_IMPL_HPP

