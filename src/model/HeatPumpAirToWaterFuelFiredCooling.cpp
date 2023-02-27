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

#include "HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "OutdoorAirNode.hpp"
#include "OutdoorAirNode_Impl.hpp"
#include "HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "HeatPumpAirToWaterFuelFiredHeating_Impl.hpp"
#include "BivariateFunctions.hpp"
#include "BivariateFunctions_Impl.hpp"
#include "BivariateFunctions.hpp"
#include "BivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "BivariateFunctions.hpp"
#include "BivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_AirToWater_FuelFired_Cooling_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeatPumpAirToWaterFuelFiredCooling_Impl::HeatPumpAirToWaterFuelFiredCooling_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatPumpAirToWaterFuelFiredCooling::iddObjectType());
  }

  HeatPumpAirToWaterFuelFiredCooling_Impl::HeatPumpAirToWaterFuelFiredCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatPumpAirToWaterFuelFiredCooling::iddObjectType());
  }

  HeatPumpAirToWaterFuelFiredCooling_Impl::HeatPumpAirToWaterFuelFiredCooling_Impl(const HeatPumpAirToWaterFuelFiredCooling_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeatPumpAirToWaterFuelFiredCooling_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatPumpAirToWaterFuelFiredCooling_Impl::iddObjectType() const {
    return HeatPumpAirToWaterFuelFiredCooling::iddObjectType();
  }

  Connection HeatPumpAirToWaterFuelFiredCooling_Impl::waterInletNode() const {
    boost::optional<Connection> value = optionalWaterInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Water Inlet Node attached.");
    }
    return value.get();
  }

  Connection HeatPumpAirToWaterFuelFiredCooling_Impl::waterOutletNode() const {
    boost::optional<Connection> value = optionalWaterOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Water Outlet Node attached.");
    }
    return value.get();
  }

  boost::optional<OutdoorAirNode> HeatPumpAirToWaterFuelFiredCooling_Impl::airSourceNode() const {
    return getObject<ModelObject>().getModelObjectTarget<OutdoorAirNode>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AirSourceNodeName);
  }

  boost::optional<HeatPumpAirToWaterFuelFiredHeating> HeatPumpAirToWaterFuelFiredCooling_Impl::companionHeatingHeatPump() const {
    return getObject<ModelObject>().getModelObjectTarget<HeatPumpAirToWaterFuelFiredHeating>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CompanionHeatingHeatPumpName);
  }

  std::string HeatPumpAirToWaterFuelFiredCooling_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_HeatPump_AirToWater_FuelFired_CoolingFields::EndUseSubcategory);
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::nominalCoolingCapacity() const {
    return getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity,true);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::isNominalCoolingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> HeatPumpAirToWaterFuelFiredCooling_Impl::autosizedNominalCoolingCapacity() {
    return getAutosizedValue("TODO_CHECK_SQL Nominal Cooling Capacity", "W");
  }

  double HeatPumpAirToWaterFuelFiredCooling_Impl::nominalCOP() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::designFlowRate() const {
    return getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate,true);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::isDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> HeatPumpAirToWaterFuelFiredCooling_Impl::autosizedDesignFlowRate() {
    return getAutosizedValue("TODO_CHECK_SQL Design Flow Rate", "m3/s");
  }

  double HeatPumpAirToWaterFuelFiredCooling_Impl::designSupplyTemperature() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignSupplyTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::designTemperatureLift() const {
    return getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignTemperatureLift,true);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::isDesignTemperatureLiftAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignTemperatureLift, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> HeatPumpAirToWaterFuelFiredCooling_Impl::autosizedDesignTemperatureLift() {
    return getAutosizedValue("TODO_CHECK_SQL Design Temperature Lift", "deltaC");
  }

  double HeatPumpAirToWaterFuelFiredCooling_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling_Impl::flowMode() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FlowMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling_Impl::outdoorAirTemperatureCurveInputVariable() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::OutdoorAirTemperatureCurveInputVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling_Impl::waterTemperatureCurveInputVariable() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterTemperatureCurveInputVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  BivariateFunctions HeatPumpAirToWaterFuelFiredCooling_Impl::normalizedCapacityFunctionofTemperatureCurve() const {
    boost::optional<BivariateFunctions> value = optionalNormalizedCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Normalized Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  BivariateFunctions HeatPumpAirToWaterFuelFiredCooling_Impl::fuelEnergyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<BivariateFunctions> value = optionalFuelEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Fuel Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  UnivariateFunctions HeatPumpAirToWaterFuelFiredCooling_Impl::fuelEnergyInputRatioFunctionofPLRCurve() const {
    boost::optional<UnivariateFunctions> value = optionalFuelEnergyInputRatioFunctionofPLRCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Fuel Energy Input Ratio Functionof PLRCurve attached.");
    }
    return value.get();
  }

  double HeatPumpAirToWaterFuelFiredCooling_Impl::minimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::MinimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpAirToWaterFuelFiredCooling_Impl::maximumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::MaximumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredCooling_Impl::cyclingRatioFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CyclingRatioFactorCurveName);
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::nominalAuxiliaryElectricPower() const {
    return getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalAuxiliaryElectricPower,true);
  }

  boost::optional<BivariateFunctions> HeatPumpAirToWaterFuelFiredCooling_Impl::auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredCooling_Impl::auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName);
  }

  double HeatPumpAirToWaterFuelFiredCooling_Impl::standbyElectricPower() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::StandbyElectricPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setWaterInletNode(const Connection& connection) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterInletNodeName, connection.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setWaterOutletNode(const Connection& connection) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterOutletNodeName, connection.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setAirSourceNode(const OutdoorAirNode& outdoorAirNode) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AirSourceNodeName, outdoorAirNode.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetAirSourceNode() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AirSourceNodeName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setCompanionHeatingHeatPump(const HeatPumpAirToWaterFuelFiredHeating& heatPumpAirToWaterFuelFiredHeating) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CompanionHeatingHeatPumpName, heatPumpAirToWaterFuelFiredHeating.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetCompanionHeatingHeatPump() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CompanionHeatingHeatPumpName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setFuelType(const std::string& fuelType) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelType, fuelType);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setNominalCoolingCapacity(double nominalCoolingCapacity) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, nominalCoolingCapacity);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetNominalCoolingCapacity() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, "");
    OS_ASSERT(result);
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::autosizeNominalCoolingCapacity() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setNominalCOP(double nominalCOP) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCOP, nominalCOP);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setDesignFlowRate(double designFlowRate) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, designFlowRate);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetDesignFlowRate() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, "");
    OS_ASSERT(result);
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::autosizeDesignFlowRate() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setDesignSupplyTemperature(double designSupplyTemperature) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignSupplyTemperature, designSupplyTemperature);
    OS_ASSERT(result);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setDesignTemperatureLift(double designTemperatureLift) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignTemperatureLift, designTemperatureLift);
    OS_ASSERT(result);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::autosizeDesignTemperatureLift() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignTemperatureLift, "autosize");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::SizingFactor, sizingFactor);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setFlowMode(const std::string& flowMode) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FlowMode, flowMode);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setOutdoorAirTemperatureCurveInputVariable(const std::string& outdoorAirTemperatureCurveInputVariable) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::OutdoorAirTemperatureCurveInputVariable, outdoorAirTemperatureCurveInputVariable);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterTemperatureCurveInputVariable, waterTemperatureCurveInputVariable);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setNormalizedCapacityFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NormalizedCapacityFunctionofTemperatureCurveName, bivariateFunctions.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setFuelEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName, bivariateFunctions.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setFuelEnergyInputRatioFunctionofPLRCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofPLRCurveName, univariateFunctions.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::MaximumPartLoadRatio, maximumPartLoadRatio);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setCyclingRatioFactorCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CyclingRatioFactorCurveName, univariateFunctions.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetCyclingRatioFactorCurve() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CyclingRatioFactorCurveName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalAuxiliaryElectricPower, nominalAuxiliaryElectricPower);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetNominalAuxiliaryElectricPower() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalAuxiliaryElectricPower, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName, bivariateFunctions.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName, univariateFunctions.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredCooling_Impl::setStandbyElectricPower(double standbyElectricPower) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::StandbyElectricPower, standbyElectricPower);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::autosize() {
    autosizeNominalCoolingCapacity();
    autosizeDesignFlowRate();
    autosizeDesignTemperatureLift();
  }

  void HeatPumpAirToWaterFuelFiredCooling_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedNominalCoolingCapacity();
    if (val) {
      setNominalCoolingCapacity(val.get());
    }

    val = autosizedDesignFlowRate();
    if (val) {
      setDesignFlowRate(val.get());
    }

    val = autosizedDesignTemperatureLift();
    if (val) {
      setDesignTemperatureLift(val.get());
    }

  }

  boost::optional<Connection> HeatPumpAirToWaterFuelFiredCooling_Impl::optionalWaterInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterInletNodeName);
  }

  boost::optional<Connection> HeatPumpAirToWaterFuelFiredCooling_Impl::optionalWaterOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterOutletNodeName);
  }

  boost::optional<BivariateFunctions> HeatPumpAirToWaterFuelFiredCooling_Impl::optionalNormalizedCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NormalizedCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<BivariateFunctions> HeatPumpAirToWaterFuelFiredCooling_Impl::optionalFuelEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredCooling_Impl::optionalFuelEnergyInputRatioFunctionofPLRCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofPLRCurveName);
  }

} // detail

HeatPumpAirToWaterFuelFiredCooling::HeatPumpAirToWaterFuelFiredCooling(const Model& model)
  : StraightComponent(HeatPumpAirToWaterFuelFiredCooling::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterInletNodeName
  //     OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterOutletNodeName
  //     OS_HeatPump_AirToWater_FuelFired_CoolingFields::NormalizedCapacityFunctionofTemperatureCurveName
  //     OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName
  //     OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofPLRCurveName
  bool ok = true;
  // ok = setWaterInletNode();
  OS_ASSERT(ok);
  // ok = setWaterOutletNode();
  OS_ASSERT(ok);
  // ok = setFuelType();
  OS_ASSERT(ok);
  // ok = setNominalCOP();
  OS_ASSERT(ok);
  // setDesignSupplyTemperature();
  // setDesignTemperatureLift();
  // ok = setSizingFactor();
  OS_ASSERT(ok);
  // ok = setFlowMode();
  OS_ASSERT(ok);
  // ok = setOutdoorAirTemperatureCurveInputVariable();
  OS_ASSERT(ok);
  // ok = setWaterTemperatureCurveInputVariable();
  OS_ASSERT(ok);
  // ok = setNormalizedCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setFuelEnergyInputRatioFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setFuelEnergyInputRatioFunctionofPLRCurve();
  OS_ASSERT(ok);
  // ok = setMinimumPartLoadRatio();
  OS_ASSERT(ok);
  // ok = setMaximumPartLoadRatio();
  OS_ASSERT(ok);
  // ok = setStandbyElectricPower();
  OS_ASSERT(ok);
}

IddObjectType HeatPumpAirToWaterFuelFiredCooling::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatPump_AirToWater_FuelFired_Cooling);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredCooling::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelType);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredCooling::flowModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_CoolingFields::FlowMode);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredCooling::outdoorAirTemperatureCurveInputVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_CoolingFields::OutdoorAirTemperatureCurveInputVariable);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredCooling::waterTemperatureCurveInputVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterTemperatureCurveInputVariable);
}

Connection HeatPumpAirToWaterFuelFiredCooling::waterInletNode() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->waterInletNode();
}

Connection HeatPumpAirToWaterFuelFiredCooling::waterOutletNode() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->waterOutletNode();
}

boost::optional<OutdoorAirNode> HeatPumpAirToWaterFuelFiredCooling::airSourceNode() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->airSourceNode();
}

boost::optional<HeatPumpAirToWaterFuelFiredHeating> HeatPumpAirToWaterFuelFiredCooling::companionHeatingHeatPump() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->companionHeatingHeatPump();
}

std::string HeatPumpAirToWaterFuelFiredCooling::fuelType() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->fuelType();
}

std::string HeatPumpAirToWaterFuelFiredCooling::endUseSubcategory() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->endUseSubcategory();
}

bool HeatPumpAirToWaterFuelFiredCooling::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->isEndUseSubcategoryDefaulted();
}

boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::nominalCoolingCapacity() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->nominalCoolingCapacity();
}

bool HeatPumpAirToWaterFuelFiredCooling::isNominalCoolingCapacityAutosized() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->isNominalCoolingCapacityAutosized();
}

  boost::optional <double> HeatPumpAirToWaterFuelFiredCooling::autosizedNominalCoolingCapacity() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizedNominalCoolingCapacity();
  }

double HeatPumpAirToWaterFuelFiredCooling::nominalCOP() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->nominalCOP();
}

boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::designFlowRate() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->designFlowRate();
}

bool HeatPumpAirToWaterFuelFiredCooling::isDesignFlowRateAutosized() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->isDesignFlowRateAutosized();
}

  boost::optional <double> HeatPumpAirToWaterFuelFiredCooling::autosizedDesignFlowRate() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizedDesignFlowRate();
  }

double HeatPumpAirToWaterFuelFiredCooling::designSupplyTemperature() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->designSupplyTemperature();
}

boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::designTemperatureLift() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->designTemperatureLift();
}

bool HeatPumpAirToWaterFuelFiredCooling::isDesignTemperatureLiftAutosized() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->isDesignTemperatureLiftAutosized();
}

  boost::optional <double> HeatPumpAirToWaterFuelFiredCooling::autosizedDesignTemperatureLift() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizedDesignTemperatureLift();
  }

double HeatPumpAirToWaterFuelFiredCooling::sizingFactor() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->sizingFactor();
}

std::string HeatPumpAirToWaterFuelFiredCooling::flowMode() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->flowMode();
}

std::string HeatPumpAirToWaterFuelFiredCooling::outdoorAirTemperatureCurveInputVariable() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->outdoorAirTemperatureCurveInputVariable();
}

std::string HeatPumpAirToWaterFuelFiredCooling::waterTemperatureCurveInputVariable() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->waterTemperatureCurveInputVariable();
}

BivariateFunctions HeatPumpAirToWaterFuelFiredCooling::normalizedCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->normalizedCapacityFunctionofTemperatureCurve();
}

BivariateFunctions HeatPumpAirToWaterFuelFiredCooling::fuelEnergyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->fuelEnergyInputRatioFunctionofTemperatureCurve();
}

UnivariateFunctions HeatPumpAirToWaterFuelFiredCooling::fuelEnergyInputRatioFunctionofPLRCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->fuelEnergyInputRatioFunctionofPLRCurve();
}

double HeatPumpAirToWaterFuelFiredCooling::minimumPartLoadRatio() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->minimumPartLoadRatio();
}

double HeatPumpAirToWaterFuelFiredCooling::maximumPartLoadRatio() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->maximumPartLoadRatio();
}

boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredCooling::cyclingRatioFactorCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->cyclingRatioFactorCurve();
}

boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::nominalAuxiliaryElectricPower() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->nominalAuxiliaryElectricPower();
}

boost::optional<BivariateFunctions> HeatPumpAirToWaterFuelFiredCooling::auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
}

boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredCooling::auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->auxiliaryElectricEnergyInputRatioFunctionofPLRCurve();
}

double HeatPumpAirToWaterFuelFiredCooling::standbyElectricPower() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->standbyElectricPower();
}

bool HeatPumpAirToWaterFuelFiredCooling::setWaterInletNode(const Connection& connection) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setWaterInletNode(connection);
}

bool HeatPumpAirToWaterFuelFiredCooling::setWaterOutletNode(const Connection& connection) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setWaterOutletNode(connection);
}

bool HeatPumpAirToWaterFuelFiredCooling::setAirSourceNode(const OutdoorAirNode& outdoorAirNode) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setAirSourceNode(outdoorAirNode);
}

void HeatPumpAirToWaterFuelFiredCooling::resetAirSourceNode() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetAirSourceNode();
}

bool HeatPumpAirToWaterFuelFiredCooling::setCompanionHeatingHeatPump(const HeatPumpAirToWaterFuelFiredHeating& heatPumpAirToWaterFuelFiredHeating) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setCompanionHeatingHeatPump(heatPumpAirToWaterFuelFiredHeating);
}

void HeatPumpAirToWaterFuelFiredCooling::resetCompanionHeatingHeatPump() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetCompanionHeatingHeatPump();
}

bool HeatPumpAirToWaterFuelFiredCooling::setFuelType(const std::string& fuelType) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setFuelType(fuelType);
}

bool HeatPumpAirToWaterFuelFiredCooling::setEndUseSubcategory(const std::string& endUseSubcategory) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void HeatPumpAirToWaterFuelFiredCooling::resetEndUseSubcategory() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetEndUseSubcategory();
}

bool HeatPumpAirToWaterFuelFiredCooling::setNominalCoolingCapacity(double nominalCoolingCapacity) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setNominalCoolingCapacity(nominalCoolingCapacity);
}

void HeatPumpAirToWaterFuelFiredCooling::resetNominalCoolingCapacity() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetNominalCoolingCapacity();
}

void HeatPumpAirToWaterFuelFiredCooling::autosizeNominalCoolingCapacity() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizeNominalCoolingCapacity();
}

bool HeatPumpAirToWaterFuelFiredCooling::setNominalCOP(double nominalCOP) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setNominalCOP(nominalCOP);
}

bool HeatPumpAirToWaterFuelFiredCooling::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setDesignFlowRate(designFlowRate);
}

void HeatPumpAirToWaterFuelFiredCooling::resetDesignFlowRate() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetDesignFlowRate();
}

void HeatPumpAirToWaterFuelFiredCooling::autosizeDesignFlowRate() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizeDesignFlowRate();
}

bool HeatPumpAirToWaterFuelFiredCooling::setDesignSupplyTemperature(double designSupplyTemperature) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setDesignSupplyTemperature(designSupplyTemperature);
}

bool HeatPumpAirToWaterFuelFiredCooling::setDesignTemperatureLift(double designTemperatureLift) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setDesignTemperatureLift(designTemperatureLift);
}

void HeatPumpAirToWaterFuelFiredCooling::autosizeDesignTemperatureLift() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizeDesignTemperatureLift();
}

bool HeatPumpAirToWaterFuelFiredCooling::setSizingFactor(double sizingFactor) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setSizingFactor(sizingFactor);
}

bool HeatPumpAirToWaterFuelFiredCooling::setFlowMode(const std::string& flowMode) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setFlowMode(flowMode);
}

bool HeatPumpAirToWaterFuelFiredCooling::setOutdoorAirTemperatureCurveInputVariable(const std::string& outdoorAirTemperatureCurveInputVariable) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setOutdoorAirTemperatureCurveInputVariable(outdoorAirTemperatureCurveInputVariable);
}

bool HeatPumpAirToWaterFuelFiredCooling::setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setWaterTemperatureCurveInputVariable(waterTemperatureCurveInputVariable);
}

bool HeatPumpAirToWaterFuelFiredCooling::setNormalizedCapacityFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setNormalizedCapacityFunctionofTemperatureCurve(bivariateFunctions);
}

bool HeatPumpAirToWaterFuelFiredCooling::setFuelEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setFuelEnergyInputRatioFunctionofTemperatureCurve(bivariateFunctions);
}

bool HeatPumpAirToWaterFuelFiredCooling::setFuelEnergyInputRatioFunctionofPLRCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setFuelEnergyInputRatioFunctionofPLRCurve(univariateFunctions);
}

bool HeatPumpAirToWaterFuelFiredCooling::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
}

bool HeatPumpAirToWaterFuelFiredCooling::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
}

bool HeatPumpAirToWaterFuelFiredCooling::setCyclingRatioFactorCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setCyclingRatioFactorCurve(univariateFunctions);
}

void HeatPumpAirToWaterFuelFiredCooling::resetCyclingRatioFactorCurve() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetCyclingRatioFactorCurve();
}

bool HeatPumpAirToWaterFuelFiredCooling::setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setNominalAuxiliaryElectricPower(nominalAuxiliaryElectricPower);
}

void HeatPumpAirToWaterFuelFiredCooling::resetNominalAuxiliaryElectricPower() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetNominalAuxiliaryElectricPower();
}

bool HeatPumpAirToWaterFuelFiredCooling::setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(bivariateFunctions);
}

void HeatPumpAirToWaterFuelFiredCooling::resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
}

bool HeatPumpAirToWaterFuelFiredCooling::setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(univariateFunctions);
}

void HeatPumpAirToWaterFuelFiredCooling::resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve();
}

bool HeatPumpAirToWaterFuelFiredCooling::setStandbyElectricPower(double standbyElectricPower) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setStandbyElectricPower(standbyElectricPower);
}

/// @cond
HeatPumpAirToWaterFuelFiredCooling::HeatPumpAirToWaterFuelFiredCooling(std::shared_ptr<detail::HeatPumpAirToWaterFuelFiredCooling_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

