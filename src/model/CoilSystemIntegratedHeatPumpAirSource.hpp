/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_HPP
#define MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class StraightComponent;
  class HVACComponent;
  class Curve;

  namespace detail {

    class CoilSystemIntegratedHeatPumpAirSource_Impl;

  }  // namespace detail

  /** CoilSystemIntegratedHeatPumpAirSource is a StraightComponent that wraps the OpenStudio IDD object 'OS:CoilSystem:IntegratedHeatPump:AirSource'. */
  class MODEL_API CoilSystemIntegratedHeatPumpAirSource : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilSystemIntegratedHeatPumpAirSource(const Model& model);

    explicit CoilSystemIntegratedHeatPumpAirSource(const Model& model, const StraightComponent& spaceCoolingCoil,
                                                   const StraightComponent& spaceHeatingCoil, const HVACComponent& dedicatedWaterHeatingCoil,
                                                   const HVACComponent& scwhCoil, const StraightComponent& scdwhCoolingCoil,
                                                   const HVACComponent& scdwhWaterHeatingCoil, const StraightComponent& shdwhHeatingCoil,
                                                   const HVACComponent& shdwhWaterHeatingCoil);

    virtual ~CoilSystemIntegratedHeatPumpAirSource() {}

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    StraightComponent spaceCoolingCoil() const;

    StraightComponent spaceHeatingCoil() const;

    HVACComponent dedicatedWaterHeatingCoil() const;

    HVACComponent scwhCoil() const;

    StraightComponent scdwhCoolingCoil() const;

    HVACComponent scdwhWaterHeatingCoil() const;

    StraightComponent shdwhHeatingCoil() const;

    HVACComponent shdwhWaterHeatingCoil() const;

    double indoorTemperatureLimitForSCWHMode() const;

    double ambientTemperatureLimitForSCWHMode() const;

    double indoorTemperatureAboveWhichWHHasHigherPriority() const;

    double ambientTemperatureAboveWhichWHHasHigherPriority() const;

    int flagtoIndicateLoadControlInSCWHMode() const;

    int minimumSpeedLevelForSCWHMode() const;

    double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode() const;

    int minimumSpeedLevelForSCDWHMode() const;

    double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode() const;

    int minimumSpeedLevelForSHDWHMode() const;

    //@}
    /** @name Setters */
    //@{

    bool setSpaceCoolingCoil(const StraightComponent& spaceCoolingCoil);

    bool setSpaceHeatingCoil(const StraightComponent& spaceHeatingCoil);

    bool setDedicatedWaterHeatingCoil(const HVACComponent& dedicatedWaterHeatingCoil);

    bool setSCWHCoil(const HVACComponent& scwhCoil);

    bool setSCDWHCoolingCoil(const StraightComponent& scdwhCoolingCoil);

    bool setSCDWHWaterHeatingCoil(const HVACComponent& scdwhWaterHeatingCoil);

    bool setSHDWHHeatingCoil(const StraightComponent& shdwhHeatingCoil);

    bool setSHDWHWaterHeatingCoil(const HVACComponent& shdwhWaterHeatingCoil);

    bool setIndoorTemperatureLimitForSCWHMode(double indoorTemperatureLimitForSCWHMode);

    bool setAmbientTemperatureLimitForSCWHMode(double ambientTemperatureLimitForSCWHMode);

    bool setIndoorTemperatureAboveWhichWHHasHigherPriority(double indoorTemperatureAboveWhichWHHasHigherPriority);

    bool setAmbientTemperatureAboveWhichWHHasHigherPriority(double ambientTemperatureAboveWhichWHHasHigherPriority);

    bool setFlagtoIndicateLoadControlInSCWHMode(int flagtoIndicateLoadControlInSCWHMode);

    bool setMinimumSpeedLevelForSCWHMode(int minimumSpeedLevelForSCWHMode);

    bool setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode);

    bool setMinimumSpeedLevelForSCDWHMode(int minimumSpeedLevelForSCDWHMode);

    bool setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(
      double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode);

    bool setMinimumSpeedLevelForSHDWHMode(int minimumSpeedLevelForSHDWHMode);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    typedef detail::CoilSystemIntegratedHeatPumpAirSource_Impl ImplType;

    explicit CoilSystemIntegratedHeatPumpAirSource(std::shared_ptr<detail::CoilSystemIntegratedHeatPumpAirSource_Impl> impl);

    friend class detail::CoilSystemIntegratedHeatPumpAirSource_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilSystemIntegratedHeatPumpAirSource");
  };

  /** \relates CoilSystemIntegratedHeatPumpAirSource*/
  typedef boost::optional<CoilSystemIntegratedHeatPumpAirSource> OptionalCoilSystemIntegratedHeatPumpAirSource;

  /** \relates CoilSystemIntegratedHeatPumpAirSource*/
  typedef std::vector<CoilSystemIntegratedHeatPumpAirSource> CoilSystemIntegratedHeatPumpAirSourceVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_HPP