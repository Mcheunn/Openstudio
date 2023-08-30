/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
