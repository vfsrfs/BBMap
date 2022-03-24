#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <llvm/IR/DebugLoc.h>
#include <llvm/IR/DebugInfoMetadata.h>


using namespace llvm;

int getLineNumber(const Instruction &inst) {
  if (!inst.hasMetadata()) {
    return -1;
  }
  return inst.getDebugLoc().getLine();
}


namespace {
  struct BBMapPass : public FunctionPass {
    static char ID;
    BBMapPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      for (auto& bb: F) {
        std::string locs = "";
        for (auto &inst : bb) {
          if (inst.hasMetadata()) {
            if (locs.size() == 0) {
              locs.append(inst.getDebugLoc()->getDirectory());
              locs.append("/");
              locs.append(inst.getDebugLoc()->getFilename());
              locs.append(":");
            }
            int line = inst.getDebugLoc().getLine();
            locs.append(std::to_string(line));
            locs.append(";");

          }
        }

        errs() << locs << "\n";
        locs = "";
      }
      return false;
    }
  };
}

char BBMapPass::ID = 0;

static RegisterPass<BBMapPass> X("bbmap", "Map Basic Blocks to Filename and Line Number",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new BBMapPass()); });


// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
//static void registerBBMapPass(const PassManagerBuilder &, legacy::PassManagerBase &PM) {
//  PM.add(new BBMapPass());
//}
//static RegisterStandardPasses
//  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
//                 registerBBMapPass);
