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

#ifndef AIRFLOW_SURFACENETWORKBUILDER_HPP
#define AIRFLOW_SURFACENETWORKBUILDER_HPP

#include <vector>

#include "AirflowAPI.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

namespace openstudio {

class ProgressBar;
namespace model {
  class Model;
  class Space;
  class ThermalZone;
  class SubSurface;
  class Surface;
}  // namespace model

namespace airflow {

  /** SurfaceNetworkBuilder objects walk the OpenStudio model as a network of surfaces.
 *
 *  The SurfaceNetworkBuilder object walks the OpenStudio model as a network of surface,
 *  calling a different function once for each pair of interior surfaces or subsurfaces and
 *  once for each exterior surface or subsurface. The intended usage is to subclass for a
 *  specific application.
 *
 */
  class AIRFLOW_API SurfaceNetworkBuilder
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a new object. */
    explicit SurfaceNetworkBuilder(ProgressBar* progressBar = nullptr);
    /** Destroy the translator.*/
    virtual ~SurfaceNetworkBuilder() = default;

    //@}
    /** @name Network Construction Functions */
    //@{

    bool build(model::Model& model);

    //@}
    /** @name Getters and Setters */
    //@{

    /** Returns the progress bar currently associated with the object. */
    ProgressBar* progressBar() const;

    /** Sets the progress bar to be updated by the object. */
    void setProgressBar(ProgressBar* progressBar);

    //@}
    /** @name Miscellaneous Functions */
    //@{

    /** Clear any internally stored information in the builder object. */
    virtual void clear() {}

    /** Returns warning messages generated by the last translation. */
    std::vector<LogMessage> warnings() const;

    /** Returns error messages generated by the last translation. */
    std::vector<LogMessage> errors() const;

    /** Returns error messages generated by the last translation. */
    std::vector<LogMessage> logMessages() const;

    //@}

   protected:
    void progress();
    void initProgress(int max, std::string label);

    virtual bool linkExteriorSurface(model::ThermalZone zone, model::Space space, model::Surface surface);
    virtual bool linkExteriorSubSurface(model::ThermalZone zone, model::Space space, model::Surface surface, model::SubSurface subSurface);
    virtual bool linkInteriorSurface(model::ThermalZone zone, model::Space space, model::Surface surface, model::Surface adjacentSurface,
                                     model::Space adjacentSpace, model::ThermalZone adjacentZone);
    virtual bool linkInteriorSubSurface(model::ThermalZone zone, model::Space space, model::Surface surface, model::SubSurface subSurface,
                                        model::SubSurface adjacentSubSurface, model::Surface adjacentSurface, model::Space adjacentSpace,
                                        model::ThermalZone adjacentZone);

   private:
    ProgressBar* m_progressBar;
    StringStreamLogSink m_logSink;

    REGISTER_LOGGER("openstudio.airflow.SurfaceNetworkBuilder");
  };

}  // namespace airflow
}  // namespace openstudio

#endif  // AIRFLOW_SURFACENETWORKBUILDER_HPP
