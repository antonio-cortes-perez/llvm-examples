#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_map>
#include <unordered_set>

using namespace llvm;

namespace {

struct Template : PassInfoMixin<Template> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    for (const auto &BB : F) {
      outs() << BB.getName() << "\n";
    }

    return PreservedAnalyses::all();
  }
};

} // namespace

// PM Registration
llvm::PassPluginLibraryInfo getDomPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "Template", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "template") {
                    FPM.addPass(Template());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getDomPluginInfo();
}
