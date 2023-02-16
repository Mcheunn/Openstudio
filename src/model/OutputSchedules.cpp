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

#include "OutputSchedules.hpp"
#include "OutputSchedules_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_Schedules_FieldEnums.hxx>

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputSchedules_Impl::OutputSchedules_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputSchedules::iddObjectType());
    }

    OutputSchedules_Impl::OutputSchedules_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputSchedules::iddObjectType());
    }

    OutputSchedules_Impl::OutputSchedules_Impl(const OutputSchedules_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputSchedules_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputSchedules_Impl::iddObjectType() const {
      return OutputSchedules::iddObjectType();
    }

    std::string OutputSchedules_Impl::keyField() const {
      boost::optional<std::string> value = getString(OS_Output_SchedulesFields::KeyField, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputSchedules_Impl::setKeyField(const std::string& keyField) {
      bool result = setString(OS_Output_SchedulesFields::KeyField, keyField);
      return result;
    }

  }  // namespace detail

  IddObjectType OutputSchedules::iddObjectType() {
    return {IddObjectType::OS_Output_Schedules};
  }

  std::vector<std::string> OutputSchedules::keyFieldValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Output_SchedulesFields::KeyField);
  }

  std::vector<std::string> OutputSchedules::validKeyFieldValues() {
    return keyFieldValues();
  }

  std::string OutputSchedules::keyField() const {
    return getImpl<detail::OutputSchedules_Impl>()->keyField();
  }

  bool OutputSchedules::setKeyField(const std::string& keyField) {
    return getImpl<detail::OutputSchedules_Impl>()->setKeyField(keyField);
  }

  /// @cond
  OutputSchedules::OutputSchedules(std::shared_ptr<detail::OutputSchedules_Impl> impl) : ModelObject(impl) {}
  OutputSchedules::OutputSchedules(Model& model) : ModelObject(OutputSchedules::iddObjectType(), model) {
    bool ok = setKeyField("Hourly");
    OS_ASSERT(ok);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
