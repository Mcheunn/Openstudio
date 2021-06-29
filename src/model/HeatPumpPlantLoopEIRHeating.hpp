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

#ifndef MODEL_HEATPUMPPLANTLOOPEIRHEATING_HPP
#define MODEL_HEATPUMPPLANTLOOPEIRHEATING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class HeatPumpPlantLoopEIRCooling;
  class CurveBiquadratic;
  class CurveQuadratic;

  namespace detail {

    class HeatPumpPlantLoopEIRHeating_Impl;

  }  // namespace detail

  /** HeatPumpPlantLoopEIRHeating is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:PlantLoop:EIR:Heating'. */
  class MODEL_API HeatPumpPlantLoopEIRHeating : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatPumpPlantLoopEIRHeating(const Model& model, const CurveBiquadratic& capacityModifierFunctionofTemperatureCurve,
                                         const CurveBiquadratic& electricInputtoOutputRatioModifierFunctionofTemperatureCurve,
                                         const CurveQuadratic& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);

    explicit HeatPumpPlantLoopEIRHeating(const Model& model);

    virtual ~HeatPumpPlantLoopEIRHeating() {}

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string condenserType() const;

    boost::optional<HeatPumpPlantLoopEIRCooling> companionCoolingHeatPump() const;

    boost::optional<double> referenceLoadSideFlowRate() const;

    bool isReferenceLoadSideFlowRateAutosized() const;

    boost::optional<double> referenceSourceSideFlowRate() const;

    bool isReferenceSourceSideFlowRateAutosized() const;

    boost::optional<double> referenceCapacity() const;

    bool isReferenceCapacityAutosized() const;

    double referenceCoefficientofPerformance() const;

    double sizingFactor() const;

    CurveBiquadratic capacityModifierFunctionofTemperatureCurve() const;

    CurveBiquadratic electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const;

    CurveQuadratic electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setCondenserType(std::string condenserType);

    bool setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP);

    bool setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate);

    void autosizeReferenceLoadSideFlowRate();

    bool setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate);

    void autosizeReferenceSourceSideFlowRate();

    bool setReferenceCapacity(double referenceCapacity);

    void autosizeReferenceCapacity();

    bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

    bool setSizingFactor(double sizingFactor);

    bool setCapacityModifierFunctionofTemperatureCurve(const CurveBiquadratic& capacityModifierFunctionofTemperatureCurve);

    bool setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      const CurveBiquadratic& electricInputtoOutputRatioModifierFunctionofTemperatureCurve);

    bool setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
      const CurveQuadratic& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedReferenceLoadSideFlowRate() const;

    boost::optional<double> autosizedReferenceSourceSideFlowRate() const;

    boost::optional<double> autosizedReferenceCapacity() const;

    //@}
   protected:
    /// @cond
    typedef detail::HeatPumpPlantLoopEIRHeating_Impl ImplType;

    explicit HeatPumpPlantLoopEIRHeating(std::shared_ptr<detail::HeatPumpPlantLoopEIRHeating_Impl> impl);

    friend class detail::HeatPumpPlantLoopEIRHeating_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpPlantLoopEIRHeating");
  };

  /** \relates HeatPumpPlantLoopEIRHeating*/
  typedef boost::optional<HeatPumpPlantLoopEIRHeating> OptionalHeatPumpPlantLoopEIRHeating;

  /** \relates HeatPumpPlantLoopEIRHeating*/
  typedef std::vector<HeatPumpPlantLoopEIRHeating> HeatPumpPlantLoopEIRHeatingVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPPLANTLOOPEIRHEATING_HPP
