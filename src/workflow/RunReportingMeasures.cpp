#include "OSWorkflow.hpp"

#include "Util.hpp"

#include "../model/Model.hpp"
#include "../utilities/idf/Workspace.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"
#include "../utilities/core/Filesystem.hpp"

namespace openstudio {

void OSWorkflow::runReportingMeasures() {
  state = State::ReportingMeasures;

  // halted workflow is handled in applyMeasures

  // TODO: urbanopt

  auto runDirPath = workflowJSON.absoluteRunDir();

  // Load simulation files as required. I suppose this is for the case where you only run post processing
  if (!runner.halted()) {  // && !urbanopt
    if (model.numObjects() == 0) {
      if (auto p = runDirPath / "in.osm"; openstudio::filesystem::is_regular_file(p)) {
        LOG(Debug, "Attempting to load " << p);
        model = openstudio::workflow::util::loadOSM(p);
      }
    }
    if (!workspace_) {
      if (auto p = runDirPath / "in.idf"; openstudio::filesystem::is_regular_file(p)) {
        LOG(Debug, "Attempting to load " << p);
        workspace_ = openstudio::workflow::util::loadIDF(p);
      }
    }
    if (sqlPath.empty()) {
      if (auto p = runDirPath / "eplusout.sql"; openstudio::filesystem::is_regular_file(p)) {
        sqlPath = p;
        LOG(Debug, "Registered the sql filepath as " << p);
      }
    }
    if (epwPath.empty()) {
      if (auto p = runDirPath / "in.epw"; openstudio::filesystem::is_regular_file(p)) {
        epwPath = p;
        LOG(Debug, "Registered the EPW filepath as " << p);
      }
    }
  }

  LOG(Info, "Beginning to execute Reporting Measures.");
  applyMeasures(MeasureType::ReportingMeasure, false);
  LOG(Info, "Finished applying Reporting Measures.");

  // TODO
  //   # Parse the files generated by the local output adapter
  //   results, objective_functions = run_extract_inputs_and_outputs @registry[:run_dir], @logger
  //   @registry.register(:results) { results }

  //   # Send the objective function results to the output adapter
  //   @logger.debug "Objective Function JSON is #{objective_functions}"
  //   @output_adapter.communicate_objective_function objective_functions
}
}  // namespace openstudio
