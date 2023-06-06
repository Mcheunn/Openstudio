/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilCoolingDXVariableSpeed.hpp"
#include "CoilCoolingDXVariableSpeed_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "CoilCoolingDXVariableSpeedSpeedData_Impl.hpp"
// #include "WaterStorageTank.hpp"
// #include "WaterStorageTank_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "AirLoopHVAC.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingDXVariableSpeed_Impl::CoilCoolingDXVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXVariableSpeed::iddObjectType());
    }

    CoilCoolingDXVariableSpeed_Impl::CoilCoolingDXVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingDXVariableSpeed::iddObjectType());
    }

    CoilCoolingDXVariableSpeed_Impl::CoilCoolingDXVariableSpeed_Impl(const CoilCoolingDXVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingDXVariableSpeed_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Cooling Coil Electricity Rate",
                                                   "Cooling Coil Total Cooling Rate",
                                                   "Cooling Coil Sensible Cooling Rate",
                                                   "Cooling Coil Source Side Heat Transfer Rate",
                                                   "Cooling Coil Part Load Ratio",
                                                   "Cooling Coil Runtime Fraction",
                                                   "Cooling Coil Air Mass Flow Rate",
                                                   "Cooling Coil Air Inlet Temperature",
                                                   "Cooling Coil Air Inlet Humidity Ratio",
                                                   "Cooling Coil Air Outlet Temperature",
                                                   "Cooling Coil Air Outlet Humidity Ratio",
                                                   "Cooling Coil Upper Speed Level",
                                                   "Cooling Coil Neighboring Speed Levels Ratio",
                                                   "VSAirtoAirHP Recoverable Waste Heat",
                                                   "Cooling Coil Electricity Energy",
                                                   "Cooling Coil Total Cooling Energy",
                                                   "Cooling Coil Sensible Cooling Energy",
                                                   "Cooling Coil Latent Cooling Energy",
                                                   "Cooling Coil Source Side Heat Transfer Energy",
                                                   "Cooling Coil Crankcase Heater Electricity Rate",
                                                   "Cooling Coil Crankcase Heater Electricity Energy",
                                                   "Cooling Coil Condensate Volume Flow Rate",
                                                   "Cooling Coil Condensate Volume",
                                                   "Cooling Coil Condenser Inlet Temperature",
                                                   "Cooling Coil Evaporative Condenser Water Volume",
                                                   "Cooling Coil Evaporative Condenser Mains Water Volume",
                                                   "Cooling Coil Evaporative Condenser Pump Electricity Rate",
                                                   "Cooling Coil Evaporative Condenser Pump Electricity Energy",
                                                   "Cooling Coil Basin Heater Electricity Rate",
                                                   "Cooling Coil Basin Heater Electricity Energy"};
      return result;
    }

    IddObjectType CoilCoolingDXVariableSpeed_Impl::iddObjectType() const {
      return CoilCoolingDXVariableSpeed::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilCoolingDXVariableSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingDXVariableSpeed", "Basin Heater Operating"));
      }
      return result;
    }

    unsigned CoilCoolingDXVariableSpeed_Impl::inletPort() const {
      return OS_Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName;
    }

    unsigned CoilCoolingDXVariableSpeed_Impl::outletPort() const {
      return OS_Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName;
    }

    int CoilCoolingDXVariableSpeed_Impl::nominalSpeedLevel() const {
      boost::optional<int> value = getInt(OS_Coil_Cooling_DX_VariableSpeedFields::NominalSpeedLevel, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXVariableSpeed_Impl::grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
      return getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, true);
    }

    bool CoilCoolingDXVariableSpeed_Impl::isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXVariableSpeed_Impl::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
      return getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
    }

    bool CoilCoolingDXVariableSpeed_Impl::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilCoolingDXVariableSpeed_Impl::nominalTimeforCondensatetoBeginLeavingtheCoil() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::NominalTimeforCondensatetoBeginLeavingtheCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXVariableSpeed_Impl::initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve CoilCoolingDXVariableSpeed_Impl::energyPartLoadFractionCurve() const {
      boost::optional<Curve> value = optionalEnergyPartLoadFractionCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Energy Part Load Fraction Curve attached.");
      }
      return value.get();
    }

    // boost::optional<std::string> CoilCoolingDXVariableSpeed_Impl::condenserAirInletNodeName() const {
    //   return getString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserAirInletNodeName,true);
    // }

    std::string CoilCoolingDXVariableSpeed_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXVariableSpeed_Impl::evaporativeCondenserPumpRatedPowerConsumption() const {
      return getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
    }

    bool CoilCoolingDXVariableSpeed_Impl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilCoolingDXVariableSpeed_Impl::crankcaseHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::CrankcaseHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXVariableSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    // boost::optional<WaterStorageTank> CoilCoolingDXVariableSpeed_Impl::supplyWaterStorageTank() const {
    //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Coil_Cooling_DX_VariableSpeedFields::SupplyWaterStorageTankName);
    // }

    // boost::optional<WaterStorageTank> CoilCoolingDXVariableSpeed_Impl::condensateCollectionWaterStorageTank() const {
    //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Coil_Cooling_DX_VariableSpeedFields::CondensateCollectionWaterStorageTankName);
    // }

    double CoilCoolingDXVariableSpeed_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXVariableSpeed_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> CoilCoolingDXVariableSpeed_Impl::basinHeaterOperatingSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName);
    }

    bool CoilCoolingDXVariableSpeed_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
      bool result = setInt(OS_Coil_Cooling_DX_VariableSpeedFields::NominalSpeedLevel, nominalSpeedLevel);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXVariableSpeed_Impl::setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(
      boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
      bool result(false);
      if (grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
        result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel,
                           grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXVariableSpeed_Impl::autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() {
      bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXVariableSpeed_Impl::setRatedAirFlowRateAtSelectedNominalSpeedLevel(
      boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel) {
      bool result(false);
      if (ratedAirFlowRateAtSelectedNominalSpeedLevel) {
        result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,
                           ratedAirFlowRateAtSelectedNominalSpeedLevel.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXVariableSpeed_Impl::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
      bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXVariableSpeed_Impl::setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil) {
      bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::NominalTimeforCondensatetoBeginLeavingtheCoil,
                              nominalTimeforCondensatetoBeginLeavingtheCoil);
      return result;
    }

    bool CoilCoolingDXVariableSpeed_Impl::setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
      double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity) {
      bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity,
                              initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);
      return result;
    }

    bool CoilCoolingDXVariableSpeed_Impl::setEnergyPartLoadFractionCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName, curve.handle());
      return result;
    }

    // void CoilCoolingDXVariableSpeed_Impl::setCondenserAirInletNodeName(boost::optional<std::string> condenserAirInletNodeName) {
    //   bool result(false);
    //   if (condenserAirInletNodeName) {
    //     result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserAirInletNodeName, condenserAirInletNodeName.get());
    //   }
    //   else {
    //     resetCondenserAirInletNodeName();
    //     result = true;
    //   }
    //   OS_ASSERT(result);
    // }

    // void CoilCoolingDXVariableSpeed_Impl::resetCondenserAirInletNodeName() {
    //   bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserAirInletNodeName, "");
    //   OS_ASSERT(result);
    // }

    bool CoilCoolingDXVariableSpeed_Impl::setCondenserType(const std::string& condenserType) {
      bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserType, condenserType);
      return result;
    }

    bool CoilCoolingDXVariableSpeed_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(
      boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption) {
      bool result(false);
      if (evaporativeCondenserPumpRatedPowerConsumption) {
        result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption,
                           evaporativeCondenserPumpRatedPowerConsumption.get());
      }
      return result;
    }

    void CoilCoolingDXVariableSpeed_Impl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
      bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXVariableSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
      bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
      return result;
    }

    bool CoilCoolingDXVariableSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
      bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,
                              maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
      return result;
    }

    // bool CoilCoolingDXVariableSpeed_Impl::setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
    //   bool result(false);
    //   if (waterStorageTank) {
    //     result = setPointer(OS_Coil_Cooling_DX_VariableSpeedFields::SupplyWaterStorageTankName, waterStorageTank.get().handle());
    //   }
    //   else {
    //     resetSupplyWaterStorageTank();
    //     result = true;
    //   }
    //   return result;
    // }

    // void CoilCoolingDXVariableSpeed_Impl::resetSupplyWaterStorageTank() {
    //   bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::SupplyWaterStorageTankName, "");
    //   OS_ASSERT(result);
    // }

    // bool CoilCoolingDXVariableSpeed_Impl::setCondensateCollectionWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
    //   bool result(false);
    //   if (waterStorageTank) {
    //     result = setPointer(OS_Coil_Cooling_DX_VariableSpeedFields::CondensateCollectionWaterStorageTankName, waterStorageTank.get().handle());
    //   }
    //   else {
    //     resetCondensateCollectionWaterStorageTank();
    //     result = true;
    //   }
    //   return result;
    // }

    // void CoilCoolingDXVariableSpeed_Impl::resetCondensateCollectionWaterStorageTank() {
    //   bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::CondensateCollectionWaterStorageTankName, "");
    //   OS_ASSERT(result);
    // }

    bool CoilCoolingDXVariableSpeed_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
      bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterCapacity, basinHeaterCapacity);
      return result;
    }

    bool CoilCoolingDXVariableSpeed_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
      bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
      return result;
    }

    bool CoilCoolingDXVariableSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName, "CoilCoolingDXVariableSpeed",
                                "Basin Heater Operating", schedule);
      return result;
    }

    void CoilCoolingDXVariableSpeed_Impl::resetBasinHeaterOperatingSchedule() {
      bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName, "");
      OS_ASSERT(result);
    }

    double CoilCoolingDXVariableSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXVariableSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
      bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                              minimumOutdoorDryBulbTemperatureforCompressorOperation);
      return result;
    }

    boost::optional<Curve> CoilCoolingDXVariableSpeed_Impl::optionalEnergyPartLoadFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName);
    }

    ModelObject CoilCoolingDXVariableSpeed_Impl::clone(Model model) const {
      auto t_clone = StraightComponent_Impl::clone(model).cast<CoilCoolingDXVariableSpeed>();

      if (auto speedDataList = this->speedDataList()) {
        auto speedDataListClone = speedDataList->clone(model).cast<ModelObjectList>();
        t_clone.getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataListClone);
      }

      return std::move(t_clone);
    }

    std::vector<ModelObject> CoilCoolingDXVariableSpeed_Impl::children() const {
      std::vector<ModelObject> children;
      children.push_back(energyPartLoadFractionCurve());
      if (auto const _stageDataList = speedDataList()) {
        for (const auto& mo : _stageDataList->modelObjects()) {
          children.push_back(mo);
        }
      }
      return children;
    }

    boost::optional<ModelObjectList> CoilCoolingDXVariableSpeed_Impl::speedDataList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Coil_Cooling_DX_VariableSpeedFields::SpeedDataList);
    }

    bool CoilCoolingDXVariableSpeed_Impl::addSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed) {
      bool result = false;
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        result = modelObjectList->addModelObject(speed);
      }
      return result;
    }

    void CoilCoolingDXVariableSpeed_Impl::removeSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed) {
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        modelObjectList->removeModelObject(speed);
      }
    }

    void CoilCoolingDXVariableSpeed_Impl::removeAllSpeeds() {
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilCoolingDXVariableSpeedSpeedData>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
        }
      }
    }

    std::vector<CoilCoolingDXVariableSpeedSpeedData> CoilCoolingDXVariableSpeed_Impl::speeds() const {
      std::vector<CoilCoolingDXVariableSpeedSpeedData> result;
      auto const modelObjectList = speedDataList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilCoolingDXVariableSpeedSpeedData>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
        }
      }
      return result;
    }

    boost::optional<HVACComponent> CoilCoolingDXVariableSpeed_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      {
        auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

        for (auto const& system : systems) {
          if (auto coolingCoil = system.coolingCoil()) {
            if (coolingCoil->handle() == this->handle()) {
              return system;
            }
          }
        }
      }

      // AirLoopHVACUnitaryHeatPumpAirToAir
      {
        auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

        for (auto const& system : systems) {
          auto coolingCoil = system.coolingCoil();
          if (coolingCoil.handle() == this->handle()) {
            return system;
          }
        }
      }

      // CoilSystemCoolingDXHeatExchangerAssisted
      {
        auto coilSystems = this->model().getConcreteModelObjects<CoilSystemCoolingDXHeatExchangerAssisted>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.coolingCoil().handle() == this->handle()) {
            return coilSystem;
          }
        }
      }

      // CoilSystemIntegratedHeatPumpAirSource
      {
        auto coilSystems = this->model().getConcreteModelObjects<CoilSystemIntegratedHeatPumpAirSource>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.spaceCoolingCoil().handle() == this->handle()) {
            return coilSystem;
          }
          if (coilSystem.scdwhCoolingCoil().handle() == this->handle()) {
            return coilSystem;
          }
        }
      }

      return boost::none;
    }

    boost::optional<ZoneHVACComponent> CoilCoolingDXVariableSpeed_Impl::containingZoneHVACComponent() const {
      // ZoneHVACPackagedTerminalAirConditioner
      {
        auto systems = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalAirConditioner>();

        for (auto const& system : systems) {
          auto coolingCoil = system.coolingCoil();
          if (coolingCoil.handle() == this->handle()) {
            return system;
          }
        }
      }

      // ZoneHVACPackagedTerminalHeatPump
      {
        auto systems = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

        for (auto const& system : systems) {
          auto coolingCoil = system.coolingCoil();
          if (coolingCoil.handle() == this->handle()) {
            return system;
          }
        }
      }

      return boost::none;
    }

    bool CoilCoolingDXVariableSpeed_Impl::addToNode(Node& node) {

      // TODO: JM 2019-03-12 I'm not sure we shouldn't just restrict to ANY containingHVACComponent (disallow if part of a UnitarySystem)
      auto t_containingHVACComponent = containingHVACComponent();
      if (t_containingHVACComponent && t_containingHVACComponent->optionalCast<CoilSystemCoolingDXHeatExchangerAssisted>()) {
        LOG(Warn, this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemCoolingDXHeatExchangerAssisted. "
                                              "Please call CoilSystemCoolingDXHeatExchangerAssisted::addToNode instead");
      } else if (t_containingHVACComponent && t_containingHVACComponent->optionalCast<CoilSystemIntegratedHeatPumpAirSource>()) {
        LOG(Warn, this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemIntegratedHeatPumpAirSource. "
                                              "Please call CoilSystemIntegratedHeatPumpAirSource::addToNode instead");
      } else {

        if (boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC()) {
          if (!airLoop->demandComponent(node.handle())) {
            return StraightComponent_Impl::addToNode(node);
          }
        }

        if (auto oa = node.airLoopHVACOutdoorAirSystem()) {
          return StraightComponent_Impl::addToNode(node);
        }
      }
      return false;
    }

    bool CoilCoolingDXVariableSpeed_Impl::setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_Coil_Cooling_DX_VariableSpeedFields::SpeedDataList, modelObjectList.get().handle());
      } else {
        resetSpeedDataList();
        result = true;
      }
      return result;
    }

    void CoilCoolingDXVariableSpeed_Impl::resetSpeedDataList() {
      bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::SpeedDataList, "");
      OS_ASSERT(result);
    }

    std::vector<IdfObject> CoilCoolingDXVariableSpeed_Impl::remove() {
      auto _stageDataList = speedDataList();
      auto result = StraightComponent_Impl::remove();
      if ((!result.empty()) && _stageDataList) {
        _stageDataList->remove();
      }

      return result;
    }

    boost::optional<double> CoilCoolingDXVariableSpeed_Impl::autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Total Cooling Capacity", "W", "COIL:COOLING:DX:VARIABLESPEED");
    }

    boost::optional<double> CoilCoolingDXVariableSpeed_Impl::autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Air Flow Rate", "m3/s", "COIL:COOLING:DX:VARIABLESPEED");
    }

    boost::optional<double> CoilCoolingDXVariableSpeed_Impl::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Evaporative Condenser Pump Rated Power Consumption", "W", "AS VS COOLING COIL");
    }

    void CoilCoolingDXVariableSpeed_Impl::autosize() {
      autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
      autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
      autosizeEvaporativeCondenserPumpRatedPowerConsumption();
    }

    void CoilCoolingDXVariableSpeed_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
      if (val) {
        setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(val.get());
      }

      val = autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel();
      if (val) {
        setRatedAirFlowRateAtSelectedNominalSpeedLevel(val.get());
      }

      val = autosizedEvaporativeCondenserPumpRatedPowerConsumption();
      if (val) {
        setEvaporativeCondenserPumpRatedPowerConsumption(val.get());
      }
    }

    ComponentType CoilCoolingDXVariableSpeed_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> CoilCoolingDXVariableSpeed_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> CoilCoolingDXVariableSpeed_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> CoilCoolingDXVariableSpeed_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  CoilCoolingDXVariableSpeed::CoilCoolingDXVariableSpeed(const Model& model) : StraightComponent(CoilCoolingDXVariableSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXVariableSpeed_Impl>());

    bool ok = true;
    setNominalSpeedLevel(1);
    autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
    autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
    ok = setNominalTimeforCondensatetoBeginLeavingtheCoil(0);
    OS_ASSERT(ok);
    ok = setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0);
    OS_ASSERT(ok);

    auto partLoadFraction = CurveQuadratic(model);
    partLoadFraction.setCoefficient1Constant(0.85);
    partLoadFraction.setCoefficient2x(0.15);
    partLoadFraction.setCoefficient3xPOW2(0.0);
    partLoadFraction.setMinimumValueofx(0.0);
    partLoadFraction.setMaximumValueofx(1.0);

    ok = setEnergyPartLoadFractionCurve(partLoadFraction);
    OS_ASSERT(ok);
    ok = setCondenserType("AirCooled");
    OS_ASSERT(ok);
    ok = setEvaporativeCondenserPumpRatedPowerConsumption(0.0);
    OS_ASSERT(ok);
    ok = setCrankcaseHeaterCapacity(0.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
    OS_ASSERT(ok);
    ok = setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-25.0);
    OS_ASSERT(ok);
    // ok = setSupplyWaterStorageTank(WaterStorageTank);
    // OS_ASSERT(ok);
    // ok = setCondensateCollectionWaterStorageTank(WaterStorageTank);
    // OS_ASSERT(ok);
    ok = setBasinHeaterCapacity(0.0);
    OS_ASSERT(ok);
    ok = setBasinHeaterSetpointTemperature(2.0);
    OS_ASSERT(ok);

    auto speedDataList = ModelObjectList(model);
    speedDataList.setName(this->name().get() + " Speed Data List");
    ok = getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataList);
    OS_ASSERT(ok);
  }

  CoilCoolingDXVariableSpeed::CoilCoolingDXVariableSpeed(const Model& model, const Curve& partLoadFraction)
    : StraightComponent(CoilCoolingDXVariableSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXVariableSpeed_Impl>());

    bool ok = true;
    setNominalSpeedLevel(1);
    autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
    autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
    ok = setNominalTimeforCondensatetoBeginLeavingtheCoil(0);
    OS_ASSERT(ok);
    ok = setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0);
    OS_ASSERT(ok);
    ok = setEnergyPartLoadFractionCurve(partLoadFraction);
    OS_ASSERT(ok);
    ok = setCondenserType("AirCooled");
    OS_ASSERT(ok);
    ok = setEvaporativeCondenserPumpRatedPowerConsumption(0.0);
    OS_ASSERT(ok);
    ok = setCrankcaseHeaterCapacity(0.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
    OS_ASSERT(ok);
    ok = setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-25.0);
    OS_ASSERT(ok);
    // ok = setSupplyWaterStorageTank(WaterStorageTank);
    // OS_ASSERT(ok);
    // ok = setCondensateCollectionWaterStorageTank(WaterStorageTank);
    // OS_ASSERT(ok);
    ok = setBasinHeaterCapacity(0.0);
    OS_ASSERT(ok);
    ok = setBasinHeaterSetpointTemperature(2.0);
    OS_ASSERT(ok);

    auto speedDataList = ModelObjectList(model);
    speedDataList.setName(this->name().get() + " Speed Data List");
    ok = getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataList);
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingDXVariableSpeed::iddObjectType() {
    return {IddObjectType::OS_Coil_Cooling_DX_VariableSpeed};
  }

  std::vector<std::string> CoilCoolingDXVariableSpeed::condenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Cooling_DX_VariableSpeedFields::CondenserType);
  }

  int CoilCoolingDXVariableSpeed::nominalSpeedLevel() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->nominalSpeedLevel();
  }

  boost::optional<double> CoilCoolingDXVariableSpeed::grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
  }

  bool CoilCoolingDXVariableSpeed::isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized();
  }

  boost::optional<double> CoilCoolingDXVariableSpeed::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->ratedAirFlowRateAtSelectedNominalSpeedLevel();
  }

  bool CoilCoolingDXVariableSpeed::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized();
  }

  double CoilCoolingDXVariableSpeed::nominalTimeforCondensatetoBeginLeavingtheCoil() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->nominalTimeforCondensatetoBeginLeavingtheCoil();
  }

  double CoilCoolingDXVariableSpeed::initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity();
  }

  Curve CoilCoolingDXVariableSpeed::energyPartLoadFractionCurve() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->energyPartLoadFractionCurve();
  }

  // boost::optional<std::string> CoilCoolingDXVariableSpeed::condenserAirInletNodeName() const {
  //   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->condenserAirInletNodeName();
  // }

  std::string CoilCoolingDXVariableSpeed::condenserType() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->condenserType();
  }

  boost::optional<double> CoilCoolingDXVariableSpeed::evaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
  }

  bool CoilCoolingDXVariableSpeed::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->isEvaporativeCondenserPumpRatedPowerConsumptionAutosized();
  }

  double CoilCoolingDXVariableSpeed::crankcaseHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->crankcaseHeaterCapacity();
  }

  double CoilCoolingDXVariableSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
  }

  // boost::optional<WaterStorageTank> CoilCoolingDXVariableSpeed::supplyWaterStorageTank() const {
  //   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->supplyWaterStorageTank();
  // }

  // boost::optional<WaterStorageTank> CoilCoolingDXVariableSpeed::condensateCollectionWaterStorageTank() const {
  //   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->condensateCollectionWaterStorageTank();
  // }

  double CoilCoolingDXVariableSpeed::basinHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->basinHeaterCapacity();
  }

  double CoilCoolingDXVariableSpeed::basinHeaterSetpointTemperature() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->basinHeaterSetpointTemperature();
  }

  boost::optional<Schedule> CoilCoolingDXVariableSpeed::basinHeaterOperatingSchedule() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->basinHeaterOperatingSchedule();
  }

  bool CoilCoolingDXVariableSpeed::setNominalSpeedLevel(int nominalSpeedLevel) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
  }

  bool CoilCoolingDXVariableSpeed::setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(
    double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(
      grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);
  }

  void CoilCoolingDXVariableSpeed::autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() {
    getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
  }

  bool CoilCoolingDXVariableSpeed::setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setRatedAirFlowRateAtSelectedNominalSpeedLevel(
      ratedAirFlowRateAtSelectedNominalSpeedLevel);
  }

  void CoilCoolingDXVariableSpeed::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
    getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  }

  bool CoilCoolingDXVariableSpeed::setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setNominalTimeforCondensatetoBeginLeavingtheCoil(
      nominalTimeforCondensatetoBeginLeavingtheCoil);
  }

  bool CoilCoolingDXVariableSpeed::setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
    double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
      initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);
  }

  bool CoilCoolingDXVariableSpeed::setEnergyPartLoadFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setEnergyPartLoadFractionCurve(curve);
  }

  // void CoilCoolingDXVariableSpeed::setCondenserAirInletNodeName(std::string condenserAirInletNodeName) {
  //   getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setCondenserAirInletNodeName(condenserAirInletNodeName);
  // }

  // void CoilCoolingDXVariableSpeed::resetCondenserAirInletNodeName() {
  //   getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->resetCondenserAirInletNodeName();
  // }

  bool CoilCoolingDXVariableSpeed::setCondenserType(const std::string& condenserType) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setCondenserType(condenserType);
  }

  bool CoilCoolingDXVariableSpeed::setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(
      evaporativeCondenserPumpRatedPowerConsumption);
  }

  void CoilCoolingDXVariableSpeed::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
    getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  }

  bool CoilCoolingDXVariableSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
  }

  bool CoilCoolingDXVariableSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
      maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
  }

  // bool CoilCoolingDXVariableSpeed::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
  //   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setSupplyWaterStorageTank(waterStorageTank);
  // }

  // void CoilCoolingDXVariableSpeed::resetSupplyWaterStorageTank() {
  //   getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->resetSupplyWaterStorageTank();
  // }

  // bool CoilCoolingDXVariableSpeed::setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank) {
  //   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setCondensateCollectionWaterStorageTank(waterStorageTank);
  // }

  // void CoilCoolingDXVariableSpeed::resetCondensateCollectionWaterStorageTank() {
  //   getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->resetCondensateCollectionWaterStorageTank();
  // }

  bool CoilCoolingDXVariableSpeed::setBasinHeaterCapacity(double basinHeaterCapacity) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
  }

  bool CoilCoolingDXVariableSpeed::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
  }

  bool CoilCoolingDXVariableSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setBasinHeaterOperatingSchedule(schedule);
  }

  void CoilCoolingDXVariableSpeed::resetBasinHeaterOperatingSchedule() {
    getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
  }

  double CoilCoolingDXVariableSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
  }

  bool CoilCoolingDXVariableSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
    double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      minimumOutdoorDryBulbTemperatureforCompressorOperation);
  }

  std::vector<CoilCoolingDXVariableSpeedSpeedData> CoilCoolingDXVariableSpeed::speeds() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->speeds();
  }

  bool CoilCoolingDXVariableSpeed::addSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed) {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->addSpeed(speed);
  }

  void CoilCoolingDXVariableSpeed::removeSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed) {
    getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->removeSpeed(speed);
  }

  void CoilCoolingDXVariableSpeed::removeAllSpeeds() {
    getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->removeAllSpeeds();
  }

  /// @cond
  CoilCoolingDXVariableSpeed::CoilCoolingDXVariableSpeed(std::shared_ptr<detail::CoilCoolingDXVariableSpeed_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> CoilCoolingDXVariableSpeed::autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
  }

  boost::optional<double> CoilCoolingDXVariableSpeed::autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel();
  }

  boost::optional<double> CoilCoolingDXVariableSpeed::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizedEvaporativeCondenserPumpRatedPowerConsumption();
  }

}  // namespace model
}  // namespace openstudio
