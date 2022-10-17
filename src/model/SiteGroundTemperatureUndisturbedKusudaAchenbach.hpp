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

#ifndef MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_HPP
#define MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureUndisturbedKusudaAchenbach is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_UndisturbedKusudaAchenbach'. */
  class MODEL_API SiteGroundTemperatureUndisturbedKusudaAchenbach : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureUndisturbedKusudaAchenbach(Model& model);

    virtual ~SiteGroundTemperatureUndisturbedKusudaAchenbach() {}

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{



    //@}
    /** @name Setters */
    //@{



    //@}
   protected:
    /// @cond
    typedef detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl ImplType;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl;

    explicit SiteGroundTemperatureUndisturbedKusudaAchenbach(std::shared_ptr<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureUndisturbedKusudaAchenbach");
  };

  /** \relates SiteGroundTemperatureUndisturbedKusudaAchenbach*/
  typedef boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> OptionalSiteGroundTemperatureUndisturbedKusudaAchenbach;

  /** \relates SiteGroundTemperatureUndisturbedKusudaAchenbach*/
  typedef std::vector<SiteGroundTemperatureUndisturbedKusudaAchenbach> SiteGroundTemperatureUndisturbedKusudaAchenbachVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_HPP
