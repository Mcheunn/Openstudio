/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "HeatPumpPlantLoopEIRHeating.hpp"
#include "HeatPumpPlantLoopEIRHeating_Impl.hpp"
#include "Model.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include "HeatPumpPlantLoopEIRCooling.hpp"
#include "HeatPumpPlantLoopEIRCooling_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_PlantLoop_EIR_Heating_FieldEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpPlantLoopEIRHeating::iddObjectType());
    }

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpPlantLoopEIRHeating::iddObjectType());
    }

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const HeatPumpPlantLoopEIRHeating_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpPlantLoopEIRHeating_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heat Pump Electricity Energy",
                                                   "Heat Pump Load Side Heat Transfer Energy",
                                                   "Heat Pump Source Side Heat Transfer Energy",
                                                   "Heat Pump Electricity Rate",
                                                   "Heat Pump Load Side Heat Transfer Rate",
                                                   "Heat Pump Source Side Heat Transfer Rate",
                                                   "Heat Pump Load Side Outlet Temperature",
                                                   "Heat Pump Load Side Inlet Temperature",
                                                   "Heat Pump Source Side Outlet Temperature",
                                                   "Heat Pump Source Side Inlet Temperature",
                                                   "Heat Pump Load Side Mass Flow Rate",
                                                   "Heat Pump Source Side Mass Flow Rate"};
      return result;
    }

    IddObjectType HeatPumpPlantLoopEIRHeating_Impl::iddObjectType() const {
      return HeatPumpPlantLoopEIRHeating::iddObjectType();
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::supplyInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::supplyOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::demandInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::demandOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName;
    }

    /** Convenience Function to return the Load Side Water Loop (HeatPump on supply side) **/
    boost::optional<PlantLoop> HeatPumpPlantLoopEIRHeating_Impl::loadSideWaterLoop() const {
      return WaterToWaterComponent_Impl::plantLoop();
    }

    /** Convenience Function to return the Source Side (Condenser) Water Loop (HeatPump on demand side) **/
    boost::optional<PlantLoop> HeatPumpPlantLoopEIRHeating_Impl::sourceSideWaterLoop() const {
      return WaterToWaterComponent_Impl::secondaryPlantLoop();
    }

    boost::optional<Node> HeatPumpPlantLoopEIRHeating_Impl::sourceSideWaterInletNode() const {
      boost::optional<Node> result;

      if (auto mo = demandInletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    boost::optional<Node> HeatPumpPlantLoopEIRHeating_Impl::sourceSideWaterOutletNode() const {
      boost::optional<Node> result;

      if (auto mo = demandOutletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    boost::optional<Node> HeatPumpPlantLoopEIRHeating_Impl::loadSideWaterInletNode() const {
      boost::optional<Node> result;

      if (auto mo = supplyInletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    boost::optional<Node> HeatPumpPlantLoopEIRHeating_Impl::loadSideWaterOutletNode() const {
      boost::optional<Node> result;

      if (auto mo = supplyOutletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::addToNode(Node& node) {

      // call the base class implementation to connect the component
      bool ok = WaterToWaterComponent_Impl::addToNode(node);

      // If there's a secondary plant loop, switch the condenser type to "WaterCooled"
      if (this->sourceSideWaterLoop()) {
        this->setCondenserType("WaterSource");
      }
      return ok;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::removeFromSecondaryPlantLoop() {
      // Disconnect the component
      bool ok = WaterToWaterComponent_Impl::removeFromSecondaryPlantLoop();

      // Switch the condenser type back to "AirSource"
      this->setCondenserType("AirSource");
      return ok;
    }

    std::string HeatPumpPlantLoopEIRHeating_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setCondenserType(const std::string& condenserType) {
      bool ok = false;
      if (openstudio::istringEqual("AirSource", condenserType) && (this->secondaryPlantLoop())) {
        LOG(Warn, "Cannot set condenserType to AirSource, HeatPumpPlantLoopEIRHeating '"
                    << this->name() << "' is connected to a Source Side Loop. Use removeFromSecondaryPlantLoop() instead.");
      } else if (istringEqual("WaterSource", condenserType) && !(this->secondaryPlantLoop())) {
        LOG(Warn, "Cannot set condenserType to 'WaterSource', HeatPumpPlantLoopEIRHeating '"
                    << this->name() << "' is not connected to a Source Side Loop. Use addToNode(PlantLoop&) instead.");
      } else {
        ok = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, condenserType);
      }

      return ok;
    }

    boost::optional<HeatPumpPlantLoopEIRCooling> HeatPumpPlantLoopEIRHeating_Impl::companionCoolingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpPlantLoopEIRCooling>(
        OS_HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName);
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::loadSideReferenceFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, true);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isLoadSideReferenceFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::sourceSideReferenceFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, true);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isSourceSideReferenceFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::referenceCapacity() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, true);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isReferenceCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double HeatPumpPlantLoopEIRHeating_Impl::referenceCoefficientofPerformance() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRHeating_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve HeatPumpPlantLoopEIRHeating_Impl::capacityModifierFunctionofTemperatureCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    Curve HeatPumpPlantLoopEIRHeating_Impl::electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    Curve HeatPumpPlantLoopEIRHeating_Impl::electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const {
      WorkspaceObject wo =
        getTarget(OS_HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    double HeatPumpPlantLoopEIRHeating_Impl::heatingToCoolingCapacitySizingRatio() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatingToCoolingCapacitySizingRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpPlantLoopEIRHeating_Impl::heatPumpSizingMethod() const {
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpSizingMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpPlantLoopEIRHeating_Impl::controlType() const {
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpPlantLoopEIRHeating_Impl::flowMode() const {
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::FlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRHeating_Impl::minimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::MinimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRHeating_Impl::minimumSourceInletTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::MinimumSourceInletTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRHeating_Impl::maximumSourceInletTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::MaximumSourceInletTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRHeating_Impl::minimumSupplyWaterTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_HeatingFields::MinimumSupplyWaterTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRHeating_Impl::maximumSupplyWaterTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_HeatingFields::MaximumSupplyWaterTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRHeating_Impl::dryOutdoorCorrectionFactorCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_HeatingFields::DryOutdoorCorrectionFactorCurveName);
    }

    double HeatPumpPlantLoopEIRHeating_Impl::maximumOutdoorDryBulbTemperatureForDefrostOperation() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::MaximumOutdoorDryBulbTemperatureForDefrostOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpPlantLoopEIRHeating_Impl::heatPumpDefrostControl() const {
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpDefrostControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRHeating_Impl::heatPumpDefrostTimePeriodFraction() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpDefrostTimePeriodFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRHeating_Impl::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_PlantLoop_EIR_HeatingFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRHeating_Impl::timedEmpiricalDefrostFrequencyCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostFrequencyCurveName);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRHeating_Impl::timedEmpiricalDefrostHeatLoadPenaltyCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostHeatLoadPenaltyCurveName);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRHeating_Impl::timedEmpiricalDefrostHeatInputEnergyFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostHeatInputEnergyFractionCurveName);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP) {
      return this->setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName, companionHP.handle());
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setLoadSideReferenceFlowRate(double loadSideReferenceFlowRate) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, loadSideReferenceFlowRate);
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeLoadSideReferenceFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setSourceSideReferenceFlowRate(double sourceSideReferenceFlowRate) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, sourceSideReferenceFlowRate);
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeSourceSideReferenceFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceCapacity(double referenceCapacity) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, referenceCapacity);
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeReferenceCapacity() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setSizingFactor(double sizingFactor) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, sizingFactor);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName,
                               capacityModifierFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName,
                               electricInputtoOutputRatioModifierFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
      const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName,
                               electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setHeatingToCoolingCapacitySizingRatio(double heatingToCoolingCapacitySizingRatio) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatingToCoolingCapacitySizingRatio, heatingToCoolingCapacitySizingRatio);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setHeatPumpSizingMethod(const std::string& heatPumpSizingMethod) {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpSizingMethod, heatPumpSizingMethod);
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setControlType(const std::string& controlType) {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ControlType, controlType);
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setFlowMode(const std::string& flowMode) {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::FlowMode, flowMode);
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setMinimumSourceInletTemperature(double minimumSourceInletTemperature) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::MinimumSourceInletTemperature, minimumSourceInletTemperature);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setMaximumSourceInletTemperature(double maximumSourceInletTemperature) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::MaximumSourceInletTemperature, maximumSourceInletTemperature);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setMinimumSupplyWaterTemperatureCurve(const Curve& minimumSupplyWaterTemperatureCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::MinimumSupplyWaterTemperatureCurveName, minimumSupplyWaterTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setMaximumSupplyWaterTemperatureCurve(const Curve& maximumSupplyWaterTemperatureCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::MaximumSupplyWaterTemperatureCurveName, maximumSupplyWaterTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setDryOutdoorCorrectionFactorCurve(const Curve& dryOutdoorCorrectionFactorCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::DryOutdoorCorrectionFactorCurveName, dryOutdoorCorrectionFactorCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setMaximumOutdoorDryBulbTemperatureForDefrostOperation(
      double maximumOutdoorDryBulbTemperatureForDefrostOperation) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::MaximumOutdoorDryBulbTemperatureForDefrostOperation,
                       maximumOutdoorDryBulbTemperatureForDefrostOperation);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setHeatPumpDefrostControl(const std::string& heatPumpDefrostControl) {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpDefrostControl, heatPumpDefrostControl);
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setHeatPumpDefrostTimePeriodFraction(double heatPumpDefrostTimePeriodFraction) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatPumpDefrostTimePeriodFraction, heatPumpDefrostTimePeriodFraction);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setDefrostEnergyInputRatioFunctionofTemperatureCurve(
      const Curve& defrostEnergyInputRatioFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName,
                               defrostEnergyInputRatioFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setTimedEmpiricalDefrostFrequencyCurve(const Curve& timedEmpiricalDefrostFrequencyCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostFrequencyCurveName, timedEmpiricalDefrostFrequencyCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setTimedEmpiricalDefrostHeatLoadPenaltyCurve(const Curve& timedEmpiricalDefrostHeatLoadPenaltyCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostHeatLoadPenaltyCurveName,
                               timedEmpiricalDefrostHeatLoadPenaltyCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setTimedEmpiricalDefrostHeatInputEnergyFractionCurve(
      const Curve& timedEmpiricalDefrostHeatInputEnergyFractionCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::TimedEmpiricalDefrostHeatInputEnergyFractionCurveName,
                               timedEmpiricalDefrostHeatInputEnergyFractionCurve.handle());
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedLoadSideReferenceFlowRate() const {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedSourceSideReferenceFlowRate() const {
      return getAutosizedValue("Design Size Source Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedReferenceCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosize() {
      autosizeLoadSideReferenceFlowRate();
      autosizeSourceSideReferenceFlowRate();
      autosizeReferenceCapacity();
    }

    void HeatPumpPlantLoopEIRHeating_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedLoadSideReferenceFlowRate();
      if (val) {
        setLoadSideReferenceFlowRate(val.get());
      }

      val = autosizedSourceSideReferenceFlowRate();
      if (val) {
        setSourceSideReferenceFlowRate(val.get());
      }

      val = autosizedReferenceCapacity();
      if (val) {
        setReferenceCapacity(val.get());
      }
    }

    ComponentType HeatPumpPlantLoopEIRHeating_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> HeatPumpPlantLoopEIRHeating_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> HeatPumpPlantLoopEIRHeating_Impl::heatingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<AppGFuelType> HeatPumpPlantLoopEIRHeating_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::HeatPump};
    }

  }  // namespace detail

  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(const Model& model, const Curve& capacityModifierFunctionofTemperatureCurve,
                                                           const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve,
                                                           const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>());

    autosizeLoadSideReferenceFlowRate();
    autosizeSourceSideReferenceFlowRate();
    autosizeReferenceCapacity();

    bool ok = setCapacityModifierFunctionofTemperatureCurve(capacityModifierFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s capacity modifier function of temperature curve to "
                                     << capacityModifierFunctionofTemperatureCurve.briefDescription() << ".");
    }

    ok = setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s electric input to output ratio modifier function of temperature curve to "
                                     << electricInputtoOutputRatioModifierFunctionofTemperatureCurve.briefDescription() << ".");
    }

    ok = setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s electric input to output ratio modifier function of part load ratio curve to "
                                     << electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.briefDescription() << ".");
    }

    // Bypass the check by calling setString directly
    setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, "AirSource");

    setReferenceCoefficientofPerformance(7.5);  // IDD default
    setSizingFactor(1.0);
    setHeatingToCoolingCapacitySizingRatio(1.0);
    setHeatPumpSizingMethod("CoolingCapacity");
    setControlType("Load");
    setFlowMode("ConstantFlow");
    setMinimumPartLoadRatio(0.0);
    setMinimumSourceInletTemperature(-100.0);
    setMaximumSourceInletTemperature(100.0);
    setMaximumOutdoorDryBulbTemperatureForDefrostOperation(10.0);
    setHeatPumpDefrostControl("None");
    setHeatPumpDefrostTimePeriodFraction(0.058333);
  }

  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(const Model& model)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>());

    autosizeLoadSideReferenceFlowRate();
    autosizeSourceSideReferenceFlowRate();
    autosizeReferenceCapacity();

    // Note: The default HAS to be AirSource (since it's not connected to a plantLoop...)
    // But in both PlantLoopHeatPump_EIR_WaterSource.idf and PlantLoopHeatPump_EIR_AirSource.idf, the curves are flatlines
    // The only difference is the Source Side Flow Rate (m3/s), but here it's autosized anyways

    CurveBiquadratic capacityModifierFunctionofTemperatureCurve(model);
    capacityModifierFunctionofTemperatureCurve.setCoefficient1Constant(1);
    capacityModifierFunctionofTemperatureCurve.setCoefficient2x(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient4y(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0);
    capacityModifierFunctionofTemperatureCurve.setMinimumValueofx(5);
    capacityModifierFunctionofTemperatureCurve.setMaximumValueofx(10);
    capacityModifierFunctionofTemperatureCurve.setMinimumValueofy(24);
    capacityModifierFunctionofTemperatureCurve.setMaximumValueofy(35);
    capacityModifierFunctionofTemperatureCurve.setInputUnitTypeforX("Temperature");
    capacityModifierFunctionofTemperatureCurve.setInputUnitTypeforY("Temperature");
    bool ok = setCapacityModifierFunctionofTemperatureCurve(capacityModifierFunctionofTemperatureCurve);
    OS_ASSERT(ok);

    CurveBiquadratic electricInputtoOutputRatioModifierFunctionofTemperatureCurve(model);  // PlantLoopHeatPump_EIR_WaterSource.idf
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient1Constant(1);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient2x(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient4y(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMinimumValueofx(5);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMaximumValueofx(10);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMinimumValueofy(24);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMaximumValueofy(35);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setInputUnitTypeforX("Temperature");
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setInputUnitTypeforY("Temperature");
    ok = setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
    OS_ASSERT(ok);

    CurveQuadratic electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(model);  // PlantLoopHeatPump_EIR_WaterSource.idf
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient1Constant(1);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient2x(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient3xPOW2(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setMinimumValueofx(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setMaximumValueofx(1);
    ok = setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
    OS_ASSERT(ok);

    // Bypass the check by calling setString directly
    setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, "AirSource");

    setReferenceCoefficientofPerformance(7.5);
    setSizingFactor(1.0);
    setHeatingToCoolingCapacitySizingRatio(1.0);
    setHeatPumpSizingMethod("CoolingCapacity");
    setControlType("Load");
    setFlowMode("ConstantFlow");
    setMinimumPartLoadRatio(0.0);
    setMinimumSourceInletTemperature(-100.0);
    setMaximumSourceInletTemperature(100.0);
    setMaximumOutdoorDryBulbTemperatureForDefrostOperation(10.0);
    setHeatPumpDefrostControl("None");
    setHeatPumpDefrostTimePeriodFraction(0.058333);
  }

  IddObjectType HeatPumpPlantLoopEIRHeating::iddObjectType() {
    return {IddObjectType::OS_HeatPump_PlantLoop_EIR_Heating};
  }

  std::vector<std::string> HeatPumpPlantLoopEIRHeating::condenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeatPump_PlantLoop_EIR_HeatingFields::CondenserType);
  }

  std::vector<std::string> HeatPumpPlantLoopEIRHeating::validCondenserTypeValues() {
    return HeatPumpPlantLoopEIRHeating::condenserTypeValues();
  }

  std::string HeatPumpPlantLoopEIRHeating::condenserType() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->condenserType();
  }

  boost::optional<HeatPumpPlantLoopEIRCooling> HeatPumpPlantLoopEIRHeating::companionCoolingHeatPump() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->companionCoolingHeatPump();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::loadSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->loadSideReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::isLoadSideReferenceFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isLoadSideReferenceFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::sourceSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->sourceSideReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::isSourceSideReferenceFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isSourceSideReferenceFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::referenceCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceCapacity();
  }

  bool HeatPumpPlantLoopEIRHeating::isReferenceCapacityAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isReferenceCapacityAutosized();
  }

  double HeatPumpPlantLoopEIRHeating::referenceCoefficientofPerformance() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceCoefficientofPerformance();
  }

  double HeatPumpPlantLoopEIRHeating::sizingFactor() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->sizingFactor();
  }

  Curve HeatPumpPlantLoopEIRHeating::capacityModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->capacityModifierFunctionofTemperatureCurve();
  }

  Curve HeatPumpPlantLoopEIRHeating::electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->electricInputtoOutputRatioModifierFunctionofTemperatureCurve();
  }

  Curve HeatPumpPlantLoopEIRHeating::electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve();
  }

  double HeatPumpPlantLoopEIRHeating::heatingToCoolingCapacitySizingRatio() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingToCoolingCapacitySizingRatio();
  }

  std::string HeatPumpPlantLoopEIRHeating::heatPumpSizingMethod() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatPumpSizingMethod();
  }

  std::string HeatPumpPlantLoopEIRHeating::controlType() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->controlType();
  }

  std::string HeatPumpPlantLoopEIRHeating::flowMode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->flowMode();
  }

  double HeatPumpPlantLoopEIRHeating::minimumPartLoadRatio() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->minimumPartLoadRatio();
  }

  double HeatPumpPlantLoopEIRHeating::minimumSourceInletTemperature() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->minimumSourceInletTemperature();
  }

  double HeatPumpPlantLoopEIRHeating::maximumSourceInletTemperature() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->maximumSourceInletTemperature();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRHeating::minimumSupplyWaterTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->minimumSupplyWaterTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRHeating::maximumSupplyWaterTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->maximumSupplyWaterTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRHeating::dryOutdoorCorrectionFactorCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->dryOutdoorCorrectionFactorCurve();
  }

  double HeatPumpPlantLoopEIRHeating::maximumOutdoorDryBulbTemperatureForDefrostOperation() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->maximumOutdoorDryBulbTemperatureForDefrostOperation();
  }

  std::string HeatPumpPlantLoopEIRHeating::heatPumpDefrostControl() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatPumpDefrostControl();
  }

  double HeatPumpPlantLoopEIRHeating::heatPumpDefrostTimePeriodFraction() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatPumpDefrostTimePeriodFraction();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRHeating::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->defrostEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRHeating::timedEmpiricalDefrostFrequencyCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->timedEmpiricalDefrostFrequencyCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRHeating::timedEmpiricalDefrostHeatLoadPenaltyCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->timedEmpiricalDefrostHeatLoadPenaltyCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRHeating::timedEmpiricalDefrostHeatInputEnergyFractionCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->timedEmpiricalDefrostHeatInputEnergyFractionCurve();
  }

  bool HeatPumpPlantLoopEIRHeating::setCondenserType(const std::string& condenserType) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setCondenserType(condenserType);
  }

  bool HeatPumpPlantLoopEIRHeating::setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setCompanionCoolingHeatPump(companionHP);
  }

  bool HeatPumpPlantLoopEIRHeating::setLoadSideReferenceFlowRate(double loadSideReferenceFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setLoadSideReferenceFlowRate(loadSideReferenceFlowRate);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeLoadSideReferenceFlowRate() {
    getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeLoadSideReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::setSourceSideReferenceFlowRate(double sourceSideReferenceFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setSourceSideReferenceFlowRate(sourceSideReferenceFlowRate);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeSourceSideReferenceFlowRate() {
    getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeSourceSideReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceCapacity(double referenceCapacity) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceCapacity(referenceCapacity);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeReferenceCapacity() {
    getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeReferenceCapacity();
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
  }

  bool HeatPumpPlantLoopEIRHeating::setSizingFactor(double sizingFactor) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setSizingFactor(sizingFactor);
  }

  bool HeatPumpPlantLoopEIRHeating::setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setCapacityModifierFunctionofTemperatureCurve(
      capacityModifierFunctionofTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
    const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
    const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
      electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingToCoolingCapacitySizingRatio(double heatingToCoolingCapacitySizingRatio) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setHeatingToCoolingCapacitySizingRatio(heatingToCoolingCapacitySizingRatio);
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatPumpSizingMethod(const std::string& heatPumpSizingMethod) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setHeatPumpSizingMethod(heatPumpSizingMethod);
  }

  bool HeatPumpPlantLoopEIRHeating::setControlType(const std::string& controlType) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setControlType(controlType);
  }

  bool HeatPumpPlantLoopEIRHeating::setFlowMode(const std::string& flowMode) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setFlowMode(flowMode);
  }

  bool HeatPumpPlantLoopEIRHeating::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
  }

  bool HeatPumpPlantLoopEIRHeating::setMinimumSourceInletTemperature(double minimumSourceInletTemperature) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setMinimumSourceInletTemperature(minimumSourceInletTemperature);
  }

  bool HeatPumpPlantLoopEIRHeating::setMaximumSourceInletTemperature(double maximumSourceInletTemperature) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setMaximumSourceInletTemperature(maximumSourceInletTemperature);
  }

  bool HeatPumpPlantLoopEIRHeating::setMinimumSupplyWaterTemperatureCurve(const Curve& minimumSupplyWaterTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setMinimumSupplyWaterTemperatureCurve(minimumSupplyWaterTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setMaximumSupplyWaterTemperatureCurve(const Curve& maximumSupplyWaterTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setMaximumSupplyWaterTemperatureCurve(maximumSupplyWaterTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setDryOutdoorCorrectionFactorCurve(const Curve& dryOutdoorCorrectionFactorCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setDryOutdoorCorrectionFactorCurve(dryOutdoorCorrectionFactorCurve);
  }

  bool
    HeatPumpPlantLoopEIRHeating::setMaximumOutdoorDryBulbTemperatureForDefrostOperation(double maximumOutdoorDryBulbTemperatureForDefrostOperation) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setMaximumOutdoorDryBulbTemperatureForDefrostOperation(
      maximumOutdoorDryBulbTemperatureForDefrostOperation);
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatPumpDefrostControl(const std::string& heatPumpDefrostControl) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setHeatPumpDefrostControl(heatPumpDefrostControl);
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatPumpDefrostTimePeriodFraction(double heatPumpDefrostTimePeriodFraction) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setHeatPumpDefrostTimePeriodFraction(heatPumpDefrostTimePeriodFraction);
  }

  bool HeatPumpPlantLoopEIRHeating::setDefrostEnergyInputRatioFunctionofTemperatureCurve(
    const Curve& defrostEnergyInputRatioFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setDefrostEnergyInputRatioFunctionofTemperatureCurve(
      defrostEnergyInputRatioFunctionofTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setTimedEmpiricalDefrostFrequencyCurve(const Curve& timedEmpiricalDefrostFrequencyCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setTimedEmpiricalDefrostFrequencyCurve(timedEmpiricalDefrostFrequencyCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setTimedEmpiricalDefrostHeatLoadPenaltyCurve(const Curve& timedEmpiricalDefrostHeatLoadPenaltyCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setTimedEmpiricalDefrostHeatLoadPenaltyCurve(
      timedEmpiricalDefrostHeatLoadPenaltyCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setTimedEmpiricalDefrostHeatInputEnergyFractionCurve(
    const Curve& timedEmpiricalDefrostHeatInputEnergyFractionCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setTimedEmpiricalDefrostHeatInputEnergyFractionCurve(
      timedEmpiricalDefrostHeatInputEnergyFractionCurve);
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedLoadSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedLoadSideReferenceFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedSourceSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedSourceSideReferenceFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedReferenceCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedReferenceCapacity();
  }

  // Convenience functions
  boost::optional<PlantLoop> HeatPumpPlantLoopEIRHeating::loadSideWaterLoop() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->loadSideWaterLoop();
  }

  boost::optional<PlantLoop> HeatPumpPlantLoopEIRHeating::sourceSideWaterLoop() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->sourceSideWaterLoop();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRHeating::sourceSideWaterInletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->sourceSideWaterInletNode();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRHeating::sourceSideWaterOutletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->sourceSideWaterOutletNode();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRHeating::loadSideWaterInletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->loadSideWaterInletNode();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRHeating::loadSideWaterOutletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->loadSideWaterOutletNode();
  }

  /// @cond
  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(std::shared_ptr<detail::HeatPumpPlantLoopEIRHeating_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
