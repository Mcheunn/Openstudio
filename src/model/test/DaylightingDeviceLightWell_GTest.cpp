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
#include "../DaylightingDeviceLightWell.hpp"
#include "../DaylightingDeviceLightWell_Impl.hpp"
#include "../SubSurface.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, DaylightingDeviceLightWell) {
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 1, 1));
  SubSurface window(points, model);
  EXPECT_EQ("FixedWindow", window.subSurfaceType());

  EXPECT_FALSE(window.daylightingDeviceLightWell());

  DaylightingDeviceLightWell lightWell(window, 1, 2, 3, 4);
  UUID lightWellHandle = lightWell.handle();
  ASSERT_TRUE(window.daylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.daylightingDeviceLightWell()->handle());
  ASSERT_TRUE(window.addDaylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.addDaylightingDeviceLightWell()->handle());
  EXPECT_EQ(window.addDaylightingDeviceLightWell()->handle(), window.daylightingDeviceLightWell()->handle());
  EXPECT_EQ(window.handle(), lightWell.subSurface().handle());

  lightWell.remove();
  EXPECT_FALSE(window.daylightingDeviceLightWell());
  ASSERT_TRUE(window.addDaylightingDeviceLightWell());
  lightWellHandle = window.addDaylightingDeviceLightWell()->handle();
  ASSERT_TRUE(window.addDaylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.addDaylightingDeviceLightWell()->handle());
  ASSERT_TRUE(window.addDaylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.addDaylightingDeviceLightWell()->handle());
  ASSERT_TRUE(window.daylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.daylightingDeviceLightWell()->handle());

  // changing to door removes light light well
  EXPECT_TRUE(window.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", window.subSurfaceType());
  EXPECT_FALSE(window.daylightingDeviceLightWell());
  EXPECT_FALSE(window.addDaylightingDeviceLightWell());
}

TEST_F(ModelFixture, DaylightingDeviceLightWell_Throw) {
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 1, 1));
  SubSurface door(points, model);
  EXPECT_TRUE(door.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door.subSurfaceType());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  bool didThrow = false;
  try {
    DaylightingDeviceLightWell lightWell(door, 1, 2, 3, 4);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // change to a window
  EXPECT_TRUE(door.setSubSurfaceType("FixedWindow"));
  EXPECT_EQ("FixedWindow", door.subSurfaceType());

  // first one succeeds
  didThrow = false;
  try {
    DaylightingDeviceLightWell lightWell(door, 1, 2, 3, 4);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_FALSE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // second call throws
  didThrow = false;
  try {
    DaylightingDeviceLightWell lightWell(door, 1, 2, 3, 4);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // changing to door removes light light well
  EXPECT_TRUE(door.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door.subSurfaceType());
  EXPECT_FALSE(door.DaylightingDeviceLightWell());
  EXPECT_FALSE(door.addDaylightingDeviceLightWell());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());
}
