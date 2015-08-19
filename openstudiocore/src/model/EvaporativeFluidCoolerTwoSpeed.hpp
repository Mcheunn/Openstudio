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

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
class Schedule;
class WaterStorageTank;

namespace detail {

  class EvaporativeFluidCoolerTwoSpeed_Impl;

} // detail

/** EvaporativeFluidCoolerTwoSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:EvaporativeFluidCooler:TwoSpeed'. */
class MODEL_API EvaporativeFluidCoolerTwoSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EvaporativeFluidCoolerTwoSpeed(const Model& model);

  virtual ~EvaporativeFluidCoolerTwoSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> performanceInputMethodValues();

  static std::vector<std::string> evaporationLossModeValues();

  static std::vector<std::string> blowdownCalculationModeValues();

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

  bool setHighFanSpeedAirFlowRate(double highFanSpeedAirFlowRate);

  void autosizeHighFanSpeedAirFlowRate();

  bool setHighFanSpeedFanPower(double highFanSpeedFanPower);

  void autosizeHighFanSpeedFanPower();

  bool setLowFanSpeedAirFlowRate(double lowFanSpeedAirFlowRate);

  void autocalculateLowFanSpeedAirFlowRate();

  void setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

  bool setLowFanSpeedFanPower(double lowFanSpeedFanPower);

  void autocalculateLowFanSpeedFanPower();

  void setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

  bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

  bool setPerformanceInputMethod(std::string performanceInputMethod);

  void setOutdoorAirInletNodeName(std::string outdoorAirInletNodeName);

  void resetOutdoorAirInletNodeName();

  void setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio);

  bool setHighSpeedStandardDesignCapacity(double highSpeedStandardDesignCapacity);

  void resetHighSpeedStandardDesignCapacity();

  bool setLowSpeedStandardDesignCapacity(double lowSpeedStandardDesignCapacity);

  void resetLowSpeedStandardDesignCapacity();

  void autocalculateLowSpeedStandardDesignCapacity();

  void setLowSpeedStandardCapacitySizingFactor(double lowSpeedStandardCapacitySizingFactor);

  bool setHighFanSpeedUfactorTimesAreaValue(double highFanSpeedUfactorTimesAreaValue);

  void resetHighFanSpeedUfactorTimesAreaValue();

  void autosizeHighFanSpeedUfactorTimesAreaValue();

  bool setLowFanSpeedUfactorTimesAreaValue(double lowFanSpeedUfactorTimesAreaValue);

  void resetLowFanSpeedUfactorTimesAreaValue();

  void autocalculateLowFanSpeedUfactorTimesAreaValue();

  void setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

  bool setDesignWaterFlowRate(double designWaterFlowRate);

  void resetDesignWaterFlowRate();

  void autosizeDesignWaterFlowRate();

  bool setHighSpeedUserSpecifiedDesignCapacity(double highSpeedUserSpecifiedDesignCapacity);

  void resetHighSpeedUserSpecifiedDesignCapacity();

  bool setLowSpeedUserSpecifiedDesignCapacity(double lowSpeedUserSpecifiedDesignCapacity);

  void resetLowSpeedUserSpecifiedDesignCapacity();

  void autocalculateLowSpeedUserSpecifiedDesignCapacity();

  void setLowSpeedUserSpecifiedDesignCapacitySizingFactor(double lowSpeedUserSpecifiedDesignCapacitySizingFactor);

  bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

  void resetDesignEnteringWaterTemperature();

  bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

  void resetDesignEnteringAirTemperature();

  bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

  void resetDesignEnteringAirWetbulbTemperature();

  bool setHighSpeedSizingFactor(double highSpeedSizingFactor);

  bool setEvaporationLossMode(std::string evaporationLossMode);

  void setEvaporationLossFactor(double evaporationLossFactor);

  void resetEvaporationLossFactor();

  void setDriftLossPercent(double driftLossPercent);

  bool setBlowdownCalculationMode(std::string blowdownCalculationMode);

  bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

  void resetBlowdownMakeupWaterUsageSchedule();

  // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
  bool setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank);

  void resetSupplyWaterStorageTank();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EvaporativeFluidCoolerTwoSpeed_Impl ImplType;

  explicit EvaporativeFluidCoolerTwoSpeed(std::shared_ptr<detail::EvaporativeFluidCoolerTwoSpeed_Impl> impl);

  friend class detail::EvaporativeFluidCoolerTwoSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerTwoSpeed");
};

/** \relates EvaporativeFluidCoolerTwoSpeed*/
typedef boost::optional<EvaporativeFluidCoolerTwoSpeed> OptionalEvaporativeFluidCoolerTwoSpeed;

/** \relates EvaporativeFluidCoolerTwoSpeed*/
typedef std::vector<EvaporativeFluidCoolerTwoSpeed> EvaporativeFluidCoolerTwoSpeedVector;

} // model
} // openstudio

#endif // MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_HPP

