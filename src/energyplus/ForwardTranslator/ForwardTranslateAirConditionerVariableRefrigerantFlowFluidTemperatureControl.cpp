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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/ZoneTerminalUnitList.hpp"
#include "../../model/ZoneTerminalUnitList_Impl.hpp"

#include "../../model/Fluid.hpp"
#include "../../model/Fluid_Impl.hpp"

#include "../../model/UnivariateFunctions.hpp"
#include "../../model/UnivariateFunctions_Impl.hpp"

#include "../../model/BivariateFunctions.hpp"
#include "../../model/BivariateFunctions_Impl.hpp"

#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirConditionerVariableRefrigerantFlowFluidTemperatureControl( model::AirConditionerVariableRefrigerantFlowFluidTemperatureControl& modelObject )
{
  boost::optional<IdfObject> result;
  boost::optional<WorkspaceObject> _wo;
  boost::optional<ModelObject> _mo;

  // Instantiate an IdfObject of the class to store the values
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl, modelObject);
  // If it doesn't have a name, or if you aren't sure you are going to want to return it
  // IdfObject idfObject( openstudio::IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl );
  // m_idfObjects.push_back(idfObject);

  // TODO: Note JM 2018-10-17
  // You are responsible for implementing any additional logic based on choice fields, etc.
  // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

  // Heat Pump Name: Required String
  std::string heatPumpName = modelObject.heatPumpName();
  idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatPumpName, heatPumpName);


  // Availability Schedule Name: Optional Object
  if (boost::optional<Schedule> _availabilitySchedule = modelObject.availabilitySchedule()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_availabilitySchedule.get()) )  {
      idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilityScheduleName, _owo->nameString());
    }
  }

  // Zone Terminal Unit List Name: Required Object
  ZoneTerminalUnitList zoneTerminalUnitList = modelObject.zoneTerminalUnitList();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(zoneTerminalUnitList) )  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitListName, _owo->nameString());
  }

  // Refrigerant Type: Optional Object
  Fluid refrigerantType = modelObject.refrigerantType();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(refrigerantType) )  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantType, _owo->nameString());
  }

  if (modelObject.isRatedEvaporativeCapacityAutosized()) {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity, "Autosize");
  } else {
    // Rated Evaporative Capacity: boost::optional<double>
    if (boost::optional<double> _ratedEvaporativeCapacity = modelObject.ratedEvaporativeCapacity()) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity, _ratedEvaporativeCapacity.get());
    }
  }

  // Rated Compressor Power Per Unit of Rated Evaporative Capacity: Optional Double
  double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity = modelObject.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity, ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);


  // Minimum Outdoor Air Temperature in Cooling Mode: Optional Double
  double minimumOutdoorAirTemperatureinCoolingMode = modelObject.minimumOutdoorAirTemperatureinCoolingMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinCoolingMode, minimumOutdoorAirTemperatureinCoolingMode);


  // Maximum Outdoor Air Temperature in Cooling Mode: Optional Double
  double maximumOutdoorAirTemperatureinCoolingMode = modelObject.maximumOutdoorAirTemperatureinCoolingMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinCoolingMode, maximumOutdoorAirTemperatureinCoolingMode);


  // Minimum Outdoor Air Temperature in Heating Mode: Optional Double
  double minimumOutdoorAirTemperatureinHeatingMode = modelObject.minimumOutdoorAirTemperatureinHeatingMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinHeatingMode, minimumOutdoorAirTemperatureinHeatingMode);


  // Maximum Outdoor Air Temperature in Heating Mode: Optional Double
  double maximumOutdoorAirTemperatureinHeatingMode = modelObject.maximumOutdoorAirTemperatureinHeatingMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinHeatingMode, maximumOutdoorAirTemperatureinHeatingMode);


  // Reference Outdoor Unit Superheating: Optional Double
  double referenceOutdoorUnitSuperheating = modelObject.referenceOutdoorUnitSuperheating();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSuperheating, referenceOutdoorUnitSuperheating);


  // Reference Outdoor Unit Subcooling: Optional Double
  double referenceOutdoorUnitSubcooling = modelObject.referenceOutdoorUnitSubcooling();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSubcooling, referenceOutdoorUnitSubcooling);


  // Refrigerant Temperature Control Algorithm for Indoor Unit: Optional String
  std::string refrigerantTemperatureControlAlgorithmforIndoorUnit = modelObject.refrigerantTemperatureControlAlgorithmforIndoorUnit();
  idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit, refrigerantTemperatureControlAlgorithmforIndoorUnit);


  // Reference Evaporating Temperature for Indoor Unit: Optional Double
  double referenceEvaporatingTemperatureforIndoorUnit = modelObject.referenceEvaporatingTemperatureforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceEvaporatingTemperatureforIndoorUnit, referenceEvaporatingTemperatureforIndoorUnit);


  // Reference Condensing Temperature for Indoor Unit: Optional Double
  double referenceCondensingTemperatureforIndoorUnit = modelObject.referenceCondensingTemperatureforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceCondensingTemperatureforIndoorUnit, referenceCondensingTemperatureforIndoorUnit);


  // Variable Evaporating Temperature Minimum for Indoor Unit: Optional Double
  double variableEvaporatingTemperatureMinimumforIndoorUnit = modelObject.variableEvaporatingTemperatureMinimumforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMinimumforIndoorUnit, variableEvaporatingTemperatureMinimumforIndoorUnit);


  // Variable Evaporating Temperature Maximum for Indoor Unit: Optional Double
  double variableEvaporatingTemperatureMaximumforIndoorUnit = modelObject.variableEvaporatingTemperatureMaximumforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMaximumforIndoorUnit, variableEvaporatingTemperatureMaximumforIndoorUnit);


  // Variable Condensing Temperature Minimum for Indoor Unit: Optional Double
  double variableCondensingTemperatureMinimumforIndoorUnit = modelObject.variableCondensingTemperatureMinimumforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMinimumforIndoorUnit, variableCondensingTemperatureMinimumforIndoorUnit);


  // Variable Condensing Temperature Maximum for Indoor Unit: Optional Double
  double variableCondensingTemperatureMaximumforIndoorUnit = modelObject.variableCondensingTemperatureMaximumforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMaximumforIndoorUnit, variableCondensingTemperatureMaximumforIndoorUnit);


  // Outdoor Unit Fan Power Per Unit of Rated Evaporative Capacity: Optional Double
  double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity = modelObject.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity, outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);


  // Outdoor Unit Fan Flow Rate Per Unit of Rated Evaporative Capacity: Optional Double
  double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity = modelObject.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity, outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);


  // Outdoor Unit Evaporating Temperature Function of Superheating Curve Name: Required Object
  UnivariateFunctions outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve = modelObject.outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve) )  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName, _owo->nameString());
  }

  // Outdoor Unit Condensing Temperature Function of Subcooling Curve Name: Required Object
  UnivariateFunctions outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve = modelObject.outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve) )  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName, _owo->nameString());
  }

  // Diameter of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Optional Double
  double diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint = modelObject.diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);


  // Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Optional Double
  double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint = modelObject.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);


  // Equivalent Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Optional Double
  double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint = modelObject.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);


  // Height Difference Between Outdoor Unit and Indoor Units: Optional Double
  double heightDifferenceBetweenOutdoorUnitandIndoorUnits = modelObject.heightDifferenceBetweenOutdoorUnitandIndoorUnits();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits, heightDifferenceBetweenOutdoorUnitandIndoorUnits);


  // Main Pipe Insulation Thickness: Optional Double
  double mainPipeInsulationThickness = modelObject.mainPipeInsulationThickness();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThickness, mainPipeInsulationThickness);


  // Main Pipe Insulation Thermal Conductivity: Optional Double
  double mainPipeInsulationThermalConductivity = modelObject.mainPipeInsulationThermalConductivity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThermalConductivity, mainPipeInsulationThermalConductivity);


  // Crankcase Heater Power per Compressor: Optional Double
  double crankcaseHeaterPowerperCompressor = modelObject.crankcaseHeaterPowerperCompressor();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CrankcaseHeaterPowerperCompressor, crankcaseHeaterPowerperCompressor);


  // Number of Compressors: Optional Integer
  int numberofCompressors = modelObject.numberofCompressors();
  idfObject.setInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors, numberofCompressors);


  // Ratio of Compressor Size to Total Compressor Capacity: Optional Double
  double ratioofCompressorSizetoTotalCompressorCapacity = modelObject.ratioofCompressorSizetoTotalCompressorCapacity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatioofCompressorSizetoTotalCompressorCapacity, ratioofCompressorSizetoTotalCompressorCapacity);


  // Maximum Outdoor Dry-Bulb Temperature for Crankcase Heater: Optional Double
  double maximumOutdoorDryBulbTemperatureforCrankcaseHeater = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeater();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater, maximumOutdoorDryBulbTemperatureforCrankcaseHeater);


  // Defrost Strategy: Optional String
  std::string defrostStrategy = modelObject.defrostStrategy();
  idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy, defrostStrategy);


  // Defrost Control: Optional String
  std::string defrostControl = modelObject.defrostControl();
  idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl, defrostControl);


  // Defrost Energy Input Ratio Modifier Function of Temperature Curve Name: Optional Object
  if (boost::optional<BivariateFunctions> _defrostEnergyInputRatioModifierFunctionofTemperatureCurve = modelObject.defrostEnergyInputRatioModifierFunctionofTemperatureCurve()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_defrostEnergyInputRatioModifierFunctionofTemperatureCurve.get()) )  {
      idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName, _owo->nameString());
    }
  }

  // Defrost Time Period Fraction: Optional Double
  double defrostTimePeriodFraction = modelObject.defrostTimePeriodFraction();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);


  if (modelObject.isResistiveDefrostHeaterCapacityAutosized()) {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, "Autosize");
  } else {
    // Resistive Defrost Heater Capacity: boost::optional<double>
    if (boost::optional<double> _resistiveDefrostHeaterCapacity = modelObject.resistiveDefrostHeaterCapacity()) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, _resistiveDefrostHeaterCapacity.get());
    }
  }

  // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Optional Double
  double maximumOutdoorDrybulbTemperatureforDefrostOperation = modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, maximumOutdoorDrybulbTemperatureforDefrostOperation);


  // Compressor maximum delta Pressure: Optional Double
  double compressormaximumdeltaPressure = modelObject.compressormaximumdeltaPressure();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CompressormaximumdeltaPressure, compressormaximumdeltaPressure);


  // Number of Compressor Loading Index Entries: Optional Integer
  int numberofCompressorLoadingIndexEntries = modelObject.numberofCompressorLoadingIndexEntries();
  idfObject.setInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressorLoadingIndexEntries, numberofCompressorLoadingIndexEntries);


  return idfObject;
} // End of translate function

} // end namespace energyplus
} // end namespace openstudio