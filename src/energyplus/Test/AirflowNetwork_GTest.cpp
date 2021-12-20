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

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirflowNetworkSurface.hpp"
#include "../../model/AirflowNetworkSurface_Impl.hpp"
#include "../../model/AirflowNetworkSimulationControl.hpp"
#include "../../model/AirflowNetworkSimulationControl_Impl.hpp"
#include "../../model/AirflowNetworkDistributionNode.hpp"
#include "../../model/AirflowNetworkDistributionNode_Impl.hpp"
#include "../../model/AirflowNetworkDistributionLinkage.hpp"
#include "../../model/AirflowNetworkDistributionLinkage_Impl.hpp"
#include "../../model/AirflowNetworkEquivalentDuct.hpp"
#include "../../model/AirflowNetworkEquivalentDuct_Impl.hpp"
#include "../../model/AirflowNetworkSimpleOpening.hpp"
#include "../../model/AirflowNetworkSimpleOpening_Impl.hpp"
#include "../../model/AirflowNetworkDetailedOpening.hpp"
#include "../../model/AirflowNetworkDetailedOpening_Impl.hpp"
#include "../../model/AirflowNetworkHorizontalOpening.hpp"
#include "../../model/AirflowNetworkHorizontalOpening_Impl.hpp"
#include "../../model/AirflowNetworkEffectiveLeakageArea.hpp"
#include "../../model/AirflowNetworkEffectiveLeakageArea_Impl.hpp"
#include "../../model/AirflowNetworkSpecifiedFlowRate.hpp"
#include "../../model/AirflowNetworkSpecifiedFlowRate_Impl.hpp"
#include "../../model/AirflowNetworkFan.hpp"
#include "../../model/AirflowNetworkFan_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/SetpointManagerSingleZoneReheat.hpp"
#include "../../model/SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/AirflowNetwork_MultiZone_Surface_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Component_SimpleOpening_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Component_DetailedOpening_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Component_HorizontalOpening_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Surface_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_SpecifiedFlowRate_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

Model createModelWithSimpleAirLoop() {

  // Generate the example Model
  Model m = openstudio::model::exampleModel();

  // Remove the existing loop
  AirLoopHVAC example_loop = m.getModelObjects<AirLoopHVAC>()[0];
  example_loop.remove();

  AirLoopHVAC a(m);

  FanConstantVolume fan(m);
  Node outletNode = a.supplyOutletNode();
  fan.addToNode(outletNode);
  fan.setName("AirLoopHVAC Supply Fan");

  CoilCoolingDXSingleSpeed coilCooling(m);
  coilCooling.addToNode(outletNode);

  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  CoilHeatingGas coilHeatingGas(m, alwaysOn);
  coilHeatingGas.addToNode(outletNode);

  SetpointManagerSingleZoneReheat setpointMSZR(m);

  auto node1 = coilHeatingGas.outletModelObject().get();  // .to_Node.get
  //setpointMSZR.addToNode(node1);

  AirTerminalSingleDuctConstantVolumeNoReheat terminal(m, alwaysOn);

  a.addBranchForHVACComponent(terminal);

  // Get the single thermal Zone in the model
  ThermalZone z = m.getModelObjects<ThermalZone>()[0];

  a.addBranchForZone(z);
  auto outlet_node = a.supplyOutletNode();
  setpointMSZR.setControlZone(z);

  return m;
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirflowNetworkFanLinkage) {
  Model model;

  AirflowNetworkSimulationControl control = model.getUniqueModelObject<AirflowNetworkSimulationControl>();

  Node node0(model);
  //Node node1(model);

  AirflowNetworkDistributionNode afnnode0 = node0.getAirflowNetworkDistributionNode();
  AirflowNetworkDistributionNode afnnode1(model);  // = node1.getAirflowNetworkDistributionNode();

  CoilCoolingDXSingleSpeed coilCooling(model);
  FanConstantVolume fan(model);
  fan.setPressureRise(500);

  AirflowNetworkFan afnfan = fan.getAirflowNetworkFan();

  AirflowNetworkDistributionLinkage link(model, afnnode0, afnnode1, afnfan);

  EXPECT_TRUE(link.setComponent(afnfan));
  ASSERT_TRUE(link.component().componentModelObject());

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> links = workspace.getObjectsByType(IddObjectType::AirflowNetwork_Distribution_Linkage);

  ASSERT_EQ(1u, links.size());

  EXPECT_EQ(std::string("Airflow Network Distribution Linkage 1"), links[0].getString(0));
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirflowNetworkEquivalentDuct) {
  Model model = createModelWithSimpleAirLoop();

  std::vector<CoilHeatingGas> heatingCoils = model.getConcreteModelObjects<CoilHeatingGas>();
  ASSERT_EQ(1, heatingCoils.size());

  std::vector<CoilCoolingDXSingleSpeed> coolingCoils = model.getConcreteModelObjects<CoilCoolingDXSingleSpeed>();
  ASSERT_EQ(1, coolingCoils.size());

  AirflowNetworkSimulationControl control = model.getUniqueModelObject<AirflowNetworkSimulationControl>();

  AirflowNetworkEquivalentDuct cool = coolingCoils[0].getAirflowNetworkEquivalentDuct(0.1, 0.1);
  ASSERT_TRUE(cool.straightComponent());
  EXPECT_TRUE(cool.straightComponent().get().optionalCast<CoilCoolingDXSingleSpeed>());
  EXPECT_EQ(coolingCoils[0], cool.straightComponent().get().optionalCast<CoilCoolingDXSingleSpeed>().get());
  ASSERT_TRUE(cool.coilObjectType());
  EXPECT_EQ(std::string("Coil:Cooling:DX:SingleSpeed"), cool.coilObjectType().get());

  AirflowNetworkEquivalentDuct eqd = heatingCoils[0].getAirflowNetworkEquivalentDuct(0.1, 0.1);

  auto comp = eqd.straightComponent();
  ASSERT_TRUE(comp);
  auto opt = comp.get().optionalCast<CoilHeatingGas>();
  auto val = comp.get().iddObjectType().value();
  ASSERT_TRUE(opt);
  EXPECT_EQ(IddObjectType::OS_Coil_Heating_Gas, val);
  ASSERT_TRUE(eqd.coilObjectType());
  EXPECT_EQ(std::string("Coil:Heating:Fuel"), eqd.coilObjectType());

  AirflowNetworkDistributionNode node0 = coolingCoils[0].inletModelObject().get().cast<Node>().getAirflowNetworkDistributionNode();
  AirflowNetworkDistributionNode node1 = coolingCoils[0].outletModelObject().get().cast<Node>().getAirflowNetworkDistributionNode();

  AirflowNetworkDistributionLinkage link(model, node0, node1, cool);

  EXPECT_EQ(cool, link.component());
  ASSERT_TRUE(link.component().componentModelObject());
  EXPECT_EQ(coolingCoils[0], link.component().componentModelObject().get());

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> coilws = workspace.getObjectsByType(IddObjectType::AirflowNetwork_Distribution_Component_Coil);

  ASSERT_EQ(2u, coilws.size());

  std::vector<WorkspaceObject> links = workspace.getObjectsByType(IddObjectType::AirflowNetwork_Distribution_Linkage);

  ASSERT_EQ(1u, links.size());

  EXPECT_EQ(std::string("Airflow Network Distribution Linkage 1"), links[0].getString(0));

  // workspace.save(toPath("./AirflowNetworkLinkage.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirflowNetworkSurfaces) {
  // SimpleOpening
  {
    Model model;

    ThermalZone thermalZone(model);

    Space space(model);
    space.setThermalZone(thermalZone);

    AirflowNetworkSimulationControl control = model.getUniqueModelObject<AirflowNetworkSimulationControl>();

    ForwardTranslator forwardTranslator;

    Point3dVector points;
    points.push_back(Point3d(0, 1, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(1, 0, 0));
    points.push_back(Point3d(1, 1, 0));
    Surface surface(points, model);
    surface.setSpace(space);

    AirflowNetworkSimpleOpening so0(model, 1, 0.65, 0.5);
    AirflowNetworkSurface afnsurf0 = surface.getAirflowNetworkSurface(so0);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Component_SimpleOpening).size());

    WorkspaceObject afnObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface)[0];
    WorkspaceObject sfrObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Component_SimpleOpening)[0];

    EXPECT_EQ(surface.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::SurfaceName, false).get());
    EXPECT_EQ(so0.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::LeakageComponentName, false).get());
    EXPECT_EQ("", afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::ExternalNodeName, false).get());
    EXPECT_EQ(1, afnObject.getDouble(AirflowNetwork_MultiZone_SurfaceFields::Window_DoorOpeningFactororCrackFactor, false).get());

    EXPECT_EQ(so0.nameString(), sfrObject.getString(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::Name, false).get());
    EXPECT_EQ(1, sfrObject.getDouble(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, false).get());
    EXPECT_EQ(0.65, sfrObject.getDouble(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed, false).get());
    EXPECT_EQ(0.65, sfrObject.getDouble(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::MinimumDensityDifferenceforTwoWayFlow, false).get());
    EXPECT_EQ(0.5, sfrObject.getDouble(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::DischargeCoefficient, false).get());
  }

  // DetailedOpening
  {
    Model model;

    ThermalZone thermalZone(model);

    Space space(model);
    space.setThermalZone(thermalZone);

    AirflowNetworkSimulationControl control = model.getUniqueModelObject<AirflowNetworkSimulationControl>();

    ForwardTranslator forwardTranslator;

    Point3dVector points;
    points.push_back(Point3d(0, 1, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(1, 0, 0));
    points.push_back(Point3d(1, 1, 0));
    Surface surface(points, model);
    surface.setSpace(space);

    std::vector<DetailedOpeningFactorData> data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0),
                                                   DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};

    AirflowNetworkDetailedOpening do0(model, 1.0, data);
    AirflowNetworkSurface afnsurf0 = surface.getAirflowNetworkSurface(do0);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Component_DetailedOpening).size());

    WorkspaceObject afnObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface)[0];
    WorkspaceObject sfrObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Component_DetailedOpening)[0];

    EXPECT_EQ(surface.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::SurfaceName, false).get());
    EXPECT_EQ(do0.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::LeakageComponentName, false).get());
    EXPECT_EQ("", afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::ExternalNodeName, false).get());
    EXPECT_EQ(1, afnObject.getDouble(AirflowNetwork_MultiZone_SurfaceFields::Window_DoorOpeningFactororCrackFactor, false).get());

    EXPECT_EQ(do0.nameString(), sfrObject.getString(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::Name, false).get());
    EXPECT_EQ(1,
              sfrObject.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, false).get());
    EXPECT_EQ(0.65,
              sfrObject.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::AirMassFlowExponentWhenOpeningisClosed, false).get());
    EXPECT_EQ("NonPivoted",
              sfrObject.getString(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::TypeofRectangularLargeVerticalOpening_LVO_, false).get());
    EXPECT_EQ(0, sfrObject.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::ExtraCrackLengthorHeightofPivotingAxis, false).get());

    ASSERT_EQ(2u, sfrObject.numExtensibleGroups());
    IdfExtensibleGroup eg1 = sfrObject.extensibleGroups()[0];
    EXPECT_EQ(0, eg1.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::OpeningFactor).get());
    EXPECT_EQ(0.01, eg1.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::DischargeCoefficientforOpeningFactor).get());
    EXPECT_EQ(0, eg1.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::WidthFactorforOpeningFactor).get());
    EXPECT_EQ(0, eg1.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::HeightFactorforOpeningFactor).get());
    EXPECT_EQ(0, eg1.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::StartHeightFactorforOpeningFactor).get());
    IdfExtensibleGroup eg2 = sfrObject.extensibleGroups()[1];
    EXPECT_EQ(1, eg2.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::OpeningFactor).get());
    EXPECT_EQ(0.5, eg2.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::DischargeCoefficientforOpeningFactor).get());
    EXPECT_EQ(1, eg2.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::WidthFactorforOpeningFactor).get());
    EXPECT_EQ(1, eg2.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::HeightFactorforOpeningFactor).get());
    EXPECT_EQ(0, eg2.getDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningExtensibleFields::StartHeightFactorforOpeningFactor).get());
  }

  // HorizontalOpening
  {
    Model model;

    ThermalZone thermalZone(model);

    Space space(model);
    space.setThermalZone(thermalZone);

    AirflowNetworkSimulationControl control = model.getUniqueModelObject<AirflowNetworkSimulationControl>();

    ForwardTranslator forwardTranslator;

    Point3dVector points;
    points.push_back(Point3d(0, 1, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(1, 0, 0));
    points.push_back(Point3d(1, 1, 0));
    Surface surface(points, model);
    surface.setSpace(space);

    AirflowNetworkHorizontalOpening ho0(model, 1.0, 0.5);
    AirflowNetworkSurface afnsurf0 = surface.getAirflowNetworkSurface(ho0);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Component_HorizontalOpening).size());

    WorkspaceObject afnObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface)[0];
    WorkspaceObject sfrObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Component_HorizontalOpening)[0];

    EXPECT_EQ(surface.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::SurfaceName, false).get());
    EXPECT_EQ(ho0.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::LeakageComponentName, false).get());
    EXPECT_EQ("", afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::ExternalNodeName, false).get());
    EXPECT_EQ(1, afnObject.getDouble(AirflowNetwork_MultiZone_SurfaceFields::Window_DoorOpeningFactororCrackFactor, false).get());

    EXPECT_EQ(ho0.nameString(), sfrObject.getString(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::Name, false).get());
    EXPECT_EQ(
      1, sfrObject.getDouble(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, false).get());
    EXPECT_EQ(0.65,
              sfrObject.getDouble(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::AirMassFlowExponentWhenOpeningisClosed, false).get());
    EXPECT_EQ(90, sfrObject.getDouble(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::SlopingPlaneAngle, false).get());
    EXPECT_EQ(0.5, sfrObject.getDouble(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::DischargeCoefficient, false).get());
  }

  // EffectiveLeakageArea
  {
    Model model;

    ThermalZone thermalZone(model);

    Space space(model);
    space.setThermalZone(thermalZone);

    AirflowNetworkSimulationControl control = model.getUniqueModelObject<AirflowNetworkSimulationControl>();

    ForwardTranslator forwardTranslator;

    Point3dVector points;
    points.push_back(Point3d(0, 1, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(1, 0, 0));
    points.push_back(Point3d(1, 1, 0));
    Surface surface(points, model);
    surface.setSpace(space);

    AirflowNetworkEffectiveLeakageArea ela0(model, 10.0);
    AirflowNetworkSurface afnsurf0 = surface.getAirflowNetworkSurface(ela0);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface_EffectiveLeakageArea).size());

    WorkspaceObject afnObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface)[0];
    WorkspaceObject sfrObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface_EffectiveLeakageArea)[0];

    EXPECT_EQ(surface.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::SurfaceName, false).get());
    EXPECT_EQ(ela0.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::LeakageComponentName, false).get());
    EXPECT_EQ("", afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::ExternalNodeName, false).get());
    EXPECT_EQ(1, afnObject.getDouble(AirflowNetwork_MultiZone_SurfaceFields::Window_DoorOpeningFactororCrackFactor, false).get());

    EXPECT_EQ(ela0.nameString(), sfrObject.getString(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::Name, false).get());
    EXPECT_EQ(10, sfrObject.getDouble(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::EffectiveLeakageArea, false).get());
    EXPECT_EQ(1, sfrObject.getDouble(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::DischargeCoefficient, false).get());
    EXPECT_EQ(4, sfrObject.getDouble(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::ReferencePressureDifference, false).get());
    EXPECT_EQ(0.65, sfrObject.getDouble(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::AirMassFlowExponent, false).get());
  }

  // SpecifiedFlowRate
  {
    Model model;

    ThermalZone thermalZone(model);

    Space space(model);
    space.setThermalZone(thermalZone);

    AirflowNetworkSimulationControl control = model.getUniqueModelObject<AirflowNetworkSimulationControl>();

    ForwardTranslator forwardTranslator;

    Point3dVector points;
    points.push_back(Point3d(0, 1, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(1, 0, 0));
    points.push_back(Point3d(1, 1, 0));
    Surface surface(points, model);
    surface.setSpace(space);

    AirflowNetworkSpecifiedFlowRate sfr0(model, 10.0);
    AirflowNetworkSurface afnsurf0 = surface.getAirflowNetworkSurface(sfr0);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_SpecifiedFlowRate).size());

    WorkspaceObject afnObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_Surface)[0];
    WorkspaceObject sfrObject = workspace.getObjectsByType(IddObjectType::AirflowNetwork_MultiZone_SpecifiedFlowRate)[0];

    EXPECT_EQ(surface.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::SurfaceName, false).get());
    EXPECT_EQ(sfr0.nameString(), afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::LeakageComponentName, false).get());
    EXPECT_EQ("", afnObject.getString(AirflowNetwork_MultiZone_SurfaceFields::ExternalNodeName, false).get());
    EXPECT_EQ(1, afnObject.getDouble(AirflowNetwork_MultiZone_SurfaceFields::Window_DoorOpeningFactororCrackFactor, false).get());

    EXPECT_EQ(sfr0.nameString(), sfrObject.getString(AirflowNetwork_MultiZone_SpecifiedFlowRateFields::Name, false).get());
    EXPECT_EQ(10, sfrObject.getDouble(AirflowNetwork_MultiZone_SpecifiedFlowRateFields::AirFlowValue, false).get());
    EXPECT_EQ("MassFlow", sfrObject.getString(AirflowNetwork_MultiZone_SpecifiedFlowRateFields::AirFlowUnits, false).get());
  }
}
