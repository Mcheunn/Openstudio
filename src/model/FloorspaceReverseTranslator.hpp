/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FLOORSPACEREVERSETRANSLATOR_HPP
#define MODEL_FLOORSPACEREVERSETRANSLATOR_HPP

#include "ModelAPI.hpp"

#include "Model.hpp"

#include "../utilities/floorspace/FSModel.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

#include <map>

namespace openstudio {
namespace model {

  class BuildingStory;
  class Space;

  class MODEL_API FloorspaceReverseTranslator
  {
   public:
    FloorspaceReverseTranslator();

    boost::optional<Model> modelFromFloorspace(const std::string& jsonOrpath);

    /// Convert a Floorspace modelto OpenStudio Model format, scene must be in OpenStudio format
    //boost::optional<Model> modelFromFloorspace(const FSModel& model);

    /// Get warning messages generated by the last translation.
    std::vector<LogMessage> warnings() const;

    /// Get error messages generated by the last translation.
    std::vector<LogMessage> errors() const;

    /// Mapping handles between the floorspac emodel and the open studio model
    std::map<UUID, UUID> handleMapping() const;

   private:
    REGISTER_LOGGER("openstudio.model.FloorspaceReverseTranslator");
    StringStreamLogSink m_logSink;

    std::map<UUID, UUID> m_handleMapping;

    static void DoSurfaceMatchingAndIntersection(Model& model);
    static std::vector<BuildingStory> SortStoriesByElevationAndName(Model& model);
    static std::vector<Space> SortSpacesByAreaAndName(BuildingStory& story);
  };

}  // namespace model
}  // namespace openstudio
#endif  //MODEL_FLOORSPACEREVERSETRANSLATOR_HPP
