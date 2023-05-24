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

#include "BCLFixture.hpp"

#include "../LocalBCL.hpp"
#include "../../core/ApplicationPathHelpers.hpp"

#include <algorithm>
#include <regex>

using openstudio::LocalBCL;
using openstudio::Logger;
using openstudio::toPath;
using openstudio::FileLogSink;

void BCLFixture::SetUp() {

  // Use a unique libraryPath to avoid concurrent access issues when running tests in parallel
  // This gets the name of the test that's being run (eg 'RemoteBCLTest')
  std::string currentTestName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
  currentLocalBCLPath = resourcesPath() / toPath("utilities/BCL") / toPath(currentTestName);

  // If for some reason (like CTRL+C) the previous pass didn't get cleaned up, do it
  try {
    openstudio::filesystem::remove_all(currentLocalBCLPath);
  } catch (...) {
  }

  // Initialize the LocalBCL Singleton at the given library path
  LocalBCL& bcl = LocalBCL::instance(currentLocalBCLPath);

  if (bcl.prodAuthKey().empty()) {
    prodAuthKey = defaultProdAuthKey;
    bcl.setProdAuthKey(prodAuthKey);
  } else {
    prodAuthKey = bcl.prodAuthKey();
  }

  // TODO Uncomment after network error handling is in place
  /*if (LocalBCL::instance().devAuthKey().empty()) {
    devAuthKey = defaultDevAuthKey;
    bcl.setDevAuthKey(devAuthKey);
  } else {
    devAuthKey = bcl.devAuthKey();
  }*/

  openstudio::path testDir = openstudio::filesystem::system_complete(openstudio::getApplicationBuildDirectory() / toPath("Testing"));
  if (!openstudio::filesystem::exists(testDir)) {
    openstudio::filesystem::create_directories(testDir);
  }
}

void BCLFixture::TearDown() {

  LocalBCL::close();
  try {
    openstudio::filesystem::remove_all(currentLocalBCLPath);
  } catch (...) {
  }
}

void BCLFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./BCLFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();
}

void BCLFixture::TearDownTestSuite() {
  logFile->disable();
}

::testing::AssertionResult BCLFixture::checkLogMessagesContain(const std::vector<openstudio::LogMessage>& logMessages,
                                                               const std::vector<std::string>& searchStrings, bool use_regex) {

  std::vector<std::string> logStrings;
  std::transform(logMessages.cbegin(), logMessages.cend(), std::back_inserter(logStrings),
                 [](const auto& logMessage) { return logMessage.logMessage(); });

  std::stringstream ss;
  int not_found = 0;

  for (const std::string& searchString : searchStrings) {
    bool match_found = false;
    for (const std::string& logString : logStrings) {
      if (use_regex) {
        match_found = std::regex_match(logString, std::regex(searchString));
      } else {
        match_found = logString.find(searchString) != std::string::npos;
      }
      if (match_found) {
        break;
      }
    }
    if (!match_found) {
      ++not_found;
      ss << " * " << not_found << ": '" << searchString << "'\n";
    }
  }

  if (not_found == 0) {
    return ::testing::AssertionSuccess();
  } else {
    return ::testing::AssertionFailure() << "Could not find " << not_found << " messages:\n" << ss.str();
  }
}

// define static storage
boost::optional<openstudio::FileLogSink> BCLFixture::logFile;
// these are Dan's API keys labelled under "Testing", delete when there is a better way to do this
const std::string BCLFixture::defaultProdAuthKey("2da842aa2d457703d8fdcb5c53080ace");
const std::string BCLFixture::defaultDevAuthKey("e8051bca77787c0df16cbe13452e7580");
