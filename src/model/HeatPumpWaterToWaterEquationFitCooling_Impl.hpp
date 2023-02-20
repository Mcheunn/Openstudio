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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

class AppGFuelType;
class ComponentType;

namespace model {

  class HeatPumpWaterToWaterEquationFitHeating;
  class CurveQuadLinear;

  namespace detail {

    /** HeatPumpWaterToWaterEquationFitCooling_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatPumpWaterToWaterEquationFitCooling.*/
    class MODEL_API HeatPumpWaterToWaterEquationFitCooling_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatPumpWaterToWaterEquationFitCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatPumpWaterToWaterEquationFitCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatPumpWaterToWaterEquationFitCooling_Impl(const HeatPumpWaterToWaterEquationFitCooling_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatPumpWaterToWaterEquationFitCooling_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned supplyInletPort() const override;

      virtual unsigned supplyOutletPort() const override;

      virtual unsigned demandInletPort() const override;

      virtual unsigned demandOutletPort() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<AppGFuelType> coolingFuelTypes() const override;
      virtual std::vector<AppGFuelType> heatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> referenceLoadSideFlowRate() const;

      double ratedLoadSideFlowRate() const;

      bool isReferenceLoadSideFlowRateAutosized() const;

      boost::optional<double> referenceSourceSideFlowRate() const;

      double ratedSourceSideFlowRate() const;

      bool isReferenceSourceSideFlowRateAutosized() const;

      boost::optional<double> ratedCoolingCapacity() const;

      bool isRatedCoolingCapacityAutosized() const;

      boost::optional<double> ratedCoolingPowerConsumption() const;

      bool isRatedCoolingPowerConsumptionAutosized() const;

      CurveQuadLinear coolingCapacityCurve() const;

      CurveQuadLinear coolingCompressorPowerCurve() const;

      double referenceCoefficientofPerformance() const;

      double sizingFactor() const;

      boost::optional<HeatPumpWaterToWaterEquationFitHeating> companionHeatingHeatPump() const;

      boost::optional<double> autosizedReferenceLoadSideFlowRate() const;

      boost::optional<double> autosizedReferenceSourceSideFlowRate() const;

      boost::optional<double> autosizedRatedCoolingCapacity() const;

      boost::optional<double> autosizedRatedCoolingPowerConsumption() const;

      //@}
      /** @name Setters */
      //@{

      bool setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate);

      void autosizeReferenceLoadSideFlowRate();

      bool setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate);

      void autosizeReferenceSourceSideFlowRate();

      bool setRatedCoolingCapacity(double ratedCoolingCapacity);

      void autosizeRatedCoolingCapacity();

      bool setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption);

      void autosizeRatedCoolingPowerConsumption();

      bool setCoolingCapacityCurve(const CurveQuadLinear& coolingCapacityCurve);

      bool setCoolingCompressorPowerCurve(const CurveQuadLinear& coolingCompressorPowerCurve);

      bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

      bool setSizingFactor(double sizingFactor);

      bool setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitCooling");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP
