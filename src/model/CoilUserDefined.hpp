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

#ifndef MODEL_COILUSERDEFINED_HPP
#define MODEL_COILUSERDEFINED_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"
#include "EnergyManagementSystemProgramCallingManager.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemActuator.hpp"

namespace openstudio {
namespace model {

class EnergyManagementSystemProgramCallingManager;
class EnergyManagementSystemProgram;
class EnergyManagementSystemActuator;
class ThermalZone;

namespace detail {

  class CoilUserDefined_Impl;

} // detail

/** CoilUserDefined is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:UserDefined'. */
class MODEL_API CoilUserDefined : public WaterToAirComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilUserDefined(const Model& model);

  virtual ~CoilUserDefined() = default;

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<EnergyManagementSystemProgramCallingManager> overallModelSimulationProgramCallingManager() const;

  boost::optional<EnergyManagementSystemProgramCallingManager> modelSetupandSizingProgramCallingManager() const;

  boost::optional<EnergyManagementSystemProgram> overallSimulationProgram() const;

  boost::optional<EnergyManagementSystemProgram> initializationSimulationProgram() const;

  boost::optional<EnergyManagementSystemActuator> airOutletTemperatureActuator() const;

  boost::optional<EnergyManagementSystemActuator> airOutletHumidityRatioActuator() const;

  boost::optional<EnergyManagementSystemActuator> airMassFlowRateActuator() const;

  boost::optional<EnergyManagementSystemActuator> plantMinimumMassFlowRateActuator() const;

  boost::optional<EnergyManagementSystemActuator> plantMaximumMassFlowRateActuator() const;

  boost::optional<EnergyManagementSystemActuator> plantDesignVolumeFlowRateActuator() const;

  boost::optional<EnergyManagementSystemActuator> plantMassFlowRateActuator() const;

  boost::optional<EnergyManagementSystemActuator> plantOutletTemperatureActuator() const;

  bool plantConnectionisUsed() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  boost::optional<ThermalZone> ambientZone() const;

  //@}
  /** @name Setters */
  //@{

  bool setOverallModelSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& erlProgram);

  void resetOverallModelSimulationProgramCallingManager();

  bool setModelSetupandSizingProgramCallingManager(const EnergyManagementSystemProgramCallingManager& erlProgram);

  void resetModelSetupandSizingProgramCallingManager();

  bool setOverallSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

  void resetOverallSimulationProgram();

  bool setInitializationSimulationProgramName(const EnergyManagementSystemProgram& energyManagementSystemProgram);

  void resetInitializationSimulationProgramName();

  bool setPlantConnectionisUsed(bool plantConnectionisUsed);

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  bool setAmbientZone(const ThermalZone& thermalZone);

  void resetAmbientZone();

  bool setAirOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

  bool setAirOutletHumidityRatioActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

  bool setAirMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

  bool setPlantMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

  bool setPlantMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

  bool setPlantDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

  bool setPlantMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

  bool setPlantOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

  EnergyManagementSystemProgram
      createInitProgram(const EnergyManagementSystemActuator& aota, const EnergyManagementSystemActuator& aohra,
          const EnergyManagementSystemActuator& amfra, const EnergyManagementSystemActuator& pminmfra,
          const EnergyManagementSystemActuator& pmaxmfra, const EnergyManagementSystemActuator& pdvfra,
          const EnergyManagementSystemActuator& pmmfra, const EnergyManagementSystemActuator& pota, const Model& model);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  using ImplType = detail::CoilUserDefined_Impl;

  explicit CoilUserDefined(std::shared_ptr<detail::CoilUserDefined_Impl> impl);

  friend class detail::CoilUserDefined_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilUserDefined");
};

/** \relates CoilUserDefined*/
using OptionalCoilUserDefined = boost::optional<CoilUserDefined>;

/** \relates CoilUserDefined*/
using CoilUserDefinedVector = std::vector<CoilUserDefined>;

} // model
} // openstudio

#endif // MODEL_COILUSERDEFINED_HPP

