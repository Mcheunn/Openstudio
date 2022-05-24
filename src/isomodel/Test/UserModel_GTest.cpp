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
#include "ISOModelFixture.hpp"

#include "../UserModel.hpp"
#include "../SimModel.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::isomodel;
using namespace openstudio;

TEST_F(ISOModelFixture, UserModel) {
  UserModel userModel;
  userModel.setTerrainClass(0.366569597990189);
  userModel.setFloorArea(0.13797878192703);
  userModel.setBuildingHeight(0.425419263581922);
  userModel.setBuildingOccupancyFrom(0.665995505182317);
  userModel.setBuildingOccupancyTo(0.400372234106352);
  userModel.setEquivFullLoadOccupancyFrom(0.254850243633116);
  userModel.setEquivFullLoadOccupancyTo(0.713362082549865);
  userModel.setPeopleDensityOccupied(0.0453028919599623);
  userModel.setPeopleDensityUnoccupied(0.374398515315959);
  userModel.setHeatingOccupiedSetpoint(0.308476836073534);
  userModel.setHeatingUnoccupiedSetpoint(0.96115521837837);
  userModel.setCoolingOccupiedSetpoint(0.0182141291000549);
  userModel.setCoolingUnoccupiedSetpoint(0.47279017381788);
  userModel.setElecPowerAppliancesOccupied(0.0159043563230605);
  userModel.setElecPowerAppliancesUnoccupied(0.877197046873451);
  userModel.setGasPowerAppliancesOccupied(0.413231779700794);
  userModel.setGasPowerAppliancesUnoccupied(0.735954395099727);
  userModel.setLightingPowerIntensityOccupied(0.827607402688993);
  userModel.setLightingPowerIntensityUnoccupied(0.210627783574828);
  userModel.setExteriorLightingPower(0.688613300586997);
  userModel.setDaylightSensorSystem(0.952066322499152);
  userModel.setLightingOccupancySensorSystem(0.191200546809349);
  userModel.setConstantIlluminationControl(0.295905191092175);
  userModel.setCoolingSystemCOP(0.977647331541828);
  userModel.setCoolingSystemIPLVToCOPRatio(0.86953551426846);
  userModel.setHeatingEnergyCarrier(0.263002176275548);
  userModel.setHeatingSystemEfficiency(0.710454137223511);
  userModel.setVentilationType(0.0841726806995226);
  userModel.setFreshAirFlowRate(0.903704085971796);
  userModel.setSupplyExhaustRate(0.724248760195895);
  userModel.setHeatRecovery(0.49985550202677);
  userModel.setExhaustAirRecirculation(0.846564029275989);
  userModel.setBuildingAirLeakage(0.863462404238138);
  userModel.setDhwDemand(0.881916031629701);
  userModel.setDhwEfficiency(0.105230439331114);
  userModel.setDhwDistributionSystem(0.791092991177229);
  userModel.setDhwEnergyCarrier(0.789220796023767);
  userModel.setBemType(0.293374792126407);
  userModel.setInteriorHeatCapacity(0.590020871911987);
  userModel.setSpecificFanPower(0.256509943938684);
  userModel.setFanFlowControlFactor(0.171213718831364);
  userModel.setRoofSHGC(0.577629926945883);
  userModel.setWallAreaS(0.351700449083525);
  userModel.setWallAreaSE(0.638796629077831);
  userModel.setWallAreaE(0.713877579934114);
  userModel.setWallAreaNE(0.0544635225207429);
  userModel.setWallAreaN(0.713312047950444);
  userModel.setWallAreaNW(0.316883353660591);
  userModel.setWallAreaW(0.963602582100428);
  userModel.setWallAreaSW(0.950016805325306);
  userModel.setRoofArea(0.401348851386038);
  userModel.setWallUvalueS(0.479173557940235);
  userModel.setWallUvalueSE(0.598665235979741);
  userModel.setWallUvalueE(0.592537203218594);
  userModel.setWallUvalueNE(0.317076189922438);
  userModel.setWallUvalueN(0.857610736439619);
  userModel.setWallUvalueNW(0.494959077705813);
  userModel.setWallUvalueW(0.710302412967452);
  userModel.setWallUvalueSW(0.755347362509827);
  userModel.setRoofUValue(0.508937055452772);
  userModel.setWallSolarAbsorptionS(0.91461449925898);
  userModel.setWallSolarAbsorptionSE(0.928931093579599);
  userModel.setWallSolarAbsorptionE(0.435542934183637);
  userModel.setWallSolarAbsorptionNE(0.793609339380358);
  userModel.setWallSolarAbsorptionN(0.902389688647158);
  userModel.setWallSolarAbsorptionNW(0.336318028981842);
  userModel.setWallSolarAbsorptionW(0.37153202026125);
  userModel.setWallSolarAbsorptionSW(0.418783890513947);
  userModel.setRoofSolarAbsorption(0.223964378497134);
  userModel.setWallThermalEmissivityS(0.583098358149272);
  userModel.setWallThermalEmissivitySE(0.141381800284656);
  userModel.setWallThermalEmissivityE(0.837222292557137);
  userModel.setWallThermalEmissivityNE(0.49538931179426);
  userModel.setWallThermalEmissivityN(0.871379477772421);
  userModel.setWallThermalEmissivityNW(0.170422643070764);
  userModel.setWallThermalEmissivityW(0.761063022176878);
  userModel.setWallThermalEmissivitySW(0.186495812844654);
  userModel.setRoofThermalEmissivity(0.907924653508436);
  userModel.setWindowAreaS(0.606074602940241);
  userModel.setWindowAreaSE(0.404342798081098);
  userModel.setWindowAreaE(0.0612029472801275);
  userModel.setWindowAreaNE(0.289843899154198);
  userModel.setWindowAreaN(0.540818859803666);
  userModel.setWindowAreaNW(0.41253025448177);
  userModel.setWindowAreaW(0.014956739105872);
  userModel.setWindowAreaSW(0.899839246505665);
  userModel.setSkylightArea(0.135269594888848);
  userModel.setWindowUvalueS(0.232560858068808);
  userModel.setWindowUvalueSE(0.431164085960324);
  userModel.setWindowUvalueE(0.00477022329159593);
  userModel.setWindowUvalueNE(0.71516207439754);
  userModel.setWindowUvalueN(0.280649559810701);
  userModel.setWindowUvalueNW(0.355908313708148);
  userModel.setWindowUvalueW(0.112872065367925);
  userModel.setWindowUvalueSW(0.398611796542468);
  userModel.setSkylightUvalue(0.712266965230007);
  userModel.setWindowSHGCS(0.255902968619523);
  userModel.setWindowSHGCSE(0.401818741289806);
  userModel.setWindowSHGCE(0.536223533889905);
  userModel.setWindowSHGCNE(0.251096592939623);
  userModel.setWindowSHGCN(0.931256342309665);
  userModel.setWindowSHGCNW(0.896808057579816);
  userModel.setWindowSHGCW(0.981291583238567);
  userModel.setWindowSHGCSW(0.148339469077549);
  userModel.setSkylightSHGC(0.531228639942613);
  userModel.setWindowSCFS(0.719753126248692);
  userModel.setWindowSCFSE(0.719295130996734);
  userModel.setWindowSCFE(0.62587251635714);
  userModel.setWindowSCFNE(0.789338364373816);
  userModel.setWindowSCFN(0.620542267432122);
  userModel.setWindowSCFNW(0.300503015955268);
  userModel.setWindowSCFW(0.128976467360588);
  userModel.setWindowSCFSW(0.947178709804832);
  userModel.setWindowSDFS(0.902216926946315);
  userModel.setWindowSDFSE(0.632486442302954);
  userModel.setWindowSDFE(0.719004834647601);
  userModel.setWindowSDFNE(0.504956302525102);
  userModel.setWindowSDFN(0.212427137938556);
  userModel.setWindowSDFNW(0.0746662195816253);
  userModel.setWindowSDFW(0.970579615803331);
  userModel.setWindowSDFSW(0.617489329894299);
  userModel.setExteriorHeatCapacity(0.523964673586454);
  userModel.setInfiltration(0.139585598177502);
  userModel.setHvacWasteFactor(0.287554068015519);
  userModel.setHvacHeatingLossFactor(0.801121347575538);
  userModel.setHvacCoolingLossFactor(0.919509843310335);
  userModel.setDhwDistributionEfficiency(0.33038965168355);
  userModel.setHeatingPumpControl(0.625403806654488);
  userModel.setCoolingPumpControl(0.0184589116025784);
  userModel.setHeatGainPerPerson(0.976673863929532);

  EXPECT_DOUBLE_EQ(0.366569597990189, userModel.terrainClass());
  EXPECT_DOUBLE_EQ(0.13797878192703, userModel.floorArea());
  EXPECT_DOUBLE_EQ(0.425419263581922, userModel.buildingHeight());
  EXPECT_DOUBLE_EQ(0.665995505182317, userModel.buildingOccupancyFrom());
  EXPECT_DOUBLE_EQ(0.400372234106352, userModel.buildingOccupancyTo());
  EXPECT_DOUBLE_EQ(0.254850243633116, userModel.equivFullLoadOccupancyFrom());
  EXPECT_DOUBLE_EQ(0.713362082549865, userModel.equivFullLoadOccupancyTo());
  EXPECT_DOUBLE_EQ(0.0453028919599623, userModel.peopleDensityOccupied());
  EXPECT_DOUBLE_EQ(0.374398515315959, userModel.peopleDensityUnoccupied());
  EXPECT_DOUBLE_EQ(0.308476836073534, userModel.heatingOccupiedSetpoint());
  EXPECT_DOUBLE_EQ(0.96115521837837, userModel.heatingUnoccupiedSetpoint());
  EXPECT_DOUBLE_EQ(0.0182141291000549, userModel.coolingOccupiedSetpoint());
  EXPECT_DOUBLE_EQ(0.47279017381788, userModel.coolingUnoccupiedSetpoint());
  EXPECT_DOUBLE_EQ(0.0159043563230605, userModel.elecPowerAppliancesOccupied());
  EXPECT_DOUBLE_EQ(0.877197046873451, userModel.elecPowerAppliancesUnoccupied());
  EXPECT_DOUBLE_EQ(0.413231779700794, userModel.gasPowerAppliancesOccupied());
  EXPECT_DOUBLE_EQ(0.735954395099727, userModel.gasPowerAppliancesUnoccupied());
  EXPECT_DOUBLE_EQ(0.827607402688993, userModel.lightingPowerIntensityOccupied());
  EXPECT_DOUBLE_EQ(0.210627783574828, userModel.lightingPowerIntensityUnoccupied());
  EXPECT_DOUBLE_EQ(0.688613300586997, userModel.exteriorLightingPower());
  EXPECT_DOUBLE_EQ(0.952066322499152, userModel.daylightSensorSystem());
  EXPECT_DOUBLE_EQ(0.191200546809349, userModel.lightingOccupancySensorSystem());
  EXPECT_DOUBLE_EQ(0.295905191092175, userModel.constantIlluminationControl());
  EXPECT_DOUBLE_EQ(0.977647331541828, userModel.coolingSystemCOP());
  EXPECT_DOUBLE_EQ(0.86953551426846, userModel.coolingSystemIPLVToCOPRatio());
  EXPECT_DOUBLE_EQ(0.263002176275548, userModel.heatingEnergyCarrier());
  EXPECT_DOUBLE_EQ(0.710454137223511, userModel.heatingSystemEfficiency());
  EXPECT_DOUBLE_EQ(0.0841726806995226, userModel.ventilationType());
  EXPECT_DOUBLE_EQ(0.903704085971796, userModel.freshAirFlowRate());
  EXPECT_DOUBLE_EQ(0.724248760195895, userModel.supplyExhaustRate());
  EXPECT_DOUBLE_EQ(0.49985550202677, userModel.heatRecovery());
  EXPECT_DOUBLE_EQ(0.846564029275989, userModel.exhaustAirRecirculation());
  EXPECT_DOUBLE_EQ(0.863462404238138, userModel.buildingAirLeakage());
  EXPECT_DOUBLE_EQ(0.881916031629701, userModel.dhwDemand());
  EXPECT_DOUBLE_EQ(0.105230439331114, userModel.dhwEfficiency());
  EXPECT_DOUBLE_EQ(0.791092991177229, userModel.dhwDistributionSystem());
  EXPECT_DOUBLE_EQ(0.789220796023767, userModel.dhwEnergyCarrier());
  EXPECT_DOUBLE_EQ(0.293374792126407, userModel.bemType());
  EXPECT_DOUBLE_EQ(0.590020871911987, userModel.interiorHeatCapacity());
  EXPECT_DOUBLE_EQ(0.256509943938684, userModel.specificFanPower());
  EXPECT_DOUBLE_EQ(0.171213718831364, userModel.fanFlowControlFactor());
  EXPECT_DOUBLE_EQ(0.577629926945883, userModel.roofSHGC());
  EXPECT_DOUBLE_EQ(0.351700449083525, userModel.wallAreaS());
  EXPECT_DOUBLE_EQ(0.638796629077831, userModel.wallAreaSE());
  EXPECT_DOUBLE_EQ(0.713877579934114, userModel.wallAreaE());
  EXPECT_DOUBLE_EQ(0.0544635225207429, userModel.wallAreaNE());
  EXPECT_DOUBLE_EQ(0.713312047950444, userModel.wallAreaN());
  EXPECT_DOUBLE_EQ(0.316883353660591, userModel.wallAreaNW());
  EXPECT_DOUBLE_EQ(0.963602582100428, userModel.wallAreaW());
  EXPECT_DOUBLE_EQ(0.950016805325306, userModel.wallAreaSW());
  EXPECT_DOUBLE_EQ(0.401348851386038, userModel.roofArea());
  EXPECT_DOUBLE_EQ(0.479173557940235, userModel.wallUvalueS());
  EXPECT_DOUBLE_EQ(0.598665235979741, userModel.wallUvalueSE());
  EXPECT_DOUBLE_EQ(0.592537203218594, userModel.wallUvalueE());
  EXPECT_DOUBLE_EQ(0.317076189922438, userModel.wallUvalueNE());
  EXPECT_DOUBLE_EQ(0.857610736439619, userModel.wallUvalueN());
  EXPECT_DOUBLE_EQ(0.494959077705813, userModel.wallUvalueNW());
  EXPECT_DOUBLE_EQ(0.710302412967452, userModel.wallUvalueW());
  EXPECT_DOUBLE_EQ(0.755347362509827, userModel.wallUvalueSW());
  EXPECT_DOUBLE_EQ(0.508937055452772, userModel.roofUValue());
  EXPECT_DOUBLE_EQ(0.91461449925898, userModel.wallSolarAbsorptionS());
  EXPECT_DOUBLE_EQ(0.928931093579599, userModel.wallSolarAbsorptionSE());
  EXPECT_DOUBLE_EQ(0.435542934183637, userModel.wallSolarAbsorptionE());
  EXPECT_DOUBLE_EQ(0.793609339380358, userModel.wallSolarAbsorptionNE());
  EXPECT_DOUBLE_EQ(0.902389688647158, userModel.wallSolarAbsorptionN());
  EXPECT_DOUBLE_EQ(0.336318028981842, userModel.wallSolarAbsorptionNW());
  EXPECT_DOUBLE_EQ(0.37153202026125, userModel.wallSolarAbsorptionW());
  EXPECT_DOUBLE_EQ(0.418783890513947, userModel.wallSolarAbsorptionSW());
  EXPECT_DOUBLE_EQ(0.223964378497134, userModel.roofSolarAbsorption());
  EXPECT_DOUBLE_EQ(0.583098358149272, userModel.wallThermalEmissivityS());
  EXPECT_DOUBLE_EQ(0.141381800284656, userModel.wallThermalEmissivitySE());
  EXPECT_DOUBLE_EQ(0.837222292557137, userModel.wallThermalEmissivityE());
  EXPECT_DOUBLE_EQ(0.49538931179426, userModel.wallThermalEmissivityNE());
  EXPECT_DOUBLE_EQ(0.871379477772421, userModel.wallThermalEmissivityN());
  EXPECT_DOUBLE_EQ(0.170422643070764, userModel.wallThermalEmissivityNW());
  EXPECT_DOUBLE_EQ(0.761063022176878, userModel.wallThermalEmissivityW());
  EXPECT_DOUBLE_EQ(0.186495812844654, userModel.wallThermalEmissivitySW());
  EXPECT_DOUBLE_EQ(0.907924653508436, userModel.roofThermalEmissivity());
  EXPECT_DOUBLE_EQ(0.606074602940241, userModel.windowAreaS());
  EXPECT_DOUBLE_EQ(0.404342798081098, userModel.windowAreaSE());
  EXPECT_DOUBLE_EQ(0.0612029472801275, userModel.windowAreaE());
  EXPECT_DOUBLE_EQ(0.289843899154198, userModel.windowAreaNE());
  EXPECT_DOUBLE_EQ(0.540818859803666, userModel.windowAreaN());
  EXPECT_DOUBLE_EQ(0.41253025448177, userModel.windowAreaNW());
  EXPECT_DOUBLE_EQ(0.014956739105872, userModel.windowAreaW());
  EXPECT_DOUBLE_EQ(0.899839246505665, userModel.windowAreaSW());
  EXPECT_DOUBLE_EQ(0.135269594888848, userModel.skylightArea());
  EXPECT_DOUBLE_EQ(0.232560858068808, userModel.windowUvalueS());
  EXPECT_DOUBLE_EQ(0.431164085960324, userModel.windowUvalueSE());
  EXPECT_DOUBLE_EQ(0.00477022329159593, userModel.windowUvalueE());
  EXPECT_DOUBLE_EQ(0.71516207439754, userModel.windowUvalueNE());
  EXPECT_DOUBLE_EQ(0.280649559810701, userModel.windowUvalueN());
  EXPECT_DOUBLE_EQ(0.355908313708148, userModel.windowUvalueNW());
  EXPECT_DOUBLE_EQ(0.112872065367925, userModel.windowUvalueW());
  EXPECT_DOUBLE_EQ(0.398611796542468, userModel.windowUvalueSW());
  EXPECT_DOUBLE_EQ(0.712266965230007, userModel.skylightUvalue());
  EXPECT_DOUBLE_EQ(0.255902968619523, userModel.windowSHGCS());
  EXPECT_DOUBLE_EQ(0.401818741289806, userModel.windowSHGCSE());
  EXPECT_DOUBLE_EQ(0.536223533889905, userModel.windowSHGCE());
  EXPECT_DOUBLE_EQ(0.251096592939623, userModel.windowSHGCNE());
  EXPECT_DOUBLE_EQ(0.931256342309665, userModel.windowSHGCN());
  EXPECT_DOUBLE_EQ(0.896808057579816, userModel.windowSHGCNW());
  EXPECT_DOUBLE_EQ(0.981291583238567, userModel.windowSHGCW());
  EXPECT_DOUBLE_EQ(0.148339469077549, userModel.windowSHGCSW());
  EXPECT_DOUBLE_EQ(0.531228639942613, userModel.skylightSHGC());
  EXPECT_DOUBLE_EQ(0.719753126248692, userModel.windowSCFS());
  EXPECT_DOUBLE_EQ(0.719295130996734, userModel.windowSCFSE());
  EXPECT_DOUBLE_EQ(0.62587251635714, userModel.windowSCFE());
  EXPECT_DOUBLE_EQ(0.789338364373816, userModel.windowSCFNE());
  EXPECT_DOUBLE_EQ(0.620542267432122, userModel.windowSCFN());
  EXPECT_DOUBLE_EQ(0.300503015955268, userModel.windowSCFNW());
  EXPECT_DOUBLE_EQ(0.128976467360588, userModel.windowSCFW());
  EXPECT_DOUBLE_EQ(0.947178709804832, userModel.windowSCFSW());
  EXPECT_DOUBLE_EQ(0.902216926946315, userModel.windowSDFS());
  EXPECT_DOUBLE_EQ(0.632486442302954, userModel.windowSDFSE());
  EXPECT_DOUBLE_EQ(0.719004834647601, userModel.windowSDFE());
  EXPECT_DOUBLE_EQ(0.504956302525102, userModel.windowSDFNE());
  EXPECT_DOUBLE_EQ(0.212427137938556, userModel.windowSDFN());
  EXPECT_DOUBLE_EQ(0.0746662195816253, userModel.windowSDFNW());
  EXPECT_DOUBLE_EQ(0.970579615803331, userModel.windowSDFW());
  EXPECT_DOUBLE_EQ(0.617489329894299, userModel.windowSDFSW());
  EXPECT_DOUBLE_EQ(0.523964673586454, userModel.exteriorHeatCapacity());
  EXPECT_DOUBLE_EQ(0.139585598177502, userModel.infiltration());
  EXPECT_DOUBLE_EQ(0.287554068015519, userModel.hvacWasteFactor());
  EXPECT_DOUBLE_EQ(0.801121347575538, userModel.hvacHeatingLossFactor());
  EXPECT_DOUBLE_EQ(0.919509843310335, userModel.hvacCoolingLossFactor());
  EXPECT_DOUBLE_EQ(0.33038965168355, userModel.dhwDistributionEfficiency());
  EXPECT_DOUBLE_EQ(0.625403806654488, userModel.heatingPumpControl());
  EXPECT_DOUBLE_EQ(0.0184589116025784, userModel.coolingPumpControl());
  EXPECT_DOUBLE_EQ(0.976673863929532, userModel.heatGainPerPerson());

  std::string wpath = "test";
  userModel.load(openstudio::toPath(wpath));
  EXPECT_FALSE(userModel.valid());
  userModel.setWeatherFilePath(openstudio::toPath(wpath));
  EXPECT_EQ(userModel.weatherFilePath(), openstudio::toPath(wpath));
  userModel.loadWeather();
  EXPECT_FALSE(userModel.valid());

  path p = resourcesPath() / openstudio::toPath("isomodel/exampleModel.ISO");
  userModel.load(p);

  EXPECT_DOUBLE_EQ(0.9, userModel.terrainClass());
  EXPECT_DOUBLE_EQ(10000.0, userModel.floorArea());
  EXPECT_DOUBLE_EQ(8.0, userModel.buildingHeight());
  EXPECT_DOUBLE_EQ(1.0, userModel.buildingOccupancyFrom());
  EXPECT_DOUBLE_EQ(5.0, userModel.buildingOccupancyTo());
  EXPECT_DOUBLE_EQ(7.0, userModel.equivFullLoadOccupancyFrom());
  EXPECT_DOUBLE_EQ(18.0, userModel.equivFullLoadOccupancyTo());
  EXPECT_DOUBLE_EQ(22.386274698794701, userModel.peopleDensityOccupied());
  EXPECT_DOUBLE_EQ(171.87446672181500, userModel.peopleDensityUnoccupied());
  EXPECT_DOUBLE_EQ(24.0, userModel.heatingOccupiedSetpoint());
  EXPECT_DOUBLE_EQ(24.0, userModel.heatingUnoccupiedSetpoint());
  EXPECT_DOUBLE_EQ(28.0, userModel.coolingOccupiedSetpoint());
  EXPECT_DOUBLE_EQ(28.0, userModel.coolingUnoccupiedSetpoint());
  EXPECT_DOUBLE_EQ(8.9340456458692508, userModel.elecPowerAppliancesOccupied());
  EXPECT_DOUBLE_EQ(1.1636399740730901, userModel.elecPowerAppliancesUnoccupied());
  EXPECT_DOUBLE_EQ(26.183103184023899, userModel.gasPowerAppliancesOccupied());
  EXPECT_DOUBLE_EQ(3.4102921249677900, userModel.gasPowerAppliancesUnoccupied());
  EXPECT_DOUBLE_EQ(8.9340456458692508, userModel.lightingPowerIntensityOccupied());
  EXPECT_DOUBLE_EQ(1.1636399740730901, userModel.lightingPowerIntensityUnoccupied());
  EXPECT_DOUBLE_EQ(0.0, userModel.exteriorLightingPower());
  EXPECT_DOUBLE_EQ(1.0, userModel.daylightSensorSystem());
  EXPECT_DOUBLE_EQ(1.0, userModel.lightingOccupancySensorSystem());
  EXPECT_DOUBLE_EQ(1.0, userModel.constantIlluminationControl());
  EXPECT_DOUBLE_EQ(3.0, userModel.coolingSystemCOP());
  EXPECT_DOUBLE_EQ(1.0, userModel.coolingSystemIPLVToCOPRatio());
  EXPECT_DOUBLE_EQ(2.0, userModel.heatingEnergyCarrier());
  EXPECT_DOUBLE_EQ(0.80000000000000004, userModel.heatingSystemEfficiency());
  EXPECT_DOUBLE_EQ(1.0, userModel.ventilationType());
  EXPECT_DOUBLE_EQ(5080, userModel.freshAirFlowRate());
  EXPECT_DOUBLE_EQ(5080, userModel.supplyExhaustRate());
  EXPECT_DOUBLE_EQ(0.0, userModel.heatRecovery());
  EXPECT_DOUBLE_EQ(0.0, userModel.exhaustAirRecirculation());
  EXPECT_DOUBLE_EQ(2.4610800585487800, userModel.buildingAirLeakage());
  EXPECT_DOUBLE_EQ(0.0, userModel.dhwDemand());
  EXPECT_DOUBLE_EQ(0.9, userModel.dhwEfficiency());
  EXPECT_DOUBLE_EQ(0.791092991177229, userModel.dhwDistributionSystem());  //unmodified by load
  EXPECT_DOUBLE_EQ(1.0, userModel.dhwEnergyCarrier());
  EXPECT_DOUBLE_EQ(1.0, userModel.bemType());
  EXPECT_DOUBLE_EQ(224141.05370757400, userModel.interiorHeatCapacity());
  EXPECT_DOUBLE_EQ(2.5, userModel.specificFanPower());
  EXPECT_DOUBLE_EQ(1.0, userModel.fanFlowControlFactor());
  EXPECT_DOUBLE_EQ(0.577629926945883, userModel.roofSHGC());  //unmodified by load
  EXPECT_DOUBLE_EQ(480.0, userModel.wallAreaS());
  EXPECT_DOUBLE_EQ(0.0, userModel.wallAreaSE());
  EXPECT_DOUBLE_EQ(240, userModel.wallAreaE());
  EXPECT_DOUBLE_EQ(0.0, userModel.wallAreaNE());
  EXPECT_DOUBLE_EQ(480.0, userModel.wallAreaN());
  EXPECT_DOUBLE_EQ(0.0, userModel.wallAreaNW());
  EXPECT_DOUBLE_EQ(240.0, userModel.wallAreaW());
  EXPECT_DOUBLE_EQ(0.0, userModel.wallAreaSW());
  EXPECT_DOUBLE_EQ(10000.0, userModel.roofArea());
  EXPECT_DOUBLE_EQ(0.30930199989518098, userModel.wallUvalueS());
  EXPECT_DOUBLE_EQ(0.29999999999999999, userModel.wallUvalueSE());
  EXPECT_DOUBLE_EQ(0.30930199989518098, userModel.wallUvalueE());
  EXPECT_DOUBLE_EQ(0.29999999999999999, userModel.wallUvalueNE());
  EXPECT_DOUBLE_EQ(0.30930199989518098, userModel.wallUvalueN());
  EXPECT_DOUBLE_EQ(0.29999999999999999, userModel.wallUvalueNW());
  EXPECT_DOUBLE_EQ(0.30930199989518098, userModel.wallUvalueW());
  EXPECT_DOUBLE_EQ(0.29999999999999999, userModel.wallUvalueSW());
  EXPECT_DOUBLE_EQ(0.22146791744515701, userModel.roofUValue());
  EXPECT_DOUBLE_EQ(0.69999999999999996, userModel.wallSolarAbsorptionS());
  EXPECT_DOUBLE_EQ(0.5, userModel.wallSolarAbsorptionSE());
  EXPECT_DOUBLE_EQ(0.69999999999999996, userModel.wallSolarAbsorptionE());
  EXPECT_DOUBLE_EQ(0.5, userModel.wallSolarAbsorptionNE());
  EXPECT_DOUBLE_EQ(0.69999999999999996, userModel.wallSolarAbsorptionN());
  EXPECT_DOUBLE_EQ(0.5, userModel.wallSolarAbsorptionNW());
  EXPECT_DOUBLE_EQ(0.69999999999999996, userModel.wallSolarAbsorptionW());
  EXPECT_DOUBLE_EQ(0.5, userModel.wallSolarAbsorptionSW());
  EXPECT_DOUBLE_EQ(0.69999999999999996, userModel.roofSolarAbsorption());
  EXPECT_DOUBLE_EQ(0.90000000000000002, userModel.wallThermalEmissivityS());
  EXPECT_DOUBLE_EQ(0.5, userModel.wallThermalEmissivitySE());
  EXPECT_DOUBLE_EQ(0.90000000000000002, userModel.wallThermalEmissivityE());
  EXPECT_DOUBLE_EQ(0.5, userModel.wallThermalEmissivityNE());
  EXPECT_DOUBLE_EQ(0.90000000000000002, userModel.wallThermalEmissivityN());
  EXPECT_DOUBLE_EQ(0.5, userModel.wallThermalEmissivityNW());
  EXPECT_DOUBLE_EQ(0.90000000000000002, userModel.wallThermalEmissivityW());
  EXPECT_DOUBLE_EQ(0.5, userModel.wallThermalEmissivitySW());
  EXPECT_DOUBLE_EQ(0.90000000000000002, userModel.roofThermalEmissivity());
  EXPECT_DOUBLE_EQ(320.0, userModel.windowAreaS());
  EXPECT_DOUBLE_EQ(0.0, userModel.windowAreaSE());
  EXPECT_DOUBLE_EQ(160.0, userModel.windowAreaE());
  EXPECT_DOUBLE_EQ(0.0, userModel.windowAreaNE());
  EXPECT_DOUBLE_EQ(320.0, userModel.windowAreaN());
  EXPECT_DOUBLE_EQ(0.0, userModel.windowAreaNW());
  EXPECT_DOUBLE_EQ(160.0, userModel.windowAreaW());
  EXPECT_DOUBLE_EQ(0.0, userModel.windowAreaSW());
  EXPECT_DOUBLE_EQ(0.0, userModel.skylightArea());
  EXPECT_DOUBLE_EQ(3.8904056162246499, userModel.windowUvalueS());
  EXPECT_DOUBLE_EQ(3.0, userModel.windowUvalueSE());
  EXPECT_DOUBLE_EQ(3.8904056162246601, userModel.windowUvalueE());
  EXPECT_DOUBLE_EQ(3.0, userModel.windowUvalueNE());
  EXPECT_DOUBLE_EQ(3.8904056162246499, userModel.windowUvalueN());
  EXPECT_DOUBLE_EQ(3.0, userModel.windowUvalueNW());
  EXPECT_DOUBLE_EQ(3.8904056162246601, userModel.windowUvalueW());
  EXPECT_DOUBLE_EQ(3.0, userModel.windowUvalueSW());
  EXPECT_DOUBLE_EQ(3.0, userModel.skylightUvalue());
  EXPECT_DOUBLE_EQ(0.49665000000000098, userModel.windowSHGCS());
  EXPECT_DOUBLE_EQ(0.5, userModel.windowSHGCSE());
  EXPECT_DOUBLE_EQ(0.49665000000000098, userModel.windowSHGCE());
  EXPECT_DOUBLE_EQ(0.5, userModel.windowSHGCNE());
  EXPECT_DOUBLE_EQ(0.49665000000000098, userModel.windowSHGCN());
  EXPECT_DOUBLE_EQ(0.5, userModel.windowSHGCNW());
  EXPECT_DOUBLE_EQ(0.49665000000000098, userModel.windowSHGCW());
  EXPECT_DOUBLE_EQ(0.5, userModel.windowSHGCSW());
  EXPECT_DOUBLE_EQ(0.5, userModel.skylightSHGC());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSCFS());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSCFSE());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSCFE());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSCFNE());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSCFN());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSCFNW());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSCFW());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSCFSW());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSDFS());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSDFSE());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSDFE());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSDFNE());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSDFN());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSDFNW());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSDFW());
  EXPECT_DOUBLE_EQ(1.0, userModel.windowSDFSW());
  EXPECT_DOUBLE_EQ(58083.259716774199, userModel.exteriorHeatCapacity());
  EXPECT_DOUBLE_EQ(0.139585598177502, userModel.infiltration());  //unmodified by load
  EXPECT_DOUBLE_EQ(0.0, userModel.hvacWasteFactor());
  EXPECT_DOUBLE_EQ(0.25, userModel.hvacHeatingLossFactor());
  EXPECT_DOUBLE_EQ(0.0, userModel.hvacCoolingLossFactor());
  EXPECT_DOUBLE_EQ(0.59999999999999998, userModel.dhwDistributionEfficiency());
  EXPECT_DOUBLE_EQ(1.0, userModel.heatingPumpControl());
  EXPECT_DOUBLE_EQ(1.0, userModel.coolingPumpControl());
  EXPECT_DOUBLE_EQ(120.0, userModel.heatGainPerPerson());
  EXPECT_EQ(userModel.weatherFilePath(), openstudio::toPath("weather.epw"));
  WeatherData wd = *userModel.loadWeather();

  Matrix msolar = wd.msolar();
  Matrix mhdbt = wd.mhdbt();
  Matrix mhEgh = wd.mhEgh();
  Vector mEgh = wd.mEgh();
  Vector mdbt = wd.mdbt();
  Vector mwind = wd.mwind();

  double msolarExp[] = {
    124.87691861424167, 87.212328068340284, 43.014836782652118, 23.680719044472859, 23.003593109413657, 26.323064923342631, 58.810694391713106,
    107.06605138452912, 142.07941057145206, 104.95709459739395, 60.608055144743467, 34.877567375719927, 32.182031608720685, 40.512611857522195,
    79.723263608096474, 126.46835992986829, 144.81173140370345, 121.04382781130641, 84.852874629300672, 52.049351968350102, 42.913180988385314,
    59.348305824960811, 100.05608586406214, 135.11048082608818, 129.94968480601199, 124.13363336598071, 104.93046355448784, 71.268044990791324,
    54.046841303275556, 84.431398040371022, 124.67206010895548, 138.56241960179872, 134.0014180459159,  144.89473278396255, 138.11786029337347,
    101.92614956383538, 76.031248511462749, 119.2149118625866,  159.07727477837827, 156.85135911310419, 121.93695049006568, 143.70220697802986,
    148.21175370167822, 115.96827063053348, 87.61144751228322,  131.92022292608829, 164.9103588838577,  151.11585933916228, 128.88622482148511,
    141.90869550792021, 140.59660603297417, 109.80195970475256, 85.448986411059963, 129.8726077645062,  165.95390783999844, 157.49259907172015,
    133.16815853745538, 137.05062177656148, 123.59932398291846, 88.888069887660137, 66.857752974728555, 104.26522936270008, 143.12511552825072,
    148.93012554924269, 149.50709237426477, 134.27135584183861, 103.12341585193052, 63.438295552277509, 48.152741557586374, 73.529793646405821,
    118.97619209349854, 146.30235058963021, 150.87461991730143, 118.71387095626463, 74.42960227453321,  40.084429290584538, 34.699028809006876,
    45.845232466627536, 88.078612041360259, 132.20661705757985, 114.51887045855386, 84.335421131574847, 45.31958757330041,  26.623381849654656,
    25.649335665397185, 28.832138655691839, 55.823463221613416, 97.159585442506852, 116.10847283220625, 82.709173830485668, 39.099086744447028,
    19.790303276802067, 19.284962462870961, 21.366911924165972, 49.125234671137314, 95.455260385147113};

  double mhdbtExp[] = {-6.283870967741934,
                       -6.1806451612903217,
                       -6.0870967741935464,
                       -6.2741935483870952,
                       -6.5225806451612884,
                       -6.8064516129032233,
                       -6.7806451612903214,
                       -6.5580645161290327,
                       -5.5516129032258075,
                       -4.3612903225806479,
                       -3.5451612903225809,
                       -2.7903225806451619,
                       -2.0290322580645159,
                       -1.6838709677419355,
                       -1.596774193548387,
                       -1.8612903225806452,
                       -2.9129032258064509,
                       -3.8258064516129031,
                       -4.5451612903225795,
                       -4.5096774193548379,
                       -4.7096774193548381,
                       -4.9774193548387098,
                       -5.2387096774193544,
                       -5.8838709677419354,
                       -3.6571428571428575,
                       -3.9285714285714297,
                       -4.1464285714285714,
                       -4.4642857142857153,
                       -4.9071428571428566,
                       -5.1214285714285728,
                       -5.2642857142857151,
                       -4.625,
                       -3.2928571428571418,
                       -2.0964285714285711,
                       -1.0214285714285718,
                       -0.24285714285714294,
                       0.25714285714285684,
                       0.39642857142857163,
                       0.49642857142857127,
                       0.26071428571428618,
                       -0.46428571428571414,
                       -1.4249999999999996,
                       -2.346428571428572,
                       -2.5821428571428577,
                       -2.850000000000001,
                       -2.9250000000000007,
                       -3.1500000000000008,
                       -3.3857142857142852,
                       2.0225806451612902,
                       1.8161290322580645,
                       1.767741935483871,
                       1.2483870967741937,
                       1.1322580645161291,
                       0.91935483870967749,
                       1.4225806451612903,
                       2.7548387096774198,
                       3.9677419354838714,
                       5.0548387096774192,
                       5.9387096774193537,
                       6.8193548387096774,
                       7.187096774193547,
                       7.3258064516129036,
                       7.1935483870967731,
                       6.7677419354838708,
                       6.1354838709677404,
                       5.0387096774193543,
                       3.9419354838709673,
                       3.3000000000000003,
                       2.9419354838709681,
                       2.6096774193548393,
                       2.3741935483870966,
                       2.0935483870967744,
                       7.7933333333333339,
                       7.6133333333333342,
                       7.5633333333333344,
                       7.1933333333333342,
                       6.9566666666666652,
                       7.0366666666666662,
                       8.0933333333333319,
                       9.2566666666666642,
                       10.186666666666669,
                       10.946666666666662,
                       11.793333333333333,
                       12.276666666666671,
                       12.763333333333337,
                       13.120000000000001,
                       13.01333333333333,
                       12.840000000000002,
                       12.409999999999998,
                       11.503333333333336,
                       10.576666666666666,
                       9.8800000000000026,
                       9.4866666666666699,
                       9.1800000000000015,
                       8.9166666666666643,
                       8.4199999999999982,
                       10.70967741935484,
                       10.316129032258063,
                       10.167741935483871,
                       10.051612903225806,
                       9.8935483870967733,
                       11.225806451612904,
                       13.56774193548387,
                       15.474193548387097,
                       17.293548387096774,
                       18.696774193548386,
                       19.819354838709682,
                       20.580645161290324,
                       21.054838709677416,
                       20.945161290322581,
                       20.574193548387097,
                       19.987096774193549,
                       19.532258064516128,
                       18.096774193548384,
                       16.0741935483871,
                       14.487096774193549,
                       13.332258064516125,
                       12.454838709677418,
                       11.761290322580646,
                       11.351612903225806,
                       17.596666666666668,
                       17.25333333333333,
                       17.206666666666667,
                       16.863333333333333,
                       16.719999999999999,
                       17.593333333333327,
                       19.573333333333327,
                       21.283333333333339,
                       22.50333333333333,
                       23.589999999999993,
                       24.383333333333333,
                       24.829999999999995,
                       25.22666666666667,
                       25.466666666666672,
                       25.390000000000001,
                       25.133333333333333,
                       24.756666666666668,
                       23.95666666666666,
                       22.333333333333329,
                       20.68,
                       19.41,
                       18.790000000000003,
                       18.133333333333336,
                       17.946666666666662,
                       20.71290322580645,
                       20.341935483870962,
                       19.87096774193548,
                       19.732258064516127,
                       19.532258064516128,
                       20.706451612903233,
                       22.400000000000002,
                       23.732258064516124,
                       24.961290322580648,
                       26.077419354838707,
                       27.038709677419345,
                       27.832258064516129,
                       28.441935483870967,
                       28.577419354838703,
                       28.496774193548383,
                       27.98064516129033,
                       27.319354838709689,
                       26.516129032258068,
                       25.400000000000002,
                       24.048387096774189,
                       23.245161290322581,
                       22.648387096774197,
                       22.041935483870969,
                       21.580645161290324,
                       18.593548387096778,
                       18.319354838709678,
                       18.161290322580644,
                       17.670967741935481,
                       17.664516129032254,
                       18.029032258064511,
                       20.087096774193547,
                       21.70967741935484,
                       23.258064516129036,
                       24.603225806451611,
                       25.203225806451609,
                       25.432258064516127,
                       25.900000000000002,
                       25.71935483870968,
                       25.429032258064517,
                       25.174193548387091,
                       24.538709677419355,
                       23.77096774193549,
                       22.364516129032257,
                       21.464516129032258,
                       20.600000000000001,
                       20.2741935483871,
                       19.580645161290327,
                       19.019354838709674,
                       14.909999999999997,
                       14.486666666666666,
                       14.140000000000002,
                       13.880000000000003,
                       13.71666666666667,
                       13.606666666666671,
                       15.53666666666666,
                       17.649999999999999,
                       19.393333333333334,
                       20.459999999999997,
                       21.403333333333332,
                       22.243333333333336,
                       22.83666666666667,
                       23.149999999999995,
                       22.886666666666667,
                       22.486666666666668,
                       21.556666666666665,
                       19.959999999999997,
                       18.600000000000001,
                       17.799999999999997,
                       16.866666666666678,
                       16.496666666666666,
                       15.876666666666667,
                       15.269999999999998,
                       8.4967741935483883,
                       8.2290322580645157,
                       8.0000000000000018,
                       7.8129032258064495,
                       7.7967741935483845,
                       7.5451612903225787,
                       8.0612903225806445,
                       9.8774193548387093,
                       11.754838709677417,
                       13.17741935483871,
                       14.325806451612904,
                       14.945161290322581,
                       15.248387096774191,
                       15.429032258064515,
                       15.345161290322583,
                       14.638709677419357,
                       13.261290322580646,
                       11.764516129032254,
                       11.019354838709678,
                       10.429032258064517,
                       9.8419354838709712,
                       9.3387096774193541,
                       8.741935483870968,
                       8.4580645161290331,
                       3.713333333333332,
                       3.4533333333333345,
                       3.3566666666666669,
                       3.3566666666666669,
                       3.0599999999999983,
                       2.8499999999999996,
                       2.7533333333333325,
                       3.476666666666667,
                       4.3433333333333319,
                       5.4866666666666655,
                       6.3133333333333308,
                       6.9866666666666655,
                       7.2433333333333341,
                       7.3933333333333335,
                       7.2733333333333317,
                       6.6866666666666665,
                       5.9233333333333329,
                       5.2766666666666655,
                       4.633333333333332,
                       4.2266666666666648,
                       4.1533333333333324,
                       4.080000000000001,
                       3.8033333333333323,
                       3.7166666666666637,
                       -4.8935483870967751,
                       -4.7838709677419358,
                       -4.9387096774193546,
                       -5.2387096774193553,
                       -5.2967741935483863,
                       -5.6451612903225818,
                       -5.693548387096774,
                       -5.3516129032258064,
                       -4.4806451612903224,
                       -3.2129032258064516,
                       -2.0999999999999996,
                       -1.4935483870967741,
                       -0.91935483870967749,
                       -0.70322580645161259,
                       -0.68387096774193534,
                       -1.4709677419354836,
                       -2.4741935483870963,
                       -3.3129032258064512,
                       -3.67741935483871,
                       -4.0225806451612902,
                       -4.1548387096774198,
                       -4.3677419354838714,
                       -4.5483870967741939,
                       -5.0032258064516126};

  double mhEghExp[] = {0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       20.322580645161292,
                       92.870967741935488,
                       191.2258064516129,
                       260.12903225806451,
                       302.74193548387098,
                       305.90322580645159,
                       276.77419354838707,
                       192.51612903225808,
                       98.806451612903231,
                       22.677419354838708,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       3.0714285714285716,
                       48.25,
                       149.5,
                       271.39285714285717,
                       364.5,
                       390.39285714285717,
                       395.17857142857144,
                       342.28571428571428,
                       273.92857142857144,
                       179.64285714285714,
                       67.607142857142861,
                       7.6071428571428568,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0.58064516129032262,
                       31.774193548387096,
                       132,
                       241.41935483870967,
                       368.48387096774195,
                       455.51612903225805,
                       479.70967741935482,
                       503.90322580645159,
                       451.09677419354841,
                       361.03225806451616,
                       252.90322580645162,
                       128.06451612903226,
                       33.12903225806452,
                       0.54838709677419351,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       7.1333333333333337,
                       148.40000000000001,
                       241.53333333333333,
                       371.53333333333336,
                       463.80000000000001,
                       516.10000000000002,
                       548.5333333333333,
                       519.5,
                       528.93333333333328,
                       438.93333333333334,
                       327.93333333333334,
                       193.56666666666666,
                       85.533333333333331,
                       2.7000000000000002,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       8.741935483870968,
                       68.548387096774192,
                       202.2258064516129,
                       355.32258064516128,
                       507.25806451612902,
                       631.90322580645159,
                       680.74193548387098,
                       710.58064516129036,
                       723.90322580645159,
                       670.80645161290317,
                       538.29032258064512,
                       418.12903225806451,
                       287.64516129032256,
                       135.7741935483871,
                       35.41935483870968,
                       0.58064516129032262,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       16.266666666666666,
                       95.099999999999994,
                       232.83333333333334,
                       384.46666666666664,
                       533.20000000000005,
                       625.5,
                       709.10000000000002,
                       702.63333333333333,
                       698.33333333333337,
                       660.89999999999998,
                       586.86666666666667,
                       475.19999999999999,
                       319.89999999999998,
                       185.59999999999999,
                       61.733333333333334,
                       5.8666666666666663,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       8.387096774193548,
                       67.548387096774192,
                       194.32258064516128,
                       338,
                       477.09677419354841,
                       600.61290322580646,
                       664.35483870967744,
                       732.29032258064512,
                       761.32258064516134,
                       681.70967741935488,
                       611.19354838709683,
                       464.16129032258067,
                       333.03225806451616,
                       181.12903225806451,
                       56.806451612903224,
                       4.806451612903226,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0.32258064516129031,
                       31.93548387096774,
                       138.45161290322579,
                       277.80645161290323,
                       413.32258064516128,
                       548.9677419354839,
                       611.09677419354841,
                       647.61290322580646,
                       624.25806451612902,
                       575.90322580645159,
                       489.19354838709677,
                       407.70967741935482,
                       253.09677419354838,
                       116.80645161290323,
                       24.774193548387096,
                       0.16129032258064516,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       10.933333333333334,
                       86.63333333333334,
                       219.06666666666666,
                       367.36666666666667,
                       479.53333333333336,
                       533.83333333333337,
                       555.5333333333333,
                       558.0333333333333,
                       500.5,
                       398.06666666666666,
                       289.89999999999998,
                       150.09999999999999,
                       40.966666666666669,
                       2.1666666666666665,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0.32258064516129031,
                       33.387096774193552,
                       136.06451612903226,
                       265.03225806451616,
                       374.87096774193549,
                       424.35483870967744,
                       427.90322580645159,
                       425.51612903225805,
                       378.19354838709677,
                       265.87096774193549,
                       156.96774193548387,
                       48.967741935483872,
                       3.7419354838709675,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       4.7333333333333334,
                       50.866666666666667,
                       146.33333333333334,
                       217.26666666666668,
                       286.43333333333334,
                       319,
                       301.60000000000002,
                       249.03333333333333,
                       160.80000000000001,
                       70.099999999999994,
                       12.466666666666667,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       22.225806451612904,
                       98.709677419354833,
                       182.58064516129033,
                       252.12903225806451,
                       262,
                       262.48387096774195,
                       213.29032258064515,
                       143.87096774193549,
                       59.741935483870968,
                       6.967741935483871,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0};

  double mEghExp[] = {73.498655913978496, 103.88988095238095, 143.34005376344086, 183.0888888888889, 248.99462365591398, 262.22916666666669,
                      257.36559139784947, 215.05913978494624, 174.69305555555556, 122.5497311827957, 75.776388888888889, 62.666666666666664};
  double mdbtExp[] = {-4.6465053763440771, -2.5202380952380974, 3.8239247311827915, 9.9508333333333443, 15.310349462365592, 21.109166666666674,
                      24.134811827956977,  21.773655913978505,  18.133888888888915, 10.980779569892471, 4.7316666666666549, -3.686155913978499};
  double mwindExp[] = {4.8819892473117852, 5.0675595238094946, 5.4897849462365063, 4.8318055555555315, 3.7471774193547995, 4.8861111111110702,
                       4.2388440860214578, 3.8454301075268433, 3.4134722222221918, 4.7880376344085489, 5.2956944444443987, 4.2947580645160786};

  int v = 0;
  for (size_t r = 0; r < msolar.size1(); r++) {
    for (size_t c = 0; c < msolar.size2(); c++, v++) {
      EXPECT_DOUBLE_EQ(msolarExp[v], msolar(r, c));
    }
  }
  v = 0;
  for (size_t r = 0; r < mhdbt.size1(); r++) {
    for (size_t c = 0; c < mhdbt.size2(); c++, v++) {
      EXPECT_DOUBLE_EQ(mhdbtExp[v], mhdbt(r, c));
    }
  }
  v = 0;
  for (size_t r = 0; r < mhEgh.size1(); r++) {
    for (size_t c = 0; c < mhEgh.size2(); c++, v++) {
      EXPECT_DOUBLE_EQ(mhEghExp[v], mhEgh(r, c));
    }
  }
  v = 0;
  for (int r = 0; r < 12; r++, v++) {
    EXPECT_DOUBLE_EQ(mEghExp[v], mEgh[r]);
  }
  v = 0;
  for (int r = 0; r < 12; r++, v++) {
    EXPECT_DOUBLE_EQ(mdbtExp[v], mdbt[r]);
  }
  v = 0;
  for (int r = 0; r < 12; r++, v++) {
    EXPECT_DOUBLE_EQ(mwindExp[v], mwind[r]);
  }
}
