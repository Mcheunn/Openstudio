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

#ifndef MODEL_TABLELOOKUP_IMPL_HPP
#define MODEL_TABLELOOKUP_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  class ModelObjectList;
  class TableIndependentVariable;

  namespace detail {

    /** TableLookup_Impl is a Curve_Impl that is the implementation class for TableLookup.*/
    class MODEL_API TableLookup_Impl : public Curve_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      TableLookup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      TableLookup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      TableLookup_Impl(const TableLookup_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~TableLookup_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual int numVariables() const override;

      virtual double evaluate(const std::vector<double>& independentVariables) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ModelObjectList> independentVariableList() const;

      std::string normalizationMethod() const;

      double normalizationDivisor() const;

      boost::optional<double> minimumOutput() const;

      boost::optional<double> maximumOutput() const;

      std::string outputUnitType() const;

      //@}
      /** @name Setters */
      //@{

      bool setNormalizationMethod(const std::string& normalizationMethod);

      bool setNormalizationDivisor(double normalizationDivisior);

      bool setMinimumOutput(double minimumOutput);
      void resetMinimumOutput();

      bool setMaximumOutput(double maximumOutput);
      void resetMaximumOutput();

      bool setOutputUnitType(const std::string& outputUnitType);

      //@}
      /** @name Other */
      //@{

      bool addOutputValue(double outputValue);
      bool removeOutputValue(unsigned groupIndex);
      void removeAllOutputValues();
      std::vector<double> outputValues() const;
      bool setOutputValues(const std::vector<double>& outputValues);
      unsigned numberofOutputValues() const;

      bool addIndependentVariable(const TableIndependentVariable& tableIndependentVariable);
      bool removeIndependentVariable(const TableIndependentVariable& tableIndependentVariable);
      void removeAllIndependentVariables();
      std::vector<TableIndependentVariable> independentVariables() const;

      bool setIndependentVariableList(const boost::optional<ModelObjectList>& modelObjectList);
      void resetIndependentVariableList();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.TableLookup");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_TABLELOOKUP_IMPL_HPP
