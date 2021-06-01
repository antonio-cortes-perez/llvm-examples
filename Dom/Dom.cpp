#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_map>
#include <unordered_set>

using namespace llvm;

namespace {

struct Dom : PassInfoMixin<Dom> {

  using DomSet = std::unordered_set<const BasicBlock *>;
  using DomMap = std::unordered_map<const BasicBlock *, DomSet>;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    dom(F);
    return PreservedAnalyses::all();
  }

  void dom(Function &F) {
    DomSet AllBB;
    for (const auto &BB : F) {
      AllBB.insert(&BB);
    }

    // Init Dom to all BBs for each BB except the entry BB which only contains
    // itself.
    DomMap D;
    for (const auto &BB : F) {
      D.emplace(&BB, AllBB);
    }
    D[&F.getEntryBlock()].clear();
    D[&F.getEntryBlock()].insert(&F.getEntryBlock());

    // Fixed-point algorithm.
    bool Changed = true;
    int Iteration = 0;
    while (Changed) {
      Changed = false;
      Iteration++;
      for (const BasicBlock *BB : AllBB) {
        DomSet &DomBB = D[BB];

        // Compute new Dom.
        DomSet Temp(DomBB);
        for (const auto *Pred : predecessors(BB)) {
          intersection(Temp, D[Pred]);
        }
        Temp.insert(BB);

        // Check if it Changed.
        if (DomBB != Temp) {
          DomBB.swap(Temp);
          Changed = true;
        }
      }
    }
    print(D);
    outs() << "Iterations: " << Iteration << "\n";
  }

  void intersection(DomSet &A, const DomSet &B) {
    DomSet ToErase;
    for (const auto *Elem : A) {
      if (B.count(Elem) == 0)
        ToErase.insert(Elem);
    }
    for (const auto *Elem : ToErase) {
      A.erase(Elem);
    }
  }

  void print(DomMap &D) {
    for (const auto &Entry : D) {
      outs() << "* " << Entry.first->getName() << "(" << Entry.second.size()
             << "): ";
      for (const auto *DomBB : Entry.second) {
        outs() << DomBB->getName() << " ";
      }
      outs() << "\n";
    }
  }
};

} // namespace

// PM Registration
llvm::PassPluginLibraryInfo getDomPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "Dom", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "dom") {
                    FPM.addPass(Dom());
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
