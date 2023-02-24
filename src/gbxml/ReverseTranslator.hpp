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

// krishnan adding a test comment for SVN check-in

#ifndef GBXML_REVERSETRANSLATOR_HPP
#define GBXML_REVERSETRANSLATOR_HPP

#include "gbXMLAPI.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

#include "../utilities/units/Unit.hpp"
#include <unordered_map>

namespace pugi {
class xml_node;
class xml_document;
}  // namespace pugi

namespace openstudio {

class ProgressBar;

namespace model {
  class Model;
  class ModelObject;
  class Surface;
}  // namespace model

namespace gbxml {

  class GBXML_API ReverseTranslator
  {
   public:
    ReverseTranslator();

    ~ReverseTranslator();

    boost::optional<openstudio::model::Model> loadModel(const openstudio::path& path, ProgressBar* progressBar = nullptr);

    /** Get warning messages generated by the last translation. */
    std::vector<LogMessage> warnings() const;

    /** Get error messages generated by the last translation. */
    std::vector<LogMessage> errors() const;

   private:
    openstudio::Unit m_temperatureUnit;
    openstudio::Unit m_lengthUnit;
    double m_nonBaseMultiplier;
    double m_lengthMultiplier;
    openstudio::Unit m_areaUnit;
    openstudio::Unit m_volumeUnit;
    bool m_useSIUnitsForResults;

   private:
    // given id and name from XML (name may be empty) return an OS name
    std::string escapeName(const std::string& id, const std::string& name);

    std::map<std::string, openstudio::model::ModelObject> m_idToObjectMap;

    // In ReverseTranslator.cpp
    boost::optional<openstudio::model::Model> convert(const pugi::xml_node& root);
    boost::optional<openstudio::model::Model> translateGBXML(const pugi::xml_node& root);
    boost::optional<openstudio::model::ModelObject> translateCampus(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBuilding(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBuildingStory(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateThermalZone(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSpace(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSurface(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSubSurface(const pugi::xml_node& element, openstudio::model::Surface& surface);

    /** Set the model object Name and gbXMLId additional property as the id. */
    void translateId(const pugi::xml_node& element, openstudio::model::ModelObject& modelObject);

    /** Set the model object displayName additional property as the Name. */
    void translateName(const pugi::xml_node& element, openstudio::model::ModelObject& modelObject);

    /** Set the model object CADObjectId additional property as the CADObjectId. */
    void translateCADObjectId(const pugi::xml_node& element, openstudio::model::ModelObject& modelObject);

    // In MapSchedules.cpp
    boost::optional<openstudio::model::ModelObject> translateScheduleDay(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateScheduleWeek(const pugi::xml_node& element, const pugi::xml_node& root,
                                                                          openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSchedule(const pugi::xml_node& element, const pugi::xml_node& root,
                                                                      openstudio::model::Model& model);

    // In MapEnvelope.cpp
    boost::optional<openstudio::model::ModelObject> translateConstruction(const pugi::xml_node& element,
                                                                          const std::unordered_map<std::string, pugi::xml_node>& layerElements,
                                                                          openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateWindowType(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateMaterial(const pugi::xml_node& element, openstudio::model::Model& model);

    void validateSpaceSurfaces(openstudio::model::Model& model);

    StringStreamLogSink m_logSink;

    ProgressBar* m_progressBar;

    REGISTER_LOGGER("openstudio.gbxml.ReverseTranslator");
  };

}  // namespace gbxml
}  // namespace openstudio

#endif  // GBXML_REVERSETRANSLATOR_HPP
