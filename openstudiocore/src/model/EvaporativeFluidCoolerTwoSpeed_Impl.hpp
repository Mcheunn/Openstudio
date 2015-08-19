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

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
class Schedule;
class WaterStorageTank;

namespace detail {

  /** EvaporativeFluidCoolerTwoSpeed_Impl is a StraightComponent_Impl that is the implementation class for EvaporativeFluidCoolerTwoSpeed.*/
  class MODEL_API EvaporativeFluidCoolerTwoSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EvaporativeFluidCoolerTwoSpeed_Impl(const IdfObject& idfObject,
                                        Model_Impl* model,
                                        bool keepHandle);

    EvaporativeFluidCoolerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    EvaporativeFluidCoolerTwoSpeed_Impl(const EvaporativeFluidCoolerTwoSpeed_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    virtual ~EvaporativeFluidCoolerTwoSpeed_Impl() {}

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
    Connection waterInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection waterOutletNode() const;

    boost::optional<double> highFanSpeedAirFlowRate() const;

    bool isHighFanSpeedAirFlowRateAutosized() const;

    boost::optional<double> highFanSpeedFanPower() const;

    bool isHighFanSpeedFanPowerAutosized() const;

    boost::optional<double> lowFanSpeedAirFlowRate() const;

    bool isLowFanSpeedAirFlowRateAutocalculated() const;

    double lowFanSpeedAirFlowRateSizingFactor() const;

    boost::optional<double> lowFanSpeedFanPower() const;

    bool isLowFanSpeedFanPowerAutocalculated() const;

    double lowFanSpeedFanPowerSizingFactor() const;

    double designSprayWaterFlowRate() const;

    std::string performanceInputMethod() const;

    boost::optional<std::string> outdoorAirInletNodeName() const;

    double heatRejectionCapacityandNominalCapacitySizingRatio() const;

    boost::optional<double> highSpeedStandardDesignCapacity() const;

    boost::optional<double> lowSpeedStandardDesignCapacity() const;

    bool isLowSpeedStandardDesignCapacityAutocalculated() const;

    double lowSpeedStandardCapacitySizingFactor() const;

    boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

    bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

    boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

    bool isLowFanSpeedUfactorTimesAreaValueAutocalculated() const;

    double lowFanSpeedUFactorTimesAreaSizingFactor() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> highSpeedUserSpecifiedDesignCapacity() const;

    boost::optional<double> lowSpeedUserSpecifiedDesignCapacity() const;

    bool isLowSpeedUserSpecifiedDesignCapacityAutocalculated() const;

    double lowSpeedUserSpecifiedDesignCapacitySizingFactor() const;

    boost::optional<double> designEnteringWaterTemperature() const;

    boost::optional<double> designEnteringAirTemperature() const;

    boost::optional<double> designEnteringAirWetbulbTemperature() const;

    double highSpeedSizingFactor() const;

    std::string evaporationLossMode() const;

    boost::optional<double> evaporationLossFactor() const;

    double driftLossPercent() const;

    std::string blowdownCalculationMode() const;

    double blowdownConcentrationRatio() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
    boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setWaterInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setWaterOutletNode(const Connection& connection);

    bool setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate);

    void autosizeHighFanSpeedAirFlowRate();

    bool setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower);

    void autosizeHighFanSpeedFanPower();

    bool setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate);

    void autocalculateLowFanSpeedAirFlowRate();

    void setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

    bool setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower);

    void autocalculateLowFanSpeedFanPower();

    void setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

    bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

    bool setPerformanceInputMethod(std::string performanceInputMethod);

    void setOutdoorAirInletNodeName(boost::optional<std::string> outdoorAirInletNodeName);

    void resetOutdoorAirInletNodeName();

    void setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio);

    bool setHighSpeedStandardDesignCapacity(boost::optional<double> highSpeedStandardDesignCapacity);

    void resetHighSpeedStandardDesignCapacity();

    bool setLowSpeedStandardDesignCapacity(boost::optional<double> lowSpeedStandardDesignCapacity);

    void resetLowSpeedStandardDesignCapacity();

    void autocalculateLowSpeedStandardDesignCapacity();

    void setLowSpeedStandardCapacitySizingFactor(double lowSpeedStandardCapacitySizingFactor);

    bool setHighFanSpeedUfactorTimesAreaValue(boost::optional<double> highFanSpeedUfactorTimesAreaValue);

    void resetHighFanSpeedUfactorTimesAreaValue();

    void autosizeHighFanSpeedUfactorTimesAreaValue();

    bool setLowFanSpeedUfactorTimesAreaValue(boost::optional<double> lowFanSpeedUfactorTimesAreaValue);

    void resetLowFanSpeedUfactorTimesAreaValue();

    void autocalculateLowFanSpeedUfactorTimesAreaValue();

    void setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setHighSpeedUserSpecifiedDesignCapacity(boost::optional<double> highSpeedUserSpecifiedDesignCapacity);

    void resetHighSpeedUserSpecifiedDesignCapacity();

    bool setLowSpeedUserSpecifiedDesignCapacity(boost::optional<double> lowSpeedUserSpecifiedDesignCapacity);

    void resetLowSpeedUserSpecifiedDesignCapacity();

    void autocalculateLowSpeedUserSpecifiedDesignCapacity();

    void setLowSpeedUserSpecifiedDesignCapacitySizingFactor(double lowSpeedUserSpecifiedDesignCapacitySizingFactor);

    bool setDesignEnteringWaterTemperature(boost::optional<double> designEnteringWaterTemperature);

    void resetDesignEnteringWaterTemperature();

    bool setDesignEnteringAirTemperature(boost::optional<double> designEnteringAirTemperature);

    void resetDesignEnteringAirTemperature();

    bool setDesignEnteringAirWetbulbTemperature(boost::optional<double> designEnteringAirWetbulbTemperature);

    void resetDesignEnteringAirWetbulbTemperature();

    bool setHighSpeedSizingFactor(double highSpeedSizingFactor);

    bool setEvaporationLossMode(std::string evaporationLossMode);

    void setEvaporationLossFactor(boost::optional<double> evaporationLossFactor);

    void resetEvaporationLossFactor();

    void setDriftLossPercent(double driftLossPercent);

    bool setBlowdownCalculationMode(std::string blowdownCalculationMode);

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

    void resetBlowdownMakeupWaterUsageSchedule();

    // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
    bool setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    void resetSupplyWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerTwoSpeed");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalWaterInletNode() const;
    boost::optional<Connection> optionalWaterOutletNode() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP

