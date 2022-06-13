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
#ifndef MODEL_GLTFMODELOBJECTMETADATAWRAPPER_HPP
#define MODEL_GLTFMODELOBJECTMETADATAWRAPPER_HPP

#include "ModelAPI.hpp"
#include "Model.hpp"

namespace openstudio {
//class GltfMetaDta;
namespace model {

  class MODEL_API GltfModelObjectMetadataWrapper
  {
   public:
    void setColor(const std::string& value);
    void setHandle(const std::string& value);
    void setIddObjectType(const std::string& value);
    void setName(const std::string& value);
    void setOpen_to_below(const bool& value);
    void setNominal_z_coordinate(const double& value);
    void setNominal_floorCeiling_Height(const double& value);
    void setMultiplier(const int& value);

    std::string getColor() const;
    std::string getHandle() const;
    std::string getIddObjectType() const;
    std::string getName() const;
    bool getOpen_to_below() const;
    double getNominal_z_coordinate() const;
    double getNominal_floorCeiling_Height() const;
    int getMultiplier() const;

   private:
    // string
    std::tuple<std::string, std::string> color;
    std::tuple<std::string, std::string> handle;
    std::tuple<std::string, std::string> iddObjectType;
    std::tuple<std::string, std::string> name;

    // bool
    std::tuple<std::string, bool> open_to_below;

    // double
    std::tuple<std::string, double> nominal_z_coordinate;
    std::tuple<std::string, double> nominal_floorCeiling_Height;

    // int
    std::tuple<std::string, int> multiplier;
  };

}  // namespace model
}  // namespace openstudio

#endif  //MODEL_GLTFMODELOBJECTMETADATAWRAPPER_HPP
