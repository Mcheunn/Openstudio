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
#include "BCLFixture.hpp"

#include "../BCLXML.hpp"
#include "../BCLFileReference.hpp"
#include "../../xml/XMLValidator.hpp"

#include <pugixml.hpp>

using namespace openstudio;

TEST_F(BCLFixture, BCLXML) {
  openstudio::path path = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/measure.xml");
  boost::optional<BCLXML> bclXML = BCLXML::load(path);
  ASSERT_TRUE(bclXML);
}

TEST_F(BCLFixture, BCLXML_New) {
  BCLXML bclXML(BCLXMLType::MeasureXML);
  EXPECT_FALSE(bclXML.uid().empty());
  EXPECT_FALSE(bclXML.versionId().empty());
  EXPECT_TRUE(bclXML.name().empty());
  EXPECT_TRUE(bclXML.description().empty());
  EXPECT_TRUE(bclXML.modelerDescription().empty());
  EXPECT_TRUE(bclXML.path().empty());
  EXPECT_TRUE(bclXML.directory().empty());
  EXPECT_TRUE(bclXML.attributes().empty());
  EXPECT_TRUE(bclXML.tags().empty());
  EXPECT_TRUE(bclXML.files().empty());

  std::string versionId = bclXML.versionId();
  bclXML.setName("Name");
  EXPECT_EQ("Name", bclXML.name());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.setDescription("Description");
  EXPECT_EQ("Description", bclXML.description());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.setModelerDescription("ModelerDescription");
  EXPECT_EQ("ModelerDescription", bclXML.modelerDescription());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.addTag("tag");
  ASSERT_EQ(1u, bclXML.tags().size());
  EXPECT_EQ("tag", bclXML.tags()[0]);
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.addTag("tag");
  ASSERT_EQ(1u, bclXML.tags().size());
  EXPECT_EQ("tag", bclXML.tags()[0]);
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  Attribute attr("attribute", 1.0);
  bclXML.addAttribute(attr);
  ASSERT_EQ(1u, bclXML.attributes().size());
  EXPECT_EQ("attribute", bclXML.attributes()[0].name());
  EXPECT_EQ(1u, bclXML.getAttributes("attribute").size());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  attr = Attribute("attribute", 2.0);
  bclXML.addAttribute(attr);
  ASSERT_EQ(2u, bclXML.attributes().size());
  EXPECT_EQ("attribute", bclXML.attributes()[0].name());
  EXPECT_EQ("attribute", bclXML.attributes()[1].name());
  EXPECT_EQ(2u, bclXML.getAttributes("attribute").size());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  attr = Attribute("another attribute", 2.0);
  bclXML.addAttribute(attr);
  ASSERT_EQ(3u, bclXML.attributes().size());
  EXPECT_EQ("attribute", bclXML.attributes()[0].name());
  EXPECT_EQ("attribute", bclXML.attributes()[1].name());
  EXPECT_EQ("another attribute", bclXML.attributes()[2].name());
  EXPECT_EQ(2u, bclXML.getAttributes("attribute").size());
  EXPECT_EQ(1u, bclXML.getAttributes("another attribute").size());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  EXPECT_FALSE(bclXML.save());
  EXPECT_EQ(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  openstudio::path xmlPath = toPath("./measure.xml");
  EXPECT_TRUE(bclXML.saveAs(xmlPath));
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  EXPECT_TRUE(bclXML.save());
  EXPECT_EQ(versionId, bclXML.versionId());

  boost::optional<BCLXML> copy = BCLXML::load(xmlPath);
  ASSERT_TRUE(copy);

  auto bclXMLValidator = XMLValidator::bclXMLValidator(BCLXMLType::MeasureXML, 3);
  EXPECT_TRUE(bclXMLValidator.validate(xmlPath));
  EXPECT_EQ(0, bclXMLValidator.errors().size()) << [&bclXMLValidator]() {
    std::string s;
    for (auto& logMessage : bclXMLValidator.errors()) {
      s += logMessage.logMessage() + "\n";
    }
    return s;
  }();
  EXPECT_EQ(0, bclXMLValidator.warnings().size());
}

TEST_F(BCLFixture, BCLXML_sorted_files) {

  BCLXML bclXML(BCLXMLType::MeasureXML);

  const openstudio::path testDir("my_measure");

  const BCLFileReference ref1(testDir, "fileA", true);
  const BCLFileReference ref2(testDir, "fileC", true);
  const BCLFileReference ref3(testDir, "fileB", true);
  const BCLFileReference ref4(testDir, "fileD", true);

  bclXML.addFile(ref1);
  bclXML.addFile(ref2);
  bclXML.addFile(ref3);
  bclXML.addFile(ref4);

  const openstudio::path xmlPath = resourcesPath() / toPath("utilities/BCL/sorted.xml");
  bclXML.saveAs(xmlPath);

  pugi::xml_document xmlDoc;
  openstudio::filesystem::ifstream file(xmlPath);
  ASSERT_TRUE(file.is_open());
  ASSERT_TRUE(xmlDoc.load(file));
  auto measureElement = xmlDoc.child("measure");
  auto xmlFilesElement = measureElement.child("files");
  std::vector<std::string> savedFiles;
  for (auto& fileElement : xmlFilesElement.children("file")) {
    if (!fileElement.first_child().empty()) {
      // Filename is a relative path, and if usageType is doc/resource/test it omits the subdirectory
      savedFiles.emplace_back(fileElement.child("filename").text().as_string());
    }
  }

  ASSERT_EQ(4, savedFiles.size());
  EXPECT_EQ("fileA", savedFiles[0]);
  EXPECT_EQ("fileB", savedFiles[1]);
  EXPECT_EQ("fileC", savedFiles[2]);
  EXPECT_EQ("fileD", savedFiles[3]);
}
