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

#include "ChillerHeaterPerformanceElectricEIR.hpp"
#include "ChillerHeaterPerformanceElectricEIR_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "BiquadraticCurves.hpp"
#include "BiquadraticCurves_Impl.hpp"
#include "BiquadraticCurves.hpp"
#include "BiquadraticCurves_Impl.hpp"
#include "BicubicCurves.hpp"
#include "BicubicCurves_Impl.hpp"
#include "BiquadraticCurves.hpp"
#include "BiquadraticCurves_Impl.hpp"
#include "BiquadraticCurves.hpp"
#include "BiquadraticCurves_Impl.hpp"
#include "BicubicCurves.hpp"
#include "BicubicCurves_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ChillerHeaterPerformance_Electric_EIR_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ChillerHeaterPerformanceElectricEIR_Impl::ChillerHeaterPerformanceElectricEIR_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ChillerHeaterPerformanceElectricEIR::iddObjectType());
  }

  ChillerHeaterPerformanceElectricEIR_Impl::ChillerHeaterPerformanceElectricEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ChillerHeaterPerformanceElectricEIR::iddObjectType());
  }

  ChillerHeaterPerformanceElectricEIR_Impl::ChillerHeaterPerformanceElectricEIR_Impl(const ChillerHeaterPerformanceElectricEIR_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ChillerHeaterPerformanceElectricEIR_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ChillerHeaterPerformanceElectricEIR_Impl::iddObjectType() const {
    return ChillerHeaterPerformanceElectricEIR::iddObjectType();
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeEvaporatorCapacity() const {
    return getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity,true);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::isReferenceCoolingModeEvaporatorCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeCOP() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeLeavingChilledWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingChilledWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeEnteringCondenserFluidTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEnteringCondenserFluidTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeLeavingCondenserWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingCondenserWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeCoolingCapacityRatio() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingCapacityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeCoolingPowerInputRatio() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingPowerInputRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeLeavingChilledWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingChilledWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeLeavingCondenserWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingCondenserWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeEnteringCondenserFluidTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeEnteringCondenserFluidTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::heatingModeEnteringChilledWaterTemperatureLowLimit() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeEnteringChilledWaterTemperatureLowLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerHeaterPerformanceElectricEIR_Impl::chilledWaterFlowModeType() const {
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::ChilledWaterFlowModeType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::designChilledWaterFlowRate() const {
    return getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate,true);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::isDesignChilledWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::designCondenserWaterFlowRate() const {
    return getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate,true);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::isDesignCondenserWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::designHotWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignHotWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::compressorMotorEfficiency() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::CompressorMotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerHeaterPerformanceElectricEIR_Impl::condenserType() const {
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::CondenserType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerHeaterPerformanceElectricEIR_Impl::coolingModeTemperatureCurveCondenserWaterIndependentVariable() const {
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeTemperatureCurveCondenserWaterIndependentVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  BiquadraticCurves ChillerHeaterPerformanceElectricEIR_Impl::coolingModeCoolingCapacityFunctionofTemperatureCurve() const {
    boost::optional<BiquadraticCurves> value = optionalCoolingModeCoolingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Mode Cooling Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  BiquadraticCurves ChillerHeaterPerformanceElectricEIR_Impl::coolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const {
    boost::optional<BiquadraticCurves> value = optionalCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Mode Electric Inputto Cooling Output Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  BicubicCurves ChillerHeaterPerformanceElectricEIR_Impl::coolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const {
    boost::optional<BicubicCurves> value = optionalCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Mode Electric Inputto Cooling Output Ratio Functionof Part Load Ratio Curve attached.");
    }
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::coolingModeCoolingCapacityOptimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityOptimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerHeaterPerformanceElectricEIR_Impl::heatingModeTemperatureCurveCondenserWaterIndependentVariable() const {
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeTemperatureCurveCondenserWaterIndependentVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  BiquadraticCurves ChillerHeaterPerformanceElectricEIR_Impl::heatingModeCoolingCapacityFunctionofTemperatureCurve() const {
    boost::optional<BiquadraticCurves> value = optionalHeatingModeCoolingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Mode Cooling Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  BiquadraticCurves ChillerHeaterPerformanceElectricEIR_Impl::heatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const {
    boost::optional<BiquadraticCurves> value = optionalHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Mode Electric Inputto Cooling Output Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  BicubicCurves ChillerHeaterPerformanceElectricEIR_Impl::heatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const {
    boost::optional<BicubicCurves> value = optionalHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Mode Electric Inputto Cooling Output Ratio Functionof Part Load Ratio Curve attached.");
    }
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::heatingModeCoolingCapacityOptimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityOptimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeEvaporatorCapacity(boost::optional<double> referenceCoolingModeEvaporatorCapacity) {
    bool result(false);
    if (referenceCoolingModeEvaporatorCapacity) {
      result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity, referenceCoolingModeEvaporatorCapacity.get());
    }
    return result;
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::autosizeReferenceCoolingModeEvaporatorCapacity() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeCOP(double referenceCoolingModeCOP) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeCOP, referenceCoolingModeCOP);
    return result;
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeLeavingChilledWaterTemperature(double referenceCoolingModeLeavingChilledWaterTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingChilledWaterTemperature, referenceCoolingModeLeavingChilledWaterTemperature);
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeEnteringCondenserFluidTemperature(double referenceCoolingModeEnteringCondenserFluidTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEnteringCondenserFluidTemperature, referenceCoolingModeEnteringCondenserFluidTemperature);
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeLeavingCondenserWaterTemperature(double referenceCoolingModeLeavingCondenserWaterTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingCondenserWaterTemperature, referenceCoolingModeLeavingCondenserWaterTemperature);
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeCoolingCapacityRatio(double referenceHeatingModeCoolingCapacityRatio) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingCapacityRatio, referenceHeatingModeCoolingCapacityRatio);
    OS_ASSERT(result);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeCoolingPowerInputRatio(double referenceHeatingModeCoolingPowerInputRatio) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingPowerInputRatio, referenceHeatingModeCoolingPowerInputRatio);
    return result;
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeLeavingChilledWaterTemperature(double referenceHeatingModeLeavingChilledWaterTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingChilledWaterTemperature, referenceHeatingModeLeavingChilledWaterTemperature);
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeLeavingCondenserWaterTemperature(double referenceHeatingModeLeavingCondenserWaterTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingCondenserWaterTemperature, referenceHeatingModeLeavingCondenserWaterTemperature);
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeEnteringCondenserFluidTemperature(double referenceHeatingModeEnteringCondenserFluidTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeEnteringCondenserFluidTemperature, referenceHeatingModeEnteringCondenserFluidTemperature);
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeEnteringChilledWaterTemperatureLowLimit(double heatingModeEnteringChilledWaterTemperatureLowLimit) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeEnteringChilledWaterTemperatureLowLimit, heatingModeEnteringChilledWaterTemperatureLowLimit);
    OS_ASSERT(result);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setChilledWaterFlowModeType(std::string chilledWaterFlowModeType) {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::ChilledWaterFlowModeType, chilledWaterFlowModeType);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate) {
    bool result(false);
    if (designChilledWaterFlowRate) {
      result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate, designChilledWaterFlowRate.get());
    }
    else {
      resetDesignChilledWaterFlowRate();
      result = true;
    }
    return result;
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::resetDesignChilledWaterFlowRate() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::autosizeDesignChilledWaterFlowRate() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate) {
    bool result(false);
    if (designCondenserWaterFlowRate) {
      result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate, designCondenserWaterFlowRate.get());
    }
    else {
      resetDesignCondenserWaterFlowRate();
      result = true;
    }
    return result;
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::resetDesignCondenserWaterFlowRate() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::autosizeDesignCondenserWaterFlowRate() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setDesignHotWaterFlowRate(double designHotWaterFlowRate) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignHotWaterFlowRate, designHotWaterFlowRate);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCompressorMotorEfficiency(double compressorMotorEfficiency) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::CompressorMotorEfficiency, compressorMotorEfficiency);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCondenserType(std::string condenserType) {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::CondenserType, condenserType);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(std::string coolingModeTemperatureCurveCondenserWaterIndependentVariable) {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeTemperatureCurveCondenserWaterIndependentVariable, coolingModeTemperatureCurveCondenserWaterIndependentVariable);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeCoolingCapacityFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityFunctionofTemperatureCurveName, biquadraticCurves.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName, biquadraticCurves.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(const BicubicCurves& bicubicCurves) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName, bicubicCurves.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeCoolingCapacityOptimumPartLoadRatio(double coolingModeCoolingCapacityOptimumPartLoadRatio) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityOptimumPartLoadRatio, coolingModeCoolingCapacityOptimumPartLoadRatio);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(std::string heatingModeTemperatureCurveCondenserWaterIndependentVariable) {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeTemperatureCurveCondenserWaterIndependentVariable, heatingModeTemperatureCurveCondenserWaterIndependentVariable);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeCoolingCapacityFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityFunctionofTemperatureCurveName, biquadraticCurves.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName, biquadraticCurves.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(const BicubicCurves& bicubicCurves) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName, bicubicCurves.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeCoolingCapacityOptimumPartLoadRatio(double heatingModeCoolingCapacityOptimumPartLoadRatio) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityOptimumPartLoadRatio, heatingModeCoolingCapacityOptimumPartLoadRatio);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::SizingFactor, sizingFactor);
    return result;
  }

  boost::optional<BiquadraticCurves> ChillerHeaterPerformanceElectricEIR_Impl::optionalCoolingModeCoolingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BiquadraticCurves>(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<BiquadraticCurves> ChillerHeaterPerformanceElectricEIR_Impl::optionalCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BiquadraticCurves>(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<BicubicCurves> ChillerHeaterPerformanceElectricEIR_Impl::optionalCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BicubicCurves>(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName);
  }

  boost::optional<BiquadraticCurves> ChillerHeaterPerformanceElectricEIR_Impl::optionalHeatingModeCoolingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BiquadraticCurves>(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<BiquadraticCurves> ChillerHeaterPerformanceElectricEIR_Impl::optionalHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BiquadraticCurves>(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<BicubicCurves> ChillerHeaterPerformanceElectricEIR_Impl::optionalHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BicubicCurves>(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName);
  }

} // detail

ChillerHeaterPerformanceElectricEIR::ChillerHeaterPerformanceElectricEIR(const Model& model)
  : ParentObject(ChillerHeaterPerformanceElectricEIR::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityFunctionofTemperatureCurveName
  //     OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName
  //     OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName
  //     OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityFunctionofTemperatureCurveName
  //     OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName
  //     OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setReferenceCoolingModeEvaporatorCapacity();
  OS_ASSERT(ok);
  // ok = setReferenceCoolingModeCOP();
  OS_ASSERT(ok);
  // setReferenceCoolingModeLeavingChilledWaterTemperature();
  // setReferenceCoolingModeEnteringCondenserFluidTemperature();
  // setReferenceCoolingModeLeavingCondenserWaterTemperature();
  // setReferenceHeatingModeCoolingCapacityRatio();
  // ok = setReferenceHeatingModeCoolingPowerInputRatio();
  OS_ASSERT(ok);
  // setReferenceHeatingModeLeavingChilledWaterTemperature();
  // setReferenceHeatingModeLeavingCondenserWaterTemperature();
  // setReferenceHeatingModeEnteringCondenserFluidTemperature();
  // setHeatingModeEnteringChilledWaterTemperatureLowLimit();
  // ok = setChilledWaterFlowModeType();
  OS_ASSERT(ok);
  // ok = setDesignHotWaterFlowRate();
  OS_ASSERT(ok);
  // ok = setCompressorMotorEfficiency();
  OS_ASSERT(ok);
  // ok = setCondenserType();
  OS_ASSERT(ok);
  // ok = setCoolingModeTemperatureCurveCondenserWaterIndependentVariable();
  OS_ASSERT(ok);
  // ok = setCoolingModeCoolingCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve();
  OS_ASSERT(ok);
  // ok = setCoolingModeCoolingCapacityOptimumPartLoadRatio();
  OS_ASSERT(ok);
  // ok = setHeatingModeTemperatureCurveCondenserWaterIndependentVariable();
  OS_ASSERT(ok);
  // ok = setHeatingModeCoolingCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve();
  OS_ASSERT(ok);
  // ok = setHeatingModeCoolingCapacityOptimumPartLoadRatio();
  OS_ASSERT(ok);
  // ok = setSizingFactor();
  OS_ASSERT(ok);
}

IddObjectType ChillerHeaterPerformanceElectricEIR::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ChillerHeaterPerformance_Electric_EIR);
}

std::vector<std::string> ChillerHeaterPerformanceElectricEIR::chilledWaterFlowModeTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ChillerHeaterPerformance_Electric_EIRFields::ChilledWaterFlowModeType);
}

std::vector<std::string> ChillerHeaterPerformanceElectricEIR::condenserTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ChillerHeaterPerformance_Electric_EIRFields::CondenserType);
}

std::vector<std::string> ChillerHeaterPerformanceElectricEIR::coolingModeTemperatureCurveCondenserWaterIndependentVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeTemperatureCurveCondenserWaterIndependentVariable);
}

std::vector<std::string> ChillerHeaterPerformanceElectricEIR::heatingModeTemperatureCurveCondenserWaterIndependentVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeTemperatureCurveCondenserWaterIndependentVariable);
}

boost::optional<double> ChillerHeaterPerformanceElectricEIR::referenceCoolingModeEvaporatorCapacity() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeEvaporatorCapacity();
}

bool ChillerHeaterPerformanceElectricEIR::isReferenceCoolingModeEvaporatorCapacityAutosized() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->isReferenceCoolingModeEvaporatorCapacityAutosized();
}

double ChillerHeaterPerformanceElectricEIR::referenceCoolingModeCOP() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeCOP();
}

double ChillerHeaterPerformanceElectricEIR::referenceCoolingModeLeavingChilledWaterTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeLeavingChilledWaterTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceCoolingModeEnteringCondenserFluidTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeEnteringCondenserFluidTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceCoolingModeLeavingCondenserWaterTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeLeavingCondenserWaterTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeCoolingCapacityRatio() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeCoolingCapacityRatio();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeCoolingPowerInputRatio() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeCoolingPowerInputRatio();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeLeavingChilledWaterTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeLeavingChilledWaterTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeLeavingCondenserWaterTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeLeavingCondenserWaterTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeEnteringCondenserFluidTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeEnteringCondenserFluidTemperature();
}

double ChillerHeaterPerformanceElectricEIR::heatingModeEnteringChilledWaterTemperatureLowLimit() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeEnteringChilledWaterTemperatureLowLimit();
}

std::string ChillerHeaterPerformanceElectricEIR::chilledWaterFlowModeType() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->chilledWaterFlowModeType();
}

boost::optional<double> ChillerHeaterPerformanceElectricEIR::designChilledWaterFlowRate() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->designChilledWaterFlowRate();
}

bool ChillerHeaterPerformanceElectricEIR::isDesignChilledWaterFlowRateAutosized() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->isDesignChilledWaterFlowRateAutosized();
}

boost::optional<double> ChillerHeaterPerformanceElectricEIR::designCondenserWaterFlowRate() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->designCondenserWaterFlowRate();
}

bool ChillerHeaterPerformanceElectricEIR::isDesignCondenserWaterFlowRateAutosized() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->isDesignCondenserWaterFlowRateAutosized();
}

double ChillerHeaterPerformanceElectricEIR::designHotWaterFlowRate() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->designHotWaterFlowRate();
}

double ChillerHeaterPerformanceElectricEIR::compressorMotorEfficiency() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->compressorMotorEfficiency();
}

std::string ChillerHeaterPerformanceElectricEIR::condenserType() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->condenserType();
}

std::string ChillerHeaterPerformanceElectricEIR::coolingModeTemperatureCurveCondenserWaterIndependentVariable() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeTemperatureCurveCondenserWaterIndependentVariable();
}

BiquadraticCurves ChillerHeaterPerformanceElectricEIR::coolingModeCoolingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeCoolingCapacityFunctionofTemperatureCurve();
}

BiquadraticCurves ChillerHeaterPerformanceElectricEIR::coolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve();
}

BicubicCurves ChillerHeaterPerformanceElectricEIR::coolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve();
}

double ChillerHeaterPerformanceElectricEIR::coolingModeCoolingCapacityOptimumPartLoadRatio() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeCoolingCapacityOptimumPartLoadRatio();
}

std::string ChillerHeaterPerformanceElectricEIR::heatingModeTemperatureCurveCondenserWaterIndependentVariable() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeTemperatureCurveCondenserWaterIndependentVariable();
}

BiquadraticCurves ChillerHeaterPerformanceElectricEIR::heatingModeCoolingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeCoolingCapacityFunctionofTemperatureCurve();
}

BiquadraticCurves ChillerHeaterPerformanceElectricEIR::heatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve();
}

BicubicCurves ChillerHeaterPerformanceElectricEIR::heatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve();
}

double ChillerHeaterPerformanceElectricEIR::heatingModeCoolingCapacityOptimumPartLoadRatio() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeCoolingCapacityOptimumPartLoadRatio();
}

double ChillerHeaterPerformanceElectricEIR::sizingFactor() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->sizingFactor();
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeEvaporatorCapacity(double referenceCoolingModeEvaporatorCapacity) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeEvaporatorCapacity(referenceCoolingModeEvaporatorCapacity);
}

void ChillerHeaterPerformanceElectricEIR::autosizeReferenceCoolingModeEvaporatorCapacity() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizeReferenceCoolingModeEvaporatorCapacity();
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeCOP(double referenceCoolingModeCOP) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeCOP(referenceCoolingModeCOP);
}

void ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeLeavingChilledWaterTemperature(double referenceCoolingModeLeavingChilledWaterTemperature) {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeLeavingChilledWaterTemperature(referenceCoolingModeLeavingChilledWaterTemperature);
}

void ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeEnteringCondenserFluidTemperature(double referenceCoolingModeEnteringCondenserFluidTemperature) {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeEnteringCondenserFluidTemperature(referenceCoolingModeEnteringCondenserFluidTemperature);
}

void ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeLeavingCondenserWaterTemperature(double referenceCoolingModeLeavingCondenserWaterTemperature) {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeLeavingCondenserWaterTemperature(referenceCoolingModeLeavingCondenserWaterTemperature);
}

void ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeCoolingCapacityRatio(double referenceHeatingModeCoolingCapacityRatio) {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeCoolingCapacityRatio(referenceHeatingModeCoolingCapacityRatio);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeCoolingPowerInputRatio(double referenceHeatingModeCoolingPowerInputRatio) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeCoolingPowerInputRatio(referenceHeatingModeCoolingPowerInputRatio);
}

void ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeLeavingChilledWaterTemperature(double referenceHeatingModeLeavingChilledWaterTemperature) {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeLeavingChilledWaterTemperature(referenceHeatingModeLeavingChilledWaterTemperature);
}

void ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeLeavingCondenserWaterTemperature(double referenceHeatingModeLeavingCondenserWaterTemperature) {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeLeavingCondenserWaterTemperature(referenceHeatingModeLeavingCondenserWaterTemperature);
}

void ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeEnteringCondenserFluidTemperature(double referenceHeatingModeEnteringCondenserFluidTemperature) {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeEnteringCondenserFluidTemperature(referenceHeatingModeEnteringCondenserFluidTemperature);
}

void ChillerHeaterPerformanceElectricEIR::setHeatingModeEnteringChilledWaterTemperatureLowLimit(double heatingModeEnteringChilledWaterTemperatureLowLimit) {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeEnteringChilledWaterTemperatureLowLimit(heatingModeEnteringChilledWaterTemperatureLowLimit);
}

bool ChillerHeaterPerformanceElectricEIR::setChilledWaterFlowModeType(std::string chilledWaterFlowModeType) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setChilledWaterFlowModeType(chilledWaterFlowModeType);
}

bool ChillerHeaterPerformanceElectricEIR::setDesignChilledWaterFlowRate(double designChilledWaterFlowRate) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setDesignChilledWaterFlowRate(designChilledWaterFlowRate);
}

void ChillerHeaterPerformanceElectricEIR::resetDesignChilledWaterFlowRate() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->resetDesignChilledWaterFlowRate();
}

void ChillerHeaterPerformanceElectricEIR::autosizeDesignChilledWaterFlowRate() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizeDesignChilledWaterFlowRate();
}

bool ChillerHeaterPerformanceElectricEIR::setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setDesignCondenserWaterFlowRate(designCondenserWaterFlowRate);
}

void ChillerHeaterPerformanceElectricEIR::resetDesignCondenserWaterFlowRate() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->resetDesignCondenserWaterFlowRate();
}

void ChillerHeaterPerformanceElectricEIR::autosizeDesignCondenserWaterFlowRate() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizeDesignCondenserWaterFlowRate();
}

bool ChillerHeaterPerformanceElectricEIR::setDesignHotWaterFlowRate(double designHotWaterFlowRate) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setDesignHotWaterFlowRate(designHotWaterFlowRate);
}

bool ChillerHeaterPerformanceElectricEIR::setCompressorMotorEfficiency(double compressorMotorEfficiency) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCompressorMotorEfficiency(compressorMotorEfficiency);
}

bool ChillerHeaterPerformanceElectricEIR::setCondenserType(std::string condenserType) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCondenserType(condenserType);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(std::string coolingModeTemperatureCurveCondenserWaterIndependentVariable) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(coolingModeTemperatureCurveCondenserWaterIndependentVariable);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeCoolingCapacityFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeCoolingCapacityFunctionofTemperatureCurve(biquadraticCurves);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(biquadraticCurves);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(const BicubicCurves& bicubicCurves) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(bicubicCurves);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeCoolingCapacityOptimumPartLoadRatio(double coolingModeCoolingCapacityOptimumPartLoadRatio) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeCoolingCapacityOptimumPartLoadRatio(coolingModeCoolingCapacityOptimumPartLoadRatio);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(std::string heatingModeTemperatureCurveCondenserWaterIndependentVariable) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(heatingModeTemperatureCurveCondenserWaterIndependentVariable);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeCoolingCapacityFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeCoolingCapacityFunctionofTemperatureCurve(biquadraticCurves);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(biquadraticCurves);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(const BicubicCurves& bicubicCurves) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(bicubicCurves);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeCoolingCapacityOptimumPartLoadRatio(double heatingModeCoolingCapacityOptimumPartLoadRatio) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeCoolingCapacityOptimumPartLoadRatio(heatingModeCoolingCapacityOptimumPartLoadRatio);
}

bool ChillerHeaterPerformanceElectricEIR::setSizingFactor(double sizingFactor) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setSizingFactor(sizingFactor);
}

/// @cond
ChillerHeaterPerformanceElectricEIR::ChillerHeaterPerformanceElectricEIR(std::shared_ptr<detail::ChillerHeaterPerformanceElectricEIR_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

