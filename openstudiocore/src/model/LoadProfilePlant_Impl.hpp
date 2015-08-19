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

#ifndef MODEL_LOADPROFILEPLANT_IMPL_HPP
#define MODEL_LOADPROFILEPLANT_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
class Schedule;
class Schedule;

namespace detail {

  /** LoadProfilePlant_Impl is a StraightComponent_Impl that is the implementation class for LoadProfilePlant.*/
  class MODEL_API LoadProfilePlant_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    LoadProfilePlant_Impl(const IdfObject& idfObject,
                          Model_Impl* model,
                          bool keepHandle);

    LoadProfilePlant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    LoadProfilePlant_Impl(const LoadProfilePlant_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    virtual ~LoadProfilePlant_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection inletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection outletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule loadSchedule() const;

    double peakFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule flowRateFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setOutletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setLoadSchedule(Schedule& schedule);

    void setPeakFlowRate(double peakFlowRate);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setFlowRateFractionSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.LoadProfilePlant");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalInletNode() const;
    boost::optional<Connection> optionalOutletNode() const;
    boost::optional<Schedule> optionalLoadSchedule() const;
    boost::optional<Schedule> optionalFlowRateFractionSchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_LOADPROFILEPLANT_IMPL_HPP

