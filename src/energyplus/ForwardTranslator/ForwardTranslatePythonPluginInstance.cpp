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
#include "../../model/PythonPluginInstance.hpp"
#include "../../model/ExternalFile.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/PythonPlugin_Instance_FieldEnums.hxx>
#include <utilities/idd/PythonPlugin_SearchPaths_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/regex.hpp>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  bool findPluginClassNameInFile(const openstudio::path& filePath, const std::string& pluginClassName) {
    bool foundPluginClassName = false;

    const boost::regex re("class\\s+" + pluginClassName + "\\s*");
    openstudio::filesystem::ifstream ifs(filePath);
    std::string line;
    while (std::getline(ifs, line)) {
      if (boost::regex_search(line, re)) {
        foundPluginClassName = true;
        break;
      }
    }
    return foundPluginClassName;
  }

  boost::optional<IdfObject> ForwardTranslator::translatePythonPluginInstance(PythonPluginInstance& modelObject) {

    path filePath = modelObject.externalFile().filePath();
    if (!exists(filePath)) {
      LOG(Warn, modelObject.briefDescription() << "will not be translated, cannot find the referenced file '" << filePath << "'");
      return boost::none;
    }

    // make the path correct for this system
    filePath = openstudio::filesystem::system_complete(filePath);

    // If the plugin class name can't be found in the referenced external file, then it shouldn't be translated
    std::string pluginClassName = modelObject.pluginClassName();
    if (!findPluginClassNameInFile(filePath, pluginClassName)) {
      LOG(Warn, "PythonPluginInstance " << modelObject.name().get() << " does not have plugin class name '" << pluginClassName
                                        << "' contained in referenced external file");
      return boost::none;
    }

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PythonPlugin_Instance, modelObject);

    // Run During Warmup Days
    if (modelObject.runDuringWarmupDays()) {
      idfObject.setString(openstudio::PythonPlugin_InstanceFields::RunDuringWarmupDays, "Yes");
    } else {
      idfObject.setString(openstudio::PythonPlugin_InstanceFields::RunDuringWarmupDays, "No");
    }

    // Python Module Name
    std::string pythonModuleName = toString(filePath.stem());
    idfObject.setString(openstudio::PythonPlugin_InstanceFields::PythonModuleName, pythonModuleName);

    // Python Class Name
    idfObject.setString(openstudio::PythonPlugin_InstanceFields::PluginClassName, pluginClassName);

    // Translate Python Plugin Search Paths
    if (!m_pythonPluginSearchPaths) {
      // Create a new IddObjectType::PythonPlugin_SearchPaths
      m_pythonPluginSearchPaths = m_idfObjects.emplace_back(IddObjectType::PythonPlugin_SearchPaths);
      m_pythonPluginSearchPaths->setName("Python Plugin Search Paths");
      // These are IDD defaults
      m_pythonPluginSearchPaths->setString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath, "Yes");
      m_pythonPluginSearchPaths->setString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath, "Yes");
      m_pythonPluginSearchPaths->setString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath, "Yes");
    }

    std::string searchPath = toString(filePath.parent_path());
    bool newSearchPath = true;
    for (const IdfExtensibleGroup& eg : m_pythonPluginSearchPaths->extensibleGroups()) {
      if (searchPath == eg.getString(0)) {
        newSearchPath = false;
        break;
      }
    }

    if (newSearchPath) {
      IdfExtensibleGroup group = m_pythonPluginSearchPaths->pushExtensibleGroup({searchPath});
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
