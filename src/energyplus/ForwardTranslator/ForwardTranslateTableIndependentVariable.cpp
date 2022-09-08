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

#include "../../model/TableIndependentVariable.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Table_IndependentVariable_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateTableIndependentVariable(model::TableIndependentVariable& modelObject) {
    boost::optional<IdfObject> result;

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Table_IndependentVariable, modelObject);

    // Interpolation Method: String
    idfObject.setString(Table_IndependentVariableFields::InterpolationMethod, modelObject.interpolationMethod());

    // Extrapolation Method: String
    idfObject.setString(Table_IndependentVariableFields::ExtrapolationMethod, modelObject.extrapolationMethod());

    // Minimum Value: boost::optional<double>
    if (boost::optional<double> minimumValue_ = modelObject.minimumValue()) {
      idfObject.setDouble(Table_IndependentVariableFields::MinimumValue, minimumValue_.get());
    }

    // Maximum Value: boost::optional<double>
    if (boost::optional<double> maximumValue_ = modelObject.maximumValue()) {
      idfObject.setDouble(Table_IndependentVariableFields::MaximumValue, maximumValue_.get());
    }

    // Normalization Reference Value: boost::optional<double>
    if (boost::optional<double> normalizationReferenceValue_ = modelObject.normalizationReferenceValue()) {
      idfObject.setDouble(Table_IndependentVariableFields::NormalizationReferenceValue, normalizationReferenceValue_.get());
    }

    // Unit Type: String
    idfObject.setString(Table_IndependentVariableFields::UnitType, modelObject.unitType());

    // Value
    for (const double& value : modelObject.values()) {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setDouble(Table_IndependentVariableExtensibleFields::Value, value);
    }

    // External File Name: boost::optional<std::string>

    // External File Column Number: boost::optional<int>

    // External File Starting Row Number: boost::optional<int>

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
