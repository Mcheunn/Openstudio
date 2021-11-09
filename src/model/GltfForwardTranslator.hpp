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
#ifndef MODEL_GLTFFORWARDTRANSLATOR_HPP
#define MODEL_GLTFFORWARDTRANSLATOR_HPP

#include "ModelAPI.hpp"

#include "Model.hpp"


#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

namespace openstudio {
namespace model {

  /** GltfForwardTranslator converts an OpenStudio Model to GLTF format. There are two variations of the GLTF format,
    *   a triangulated one which is suitable for rendering with GLTF and non-triangulated one that preserves all vertices in a
    *   surface for conversion to OpenStudio Model format.
    *
    *   The FloorplanJS in the Utilities/Geometry project converts a FloorspaceJS JSON file to GLTF format, code should be shared between these two classes as much as possible.
    */
  class MODEL_API GltfForwardTranslator
  {
   public:
    GltfForwardTranslator();

    /// Convert an OpenStudio Model to Gltf format
    /// Triangulate surfaces if the Gltf representation will be used for display
    /// Do not triangulate surfaces if the Gltf representation will be translated back to a model
    bool modelToGLTF(const Model& model, bool triangulateSurfaces,const path& outputPath);
    bool modelToGLTF(const Model& model, bool triangulateSurfaces, std::function<void(double)> updatePercentage, const path& outputpath);

    //tinygltf::Model to json 
    bool toJSON();

    //Create Triangle gltf
    bool CreateTriangleGLTF(const path& outputPath);
    bool CreateTriangleGLTF_2(const path& outputPath);

    //load minimal gltf
    bool loadGLTF(const path& inputPath);

    /// Get warning messages generated by the last translation.
    std::vector<LogMessage> warnings() const;

    /// Get error messages generated by the last translation.
    std::vector<LogMessage> errors() const;

   private:
    //tinygltf::Model gltf_Model;
    REGISTER_LOGGER("openstudio.model.GltfForwardTranslator");

    StringStreamLogSink m_logSink;
  };

}  // namespace model
}  // namespace openstudio
#endif  //MODEL_GLTFFORWARDTRANSLATOR_HPP