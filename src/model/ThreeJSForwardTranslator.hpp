/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THREEJSFORWARDTRANSLATOR_HPP
#define MODEL_THREEJSFORWARDTRANSLATOR_HPP

#include "ModelAPI.hpp"

#include "Model.hpp"

#include "../utilities/geometry/ThreeJS.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

namespace openstudio {
namespace model {

  /** ThreeJSForwardTranslator converts an OpenStudio Model to ThreeJS format. There are two variations of the ThreeJS format,
    *   a triangulated one which is suitable for rendering with ThreeJS and non-triangulated one that preserves all vertices in a
    *   surface for conversion to OpenStudio Model format.
    *
    *   The FloorplanJS in the Utilities/Geometry project converts a FloorspaceJS JSON file to ThreeJS format, code should be shared between these two classes as much as possible.
    */
  class MODEL_API ThreeJSForwardTranslator
  {
   public:
    ThreeJSForwardTranslator();

    /// Convert an OpenStudio Model to ThreeJS format
    /// Triangulate surfaces if the ThreeJS representation will be used for display
    /// Do not triangulate surfaces if the ThreeJs representation will be translated back to a model
    ThreeScene modelToThreeJS(const Model& model, bool triangulateSurfaces);
    ThreeScene modelToThreeJS(const Model& model, bool triangulateSurfaces, std::function<void(double)> updatePercentage);

    /// Get warning messages generated by the last translation.
    std::vector<LogMessage> warnings() const;

    /// Get error messages generated by the last translation.
    std::vector<LogMessage> errors() const;

    // Check if Surface is Convex, and check if its correctly oriented given the containing space, whether the space itself is Convex, Enclosed
    // This is computationally intensive, so it's opt-in
    bool includeGeometryDiagnostics() const;
    void setIncludeGeometryDiagnostics(bool includeGeometryDiagnostics);

   private:
    REGISTER_LOGGER("openstudio.model.ThreeJSForwardTranslator");

    StringStreamLogSink m_logSink;
    bool m_includeGeometryDiagnostics = false;
  };

}  // namespace model
}  // namespace openstudio
#endif  //MODEL_THREEJSFORWARDTRANSLATOR_HPP
