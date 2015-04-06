#include <cppunit/config/SourcePrefix.h>

#include "ParserTest.h"
#include "PKB.h"
#include "Parser.h"
#include "GlobalType.h"
#include "Modifies.h"
#include <fstream>

void ParserTest::setUp()
{
	Parser::parse("sample_SIMPLE_source.txt");
}

void ParserTest::tearDown()
{

}

CPPUNIT_TEST_SUITE_REGISTRATION(ParserTest);

void ParserTest::testNodeType()
{
	// write result to a file for testing purpose
	ofstream output;
	output.open("result.txt");

	AST* ast = PKB::getPKB()->getAst();
	TNode* root = ast->getRoot();
	CPPUNIT_ASSERT(root->getTType() == TType::PROCEDUREN);
	CPPUNIT_ASSERT(root->getValue() == "ABC");

	TNode* stmt1Node = ast->getTNode(1);
	CPPUNIT_ASSERT(stmt1Node->getTType() == TType::ASSIGNN);

	TNode* stmt2Node = ast->getTNode(2);
	CPPUNIT_ASSERT(stmt2Node->getTType() == TType::ASSIGNN);

	TNode* stmt3Node = ast->getTNode(3);
	CPPUNIT_ASSERT(stmt3Node->getTType() == TType::ASSIGNN);

	TNode* stmt4Node = ast->getTNode(4);
	CPPUNIT_ASSERT(stmt4Node->getTType() == TType::WHILEN);
	TNode* stmt4NodeChild = stmt4Node->getChildren().at(0);
	VARINDEX index_a = PKB::getPKB()->getVarTable()->getVarIndex("a"); 
	CPPUNIT_ASSERT(stmt4NodeChild->getTType() == TType::VARN);
	CPPUNIT_ASSERT(stmt4NodeChild->getValue() == Parser::convertIntToString(index_a));

	TNode* stmt5Node = ast->getTNode(5);
	CPPUNIT_ASSERT(stmt5Node->getTType() == TType::WHILEN);
	TNode* stmt5NodeChild = stmt5Node->getChildren().at(0);
	VARINDEX index_beta = PKB::getPKB()->getVarTable()->getVarIndex("beta"); 
	CPPUNIT_ASSERT(stmt5NodeChild->getTType() == TType::VARN);
	CPPUNIT_ASSERT(stmt5NodeChild->getValue() == Parser::convertIntToString(index_beta));

	TNode* stmt6Node = ast->getTNode(6);
	CPPUNIT_ASSERT(stmt6Node->getTType() == TType::ASSIGNN);

	TNode* stmt7Node = ast->getTNode(7);
	CPPUNIT_ASSERT(stmt7Node->getTType() == TType::WHILEN);
	TNode* stmt7NodeChild = stmt7Node->getChildren().at(0);
	VARINDEX index_tmp = PKB::getPKB()->getVarTable()->getVarIndex("tmp"); 
	CPPUNIT_ASSERT(stmt7NodeChild->getTType() == TType::VARN);
	CPPUNIT_ASSERT(stmt7NodeChild->getValue() == Parser::convertIntToString(index_tmp));

	TNode* stmt8Node = ast->getTNode(8);
	CPPUNIT_ASSERT(stmt8Node->getTType() == TType::ASSIGNN);

	TNode* stmt9Node = ast->getTNode(9);
	CPPUNIT_ASSERT(stmt9Node->getTType() == TType::WHILEN);
	TNode* stmt9NodeChild = stmt9Node->getChildren().at(0);
	VARINDEX index_x = PKB::getPKB()->getVarTable()->getVarIndex("x"); 
	CPPUNIT_ASSERT(stmt9NodeChild->getTType() == TType::VARN);
	CPPUNIT_ASSERT(stmt9NodeChild->getValue() == Parser::convertIntToString(index_x));

	TNode* stmt10Node = ast->getTNode(10);
	CPPUNIT_ASSERT(stmt10Node->getTType() == TType::ASSIGNN);

	TNode* stmt11Node = ast->getTNode(11);
	CPPUNIT_ASSERT(stmt11Node->getTType() == TType::WHILEN);
	TNode* stmt11NodeChild = stmt11Node->getChildren().at(0);
	VARINDEX index_left = PKB::getPKB()->getVarTable()->getVarIndex("left"); 
	CPPUNIT_ASSERT(stmt11NodeChild->getTType() == TType::VARN);
	CPPUNIT_ASSERT(stmt11NodeChild->getValue() == Parser::convertIntToString(index_left));

	TNode* stmt12Node = ast->getTNode(12);
	CPPUNIT_ASSERT(stmt12Node->getTType() == TType::WHILEN);
	TNode* stmt12NodeChild = stmt12Node->getChildren().at(0);
	VARINDEX index_right = PKB::getPKB()->getVarTable()->getVarIndex("right"); 
	CPPUNIT_ASSERT(stmt12NodeChild->getTType() == TType::VARN);
	CPPUNIT_ASSERT(stmt12NodeChild->getValue() == Parser::convertIntToString(index_right));

	TNode* stmt13Node = ast->getTNode(13);
	CPPUNIT_ASSERT(stmt13Node->getTType() == TType::ASSIGNN);

	TNode* stmt14Node = ast->getTNode(14);
	CPPUNIT_ASSERT(stmt14Node->getTType() == TType::ASSIGNN);

	TNode* stmt15Node = ast->getTNode(15);
	CPPUNIT_ASSERT(stmt15Node->getTType() == TType::ASSIGNN);

	TNode* stmt16Node = ast->getTNode(16);
	CPPUNIT_ASSERT(stmt16Node->getTType() == TType::WHILEN);
	TNode* stmt16NodeChild = stmt16Node->getChildren().at(0);
	VARINDEX index_c = PKB::getPKB()->getVarTable()->getVarIndex("c"); 
	CPPUNIT_ASSERT(stmt16NodeChild->getTType() == TType::VARN);
	CPPUNIT_ASSERT(stmt16NodeChild->getValue() == Parser::convertIntToString(index_c));

	TNode* stmt17Node = ast->getTNode(17);
	CPPUNIT_ASSERT(stmt17Node->getTType() == TType::ASSIGNN);

	TNode* stmt18Node = ast->getTNode(18);
	CPPUNIT_ASSERT(stmt18Node->getTType() == TType::ASSIGNN);

	TNode* stmt19Node = ast->getTNode(19);
	CPPUNIT_ASSERT(stmt19Node->getTType() == TType::ASSIGNN);

	TNode* stmt20Node = ast->getTNode(20);
	CPPUNIT_ASSERT(stmt20Node->getTType() == TType::ASSIGNN);
}

void ParserTest::testRelationsOfNodes()
{
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(1,2));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(2,3));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(3,4));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(4, 5));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(5, 6));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(6,7));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(5, 7));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(7, 8));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(7,9));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(5, 9));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(9, 10));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(10, 11));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(9, 11));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(11, 12));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(12, 13));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(13, 14));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(12, 14));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(14, 15));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(12, 15));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(12, 16));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(11, 16));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(16, 17));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(16, 18));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(11, 18));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(9, 19));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(5, 19));
	CPPUNIT_ASSERT(PKB::getPKB()->getFollows()->isFollows(5, 20));
	CPPUNIT_ASSERT(PKB::getPKB()->getParent()->isParent(4, 20));
}

void ParserTest::testModifies()
{
	VARINDEX index_i = PKB::getPKB()->getVarTable()->getVarIndex("i");
	CPPUNIT_ASSERT(ParserTest::isModifies(1, index_i));

	VARINDEX index_b = PKB::getPKB()->getVarTable()->getVarIndex("b");
	CPPUNIT_ASSERT(ParserTest::isModifies(2, index_b));
	CPPUNIT_ASSERT(ParserTest::isModifies(14, index_b));

	VARINDEX index_c = PKB::getPKB()->getVarTable()->getVarIndex("c");
	CPPUNIT_ASSERT(ParserTest::isModifies(3, index_c));
	CPPUNIT_ASSERT(ParserTest::isModifies(15, index_c));
	CPPUNIT_ASSERT(ParserTest::isModifies(17, index_c));

	VARINDEX index_oSCar = PKB::getPKB()->getVarTable()->getVarIndex("oSCar");
	CPPUNIT_ASSERT(ParserTest::isModifies(6, index_oSCar));
	CPPUNIT_ASSERT(ParserTest::isModifies(8, index_oSCar));

	VARINDEX index_x = PKB::getPKB()->getVarTable()->getVarIndex("x");
	CPPUNIT_ASSERT(ParserTest::isModifies(10, index_x));
	CPPUNIT_ASSERT(ParserTest::isModifies(18, index_x));

	VARINDEX index_Romeo = PKB::getPKB()->getVarTable()->getVarIndex("Romeo");
	CPPUNIT_ASSERT(ParserTest::isModifies(13, index_Romeo));

	VARINDEX index_a = PKB::getPKB()->getVarTable()->getVarIndex("a");
	CPPUNIT_ASSERT(ParserTest::isModifies(19, index_a));

	VARINDEX index_w = PKB::getPKB()->getVarTable()->getVarIndex("w");
	CPPUNIT_ASSERT(ParserTest::isModifies(20, index_w));
}

void ParserTest::testUses()
{
	VARINDEX index_a = PKB::getPKB()->getVarTable()->getVarIndex("a");
	CPPUNIT_ASSERT(ParserTest::isUses(3, index_a));
	CPPUNIT_ASSERT(ParserTest::isUses(4, index_a));

	VARINDEX index_beta = PKB::getPKB()->getVarTable()->getVarIndex("beta");
	CPPUNIT_ASSERT(ParserTest::isUses(5, index_beta));
	CPPUNIT_ASSERT(ParserTest::isUses(6, index_beta));

	VARINDEX index_tmp = PKB::getPKB()->getVarTable()->getVarIndex("tmp");
	CPPUNIT_ASSERT(ParserTest::isUses(6, index_tmp));
	CPPUNIT_ASSERT(ParserTest::isUses(7, index_tmp));

	VARINDEX index_I = PKB::getPKB()->getVarTable()->getVarIndex("I");
	CPPUNIT_ASSERT(ParserTest::isUses(8, index_I));

	VARINDEX index_k = PKB::getPKB()->getVarTable()->getVarIndex("k");
	CPPUNIT_ASSERT(ParserTest::isUses(8, index_k));

	VARINDEX index_j1k = PKB::getPKB()->getVarTable()->getVarIndex("j1k");
	CPPUNIT_ASSERT(ParserTest::isUses(8, index_j1k));

	VARINDEX index_chArlie = PKB::getPKB()->getVarTable()->getVarIndex("chArlie");
	CPPUNIT_ASSERT(ParserTest::isUses(8, index_chArlie));

	VARINDEX index_x = PKB::getPKB()->getVarTable()->getVarIndex("x");
	CPPUNIT_ASSERT(ParserTest::isUses(9, index_x));
	CPPUNIT_ASSERT(ParserTest::isUses(10, index_x));
	CPPUNIT_ASSERT(ParserTest::isUses(18, index_x));

	VARINDEX index_left = PKB::getPKB()->getVarTable()->getVarIndex("left");
	CPPUNIT_ASSERT(ParserTest::isUses(11, index_left));

	VARINDEX index_right = PKB::getPKB()->getVarTable()->getVarIndex("right");
	CPPUNIT_ASSERT(ParserTest::isUses(12, index_right));

	VARINDEX index_Romeo = PKB::getPKB()->getVarTable()->getVarIndex("Romeo");
	CPPUNIT_ASSERT(ParserTest::isUses(13, index_Romeo));
	CPPUNIT_ASSERT(ParserTest::isUses(15, index_Romeo));

	VARINDEX index_delta = PKB::getPKB()->getVarTable()->getVarIndex("delta");
	CPPUNIT_ASSERT(ParserTest::isUses(15, index_delta));

	VARINDEX index_l = PKB::getPKB()->getVarTable()->getVarIndex("l");
	CPPUNIT_ASSERT(ParserTest::isUses(15, index_l));

	VARINDEX index_width = PKB::getPKB()->getVarTable()->getVarIndex("width");
	CPPUNIT_ASSERT(ParserTest::isUses(15, index_width));

	VARINDEX index_c = PKB::getPKB()->getVarTable()->getVarIndex("c");
	CPPUNIT_ASSERT(ParserTest::isUses(16, index_c));
	CPPUNIT_ASSERT(ParserTest::isUses(17, index_c));

	VARINDEX index_w = PKB::getPKB()->getVarTable()->getVarIndex("w");
	CPPUNIT_ASSERT(ParserTest::isUses(20, index_w));
}

bool ParserTest::isModifies(STMTLINE stmt, VARINDEX varIndex)
{
	vector<VARINDEX> varIndexList = PKB::getPKB()->getModifies()->getModifiedByStmt(stmt);
	for (int i=0; i<varIndexList.size(); i++) {
		if (varIndex == varIndexList.at(i)) 
			return true;
	}

	return false;
}

bool ParserTest::isUses(STMTLINE stmt, VARINDEX varIndex)
{
	vector<VARINDEX> varIndexList = PKB::getPKB()->getUses()->getUsedByStmt(stmt);
	for (int i=0; i<varIndexList.size(); i++) {
		if (varIndex == varIndexList.at(i)) 
			return true;
	}

	return false;
}