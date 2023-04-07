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

#include "../ReverseTranslator.hpp"

#include "../../model/OutputDebuggingData.hpp"
#include "../../model/OutputDebuggingData_Impl.hpp"

#include "../../utilities/math/FloatCompare.hpp"

#include <utilities/idd/Output_DebuggingData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputDebuggingData(const WorkspaceObject& wo) {
    boost::optional<ModelObject> result;

    // Check whether we have either field set to True before we bother instantiating the object...
    bool reportDebuggingData = false;
    bool reportDuringWarmup = false;

    // Note: E+ is wrong here: IDD uses numeric fields to store what is really a boolean. If 1 = true, else false.

    // Report Debugging Data: Optional Boolean
    if (boost::optional<std::string> _reportDebuggingData = wo.getString(Output_DebuggingDataFields::ReportDebuggingData, true)) {
      if (istringEqual("Yes", _reportDebuggingData.get())) {
        reportDebuggingData = true;
      }
    }

    // Report During Warmup: Optional Boolean
    if (boost::optional<std::string> _reportDuringWarmup = wo.getString(Output_DebuggingDataFields::ReportDuringWarmup, true)) {
      if (istringEqual("Yes", _reportDuringWarmup.get())) {
        reportDuringWarmup = true;
      }
    }

    if (reportDebuggingData || reportDuringWarmup) {
      // This is a Unique ModelObject
      auto modelObject = m_model.getUniqueModelObject<OutputDebuggingData>();
      modelObject.setReportDebuggingData(reportDebuggingData);
      modelObject.setReportDuringWarmup(reportDuringWarmup);
      result = modelObject;
    }

    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
