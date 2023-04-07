/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef GBXML_FORWARDTRANSLATOR_HPP
#define GBXML_FORWARDTRANSLATOR_HPP

#include "gbXMLAPI.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"
// This is needed to declare the set<Material, IdfObjectImplLess>, so everything is a complete type
#include "../utilities/idf/IdfObject.hpp"
#include "../model/Material.hpp"

#include "../model/ModelObject.hpp"

#include <map>

namespace pugi {
class xml_node;
class xml_document;
}  // namespace pugi

namespace openstudio {

class ProgressBar;
class Transformation;

namespace model {
  class Model;
  class ConstructionBase;
  class Facility;
  class Building;
  class BuildingStory;
  class ThermalZone;
  class Space;
  class ShadingSurfaceGroup;
  class BuildingStory;
  class Surface;
  class SubSurface;
  class ShadingSurface;
}  // namespace model

namespace gbxml {

  class GBXML_API ForwardTranslator
  {
   public:
    ForwardTranslator();

    ~ForwardTranslator();

    // Save the GbXML to a file
    bool modelToGbXML(const openstudio::model::Model& model, const openstudio::path& path, ProgressBar* progressBar = nullptr);

    // Return a string representation of the GbXML document
    std::string modelToGbXMLString(const openstudio::model::Model& model, ProgressBar* progressBar = nullptr);

    /** Get warning messages generated by the last translation. */
    std::vector<LogMessage> warnings() const;

    /** Get error messages generated by the last translation. */
    std::vector<LogMessage> errors() const;

   private:
    std::string escapeName(const std::string& name);

    // listed in translation order
    bool translateModel(const openstudio::model::Model& model, pugi::xml_document& document);

    // Facility and Building could not be explicitly instantiated in the model, but the functions still need to be called so that Spaces and surfaces
    // are translated. Facility and Building both are UniqueModelObjects, so passing model here as an argument is harmless
    boost::optional<pugi::xml_node> translateFacility(const openstudio::model::Model& model, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateBuilding(const openstudio::model::Model& model, pugi::xml_node& parent);

    boost::optional<pugi::xml_node> translateSpace(const openstudio::model::Space& space, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateShadingSurfaceGroup(const openstudio::model::ShadingSurfaceGroup& shadingSurfaceGroup,
                                                                 pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateBuildingStory(const openstudio::model::BuildingStory& story, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateSurface(const openstudio::model::Surface& surface, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateSubSurface(const openstudio::model::SubSurface& subSurface,
                                                        const openstudio::Transformation& transformation, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateShadingSurface(const openstudio::model::ShadingSurface& shadingSurface, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateThermalZone(const openstudio::model::ThermalZone& thermalZone, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateLayer(const openstudio::model::Material& material, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateMaterial(const openstudio::model::Material& material, pugi::xml_node& parent);
    boost::optional<pugi::xml_node> translateConstructionBase(const openstudio::model::ConstructionBase& constructionBase, pugi::xml_node& parent);

    /** Set id as the model object name, otherwise use the gbXMLId additional property if it exists. */
    void translateId(const openstudio::model::ModelObject& modelObject, pugi::xml_node& parentElement);

    /** Set the Name as the model object name, otherwise use the displayName additional property if it exists. */
    void translateName(const openstudio::model::ModelObject& modelObject, pugi::xml_node& parentElement);

    /** Set the CADObjectId as the CADObjectId additional property if it exists. */
    boost::optional<pugi::xml_node> translateCADObjectId(const openstudio::model::ModelObject& modelObject, pugi::xml_node& parentElement);

    std::map<openstudio::Handle, pugi::xml_node> m_translatedObjects;

    std::set<openstudio::model::Material, openstudio::IdfObjectImplLess> m_materials;

    // Assigning a Shading Surface Construction is mostly pointless, so to avoid gbXML validation errors, we create one if need be
    bool m_placeholderShadingSurfaceConstructionAlreadyCreated = false;

    StringStreamLogSink m_logSink;

    ProgressBar* m_progressBar;

    REGISTER_LOGGER("openstudio.gbxml.ForwardTranslator");
  };

}  // namespace gbxml
}  // namespace openstudio

#endif  // GBXML_FORWARDTRANSLATOR_HPP
