#include "QueryEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"
#include "ModifiesEvaluator.h"
#include "CallsEvaluator.h"
#include "CallsStarEvaluator.h"
#include "UsesEvaluator.h"
#include "WithEvaluator.h"
#include "NextEvaluator.h"
#include "NextStarEvaluator.h"
#include "UsesProcEvaluator.h"
#include "ModifiesProcEvaluator.h"
#include "AffectsEvaluator.h"
#include "AffectsStarEvaluator.h"
#include "AffectsBipEvaluator.h"
#include "AffectsBipStarEvaluator.h"
#include "NextBipEvaluator.h"
#include "NextBipStarEvaluator.h"
#include "ContainsEvaluator.h"
#include "ContainsStarEvaluator.h"
#include "SiblingEvaluator.h"
#include <iostream>

QueryEvaluator::QueryEvaluator(PKB* pkb) {
    pkbInstance = pkb;
}

std::list<string> QueryEvaluator::evaluate(QueryTree* tree) {
    if (tree == NULL) {
        cout << "NULL" << endl;
        return std::list<string>();
    }
    QueryResult result = evaluate(tree->getRoot());

    vector <string> resultSynonym;
    map <string, TType> synonymMap;
    vector<QueryResult> resultFilters;

    vector <QNode*> children = tree->getRoot()->getChildren();
    bool isBoolean = false;


    for (int i = 0; i < (int) children.size(); i++) {
        if (children[i]->getQType() == RESULTLIST) {
            if (children[i]->getChildren().size() == 1 && children[i]->getChildren()[0]->getQType() == BOOLEAN) {
                isBoolean = true;
            } else {
                resultSynonym = getResultSynonyms(children[i]);
                synonymMap = getSynonymMap(children[i]);
                resultFilters = getResultFilters(children[i], result);
            }
        }
    }

    vector <string> existingSynonym;
    for (vector<string>::iterator it = resultSynonym.begin(); it != resultSynonym.end(); it++) {
        if (result.getIndex(*it) != -1) {
            existingSynonym.push_back(*it);
        }
    }

    if (!isBoolean) {
        result = result.filter(existingSynonym);

        for (int i = 0; i < (int) resultFilters.size(); i++) {
            result = result.merge(resultFilters[i]);
        }
        result = result.filter(resultSynonym);
    }

    int solutionsSize = result.getSolutionsSize();

    std::list<string> resultList;
    if (isBoolean) {
        if (solutionsSize > 0) {
            resultList.push_back("TRUE");
        } else {
            resultList.push_back("FALSE");
        }

    } else {
        for (int i = 0; i < solutionsSize; i++) {
            ostringstream oss;
            for (int j = 0; j < (int) resultSynonym.size(); j++) {
                if (j > 0) {
                    oss << " ";
                }
                if (synonymMap[resultSynonym[j]] == VARN) {
                    oss << pkbInstance->getVarTable()->getVarName(result.getSolutionForSynonym(i, resultSynonym[j]));
                } else if (synonymMap[resultSynonym[j]] == PROCEDUREN) {
                    oss << pkbInstance->getProcTable()->getProcName(result.getSolutionForSynonym(i, resultSynonym[j]));
                } else {
                    oss << result.getSolutionForSynonym(i, resultSynonym[j]);
                }
            }
            resultList.push_back(oss.str());
        }
    }

    return resultList;
}

vector<QueryResult> QueryEvaluator::getResultFilters(QNode* node, QueryResult& result) {
    assert(node->getQType() == RESULTLIST);

    vector <QNode*> children = node->getChildren();
    vector <QueryResult> resultList;
    for (int i = 0; i < (int) children.size(); i++) {
        if (result.getIndex(children[i]->getString()) != -1) {
            continue;
        }
        vector <int> result;
        if (children[i]->getQType() == WHILESYNONYM) {
            result = pkbInstance->getAst()->getStmtLines(WHILEN);
        } else if (children[i]->getQType() == STMTSYNONYM) {
            result = pkbInstance->getAst()->getStmtLines(STMTN);
        } else if (children[i]->getQType() == ASSIGNSYNONYM) {
            result = pkbInstance->getAst()->getStmtLines(ASSIGNN);
        } else if (children[i]->getQType() == PROCEDURESYNONYM) {
            result = pkbInstance->getProcTable()->getAllProcIndex();
        } else if (children[i]->getQType() == VARIABLESYNONYM) {
            result = pkbInstance->getVarTable()->getAllVarIndex();
        } else if (children[i]->getQType() == PROGLINESYNONYM) {
            result = pkbInstance->getAst()->getStmtLines(STMTN);
        } else if (children[i]->getQType() == CALLSYNONYM) {
            result = pkbInstance->getAst()->getStmtLines(CALLN);
        } else if (children[i]->getQType() == IFSYNONYM) {
            result = pkbInstance->getAst()->getStmtLines(IFN);
        } else if (children[i]->getQType() == CONSTSYNONYM) {
            result = pkbInstance->getConstTable()->getAllConstValue();
        } else if (children[i]->getQType() == STMTLSTSYNONYM) {
            vector <TNode*> nodes = pkbInstance->getAllStmtLstNodes();
            for (vector<TNode*>::iterator it = nodes.begin(); it != nodes.end(); it++) {
                result.push_back((*it)->getChildren()[0]->getStmtLine());
            }
        }
        vector<string> synonyms;
        resultList.push_back(QueryResult(result, children[i]->getString()));
    }
    return resultList;
}

vector<STMTLINE> QueryEvaluator::filter(vector<STMTLINE> original, TType type) {
    if (type == STMTN) {
        return original;
    }
    vector <STMTLINE> result;
    for (vector<STMTLINE>::iterator it = original.begin();
            it != original.end(); it++) {
        TType lineType = pkbInstance->getAst()->getTNode(*it)->getTType();
        if (lineType == type) {
            result.push_back(*it);
        }
    }
    return result;
}

vector<string> QueryEvaluator::getResultSynonyms(QNode* node) {
    assert(node->getQType() == RESULTLIST);
    vector <string> result;
    vector<QNode*> children = node->getChildren();
    for (int i = 0; i < (int) children.size(); i++) {
        result.push_back(children[i]->getString());
    }
    return result;
}

map<string, TType> QueryEvaluator::getSynonymMap(QNode* node) {
    assert(node->getQType() == RESULTLIST);
    map <string, TType> synonymMap;
    vector<QNode*> children = node->getChildren();
    for (int i = 0; i < (int) children.size(); i++) {
        synonymMap[children[i]->getString()] =
            synonymToTType(children[i]->getQType());
    }

    return synonymMap;
}

QueryResult QueryEvaluator::evaluate(QNode* node) {
    if (node->getQType() == CONDITIONLIST ||
            node->getQType() == QUERY) {
        vector <string> emptySynonym;
        QueryResult result = QueryResult(true);

        vector<QNode*> children = node->getChildren();
        for (int i = 0; i < (int)children.size(); i++) {
            if (children[i]->getQType() == CONDITIONLIST ||
                    children[i]->getQType() == RELATION ||
                    children[i]->getQType() == PATTERNASSIGN ||
                    children[i]->getQType() == PATTERNIF ||
                    children[i]->getQType() == PATTERNWHILE ||
                    children[i]->getQType() == WITH) {
                QueryResult tmp = evaluate(children[i]);
                result = result.merge(tmp);
            }
        }


        return result;
    } else if (node->getQType() == RELATION) {
        QueryResult result = solveRelation(node);
        return result;
    } else if (node->getQType() == PATTERNASSIGN) {
        return solvePattern(node);
    } else if (node->getQType() == PATTERNIF) {
        return solvePatternIf(node);
    } else if (node->getQType() == PATTERNWHILE) {
        return solvePatternWhile(node);
    } else if (node->getQType() == WITH) {
        return solveWith(node);
    }
}

QueryResult QueryEvaluator::solveRelation(QNode* node) {
    assert(node->getQType() == RELATION);
    if (node->getString() == "Follows") {
        return solveFollows(node);
    } else if (node->getString() == "Follows*") {
        return solveFollowsStar(node);
    } else if (node->getString() == "Parent") {
        return solveParent(node);
    } else if (node->getString() == "Parent*") {
        return solveParentStar(node);
    } else if (node->getString() == "Modifies") {
        if (node->getChildren()[0]->getQType() == VAR ||
            node->getChildren()[0]->getQType() == PROCEDURESYNONYM) {
            return solveModifiesProc(node);
        } else {
            return solveModifies(node);
        }
    } else if (node->getString() == "Uses") {
        if (node->getChildren()[0]->getQType() == VAR ||
            node->getChildren()[0]->getQType() == PROCEDURESYNONYM) {
            return solveUsesProc(node);
        } else {
            return solveUses(node);
        }
    } else if (node->getString() == "Calls") {
        return solveCalls(node);
    } else if (node->getString() == "Calls*") {
        return solveCallsStar(node);
    } else if (node->getString() == "Next") {
        return solveNext(node);
    } else if (node->getString() == "Next*") {
        return solveNextStar(node);
    } else if (node->getString() == "Affects") {
        return solveAffects(node);
    } else if (node->getString() == "Affects*") {
        return solveAffectsStar(node);
    } else if (node->getString() == "AffectsBip") {
      return solveAffectsBip(node);
    } else if (node->getString() == "AffectsBip*") {
      return solveAffectsBipStar(node);
    } else if (node->getString() == "NextBip") {
      return solveNextBip(node);
    } else if (node->getString() == "NextBip*") {
      return solveNextBipStar(node);
    } else if (node->getString() == "Contains") {
      return solveContains(node);
    } else if (node->getString() == "Contains*") {
      return solveContainsStar(node);
    } else if (node->getString() == "Sibling") {
      return solveSibling(node);
    } else {
        return QueryResult(false);
    }
}


QueryResult QueryEvaluator::solveFollows(QNode* node) {
    FollowsEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveFollowsStar(QNode* node) {
    FollowsStarEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveParent(QNode* node) {
    ParentEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveParentStar(QNode* node) {
    ParentStarEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveModifies(QNode* node) {
    assert(node->getQType() == RELATION && node->getString() == "Modifies");
    ModifiesEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveUses(QNode* node) {
    assert(node->getQType() == RELATION && node->getString() == "Uses");
    UsesEvaluator eval(pkbInstance);
    QueryResult result = eval.evaluate(node);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveCalls(QNode* node) {
    assert(node->getQType() == RELATION && node->getString() == "Calls");
    CallsEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveCallsStar(QNode* node) {
    CallsStarEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveNext(QNode* node) {
    NextEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveNextStar(QNode* node) {
    NextStarEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveModifiesProc(QNode* node) {
    ModifiesProcEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveUsesProc(QNode* node) {
    UsesProcEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveAffects(QNode* node) {
  AffectsEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveAffectsStar(QNode* node) {
  AffectsStarEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveAffectsBip(QNode* node) {
  AffectsBipEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveContainsStar(QNode* node) {
  ContainsStarEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveAffectsBipStar(QNode* node) {
  AffectsBipStarEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveNextBip(QNode* node) {
  NextBipEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveContains(QNode* node) {
  ContainsEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveNextBipStar(QNode* node) {
  NextBipStarEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solveSibling(QNode* node) {
  SiblingEvaluator eval(pkbInstance);
  return eval.evaluate(node);
}

QueryResult QueryEvaluator::solvePatternIf(QNode* node) {
    QNode* ifNode = node->getChildren()[0];
    QNode* varNode = node->getChildren()[1];
    QNode* thenNode = node->getChildren()[2];
    QNode* elseNode = node->getChildren()[3];
    vector <int> stmtLines = pkbInstance->getAst()->getStmtLines(IFN);
    vector <string> synonyms;
    synonyms.push_back(ifNode->getString());
    if (varNode->getQType() == VARIABLESYNONYM) {
        synonyms.push_back(varNode->getString());
    }
    if (thenNode->getQType() == STMTLSTSYNONYM) {
        synonyms.push_back(thenNode->getString());
    }
    if (elseNode->getQType() == STMTLSTSYNONYM) {
        synonyms.push_back(elseNode->getString());
    }
    QueryResult result(synonyms);
    for (vector<int>::iterator it = stmtLines.begin(); it != stmtLines.end(); it++) {
        int stmt = *it;
        TNode* stmtTNode = pkbInstance->getAst()->getTNode(stmt);
        TNode* varTNode = stmtTNode->getChildren()[0];
        TNode* thenStmtTNode = stmtTNode->getChildren()[1]->getChildren()[0];
        TNode* elseStmtTNode = stmtTNode->getChildren()[2]->getChildren()[0];
        if (varNode->getQType() != ANY &&
            varNode->getQType() != VARIABLESYNONYM && 
            varNode->getString() != varTNode->getValue()) {
            continue;
        }
        vector <int> thisResult;
        thisResult.push_back(stmtTNode->getStmtLine());
        if (varNode->getQType() == VARIABLESYNONYM) {
            thisResult.push_back(pkbInstance->getVarTable()->getVarIndex(varTNode->getValue()));
        }
        if (thenNode->getQType() == STMTLSTSYNONYM) {
            thisResult.push_back(thenStmtTNode->getStmtLine());
        }
        if (elseNode->getQType() == STMTLSTSYNONYM) {
            thisResult.push_back(elseStmtTNode->getStmtLine());
        }
        result.addSolution(thisResult);
    }
    return result;
}

QueryResult QueryEvaluator::solvePatternWhile(QNode* node) {
    QNode* whileNode = node->getChildren()[0];
    QNode* varNode = node->getChildren()[1];
    QNode* bodyNode = node->getChildren()[2];
    vector <int> stmtLines = pkbInstance->getAst()->getStmtLines(WHILEN);
    vector <string> synonyms;
    synonyms.push_back(whileNode->getString());
    if (varNode->getQType() == VARIABLESYNONYM) {
        synonyms.push_back(varNode->getString());
    }
    if (bodyNode->getQType() == STMTLSTSYNONYM) {
        synonyms.push_back(bodyNode->getString());
    }
    QueryResult result(synonyms);
    for (vector<int>::iterator it = stmtLines.begin(); it != stmtLines.end(); it++) {
        int stmt = *it;
        TNode* stmtTNode = pkbInstance->getAst()->getTNode(stmt);
        TNode* varTNode = stmtTNode->getChildren()[0];
        TNode* bodyStmtTNode = stmtTNode->getChildren()[1]->getChildren()[0];
        if (varNode->getQType() != ANY &&
            varNode->getQType() != VARIABLESYNONYM && 
            varNode->getString() != varTNode->getValue()) {
            continue;
        }
        vector <int> thisResult;
        thisResult.push_back(stmtTNode->getStmtLine());
        if (varNode->getQType() == VARIABLESYNONYM) {
            thisResult.push_back(pkbInstance->getVarTable()->getVarIndex(varTNode->getValue()));
        }
        if (bodyNode->getQType() == STMTLSTSYNONYM) {
            thisResult.push_back(bodyStmtTNode->getStmtLine());
        }
        result.addSolution(thisResult);
    }
    return result;
}

QueryResult QueryEvaluator::solvePattern(QNode* node) {
    assert(node->getQType() == PATTERNASSIGN);
    QNode* assignNode = node->getChildren()[0];
    QNode* varNode = node->getChildren()[1];
    string expression = node->getChildren()[2]->getString();
    if (varNode->getQType() == ANY) {
        vector <string> vars = pkbInstance->getVarTable()->getAllVarName();
        vector <QueryResult> results;
        for (int i = 0; i < (int) vars.size(); i++) {
            QNode* node = new QNode(PATTERNASSIGN, "");
            QNode* newAssignNode = new QNode(assignNode->getQType(), assignNode->getString());
            QNode* newVarNode = new QNode(CONST, vars[i]);
            QNode* newExpressionNode = new QNode(CONST, expression);
            node->addChild(newAssignNode);
            node->addChild(newVarNode);
            node->addChild(newExpressionNode);
            results.push_back(solvePattern(node));
            delete newAssignNode;
            delete newVarNode;
            delete newExpressionNode;
            delete node;
        }
        for (int j = 1; j < (int) results.size(); j++) {
            results[0].append(results[j]);
        }
        return results[0];
    }
    bool strict = true;
    bool any = false;
    if (expression[0] == '_' && (int) expression.length() > 1) {
        //non-strict
        expression = expression.substr(1, expression.length() - 2);
        strict = false;
        cout << expression << endl;
    } else if (expression[0] == '_') {
        any = true;
    }

    if (isSynonym(varNode->getQType())) {
        vector<pair<int,int> > resultPairs;
        vector<STMTLINE> assignments = pkbInstance->getAst()->getStmtLines(ASSIGNN);
        for (int i = 0; i < (int) assignments.size(); i++) {
            vector<VARINDEX> variables = pkbInstance->getVarTable()->getAllVarIndex();
            for (int j = 0; j < (int) variables.size(); j++) {
                cout << j << endl;
                cout << assignments[i] << " " << variables[j] << endl;
                if (pkbInstance->getAst()->matchLeftPattern(assignments[i], variables[j]) &&
                        (any || pkbInstance->getAst()->matchRightPattern(assignments[i], expression, strict))) {
                    resultPairs.push_back(make_pair(assignments[i], variables[j]));
                }
                cout << j << endl;
            }
        }
        return QueryResult(resultPairs, assignNode->getString(), varNode->getString());
    } else {
        vector<int> resultList;
        vector<STMTLINE> assignments = pkbInstance->getAst()->getStmtLines(ASSIGNN);
        VARINDEX variable = pkbInstance->getVarTable()->getVarIndex(varNode->getString());
        for (int i = 0; i < (int) assignments.size(); i++) {
            if (pkbInstance->getAst()->matchLeftPattern(assignments[i], variable) &&
                    (any || pkbInstance->getAst()->matchRightPattern(assignments[i], expression, strict))) {
                resultList.push_back(assignments[i]);
            }
        }
        return QueryResult(resultList, assignNode->getString());
    }
}


int QueryEvaluator::getInteger(QNode* node) {
    istringstream iss(node->getString());
    int result;
    iss >> result;
    return result;
}

TType QueryEvaluator::synonymToTType(QNodeType type) {
    if (type == WHILESYNONYM) {
        return WHILEN;
    } else if (type == ASSIGNSYNONYM) {
        return ASSIGNN;
    } else if (type == VARIABLESYNONYM) {
        return VARN;
    } else if (type == PROCEDURESYNONYM) {
        return PROCEDUREN;
    } else if (type == STMTSYNONYM) {
        return STMTN;
    } else if (type == PROGLINESYNONYM) {
		return STMTN;
	} else if (type == CONSTSYNONYM) {
        return CONSTN;
    }
}

bool QueryEvaluator::isSynonym(QNodeType type) {
    return type == WHILESYNONYM ||
           type == ASSIGNSYNONYM ||
           type == VARIABLESYNONYM ||
           type == PROCEDURESYNONYM ||
           type == CONSTSYNONYM ||
           type == PROGLINESYNONYM ||
           type == IFSYNONYM ||
           type == STMTSYNONYM;
}

QueryResult QueryEvaluator::solveWith(QNode* node) {
    WithEvaluator eval(pkbInstance);
    return eval.evaluate(node);
}