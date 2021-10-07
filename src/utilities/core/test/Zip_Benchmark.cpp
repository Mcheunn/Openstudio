#include <benchmark/benchmark.h>

#include "../UnzipFile.hpp"
#include "../Path.hpp"
#include "utilities/core/FilesystemHelpers.hpp"
#include <resources.hxx>

#include <string>
#include <vector>

openstudio::path prepareOutDir(const std::string& test_case) {
  openstudio::path outpath = openstudio::tempDir() / openstudio::toPath(test_case);
  openstudio::filesystem::remove_all(outpath);
  return outpath;
}

static void BM_Current(benchmark::State& state) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/test1.zip");
  openstudio::path outpath = prepareOutDir("Current");

  openstudio::UnzipFile uf(p);
  uf.setChunksize(state.range(0));

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    uf.extractAllFiles(outpath);
    openstudio::filesystem::remove_all(outpath);
  }
}

static void BM_Mod(benchmark::State& state) {
  openstudio::path p = resourcesPath() / openstudio::toPath("utilities/Zip/test1.zip");
  openstudio::path outpath = prepareOutDir("Mod");

  openstudio::UnzipFile uf(p);
  uf.setChunksize(state.range(0));

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    uf.extractAllFilesMod(outpath);
    openstudio::filesystem::remove_all(outpath);
  }
}

BENCHMARK(BM_Current)->Iterations(5)->Unit(benchmark::kMillisecond)->Arg(32768);  // ->RangeMultiplier(2)->Range(1024, 8 << 13);
BENCHMARK(BM_Mod)->Iterations(5)->Unit(benchmark::kMillisecond)->Arg(32768);      // RangeMultiplier(2)->Range(1024, 8 << 13);
