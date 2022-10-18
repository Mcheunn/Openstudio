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

#ifndef MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl is a ModelObject_Impl that is the implementation class for SiteGroundTemperatureUndisturbedKusudaAchenbach.*/
    class MODEL_API SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(const SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl& other, Model_Impl* model,
                                                           bool keepHandle);

      virtual ~SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl() {}

      //@}

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      double soilThermalConductivity() const;

      double soilDensity() const;

      double soilSpecificHeat() const;

      boost::optional<double> averageSoilSurfaceTemperature() const;

      boost::optional<double> averageAmplitudeofSurfaceTemperature() const;

      boost::optional<double> phaseShiftofMinimumSurfaceTemperature() const;

      //@}
      /** @name Setters */
      //@{

      bool setSoilThermalConductivity(double soilThermalConductivity);

      bool setSoilDensity(double soilDensity);

      bool setSoilSpecificHeat(double soilSpecificHeat);

      bool setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature);

      void resetAverageSoilSurfaceTemperature();

      bool setAverageAmplitudeofSurfaceTemperature(double averageAmplitudeofSurfaceTemperature);

      void resetAverageAmplitudeofSurfaceTemperature();

      bool setPhaseShiftofMiniumSurfaceTemperature(double phaseShiftofMinimumSurfaceTemperature);

      void resetPhaseShiftofMinimumSurfaceTemperature();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureUndisturbedKusudaAchenbach");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_IMPL_HPP
