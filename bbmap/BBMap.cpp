#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <llvm/IR/DebugLoc.h>
#include <llvm/IR/DebugInfoMetadata.h>


using namespace llvm;

namespace {
  struct BBMapPass : public FunctionPass {
    static char ID;
    BBMapPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      for (auto& bb: F) {
  Instruction* begin = bb.getFirstNonPHI();
	Instruction* end = bb.getTerminator();
  const DebugLoc &debugInfo_begin = begin->getDebugLoc();
	const DebugLoc &debugInfo_end = end->getDebugLoc();
	if (debugInfo_begin && debugInfo_end){
	std::string directory = debugInfo_begin->getDirectory();
	std::string filePath = debugInfo_begin->getFilename();
	int line_begin = debugInfo_begin->getLine();
  int line_end = debugInfo_end->getLine();
	//errs() << "\n LAST: "<<*current<< directory << " " << filePath << " " << line  << "\n";
  errs() << directory << "/" << filePath << ":" << line_begin << "-" << line_end << "\n";
	}
      }
      //errs() << "I saw a function called " << F.getName() << "!\n";
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
