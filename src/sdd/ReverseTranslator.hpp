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

#ifndef SDD_REVERSETRANSLATOR_HPP
#define SDD_REVERSETRANSLATOR_HPP

#include "SDDAPI.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

#include "../model/Schedule.hpp"
#include "../model/AvailabilityManagerOptimumStart.hpp"
#include "../model/AvailabilityManagerNightCycle.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/AirConditionerVariableRefrigerantFlow.hpp"

namespace pugi {
class xml_node;
class xml_document;
}  // namespace pugi

namespace openstudio {

class ProgressBar;

namespace model {
  class Model;
  class ModelObject;
  class BuildingStory;
  class Space;
  class ShadingSurfaceGroup;
  class PlanarSurface;
  class Surface;
  class SubSurface;
  class PlantLoop;
}  // namespace model

namespace sdd {

  class SDD_API ReverseTranslator
  {
   public:
    // If masterAutosize = true the translator will ignore choices in the sdd file and forcibly autosize the entire model.
    ReverseTranslator(bool masterAutosize = false);

    virtual ~ReverseTranslator();

    boost::optional<openstudio::model::Model> loadModel(const openstudio::path& path, ProgressBar* progressBar = nullptr);

    /** Get warning messages generated by the last translation. */
    std::vector<LogMessage> warnings() const;

    /** Get error messages generated by the last translation. */
    std::vector<LogMessage> errors() const;

   private:
    std::string escapeName(std::string name);

    // listed in translation order
    boost::optional<openstudio::model::Model> convert(const pugi::xml_node& root);
    boost::optional<openstudio::model::Model> translateSDD(const pugi::xml_node& root);

    boost::optional<openstudio::model::ModelObject> translateRunPeriod(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSite(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateWaterMainsTemperature(const pugi::xml_node& element, openstudio::model::Model& model);
    std::vector<openstudio::WorkspaceObject> translateDesignDays(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateWeatherFile(const pugi::xml_node& element, openstudio::model::Model& model);

    boost::optional<openstudio::model::ModelObject> translateMaterial(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateConstructAssembly(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateScheduleDay(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateScheduleWeek(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSchedule(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateHoliday(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateDoorConstruction(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateFenestrationConstruction(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvCubic(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvDblQuad(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvQuad(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvLin(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvMapSglVar(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCrvMapDblVar(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCoilCoolingDXVariableRefrigerantFlow(const pugi::xml_node& element,
                                                                                                  openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCoilHeatingDXVariableRefrigerantFlow(const pugi::xml_node& element,
                                                                                                  openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateFluidSys(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translatePump(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBoiler(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateChiller(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateThrmlEngyStor(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateWtrHtr(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateHtRej(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateHX(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBuilding(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> createSpace(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> createThermalZone(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBuildingStory(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSpace(const pugi::xml_node& element, openstudio::model::BuildingStory& buildingStory);
    boost::optional<openstudio::model::ModelObject> translateLoads(const pugi::xml_node& element, openstudio::model::Space& space);
    boost::optional<openstudio::model::ModelObject> translateConvectionCoefficients(const pugi::xml_node& element,
                                                                                    openstudio::model::PlanarSurface& surface);
    boost::optional<openstudio::model::ModelObject> translateSurface(const pugi::xml_node& element, openstudio::model::Space& space);
    boost::optional<openstudio::model::ModelObject> translateSubSurface(const pugi::xml_node& element, openstudio::model::Surface& surface);
    boost::optional<openstudio::model::ModelObject> translateShadingSurface(const pugi::xml_node& element,
                                                                            openstudio::model::ShadingSurfaceGroup& shadingSurfaceGroup);
    boost::optional<openstudio::model::ModelObject> translateAirSystem(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCoilCooling(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateCoilHeating(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateEvapClr(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateFan(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateHtRcvry(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateThermalZone(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateTrmlUnit(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateVRFSys(const pugi::xml_node& element, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateZnSys(const pugi::xml_node& element, openstudio::model::Model& model);

    // Looks for a loop in the SDD instance with a segment named like the fluidSegInRefElement.text().as_string()
    // fluidSegInRefElement must correspond to the primary/secondary SUPPLY segment. If the object is supposed to be in the demand side
    // it's usually the 'FluidSegInRef' key (demand means the inlet is the supply, the outlet is the return)
    // If found then looks for a model::Loop with that name and returns it
    // This is useful for hooking water coils up to their plant and maybe other things.
    boost::optional<model::PlantLoop> loopForSupplySegment(const pugi::xml_node& fluidSegInRefElement, openstudio::model::Model& model);

    // Return the supply segment by the fluidSegInRefElement's text
    pugi::xml_node supplySegment(const pugi::xml_node& fluidSegInRefElement);

    // Returns the ServiceHotWater loop in the SDD instance with a segment named like the fluidSegInRefElement.text().as_string()
    // // If the loop is not found in the model, this function will attempt to translate it out of the SDD.
    // If the loop is found in the model it will simply be returned.
    boost::optional<model::PlantLoop> serviceHotWaterLoopForSupplySegment(const pugi::xml_node& fluidSegInRefElement,
                                                                          openstudio::model::Model& model);

    // Return the "ZnSys" element with the name matching the znSysRefElement.text().as_string()
    pugi::xml_node findZnSysElement(const pugi::xml_node& znSysRefElement);

    // Find the AirSys element that is named like the airSyRefElement.text().as_string()
    pugi::xml_node findAirSysElement(const pugi::xml_node& airSyRefElement);
    // projectElement has to be the 'Proj' one
    pugi::xml_node findAirSysElement(const std::string& airSysName, const pugi::xml_node& projectElement);

    // Return the "TrmlUnit" element serving a zone named znNameElement.text().as_string()
    pugi::xml_node findTrmlUnitElementForZone(const pugi::xml_node& znNameElement);

    model::Schedule defaultDeckTempSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_defaultDeckTempSchedule;

    model::Schedule hotWaterPlantSetpointSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_hotWaterPlantSetpointSchedule;

    model::Schedule chilledWaterPlantSetpointSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_chilledWaterPlantSetpointSchedule;

    model::Schedule serviceHotWaterSetpointSchedule(openstudio::model::Model& model);
    boost::optional<model::Schedule> m_serviceHotWaterSetpointSchedule;

    model::ConstructionBase shadingConstruction(openstudio::model::Model& model, double solRefl, double visRefl);
    std::map<std::pair<double, double>, model::ConstructionBase> m_shadingConstructionMap;

    model::Schedule shadingSchedule(openstudio::model::Model& model, double trans);
    std::map<double, model::Schedule> m_shadingScheduleMap;

    //helper method to do unit conversions; probably should be in OS proper
    boost::optional<double> unitToUnit(double val, const std::string& ipUnitString, const std::string& siUnitString);

    bool m_isInputXML;

    bool autosize() const;

    bool m_autosize;

    bool m_masterAutosize;

    StringStreamLogSink m_logSink;

    openstudio::path m_path;

    ProgressBar* m_progressBar;

    // This is storage to match control zones with optimum start AVMs
    // This is used because the ThermalZone instances are not yet created when
    // the air system and AVMs are translated.
    // This map is populated when the air systems are translated,
    // then used to add the control zones when the zones are translated later.
    // The map is zone name => AVM
    std::map<std::string, model::AvailabilityManagerOptimumStart> m_optimumStartControlZones;

    // Similar to m_optimumStartControlZones but for night cycle manager
    std::map<std::string, model::AvailabilityManagerNightCycle> m_nightCycleControlZones;

    // Map from vrf system to master control zone name
    std::map<std::string, model::AirConditionerVariableRefrigerantFlow> m_vrfSystemControlZones;

    REGISTER_LOGGER("openstudio.sdd.ReverseTranslator");
  };

}  // namespace sdd
}  // namespace openstudio

#endif  // SDD_REVERSETRANSLATOR_HPP
