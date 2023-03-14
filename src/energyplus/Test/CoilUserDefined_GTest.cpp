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

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilUserDefined.hpp"
#include "../../model/CoilUserDefined_Impl.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemActuator.hpp"
#include "../../model/Node.hpp"
#include "../../model/ThermalZone.hpp"

#include <utilities/idd/Coil_UserDefined_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilUserDefined_model) {
  Model model;

  CoilUserDefined coil(model);

  AirLoopHVAC airLoop(model);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  PlantLoop plant(model);
  plant.addDemandBranchForComponent(coil);

  std::string air_inname = coil.airInletModelObject().get().nameString();
  std::string air_outname = coil.airOutletModelObject().get().nameString();
  std::string plant_inname = coil.waterInletModelObject().get().nameString();
  std::string plant_outname = coil.waterOutletModelObject().get().nameString();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  // check objects and children are translated
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_UserDefined).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program).size());
  EXPECT_EQ(8u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());
  // check actuators are setup
  WorkspaceObjectVector actuators = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
  EXPECT_EQ(8u, actuators.size());
  for (const auto& actuator : actuators) {
    EXPECT_EQ("Coil User Defined 1", actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    EXPECT_TRUE(actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get() == "Air Connection 1"
                || actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get() == "Plant Connection");
  }

  WorkspaceObjectVector idf_coil(workspace.getObjectsByType(IddObjectType::Coil_UserDefined));
  EXPECT_EQ(1u, idf_coil.size());
  WorkspaceObject ws_coil(idf_coil[0]);
  EXPECT_EQ("overallModelSimulationProgramCallingManager",
            ws_coil.getString(Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName, false).get());
  EXPECT_EQ("Yes", ws_coil.getString(Coil_UserDefinedFields::PlantConnectionisUsed, false).get());
  // check air connections
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::AirConnection1InletNodeName, false));
  EXPECT_EQ(air_inname, ws_coil.getString(Coil_UserDefinedFields::AirConnection1InletNodeName, false).get());
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::AirConnection1OutletNodeName, false));
  EXPECT_EQ(air_outname, ws_coil.getString(Coil_UserDefinedFields::AirConnection1OutletNodeName, false).get());

  // check plant connections
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::PlantConnectionInletNodeName, false));
  EXPECT_EQ(plant_inname, ws_coil.getString(Coil_UserDefinedFields::PlantConnectionInletNodeName, false).get());
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::PlantConnectionOutletNodeName, false));
  EXPECT_EQ(plant_outname, ws_coil.getString(Coil_UserDefinedFields::PlantConnectionOutletNodeName, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilUserDefined_examplemodel) {
  // Generate the example Model
  Model model = openstudio::model::exampleModel();
  // Get the single thermal Zone in the model
  AirLoopHVAC airLoop = model.getConcreteModelObjects<AirLoopHVAC>()[0];
  //OS:Coil:Cooling:DX:SingleSpeed
  Node supplyOutletNode = airLoop.supplyOutletNode();

  // Get the single thermal Zone in the model
  ThermalZone z = model.getConcreteModelObjects<ThermalZone>()[0];
  std::string tz_name = z.nameString();
  CoilUserDefined coil(model);
  coil.setAmbientZone(z);
  coil.addToNode(supplyOutletNode);

  PlantLoop plant(model);
  plant.addDemandBranchForComponent(coil);

  std::string air_inname = coil.airInletModelObject().get().nameString();
  std::string air_outname = coil.airOutletModelObject().get().nameString();
  std::string plant_inname = coil.waterInletModelObject().get().nameString();
  std::string plant_outname = coil.waterOutletModelObject().get().nameString();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  // check objects and children are translated
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_UserDefined).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program).size());
  EXPECT_EQ(8u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());
  // check actuators are setup
  WorkspaceObjectVector actuators = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
  EXPECT_EQ(8u, actuators.size());
  for (const auto& actuator : actuators) {
    EXPECT_EQ("Coil User Defined 1", actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    EXPECT_TRUE(actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get() == "Air Connection 1"
                || actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get() == "Plant Connection");
  }

  WorkspaceObjectVector idf_coil(workspace.getObjectsByType(IddObjectType::Coil_UserDefined));
  EXPECT_EQ(1u, idf_coil.size());
  WorkspaceObject ws_coil(idf_coil[0]);
  EXPECT_EQ("overallModelSimulationProgramCallingManager",
            ws_coil.getString(Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName, false).get());
  EXPECT_EQ("Yes", ws_coil.getString(Coil_UserDefinedFields::PlantConnectionisUsed, false).get());
  // check air connections
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::AirConnection1InletNodeName, false));
  EXPECT_EQ(air_inname, ws_coil.getString(Coil_UserDefinedFields::AirConnection1InletNodeName, false).get());
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::AirConnection1OutletNodeName, false));
  EXPECT_EQ(air_outname, ws_coil.getString(Coil_UserDefinedFields::AirConnection1OutletNodeName, false).get());

  // check plant connections
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::PlantConnectionInletNodeName, false));
  EXPECT_EQ(plant_inname, ws_coil.getString(Coil_UserDefinedFields::PlantConnectionInletNodeName, false).get());
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::PlantConnectionOutletNodeName, false));
  EXPECT_EQ(plant_outname, ws_coil.getString(Coil_UserDefinedFields::PlantConnectionOutletNodeName, false).get());

  // check ambient zone
  ASSERT_TRUE(ws_coil.getString(Coil_UserDefinedFields::AmbientZoneName, false));
  EXPECT_EQ(tz_name, ws_coil.getString(Coil_UserDefinedFields::AmbientZoneName, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilUserDefined_NoPlant) {
  // Generate the example Model
  Model model = openstudio::model::exampleModel();
  // Get the single thermal Zone in the model
  AirLoopHVAC airLoop = model.getConcreteModelObjects<AirLoopHVAC>()[0];
  //OS:Coil:Cooling:DX:SingleSpeed
  Node supplyOutletNode = airLoop.supplyOutletNode();

  // Get the single thermal Zone in the model
  ThermalZone z = model.getConcreteModelObjects<ThermalZone>()[0];
  std::string tz_name = z.nameString();
  CoilUserDefined coil(model);
  coil.setAmbientZone(z);
  coil.addToNode(supplyOutletNode);

  std::string air_inname = coil.airInletModelObject().get().nameString();
  std::string air_outname = coil.airOutletModelObject().get().nameString();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  // check objects and children are translated
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_UserDefined).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program).size());
  EXPECT_EQ(3u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());
  // check actuators are setup
  WorkspaceObjectVector actuators = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
  EXPECT_EQ(3u, actuators.size());
  for (const auto& actuator : actuators) {
    EXPECT_EQ("Coil User Defined 1", actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
    EXPECT_TRUE(actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get() == "Air Connection 1");
  }
  std::string file_path = "c:\\Temp\\CoilUserDefined_constructor.osm";
  model.save(toPath(file_path), true);
  file_path = "c:\\Temp\\CoilUserDefined_constructor.idf";
  workspace.save(toPath(file_path), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilUserDefined_setters) {
    Model model;

    CoilUserDefined coil(model);

    AirLoopHVAC airLoop(model);
    Node supplyOutletNode = airLoop.supplyOutletNode();

    coil.addToNode(supplyOutletNode);

    PlantLoop plant(model);
    plant.addDemandBranchForComponent(coil);

    // make new actuators to test setters and remove the old ones
    coil.airOutletTemperatureActuator().get().remove();
    EnergyManagementSystemActuator aota(coil, "Air Connection 1", "Outlet Temperature");
    aota.setName("airOutletTemperature new");
    EXPECT_TRUE(coil.setAirOutletTemperatureActuator(aota));    

    coil.airOutletHumidityRatioActuator().get().remove();
    EnergyManagementSystemActuator aohra(coil, "Air Connection 1", "Outlet Humidity Ratio");
    aohra.setName("airOutletHumidityRatio new");
    EXPECT_TRUE(coil.setAirOutletHumidityRatioActuator(aohra));

    coil.airMassFlowRateActuator().get().remove();
    EnergyManagementSystemActuator amfra(coil, "Air Connection 1", "Mass Flow Rate");
    amfra.setName("airMassFlowRate new");
    EXPECT_TRUE(coil.setAirMassFlowRateActuator(amfra));

    coil.plantMinimumMassFlowRateActuator().get().remove();
    EnergyManagementSystemActuator pminmfra(coil, "Plant Connection", "Minimum Mass Flow Rate");
    pminmfra.setName("plantMinimumMassFlowRate new");
    EXPECT_TRUE(coil.setPlantMinimumMassFlowRateActuator(pminmfra));

    coil.plantMaximumMassFlowRateActuator().get().remove();
    EnergyManagementSystemActuator pmaxmfra(coil, "Plant Connection", "Maximum Mass Flow Rate");
    pmaxmfra.setName("plantMaximumMassFlowRate new");
    EXPECT_TRUE(coil.setPlantMaximumMassFlowRateActuator(pmaxmfra));

    coil.plantDesignVolumeFlowRateActuator().get().remove();
    EnergyManagementSystemActuator pdvfra(coil, "Plant Connection", "Design Volume Flow Rate");
    pdvfra.setName("plantDesignVolumeFlowRate new");
    EXPECT_TRUE(coil.setPlantDesignVolumeFlowRateActuator(pdvfra));

    coil.plantMassFlowRateActuator().get().remove();
    EnergyManagementSystemActuator pota(coil, "Plant Connection", "Mass Flow Rate");
    pota.setName("plantMassFlowRate new");
    EXPECT_TRUE(coil.setPlantMassFlowRateActuator(pota));

    coil.plantOutletTemperatureActuator().get().remove();
    EnergyManagementSystemActuator pmmfra(coil, "Plant Connection", "Outlet Temperature");
    pmmfra.setName("plantOutletTemperature new");
    EXPECT_TRUE(coil.setPlantOutletTemperatureActuator(pmmfra));


    //use setters for program and programmanager
    coil.overallModelSimulationProgramCallingManager().get().remove();
    coil.overallSimulationProgram().get().remove();
    EnergyManagementSystemProgram overAll(model);
    overAll.setName("overAllNew");
    EnergyManagementSystemProgramCallingManager overAllmgr(model);
    overAllmgr.setName("overAllmgrNew");
    overAllmgr.addProgram(overAll);
    EXPECT_TRUE(coil.setOverallModelSimulationProgramCallingManager(overAllmgr));
    EXPECT_TRUE(coil.setOverallSimulationProgram(overAll));

    coil.modelSetupandSizingProgramCallingManager().get().remove();
    coil.initializationSimulationProgram().get().remove();
    EnergyManagementSystemProgram init(model);
    init.setName("initNew");
    EnergyManagementSystemProgramCallingManager initmgr(model);
    initmgr.setName("initmgrNew");
    initmgr.addProgram(init);
    EXPECT_TRUE(coil.setModelSetupandSizingProgramCallingManager(initmgr));
    EXPECT_TRUE(coil.setInitializationSimulationProgram(init));

    ForwardTranslator forwardTranslator;
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(0u, forwardTranslator.errors().size());
    // check objects and children are translated
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_UserDefined).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_ProgramCallingManager).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Program).size());
    EXPECT_EQ(8u, workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator).size());
    // check actuators are setup
    WorkspaceObjectVector actuators = workspace.getObjectsByType(IddObjectType::EnergyManagementSystem_Actuator);
    EXPECT_EQ(8u, actuators.size());

    // loop thru the newly set actuators and expect names to be the new ones
    std::string act_name = "";
    std::string str1 = "airOutletTemperature_new";
    std::string str2 = "airOutletHumidityRatio_new";
    std::string str3 = "airMassFlowRate_new";
    std::string str4 = "plantMinimumMassFlowRate_new";
    std::string str5 = "plantMaximumMassFlowRate_new";
    std::string str6 = "plantDesignVolumeFlowRate_new";
    std::string str7 = "plantMassFlowRate_new";
    std::string str8 = "plantOutletTemperature_new";

    for (const auto& actuator : actuators) {
        EXPECT_EQ("Coil User Defined 1", actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName, false).get());
        EXPECT_TRUE(actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get() == "Air Connection 1"
            || actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType, false).get() == "Plant Connection");
        act_name = actuator.getString(EnergyManagementSystem_ActuatorFields::Name, false).get();
        EXPECT_TRUE(act_name == str1 || act_name == str2 || act_name == str3 || act_name == str4 || act_name == str5 || act_name == str6 || act_name == str7 || act_name == str8);
    }

    WorkspaceObjectVector idf_coil(workspace.getObjectsByType(IddObjectType::Coil_UserDefined));
    EXPECT_EQ(1u, idf_coil.size());
    WorkspaceObject ws_coil(idf_coil[0]);
    EXPECT_EQ("overAllmgrNew",
        ws_coil.getString(Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName, false).get());
    EXPECT_EQ("initmgrNew",
        ws_coil.getString(Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName, false).get());

     std::string file_path = "c:\\Temp\\CoilUserDefined_constructor.osm";
     model.save(toPath(file_path), true);
     file_path = "c:\\Temp\\CoilUserDefined_constructor.idf";
     workspace.save(toPath(file_path), true);
}
