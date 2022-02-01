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

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "ZoneHVACLowTempRadiantVarFlow.hpp"
#include "ZoneHVACLowTempRadiantVarFlow_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class CoilHeatingLowTempRadiantVarFlow_Impl;

  }  // namespace detail

  /** CoilHeatingLowTempRadiantVarFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:LowTemperatureRadiant:VariableFlow'. */
  class MODEL_API CoilHeatingLowTempRadiantVarFlow : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingLowTempRadiantVarFlow(const Model& model, Schedule& heatingControlTemperature);

    virtual ~CoilHeatingLowTempRadiantVarFlow() = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    boost::optional<double> maximumHotWaterFlow() const;

    bool isMaximumHotWaterFlowDefaulted() const;

    bool isMaximumHotWaterFlowAutosized() const;

    double heatingControlThrottlingRange() const;

    bool isHeatingControlThrottlingRangeDefaulted() const;

    boost::optional<Schedule> heatingControlTemperatureSchedule() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;
    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumHotWaterFlow(double maximumHotWaterFlow);

    void resetMaximumHotWaterFlow();

    void autosizeMaximumHotWaterFlow();

    bool setHeatingControlThrottlingRange(double heatingControlThrottlingRange);

    void resetHeatingControlThrottlingRange();

    bool setHeatingControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingControlTemperatureSchedule();

    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);
    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumHotWaterFlow() const;

    boost::optional<double> autosizedHeatingDesignCapacity() const;

    //@}
   protected:
    /// @cond
    typedef detail::CoilHeatingLowTempRadiantVarFlow_Impl ImplType;

    explicit CoilHeatingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilHeatingLowTempRadiantVarFlow_Impl> impl);

    friend class detail::CoilHeatingLowTempRadiantVarFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantVarFlow");

    // Used to find the ZoneHVAC that contains this coil
    boost::optional<ZoneHVACLowTempRadiantVarFlow> parentZoneHVAC() const;
  };

  /** \relates CoilHeatingLowTempRadiantVarFlow*/
  typedef boost::optional<CoilHeatingLowTempRadiantVarFlow> OptionalCoilHeatingLowTempRadiantVarFlow;

  /** \relates CoilHeatingLowTempRadiantVarFlow*/
  typedef std::vector<CoilHeatingLowTempRadiantVarFlow> CoilHeatingLowTempRadiantVarFlowVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP
