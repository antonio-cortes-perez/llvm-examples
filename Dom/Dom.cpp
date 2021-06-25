#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/DenseMap.h"

using namespace llvm;

namespace {

struct Dom : PassInfoMixin<Dom> {

  using DomSet = SmallPtrSet<const BasicBlock *, 32>;
  using DomMap = DenseMap<const BasicBlock *, DomSet>;

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    SmallVector<const BasicBlock *, 16> PostOrder;
    postOrder(&F.getEntryBlock(), PostOrder);
    outs() << "Post-order\n";
    dom(F, PostOrder);

    SmallVector<const BasicBlock *, 16> ReversePostOrder(PostOrder.rbegin(),
                                                         PostOrder.rend());
    outs() << "\nReverse post-order\n";
    dom(F, ReversePostOrder);

    return PreservedAnalyses::all();
  }

  void dom(Function &F, SmallVectorImpl<const BasicBlock *> &Order) {
    DomSet AllBB;
    for (const auto &BB : F) {
      AllBB.insert(&BB);
    }

    // Init Dom to all BBs for each BB except the entry BB which only contains
    // itself.
    DomMap D;
    for (const auto &BB : F) {
      D.try_emplace(&BB, AllBB);
    }
    D[&F.getEntryBlock()].clear();
    D[&F.getEntryBlock()].insert(&F.getEntryBlock());

    // Fixed-point algorithm.
    bool Changed = true;
    int Iteration = 0;
    while (Changed) {
      Changed = false;
      Iteration++;
      for (const BasicBlock *BB : Order) {
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
    outs() << "Iterations: " << Iteration << "\n";
    print(D);
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

  void postOrder(const BasicBlock *BB,
                 SmallVectorImpl<const BasicBlock *> &PostOrder) {
    SmallPtrSet<const BasicBlock *, 32> Visited;
    dfs(BB, Visited, PostOrder);
  }

  void dfs(const BasicBlock *BB, SmallPtrSetImpl<const BasicBlock *> &Visited,
           SmallVectorImpl<const BasicBlock *> &PostOrder) {
    Visited.insert(BB);
    for (auto *S : successors(BB)) {
      if (Visited.count(S) == 0) {
        dfs(S, Visited, PostOrder);
      }
    }
    PostOrder.push_back(BB);
  }
};

} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
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
