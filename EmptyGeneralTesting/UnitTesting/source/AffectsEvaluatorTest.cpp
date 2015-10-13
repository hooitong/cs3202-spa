﻿#include <cppunit/config/SourcePrefix.h>

#include "AffectsEvaluatorTest.h"
#include <DesignExtractor.h>
#include <Parser.h>

void AffectsEvaluatorTest::setUp() {

}

void AffectsEvaluatorTest::tearDown() {
  PKB::deletePKB();

}

CPPUNIT_TEST_SUITE_REGISTRATION(AffectsEvaluatorTest);

void AffectsEvaluatorTest::testSynSyn() {

}

void AffectsEvaluatorTest::testSynConst() {

}

void AffectsEvaluatorTest::testSynAny() {

}

void AffectsEvaluatorTest::testConstAny() {

}

void AffectsEvaluatorTest::testConstConst() {
  AffectsEvaluator eval(PKB::getPKB());
  Parser::parse("affects_sample.txt");
  TNode* root = Parser::buildAst();
  DesignExtractor::extract();

  QNode* node = createNode("Calls", CONST, "2", CONST, "6");
  QueryResult result = eval.evaluate(node);
  QueryResult expected(true);
  CPPUNIT_ASSERT(result == expected);

  QNode* node_two = createNode("Calls", CONST, "15", CONST, "19");
  QueryResult result_two = eval.evaluate(node_two);
  QueryResult expected_two(true);
  CPPUNIT_ASSERT(result_two == expected_two);

  QNode* node_three = createNode("Calls", CONST, "50", CONST, "59");
  QueryResult result_three = eval.evaluate(node_three);
  QueryResult expected_three(true);
  CPPUNIT_ASSERT(result_three == expected_three);

  /* Failed case as line 2 is modifying as well */
  QNode* node_four = createNode("Calls", CONST, "1", CONST, "6");
  QueryResult result_four = eval.evaluate(node_four);
  QueryResult expected_four(false);
  CPPUNIT_ASSERT(result_four == expected_four);
}

void AffectsEvaluatorTest::testConstSyn() {

}

void AffectsEvaluatorTest::testAnySyn() {

}

void AffectsEvaluatorTest::testAnyConst() {

}

void AffectsEvaluatorTest::testAnyAny() {

}

QNode* AffectsEvaluatorTest::createNode(string relationString, QNodeType type1, string s1, QNodeType type2, string s2) {
  QNode* relation = new QNode(RELATION, relationString);
  QNode* arg1 = new QNode(type1, s1);
  QNode* arg2 = new QNode(type2, s2);

  relation->addChild(arg1);
  relation->addChild(arg2);
  return relation;
}

void AffectsEvaluatorTest::cleanUp(QNode* node) {
  for (vector<QNode*>::size_type i = 0; i < node->getChildren().size(); i++) {
    cleanUp(node->getChildren()[i]);
  }
  delete node;
}

