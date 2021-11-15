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

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../DaylightingDeviceTubular.hpp"
#include "../DaylightingDeviceTubular_Impl.hpp"
#include "../SubSurface.hpp"
#include "../Construction.hpp"
#include "../ThermalZone.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, DaylightingDeviceTubular) {
  Model model;

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 1));
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(0, 1, 0));
  points1.push_back(Point3d(0, 1, 1));
  SubSurface dome(points1, model);
  EXPECT_TRUE(dome.setSubSurfaceType("TubularDaylightDome"));
  EXPECT_EQ("TubularDaylightDome", dome.subSurfaceType());

  EXPECT_FALSE(dome.daylightingDeviceTubular());

  Point3dVector points2;
  points2.push_back(Point3d(0, 0, 2));
  points2.push_back(Point3d(0, 0, 0));
  points2.push_back(Point3d(0, 2, 0));
  points2.push_back(Point3d(0, 2, 2));
  SubSurface diffuser(points2, model);
  EXPECT_TRUE(diffuser.setSubSurfaceType("TubularDaylightDiffuser"));
  EXPECT_EQ("TubularDaylightDiffuser", diffuser.subSurfaceType());

  EXPECT_FALSE(diffuser.daylightingDeviceTubular());

  Construction construction(model);

  DaylightingDeviceTubular tubular(dome, diffuser, construction, 1, 2, 3);
  UUID tubularHandle = tubular.handle();
  ASSERT_TRUE(dome.daylightingDeviceTubular());
  ASSERT_TRUE(diffuser.daylightingDeviceTubular());
  EXPECT_EQ(tubularHandle, dome.daylightingDeviceTubular()->handle());
  EXPECT_EQ(tubularHandle, diffuser.daylightingDeviceTubular()->handle());
  /* ASSERT_TRUE(window.addDaylightingDeviceShelf()); */
  /* EXPECT_EQ(tubularHandle, window.addDaylightingDeviceShelf()->handle()); */
  /* EXPECT_EQ(window.addDaylightingDeviceShelf()->handle(), window.daylightingDeviceTubular()->handle()); */
  EXPECT_EQ(dome.handle(), tubular.subSurfaceDome().handle());
  EXPECT_EQ(diffuser.handle(), tubular.subSurfaceDiffuser().handle());

  tubular.remove();
  EXPECT_FALSE(dome.daylightingDeviceTubular());
  EXPECT_FALSE(diffuser.daylightingDeviceTubular());
  /* ASSERT_TRUE(window.addDaylightingDeviceShelf()); */
  /* tubularHandle = window.addDaylightingDeviceShelf()->handle(); */
  /* ASSERT_TRUE(window.addDaylightingDeviceShelf()); */
  /* EXPECT_EQ(tubularHandle, window.addDaylightingDeviceShelf()->handle()); */
  /* ASSERT_TRUE(window.addDaylightingDeviceShelf()); */
  /* EXPECT_EQ(tubularHandle, window.addDaylightingDeviceShelf()->handle()); */
  /* ASSERT_TRUE(dome.daylightingDeviceTubular()); */
  /* ASSERT_TRUE(diffuser.daylightingDeviceTubular()); */
  /* EXPECT_EQ(tubularHandle, dome.daylightingDeviceTubular()->handle()); */
  /* EXPECT_EQ(tubularHandle, diffuser.daylightingDeviceTubular()->handle()); */

  // changing to door removes tubular
  /* EXPECT_TRUE(dome.setSubSurfaceType("Door")); */
  /* EXPECT_EQ("Door", dome.subSurfaceType()); */
  /* EXPECT_FALSE(dome.daylightingDeviceTubular()); */
  /* EXPECT_FALSE(window.addDaylightingDeviceShelf()); */
}

TEST_F(ModelFixture, DaylightingDeviceTubular_Throw) {
  Model model;

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 1));
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(0, 1, 0));
  points1.push_back(Point3d(0, 1, 1));
  SubSurface door1(points1, model);
  EXPECT_TRUE(door1.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door1.subSurfaceType());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  Point3dVector points2;
  points2.push_back(Point3d(0, 0, 1));
  points2.push_back(Point3d(0, 0, 0));
  points2.push_back(Point3d(0, 1, 0));
  points2.push_back(Point3d(0, 1, 1));
  SubSurface door2(points2, model);
  EXPECT_TRUE(door2.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door2.subSurfaceType());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  Construction construction(model);

  bool didThrow = false;
  try {
    DaylightingDeviceTubular tubular(door1, door2, construction, 1, 2, 3);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  // change to a window
  EXPECT_TRUE(door1.setSubSurfaceType("TubularDaylightDome"));
  EXPECT_EQ("TubularDaylightDome", door1.subSurfaceType());
  EXPECT_TRUE(door2.setSubSurfaceType("TubularDaylightDiffuser"));
  EXPECT_EQ("TubularDaylightDiffuser", door2.subSurfaceType());

  // first one succeeds
  didThrow = false;
  try {
    DaylightingDeviceTubular tubular(door1, door2, construction, 1, 2, 3);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_FALSE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  // second call throws
  didThrow = false;
  try {
    DaylightingDeviceTubular tubular(door1, door2, construction, 1, 2, 3);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  // changing to door removes tubular
  EXPECT_TRUE(door1.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door1.subSurfaceType());
  EXPECT_FALSE(door1.daylightingDeviceTubular());
  /* EXPECT_FALSE(door.addDaylightingDeviceShelf()); */
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());
}

TEST_F(ModelFixture, DaylightingDeviceTubular_Throw2) {
  Model model;

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 1));
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(0, 1, 0));
  points1.push_back(Point3d(0, 1, 1));
  SubSurface door1(points1, model);
  EXPECT_TRUE(door1.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door1.subSurfaceType());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  Point3dVector points2;
  points2.push_back(Point3d(0, 0, 1));
  points2.push_back(Point3d(0, 0, 0));
  points2.push_back(Point3d(0, 1, 0));
  points2.push_back(Point3d(0, 1, 1));
  SubSurface door2(points2, model);
  EXPECT_TRUE(door2.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door2.subSurfaceType());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  Construction construction(model);

  bool didThrow = false;
  try {
    DaylightingDeviceTubular tubular(door1, door2, construction);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  // change to a window
  EXPECT_TRUE(door1.setSubSurfaceType("TubularDaylightDome"));
  EXPECT_EQ("TubularDaylightDome", door1.subSurfaceType());
  EXPECT_TRUE(door2.setSubSurfaceType("TubularDaylightDiffuser"));
  EXPECT_EQ("TubularDaylightDiffuser", door2.subSurfaceType());

  // first one succeeds
  didThrow = false;
  try {
    DaylightingDeviceTubular tubular(door1, door2, construction);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_FALSE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  // second call throws
  didThrow = false;
  try {
    DaylightingDeviceTubular tubular(door1, door2, construction);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  // changing to door removes light tubular
  EXPECT_TRUE(door1.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door1.subSurfaceType());
  EXPECT_FALSE(door1.daylightingDeviceTubular());
  /* EXPECT_FALSE(door.addDaylightingDeviceShelf()); */
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());
}

TEST_F(ModelFixture, DaylightingDeviceTubular_SettersGetters) {
  Model model;

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 1));
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(0, 1, 0));
  points1.push_back(Point3d(0, 1, 1));
  SubSurface dome(points1, model);
  EXPECT_TRUE(dome.setSubSurfaceType("TubularDaylightDome"));
  EXPECT_EQ("TubularDaylightDome", dome.subSurfaceType());

  Point3dVector points2;
  points2.push_back(Point3d(0, 0, 2));
  points2.push_back(Point3d(0, 0, 0));
  points2.push_back(Point3d(0, 2, 0));
  points2.push_back(Point3d(0, 2, 2));
  SubSurface diffuser(points2, model);
  EXPECT_TRUE(diffuser.setSubSurfaceType("TubularDaylightDiffuser"));
  EXPECT_EQ("TubularDaylightDiffuser", diffuser.subSurfaceType());

  Construction construction(model);

  DaylightingDeviceTubular tubular(dome, diffuser, construction);

  EXPECT_EQ(dome, tubular.subSurfaceDome());
  EXPECT_EQ(diffuser, tubular.subSurfaceDiffuser());
  EXPECT_EQ(construction, tubular.construction());
  EXPECT_EQ(0.3556, tubular.diameter());
  EXPECT_EQ(1.4, tubular.totalLength());
  EXPECT_EQ(0.28, tubular.effectiveThermalResistance());

  Construction construction2(model);

  EXPECT_TRUE(tubular.setConstruction(construction2));
  EXPECT_TRUE(tubular.setDiameter(1));
  EXPECT_TRUE(tubular.setTotalLength(2));
  EXPECT_TRUE(tubular.setEffectiveThermalResistance(3));

  EXPECT_EQ(construction2, tubular.construction());
  EXPECT_EQ(1, tubular.diameter());
  EXPECT_EQ(2, tubular.totalLength());
  EXPECT_EQ(3, tubular.effectiveThermalResistance());
}

TEST_F(ModelFixture, DaylightingDeviceTubular_TransitionZones) {
  Model model;

  Point3dVector points1;
  points1.push_back(Point3d(0, 0, 1));
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(0, 1, 0));
  points1.push_back(Point3d(0, 1, 1));
  SubSurface dome(points1, model);
  EXPECT_TRUE(dome.setSubSurfaceType("TubularDaylightDome"));

  EXPECT_FALSE(dome.daylightingDeviceTubular());

  Point3dVector points2;
  points2.push_back(Point3d(0, 0, 2));
  points2.push_back(Point3d(0, 0, 0));
  points2.push_back(Point3d(0, 2, 0));
  points2.push_back(Point3d(0, 2, 2));
  SubSurface diffuser(points2, model);
  EXPECT_TRUE(diffuser.setSubSurfaceType("TubularDaylightDiffuser"));

  EXPECT_FALSE(diffuser.daylightingDeviceTubular());

  Construction construction(model);

  DaylightingDeviceTubular tubular(dome, diffuser, construction);

  tubular.removeAllTransitionZones();

  ThermalZone thermalZone1(model);
  ThermalZone thermalZone2(model);
  ThermalZone thermalZone3(model);

  EXPECT_EQ(0, tubular.numberofTransitionZones());
  EXPECT_TRUE(tubular.addTransitionZone(thermalZone1, 1));
  EXPECT_EQ(1, tubular.numberofTransitionZones());
  EXPECT_TRUE(tubular.addTransitionZone(thermalZone2, 2));
  EXPECT_EQ(2, tubular.numberofTransitionZones());
  EXPECT_TRUE(tubular.addTransitionZone(thermalZone3, 3));
  EXPECT_EQ(3, tubular.numberofTransitionZones());

  tubular.removeTransitionZone(1);
  EXPECT_EQ(2, tubular.numberofTransitionZones());

  std::vector<TransitionZone> transitionZones = tubular.transitionZones();
  EXPECT_EQ(1, transitionZones[0].length());
  EXPECT_EQ(3, transitionZones[1].length());

  tubular.removeAllTransitionZones();
  EXPECT_EQ(0, tubular.numberofTransitionZones());
  tubular.removeTransitionZone(0);
  EXPECT_EQ(0, tubular.numberofTransitionZones());

  TransitionZone transitionZone(thermalZone1, 4);
  EXPECT_TRUE(tubular.addTransitionZone(transitionZone));
  EXPECT_EQ(1, tubular.numberofTransitionZones());

  std::vector<TransitionZone> transitionZonesToAdd;
  TransitionZone transitionZone1(thermalZone1, 5);
  transitionZonesToAdd.push_back(transitionZone1);
  TransitionZone transitionZone2(thermalZone2, 5);
  transitionZonesToAdd.push_back(transitionZone2);
  ASSERT_TRUE(tubular.addTransitionZones(transitionZonesToAdd));
  EXPECT_EQ(3, tubular.numberofTransitionZones());
  EXPECT_EQ(3, tubular.transitionZones().size());
}