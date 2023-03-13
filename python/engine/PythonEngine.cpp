#include "PythonEngine.hpp"
#include <utilities/core/ApplicationPathHelpers.hpp>
#include "../../src/utilities/core/Filesystem.hpp"
#include <fmt/format.h>
#include <stdexcept>
#include <string>
#include <iostream>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include <PythonConfig.hxx>
#include <Python.h>
#include <SWIGPythonRuntime.hxx>

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

namespace openstudio {

void addToPythonPath(const openstudio::path& includePath) {
  if (!includePath.empty()) {
    PyObject* sys = PyImport_ImportModule("sys");
    PyObject* sysPath = PyObject_GetAttrString(sys, "path");
    // fmt::print("Prepending '{}' to sys.path\n", includePath);
    PyObject* unicodeIncludePath = PyUnicode_FromString(includePath.string().c_str());
    PyList_Insert(sysPath, 0, unicodeIncludePath);
  }
}

void PythonEngine::pyimport(const std::string& importName, const std::string& includePath) {
  addToPythonPath(includePath);
  PyImport_ImportModule(importName.c_str());
}

void PythonEngine::setupPythonPath(const std::vector<openstudio::path>& includeDirs, const openstudio::path& pythonHomeDir) {
  for (const auto& includeDir : includeDirs) {
    addToPythonPath(includeDir);
  }
  if (!pythonHomeDir.empty()) {
    wchar_t* a = Py_DecodeLocale(pythonHomeDir.generic_string().c_str(), nullptr);
    Py_SetPythonHome(a);
  }
}

PythonEngine::PythonEngine(int argc, char* argv[]) : ScriptEngine(argc, argv), program(Py_DecodeLocale(pythonProgramName, nullptr)) {
  // this frozen flag tells Python that the package and library have been frozen for embedding, so it shouldn't warn about missing prefixes
  Py_FrozenFlag = 1;

  // Set the PYTHONPATH / PYTHONHOME to the E+ shipped standard library
  // I think we need to set the python path before initializing the library
  auto pathToPythonPackages = getEnergyPlusDirectory() / "python_standard_lib";
  wchar_t* a = Py_DecodeLocale(pathToPythonPackages.make_preferred().string().c_str(), nullptr);
  Py_SetPath(a);
  Py_SetPythonHome(a);

  Py_SetProgramName(program);  // optional but recommended

  Py_Initialize();
  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }
  m_globalDict = PyModule_GetDict(m);

  //PyRun_SimpleString("from time import time,ctime\n"
  //                   "print('Today is', ctime(time()))\n");
  importOpenStudio();
}

PythonEngine::~PythonEngine() {
  if (Py_FinalizeEx() < 0) {
    exit(120);
  }
  PyMem_RawFree(program);
}

void PythonEngine::importOpenStudio() {
  // generic_string() converts to a POSIX path, with forward slashes, so that pyimport doesn't choke on backslashes understood as escape sequence
  if (moduleIsRunningFromBuildDirectory()) {
    const auto bindingsDir = getOpenStudioModuleDirectory();
    pyimport("openstudiodev", bindingsDir.generic_string());
  } else {
    const auto bindingsDir = getOpenStudioModuleDirectory() / "../Python";
    pyimport("openstudio", bindingsDir.generic_string());
  }
  // Somehow that doesn't suffice to register it...
  exec("import openstudio");
}

struct PythonObject
{
  PythonObject() = default;

  PythonObject(PyObject* obj) noexcept : obj_(obj) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(const PythonObject& other) noexcept : obj_(other.obj_) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(PythonObject&& other) noexcept : obj_(other.obj_) {
    // no reason to inc/dec, we just stole the ref counted object
    // from other
    other.obj_ = nullptr;
  }

  PythonObject& operator=(const PythonObject& rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;

      if (obj_) {
        Py_INCREF(obj_);
      }
    }

    return *this;
  }

  PythonObject& operator=(PythonObject&& rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;
      rhs.obj_ = nullptr;
    }

    return *this;
  }

  ~PythonObject() {
    if (obj_) {
      Py_DECREF(obj_);
    }
  }

  PyObject* obj_ = nullptr;
};

void PythonEngine::exec(std::string_view sv) {
  std::string command{sv};

  PyObject* v = PyRun_String(command.c_str(), Py_file_input, m_globalDict, m_globalDict);
  // PyObject* v = PyRun_SimpleString(command.c_str());
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  Py_DECREF(v);
}

ScriptObject PythonEngine::eval(std::string_view sv) {
  std::string command{sv};

  PyObject* v = PyRun_String(command.c_str(), Py_eval_input, m_globalDict, m_globalDict);

  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  // share in ownership
  PythonObject return_value(v);

  // decref count returned from Python
  Py_DECREF(v);

  return ScriptObject{return_value};
}

void* PythonEngine::getAs_impl(ScriptObject& obj, const std::type_info& ti) {

  auto val = std::any_cast<PythonObject>(obj.object);

  void* return_value = nullptr;

  if (ti == typeid(std::string*)) {
    // TODO: this sucks, and probably needs a PyDecref or two
    Py_ssize_t size = 0;
    char const* pc = PyUnicode_AsUTF8AndSize(val.obj_, &size);
    if (pc) {
      return_value = new std::string(pc, size);
    } else {
      throw std::runtime_error("Unable to convert to std::string in SWIG Python");
    }
  } else {
    const auto& type_name = getRegisteredTypeName(ti);

    auto* type = SWIG_Python_TypeQuery(type_name.c_str());

    if (!type) {
      throw std::runtime_error("Unable to find type in SWIG");
    }

    const auto result = SWIG_Python_ConvertPtr(val.obj_, &return_value, type, 0);

    if (!SWIG_IsOK(result)) {
      throw std::runtime_error(fmt::format("Error getting object from SWIG/Python of supposed type {}, {}\n", ti.name(), type_name.c_str()));
    }
  }

  return return_value;
}

std::pair<std::string, measure::OSMeasure*> PythonEngine::loadMeasureInferClassName(const openstudio::path& measureScriptPath) {

  auto importCmd = fmt::format(R"python(
import importlib.util
import inspect
spec = importlib.util.spec_from_file_location(f"throwaway", "{}")

module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
class_members = inspect.getmembers(module, lambda x: inspect.isclass(x) and issubclass(x, openstudio.measure.OSMeasure))
assert len(class_members) == 1
measure_name, measure_typeinfo = class_members[0]
print(f"{{measure_name}}, {{measure_typeinfo}}")
measure_type = None
if issubclass(measure_typeinfo, openstudio.measure.ModelMeasure):
    measure_type = "ModelMeasure"
elif issubclass(measure_typeinfo, openstudio.measure.EnergyPlusMeasure):
    measure_type = "EnergyPlusMeasure"
elif issubclass(measure_typeinfo, openstudio.measure.ReportingMeasure):
    measure_type = "ReportingMeasure"
print(f"{{measure_name}}, {{measure_typeinfo}}, {{measure_type}}")
)python",
                               measureScriptPath.generic_string());
  this->exec(importCmd);
  // measureScriptObject = pythonEngine->eval(fmt::format("module.{}()", className));
  ScriptObject measureScriptObject = this->eval("measure_typeinfo()");
  auto* measurePtr = this->getAs<openstudio::measure::OSMeasure*>(measureScriptObject);

  ScriptObject measureClassNameObject = this->eval("measure_name");
  std::string className = *(this->getAs<std::string*>(measureClassNameObject));

  return {className, measurePtr};
}

measure::OSMeasure* PythonEngine::loadMeasureKnownClassName(const openstudio::path& measureScriptPath, std::string_view className) {

  // place measureDirPath in sys.path; do from measure import MeasureName
  // I think this can't work without a "as xxx" otherwise we'll repeatedly try to import a module named 'measure'
  // pythonEngine->pyimport("measure", openstudio::toString(measureDirPath.get()));
  //         auto importCmd = fmt::format(R"python(
  // import sys
  // sys.path.insert(0, r'{}')
  // force_reload = 'measure' in sys.modules
  // import measure
  // if force_reload:
  //     # print("force reload measure")
  //     import importlib
  //     importlib.reload(measure)
  // )python",
  //                                      scriptPath_->parent_path().generic_string());
  auto importCmd = fmt::format(R"python(
import importlib.util
spec = importlib.util.spec_from_file_location('{}', r'{}')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
)python",
                               className, measureScriptPath.generic_string());
  // fmt::print("\nimportCmd:\n{}\n", importCmd);
  this->exec(importCmd);

  // measureScriptObject = pythonEngine->eval(fmt::format("measure.{}()", className));
  ScriptObject measureScriptObject = this->eval(fmt::format("module.{}()", className));

  openstudio::measure::OSMeasure* measurePtr = this->getAs<openstudio::measure::OSMeasure*>(measureScriptObject);

  return measurePtr;
}

}  // namespace openstudio

extern "C"
{
  openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::PythonEngine(argc, argv);
  }
}
