/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "HeaderedPumpsVariableSpeed.hpp"
#include "HeaderedPumpsVariableSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_HeaderedPumps_VariableSpeed_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeaderedPumpsVariableSpeed_Impl::HeaderedPumpsVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeaderedPumpsVariableSpeed::iddObjectType());
    }

    HeaderedPumpsVariableSpeed_Impl::HeaderedPumpsVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeaderedPumpsVariableSpeed::iddObjectType());
    }

    HeaderedPumpsVariableSpeed_Impl::HeaderedPumpsVariableSpeed_Impl(const HeaderedPumpsVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeaderedPumpsVariableSpeed_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Pump Electricity Rate", "Pump Electricity Energy", "Pump Shaft Power", "Pump Fluid Heat Gain Rate", "Pump Fluid Heat Gain Energy",
        "Pump Outlet Temperature", "Pump Mass Flow Rate", "Number of pumps operating",

        // The Key is the Pump, not the zone, so it's right to report here
        // EnergyPlus/Pumps.cc::GetPumpInput()
        // TODO: Implement this check and make not static above once ModelObject return type has changed
        //if (! p.zone().empty() ) {
        "Pump Zone Total Heating Rate", "Pump Zone Total Heating Energy", "Pump Zone Convective Heating Rate", "Pump Zone Radiative Heating Rate"
        // }
      };
      return result;
    }

    IddObjectType HeaderedPumpsVariableSpeed_Impl::iddObjectType() const {
      return HeaderedPumpsVariableSpeed::iddObjectType();
    }

    std::vector<ScheduleTypeKey> HeaderedPumpsVariableSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_HeaderedPumps_VariableSpeedFields::PumpFlowRateSchedule) != e) {
        result.push_back(ScheduleTypeKey("HeaderedPumpsVariableSpeed", "Pump Flow Rate Schedule"));
      }
      return result;
    }

    boost::optional<double> HeaderedPumpsVariableSpeed_Impl::totalRatedFlowRate() const {
      return getDouble(OS_HeaderedPumps_VariableSpeedFields::TotalRatedFlowRate, true);
    }

    bool HeaderedPumpsVariableSpeed_Impl::isTotalRatedFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeaderedPumps_VariableSpeedFields::TotalRatedFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    int HeaderedPumpsVariableSpeed_Impl::numberofPumpsinBank() const {
      boost::optional<int> value = getInt(OS_HeaderedPumps_VariableSpeedFields::NumberofPumpsinBank, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeaderedPumpsVariableSpeed_Impl::flowSequencingControlScheme() const {
      boost::optional<std::string> value = getString(OS_HeaderedPumps_VariableSpeedFields::FlowSequencingControlScheme, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeaderedPumpsVariableSpeed_Impl::ratedPumpHead() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::RatedPumpHead, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> HeaderedPumpsVariableSpeed_Impl::ratedPowerConsumption() const {
      return getDouble(OS_HeaderedPumps_VariableSpeedFields::RatedPowerConsumption, true);
    }

    bool HeaderedPumpsVariableSpeed_Impl::isRatedPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeaderedPumps_VariableSpeedFields::RatedPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double HeaderedPumpsVariableSpeed_Impl::motorEfficiency() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::MotorEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeaderedPumpsVariableSpeed_Impl::fractionofMotorInefficienciestoFluidStream() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeaderedPumpsVariableSpeed_Impl::coefficient1ofthePartLoadPerformanceCurve() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeaderedPumpsVariableSpeed_Impl::coefficient2ofthePartLoadPerformanceCurve() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeaderedPumpsVariableSpeed_Impl::coefficient3ofthePartLoadPerformanceCurve() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeaderedPumpsVariableSpeed_Impl::coefficient4ofthePartLoadPerformanceCurve() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeaderedPumpsVariableSpeed_Impl::minimumFlowRateFraction() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::MinimumFlowRateFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeaderedPumpsVariableSpeed_Impl::pumpControlType() const {
      boost::optional<std::string> value = getString(OS_HeaderedPumps_VariableSpeedFields::PumpControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> HeaderedPumpsVariableSpeed_Impl::pumpFlowRateSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HeaderedPumps_VariableSpeedFields::PumpFlowRateSchedule);
    }

    boost::optional<ThermalZone> HeaderedPumpsVariableSpeed_Impl::thermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_HeaderedPumps_VariableSpeedFields::ThermalZone);
    }

    double HeaderedPumpsVariableSpeed_Impl::skinLossRadiativeFraction() const {
      boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::SkinLossRadiativeFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeaderedPumpsVariableSpeed_Impl::setTotalRatedFlowRate(boost::optional<double> totalRatedFlowRate) {
      bool result(false);
      if (totalRatedFlowRate) {
        result = setDouble(OS_HeaderedPumps_VariableSpeedFields::TotalRatedFlowRate, totalRatedFlowRate.get());
      }
      return result;
    }

    void HeaderedPumpsVariableSpeed_Impl::autosizeTotalRatedFlowRate() {
      bool result = setString(OS_HeaderedPumps_VariableSpeedFields::TotalRatedFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool HeaderedPumpsVariableSpeed_Impl::setNumberofPumpsinBank(int numberofPumpsinBank) {
      bool result = setInt(OS_HeaderedPumps_VariableSpeedFields::NumberofPumpsinBank, numberofPumpsinBank);
      OS_ASSERT(result);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setFlowSequencingControlScheme(const std::string& flowSequencingControlScheme) {
      bool result = setString(OS_HeaderedPumps_VariableSpeedFields::FlowSequencingControlScheme, flowSequencingControlScheme);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setRatedPumpHead(double ratedPumpHead) {
      bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::RatedPumpHead, ratedPumpHead);
      OS_ASSERT(result);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption) {
      bool result(false);
      if (ratedPowerConsumption) {
        result = setDouble(OS_HeaderedPumps_VariableSpeedFields::RatedPowerConsumption, ratedPowerConsumption.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void HeaderedPumpsVariableSpeed_Impl::autosizeRatedPowerConsumption() {
      bool result = setString(OS_HeaderedPumps_VariableSpeedFields::RatedPowerConsumption, "autosize");
      OS_ASSERT(result);
    }

    bool HeaderedPumpsVariableSpeed_Impl::setMotorEfficiency(double motorEfficiency) {
      bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::MotorEfficiency, motorEfficiency);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
      bool result =
        setDouble(OS_HeaderedPumps_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, fractionofMotorInefficienciestoFluidStream);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve) {
      bool result =
        setDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, coefficient1ofthePartLoadPerformanceCurve);
      OS_ASSERT(result);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve) {
      bool result =
        setDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, coefficient2ofthePartLoadPerformanceCurve);
      OS_ASSERT(result);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve) {
      bool result =
        setDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, coefficient3ofthePartLoadPerformanceCurve);
      OS_ASSERT(result);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve) {
      bool result =
        setDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, coefficient4ofthePartLoadPerformanceCurve);
      OS_ASSERT(result);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setMinimumFlowRateFraction(double minimumFlowRateFraction) {
      bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::MinimumFlowRateFraction, minimumFlowRateFraction);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setPumpControlType(const std::string& pumpControlType) {
      bool result = setString(OS_HeaderedPumps_VariableSpeedFields::PumpControlType, pumpControlType);
      return result;
    }

    bool HeaderedPumpsVariableSpeed_Impl::setPumpFlowRateSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_HeaderedPumps_VariableSpeedFields::PumpFlowRateSchedule, "HeaderedPumpsVariableSpeed", "Pump Flow Rate Schedule", schedule);
      return result;
    }

    void HeaderedPumpsVariableSpeed_Impl::resetPumpFlowRateSchedule() {
      bool result = setString(OS_HeaderedPumps_VariableSpeedFields::PumpFlowRateSchedule, "");
      OS_ASSERT(result);
    }

    bool HeaderedPumpsVariableSpeed_Impl::setThermalZone(const boost::optional<ThermalZone>& thermalZone) {
      bool result(false);
      if (thermalZone) {
        result = setPointer(OS_HeaderedPumps_VariableSpeedFields::ThermalZone, thermalZone.get().handle());
      } else {
        resetThermalZone();
        result = true;
      }
      return result;
    }

    void HeaderedPumpsVariableSpeed_Impl::resetThermalZone() {
      bool result = setString(OS_HeaderedPumps_VariableSpeedFields::ThermalZone, "");
      OS_ASSERT(result);
    }

    bool HeaderedPumpsVariableSpeed_Impl::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
      bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::SkinLossRadiativeFraction, skinLossRadiativeFraction);
      return result;
    }

    unsigned HeaderedPumpsVariableSpeed_Impl::inletPort() const {
      return OS_HeaderedPumps_VariableSpeedFields::InletNodeName;
    }

    unsigned HeaderedPumpsVariableSpeed_Impl::outletPort() const {
      return OS_HeaderedPumps_VariableSpeedFields::OutletNodeName;
    }

    bool HeaderedPumpsVariableSpeed_Impl::addToNode(Node& node) {
      if (node.plantLoop()) {
        return StraightComponent_Impl::addToNode(node);
      }

      return false;
    }

    boost::optional<double> HeaderedPumpsVariableSpeed_Impl::autosizedTotalRatedFlowRate() const {
      return getAutosizedValue("Design Flow Rate", "m3/s");
    }

    boost::optional<double> HeaderedPumpsVariableSpeed_Impl::autosizedRatedPowerConsumption() const {
      return getAutosizedValue("Design Power Consumption", "W");
    }

    void HeaderedPumpsVariableSpeed_Impl::autosize() {
      autosizeTotalRatedFlowRate();
      autosizeRatedPowerConsumption();
    }

    void HeaderedPumpsVariableSpeed_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedTotalRatedFlowRate();
      if (val) {
        setTotalRatedFlowRate(val.get());
      }

      val = autosizedRatedPowerConsumption();
      if (val) {
        setRatedPowerConsumption(val.get());
      }
    }

    std::vector<EMSActuatorNames> HeaderedPumpsVariableSpeed_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Pump", "Pump Mass Flow Rate"}, {"Pump", "Pump Pressure Rise"}};
      return actuators;
    }

    std::vector<std::string> HeaderedPumpsVariableSpeed_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Pump Maximum Mass Flow Rate"};
      return types;
    }

    std::string HeaderedPumpsVariableSpeed_Impl::designPowerSizingMethod() const {
      auto value = getString(OS_HeaderedPumps_VariableSpeedFields::DesignPowerSizingMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeaderedPumpsVariableSpeed_Impl::setDesignPowerSizingMethod(const std::string& designPowerSizingMethod) {
      return setString(OS_HeaderedPumps_VariableSpeedFields::DesignPowerSizingMethod, designPowerSizingMethod);
    }

    double HeaderedPumpsVariableSpeed_Impl::designElectricPowerPerUnitFlowRate() const {
      auto value = getDouble(OS_HeaderedPumps_VariableSpeedFields::DesignElectricPowerperUnitFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeaderedPumpsVariableSpeed_Impl::setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate) {
      return setDouble(OS_HeaderedPumps_VariableSpeedFields::DesignElectricPowerperUnitFlowRate, designElectricPowerPerUnitFlowRate);
    }

    double HeaderedPumpsVariableSpeed_Impl::designShaftPowerPerUnitFlowRatePerUnitHead() const {
      auto value = getDouble(OS_HeaderedPumps_VariableSpeedFields::DesignShaftPowerperUnitFlowRateperUnitHead, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeaderedPumpsVariableSpeed_Impl::setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead) {
      return setDouble(OS_HeaderedPumps_VariableSpeedFields::DesignShaftPowerperUnitFlowRateperUnitHead, designShaftPowerPerUnitFlowRatePerUnitHead);
    }

    std::string HeaderedPumpsVariableSpeed_Impl::endUseSubcategory() const {
      auto value = getString(OS_HeaderedPumps_VariableSpeedFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeaderedPumpsVariableSpeed_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      return setString(OS_HeaderedPumps_VariableSpeedFields::EndUseSubcategory, endUseSubcategory);
    }

    ComponentType HeaderedPumpsVariableSpeed_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> HeaderedPumpsVariableSpeed_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> HeaderedPumpsVariableSpeed_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> HeaderedPumpsVariableSpeed_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  HeaderedPumpsVariableSpeed::HeaderedPumpsVariableSpeed(const Model& model) : StraightComponent(HeaderedPumpsVariableSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeaderedPumpsVariableSpeed_Impl>());

    autosizeTotalRatedFlowRate();
    setNumberofPumpsinBank(2);
    setFlowSequencingControlScheme("Sequential");
    setRatedPumpHead(179352);
    autosizeRatedPowerConsumption();
    setMotorEfficiency(0.9);
    setFractionofMotorInefficienciestoFluidStream(0.0);
    setCoefficient1ofthePartLoadPerformanceCurve(0.0);
    setCoefficient2ofthePartLoadPerformanceCurve(1.0);
    setCoefficient3ofthePartLoadPerformanceCurve(0.0);
    setCoefficient4ofthePartLoadPerformanceCurve(0.0);
    setMinimumFlowRateFraction(0.0);
    setPumpControlType("Continuous");
    setSkinLossRadiativeFraction(0.1);

    setDesignPowerSizingMethod("PowerPerFlowPerPressure");
    setDesignElectricPowerPerUnitFlowRate(348701.1);
    setDesignShaftPowerPerUnitFlowRatePerUnitHead(1.282051282);

    setEndUseSubcategory("General");
  }

  IddObjectType HeaderedPumpsVariableSpeed::iddObjectType() {
    return {IddObjectType::OS_HeaderedPumps_VariableSpeed};
  }

  std::vector<std::string> HeaderedPumpsVariableSpeed::flowSequencingControlSchemeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeaderedPumps_VariableSpeedFields::FlowSequencingControlScheme);
  }

  std::vector<std::string> HeaderedPumpsVariableSpeed::pumpControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeaderedPumps_VariableSpeedFields::PumpControlType);
  }

  std::vector<std::string> HeaderedPumpsVariableSpeed::designPowerSizingMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeaderedPumps_VariableSpeedFields::DesignPowerSizingMethod);
  }

  boost::optional<double> HeaderedPumpsVariableSpeed::totalRatedFlowRate() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->totalRatedFlowRate();
  }

  bool HeaderedPumpsVariableSpeed::isTotalRatedFlowRateAutosized() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->isTotalRatedFlowRateAutosized();
  }

  int HeaderedPumpsVariableSpeed::numberofPumpsinBank() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->numberofPumpsinBank();
  }

  std::string HeaderedPumpsVariableSpeed::flowSequencingControlScheme() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->flowSequencingControlScheme();
  }

  double HeaderedPumpsVariableSpeed::ratedPumpHead() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->ratedPumpHead();
  }

  boost::optional<double> HeaderedPumpsVariableSpeed::ratedPowerConsumption() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->ratedPowerConsumption();
  }

  bool HeaderedPumpsVariableSpeed::isRatedPowerConsumptionAutosized() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->isRatedPowerConsumptionAutosized();
  }

  double HeaderedPumpsVariableSpeed::motorEfficiency() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->motorEfficiency();
  }

  double HeaderedPumpsVariableSpeed::fractionofMotorInefficienciestoFluidStream() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->fractionofMotorInefficienciestoFluidStream();
  }

  double HeaderedPumpsVariableSpeed::coefficient1ofthePartLoadPerformanceCurve() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->coefficient1ofthePartLoadPerformanceCurve();
  }

  double HeaderedPumpsVariableSpeed::coefficient2ofthePartLoadPerformanceCurve() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->coefficient2ofthePartLoadPerformanceCurve();
  }

  double HeaderedPumpsVariableSpeed::coefficient3ofthePartLoadPerformanceCurve() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->coefficient3ofthePartLoadPerformanceCurve();
  }

  double HeaderedPumpsVariableSpeed::coefficient4ofthePartLoadPerformanceCurve() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->coefficient4ofthePartLoadPerformanceCurve();
  }

  double HeaderedPumpsVariableSpeed::minimumFlowRateFraction() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->minimumFlowRateFraction();
  }

  std::string HeaderedPumpsVariableSpeed::pumpControlType() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->pumpControlType();
  }

  boost::optional<Schedule> HeaderedPumpsVariableSpeed::pumpFlowRateSchedule() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->pumpFlowRateSchedule();
  }

  boost::optional<ThermalZone> HeaderedPumpsVariableSpeed::thermalZone() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->thermalZone();
  }

  double HeaderedPumpsVariableSpeed::skinLossRadiativeFraction() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->skinLossRadiativeFraction();
  }

  bool HeaderedPumpsVariableSpeed::setTotalRatedFlowRate(double totalRatedFlowRate) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setTotalRatedFlowRate(totalRatedFlowRate);
  }

  void HeaderedPumpsVariableSpeed::autosizeTotalRatedFlowRate() {
    getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->autosizeTotalRatedFlowRate();
  }

  bool HeaderedPumpsVariableSpeed::setNumberofPumpsinBank(int numberofPumpsinBank) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setNumberofPumpsinBank(numberofPumpsinBank);
  }

  bool HeaderedPumpsVariableSpeed::setFlowSequencingControlScheme(const std::string& flowSequencingControlScheme) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setFlowSequencingControlScheme(flowSequencingControlScheme);
  }

  bool HeaderedPumpsVariableSpeed::setRatedPumpHead(double ratedPumpHead) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setRatedPumpHead(ratedPumpHead);
  }

  bool HeaderedPumpsVariableSpeed::setRatedPowerConsumption(double ratedPowerConsumption) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setRatedPowerConsumption(ratedPowerConsumption);
  }

  void HeaderedPumpsVariableSpeed::autosizeRatedPowerConsumption() {
    getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->autosizeRatedPowerConsumption();
  }

  bool HeaderedPumpsVariableSpeed::setMotorEfficiency(double motorEfficiency) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setMotorEfficiency(motorEfficiency);
  }

  bool HeaderedPumpsVariableSpeed::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setFractionofMotorInefficienciestoFluidStream(
      fractionofMotorInefficienciestoFluidStream);
  }

  bool HeaderedPumpsVariableSpeed::setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setCoefficient1ofthePartLoadPerformanceCurve(
      coefficient1ofthePartLoadPerformanceCurve);
  }

  bool HeaderedPumpsVariableSpeed::setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setCoefficient2ofthePartLoadPerformanceCurve(
      coefficient2ofthePartLoadPerformanceCurve);
  }

  bool HeaderedPumpsVariableSpeed::setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setCoefficient3ofthePartLoadPerformanceCurve(
      coefficient3ofthePartLoadPerformanceCurve);
  }

  bool HeaderedPumpsVariableSpeed::setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setCoefficient4ofthePartLoadPerformanceCurve(
      coefficient4ofthePartLoadPerformanceCurve);
  }

  bool HeaderedPumpsVariableSpeed::setMinimumFlowRateFraction(double minimumFlowRateFraction) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setMinimumFlowRateFraction(minimumFlowRateFraction);
  }

  bool HeaderedPumpsVariableSpeed::setPumpControlType(const std::string& pumpControlType) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setPumpControlType(pumpControlType);
  }

  bool HeaderedPumpsVariableSpeed::setPumpFlowRateSchedule(Schedule& schedule) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setPumpFlowRateSchedule(schedule);
  }

  void HeaderedPumpsVariableSpeed::resetPumpFlowRateSchedule() {
    getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->resetPumpFlowRateSchedule();
  }

  bool HeaderedPumpsVariableSpeed::setThermalZone(const ThermalZone& thermalZone) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setThermalZone(thermalZone);
  }

  void HeaderedPumpsVariableSpeed::resetThermalZone() {
    getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->resetThermalZone();
  }

  bool HeaderedPumpsVariableSpeed::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setSkinLossRadiativeFraction(skinLossRadiativeFraction);
  }

  std::string HeaderedPumpsVariableSpeed::designPowerSizingMethod() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->designPowerSizingMethod();
  }

  bool HeaderedPumpsVariableSpeed::setDesignPowerSizingMethod(const std::string& designPowerSizingMethod) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setDesignPowerSizingMethod(designPowerSizingMethod);
  }

  double HeaderedPumpsVariableSpeed::designElectricPowerPerUnitFlowRate() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->designElectricPowerPerUnitFlowRate();
  }

  bool HeaderedPumpsVariableSpeed::setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setDesignElectricPowerPerUnitFlowRate(designElectricPowerPerUnitFlowRate);
  }

  double HeaderedPumpsVariableSpeed::designShaftPowerPerUnitFlowRatePerUnitHead() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->designShaftPowerPerUnitFlowRatePerUnitHead();
  }

  bool HeaderedPumpsVariableSpeed::setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setDesignShaftPowerPerUnitFlowRatePerUnitHead(
      designShaftPowerPerUnitFlowRatePerUnitHead);
  }

  std::string HeaderedPumpsVariableSpeed::endUseSubcategory() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->endUseSubcategory();
  }

  bool HeaderedPumpsVariableSpeed::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  /// @cond
  HeaderedPumpsVariableSpeed::HeaderedPumpsVariableSpeed(std::shared_ptr<detail::HeaderedPumpsVariableSpeed_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> HeaderedPumpsVariableSpeed::autosizedTotalRatedFlowRate() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->autosizedTotalRatedFlowRate();
  }

  boost::optional<double> HeaderedPumpsVariableSpeed::autosizedRatedPowerConsumption() const {
    return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->autosizedRatedPowerConsumption();
  }

}  // namespace model
}  // namespace openstudio
