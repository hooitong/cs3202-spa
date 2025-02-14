#pragma once

#include "VarTable.h"
#include "AST.h"
#include "Follows.h"
#include "Parent.h"
#include "Modifies.h"
#include "Uses.h"
#include "ProcTable.h"
#include "Calls.h"
#include "CFG.h"
#include "Next.h"
#include "NextBip.h"
#include "ConstTable.h"
#include "CFGBip.h"
#include "Sibling.h"
#include "Contains.h"

class PKB {
private:
  static PKB* pkbInstance;

  VarTable* varTable;
  AST* ast;
  CFG* cfg;
  Follows* follows;
  Parent* parent;
  Modifies* modifies;
  Uses* uses;
  ProcTable* procTable;
  Calls* calls;
  Next* next;
  NextBip* nextBip;
  ConstTable* constTable;
  CFGBip* cfgBip;
  Sibling* sibling;
  Contains* contains;

public:
  /* Static method to retrieve singleton instance */
  static PKB* getPKB();
  static void deletePKB();
  vector<TNode*> getAllStmtLstNodes();

  /* Accessors methods to Design Abstractions in PKB */
  PKB();
  ~PKB();
  VarTable* getVarTable();
  AST* getAst();
  Follows* getFollows();
  Parent* getParent();
  Modifies* getModifies();
  Uses* getUses();
  ProcTable* getProcTable();
  Calls* getCalls();
  CFG* getCfg();
  Next* getNext();
  NextBip* getNextBip();
  ConstTable* getConstTable();
  CFGBip* getCfgBip();
  Sibling* getSibling();
  Contains* getContains();
};