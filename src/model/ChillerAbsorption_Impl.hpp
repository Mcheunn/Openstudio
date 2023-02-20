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

#ifndef MODEL_CHILLERABSORPTION_IMPL_HPP
#define MODEL_CHILLERABSORPTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    /** ChillerAbsorption_Impl is a WaterToWaterComponent_Impl that is the implementation class for ChillerAbsorption.*/
    class MODEL_API ChillerAbsorption_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ChillerAbsorption_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ChillerAbsorption_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ChillerAbsorption_Impl(const ChillerAbsorption_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ChillerAbsorption_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // chilledWaterLoop
      virtual unsigned supplyInletPort() const override;
      virtual unsigned supplyOutletPort() const override;

      // condenserWaterLoop
      virtual unsigned demandInletPort() const override;
      virtual unsigned demandOutletPort() const override;

      //generatorLoop
      virtual unsigned tertiaryInletPort() const override;
      virtual unsigned tertiaryOutletPort() const override;

      /* This function will perform a check if trying to add it to a node that is on the demand side of a plant loop.
     * If:
     *     - the node is on the demand side of a loop
     *     - the node isn't on the current condenser water loop itself
     *     - the chiller doesn't already have a generator (tertiary) loop,
     * then it tries to add it to the Tertiary loop.
     * In all other cases, it will call the base class' method WaterToWaterComponent_Impl::addToNode()
     */
      virtual bool addToNode(Node& node) override;

      /* Restricts addToTertiaryNode to a node that is on the supply side of a plant loop (tertiary = generator (heating) Loop)
     * Will also set the Generator Heat Source Type to 'HotWater' */
      virtual bool addToTertiaryNode(Node& node) override;

      /** Override to switch the Generator Heat Source Type to 'Steam' **/
      virtual bool removeFromTertiaryPlantLoop() override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<AppGFuelType> coolingFuelTypes() const override;
      virtual std::vector<AppGFuelType> heatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      boost::optional<double> nominalPumpingPower() const;

      bool isNominalPumpingPowerAutosized() const;

      double minimumPartLoadRatio() const;

      double maximumPartLoadRatio() const;

      double optimumPartLoadRatio() const;

      double designCondenserInletTemperature() const;

      boost::optional<double> designChilledWaterFlowRate() const;

      bool isDesignChilledWaterFlowRateAutosized() const;

      boost::optional<double> designCondenserWaterFlowRate() const;

      bool isDesignCondenserWaterFlowRateAutosized() const;

      double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve() const;

      double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve() const;

      double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve() const;

      double coefficient1ofthePumpElectricUsePartLoadRatioCurve() const;

      double coefficient2ofthePumpElectricUsePartLoadRatioCurve() const;

      double coefficient3ofthePumpElectricUsePartLoadRatioCurve() const;

      double chilledWaterOutletTemperatureLowerLimit() const;

      std::string chillerFlowMode() const;

      std::string generatorHeatSourceType() const;

      boost::optional<double> designGeneratorFluidFlowRate() const;

      bool isDesignGeneratorFluidFlowRateAutosized() const;

      double degreeofSubcoolinginSteamGenerator() const;

      double sizingFactor() const;

      //@}
      /** @name Setters */
      //@{

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void autosizeNominalCapacity();

      bool setNominalPumpingPower(boost::optional<double> nominalPumpingPower);

      void autosizeNominalPumpingPower();

      bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

      bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

      bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

      bool setDesignCondenserInletTemperature(double designCondenserInletTemperature);

      bool setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate);

      void autosizeDesignChilledWaterFlowRate();

      bool setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate);

      void autosizeDesignCondenserWaterFlowRate();

      bool setCoefficient1oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve);

      bool setCoefficient2oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve);

      bool setCoefficient3oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve);

      bool setCoefficient1ofthePumpElectricUsePartLoadRatioCurve(double coefficient1ofthePumpElectricUsePartLoadRatioCurve);

      bool setCoefficient2ofthePumpElectricUsePartLoadRatioCurve(double coefficient2ofthePumpElectricUsePartLoadRatioCurve);

      bool setCoefficient3ofthePumpElectricUsePartLoadRatioCurve(double coefficient3ofthePumpElectricUsePartLoadRatioCurve);

      bool setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit);

      bool setChillerFlowMode(const std::string& chillerFlowMode);

      bool setGeneratorHeatSourceType(const std::string& generatorHeatSourceType);

      bool setDesignGeneratorFluidFlowRate(boost::optional<double> designGeneratorFluidFlowRate);

      void autosizeDesignGeneratorFluidFlowRate();

      bool setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator);

      bool setSizingFactor(double sizingFactor);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedNominalCapacity() const;

      boost::optional<double> autosizedNominalPumpingPower() const;

      boost::optional<double> autosizedDesignChilledWaterFlowRate() const;

      boost::optional<double> autosizedDesignCondenserWaterFlowRate() const;

      boost::optional<double> autosizedDesignGeneratorFluidFlowRate() const;

      /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
      boost::optional<PlantLoop> chilledWaterLoop() const;

      /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
      boost::optional<PlantLoop> condenserWaterLoop() const;

      /** Convenience Function to return the Generator Loop (chiller on the demand side - tertiary) **/
      boost::optional<PlantLoop> generatorLoop() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ChillerAbsorption");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERABSORPTION_IMPL_HPP
