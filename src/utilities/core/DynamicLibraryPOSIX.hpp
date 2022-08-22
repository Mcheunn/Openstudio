#ifndef dynamiclibrary_posix_hpp_INCLUDED
#define dynamiclibrary_posix_hpp_INCLUDED

#include "Path.hpp"
#include <dlfcn.h>
#include <fmt/format.h>
#include <string>

namespace openstudio {

struct DynamicLibrary
{
  template <typename Signature>
  [[nodiscard]] Signature* load_symbol(const std::string& name) {
    // reinterpret_cast is necessary here
    const auto symbol = reinterpret_cast<Signature*>(dlsym(m_handle.get(), name.c_str()));

    if (symbol == nullptr) {
      throw std::runtime_error(fmt::format("Unable to load symbol: '{}', reason: '{}'", name, dlerror()));
    }

    return symbol;
  }

  explicit DynamicLibrary(openstudio::path location)
    : m_location{std::move(location)}, m_handle{dlopen(m_location.c_str(), RTLD_LAZY), m_handle_deleter} {
    if (!m_handle) {
      throw std::runtime_error(fmt::format("Unable to load library '{}', reason: '{}'", m_location.string(), dlerror()));
    }
  }

  static void m_handle_deleter(void* h) {
    if (h) {
      dlclose(h);
    }
  }

  openstudio::path m_location{};
  std::unique_ptr<void, decltype(&m_handle_deleter)> m_handle{nullptr, m_handle_deleter};
};
}  // namespace openstudio

#endif  // dynamiclibrary_posix_hpp_INCLUDED
