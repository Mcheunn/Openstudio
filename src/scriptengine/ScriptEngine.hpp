#ifndef SCRIPTENGINE_included
#define SCRIPTENGINE_included

#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/DynamicLibrary.hpp"
#include <any>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>

namespace openstudio {

namespace measure {
  class ModelMeasure;
}  // namespace measure

class ScriptEngine;

struct ScriptObject
{
  std::any object;
};
}  // namespace openstudio

using ScriptEngineFactoryType = openstudio::ScriptEngine*(int, char**);

namespace openstudio {

class ScriptEngine
{
 public:
  ScriptEngine([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    // registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
  }

  virtual ~ScriptEngine() = default;
  ScriptEngine(const ScriptEngine&) = delete;
  ScriptEngine(ScriptEngine&&) = delete;
  ScriptEngine& operator=(const ScriptEngine&) = delete;
  ScriptEngine& operator=(ScriptEngine&&) = delete;
  ScriptEngine* operator->() {
    return this;
  }

  virtual ScriptObject eval(std::string_view sv) = 0;

  // execute string without expecting a return value
  virtual void exec(std::string_view sv) = 0;

  template <typename T>
  T getAs(ScriptObject& obj) {
    void* result = getAs_impl(obj, typeid(T));
    if (result) {
      return static_cast<T>(result);
    } else {
      throw std::bad_cast();
    }
  }

  template <typename T>
  void registerType(std::string name) {
    types.emplace(std::cref(typeid(T)), std::move(name));
  }

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  virtual void* getAs_impl(ScriptObject& obj, const std::type_info&) = 0;

  const std::string& getRegisteredTypeName(const std::type_info& type) {
    const auto& found_name = types.find(type);

    if (found_name != types.end()) {
      return found_name->second;
    }

    throw std::runtime_error("unknown type requested");
  }

 private:
  struct Compare
  {
    bool operator()(const std::reference_wrapper<const std::type_info>& lhs, const std::reference_wrapper<const std::type_info>& rhs) const {
      return lhs.get().before(rhs.get());
    }
  };

  std::map<std::reference_wrapper<const std::type_info>, std::string, Compare> types;
};

inline std::unique_ptr<openstudio::ScriptEngine> loadScriptEngine(std::string_view libraryBaseName, int argc, char* argv[]) {
  auto enginePath = openstudio::getOpenStudioModuleDirectory() / openstudio::getSharedLibraryName(libraryBaseName);
  openstudio::DynamicLibrary engineLib(enginePath);

  const std::function<ScriptEngineFactoryType> factory = engineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");

  return std::unique_ptr<openstudio::ScriptEngine>(factory(argc, argv));
}

// The purpose of this class is to delay creating the scripting engine
// until it is needed, while still initializing with argc and argv
class ScriptEngineInstance
{
 public:
  ScriptEngineInstance(std::string_view libraryBaseName, int argc, char* argv[]) : libraryName(libraryBaseName), args(argv, argv + argc) {}

  ScriptEngineInstance(const ScriptEngineInstance&) = default;
  ScriptEngineInstance(ScriptEngineInstance&&) = delete;
  ScriptEngineInstance& operator=(const ScriptEngineInstance&) = delete;
  ScriptEngineInstance& operator=(ScriptEngineInstance&&) = delete;

  openstudio::ScriptEngine& operator->() {
    if (instance) {
      return *(instance.get());
    } else {
      std::vector<char*> argv;

      std::transform(args.begin(), args.end(), std::back_inserter(argv), [](const std::string& item) { return const_cast<char*>(item.c_str()); });

      instance = openstudio::loadScriptEngine(libraryName, args.size(), argv.data());
      return *instance;
    }
  }
  explicit operator bool() {
    return (bool)instance;
  }
  void reset() {
    instance.reset();
  }

 private:
  std::string libraryName;
  std::vector<std::string> args;
  std::shared_ptr<openstudio::ScriptEngine> instance;
};

}  // namespace openstudio

#endif
