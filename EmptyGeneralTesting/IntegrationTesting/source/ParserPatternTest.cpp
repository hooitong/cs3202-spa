#include <cppunit/config/SourcePrefix.h>

#include "ParserPatternTest.h"

#include <iostream>
void ParserPatternTest::setUp() {
	ast = (*PKB::getPKB()).getAst();
	vTable = (*PKB::getPKB()).getVarTable();
}

void ParserPatternTest::tearDown() {
	ast = nullptr;
}

CPPUNIT_TEST_SUITE_REGISTRATION(ParserPatternTest);

//test make sure all left pattern are working, for both control and assignments
void ParserPatternTest::testLeftPatternStrict() {
	VARINDEX i = vTable->getVarIndex("i");
	VARINDEX k = vTable->getVarIndex("k");
	VARINDEX b = vTable->getVarIndex("b");
	VARINDEX c = vTable->getVarIndex("c");
	VARINDEX a = vTable->getVarIndex("a");
	VARINDEX w = vTable->getVarIndex("w");
	VARINDEX oSCar = vTable->getVarIndex("oSCar");
	VARINDEX beta = vTable->getVarIndex("beta");
	VARINDEX tmp = vTable->getVarIndex("tmp");
	VARINDEX I = vTable->getVarIndex("I");
	VARINDEX j1k = vTable->getVarIndex("j1k");
	VARINDEX chArlie = vTable->getVarIndex("chArlie");
	VARINDEX x = vTable->getVarIndex("x");
	VARINDEX left = vTable->getVarIndex("left");
	VARINDEX right = vTable->getVarIndex("right");
	VARINDEX Romeo = vTable->getVarIndex("Romeo");
	VARINDEX width = vTable->getVarIndex("width");
	VARINDEX delta = vTable->getVarIndex("delta");
	VARINDEX l = vTable->getVarIndex("l");

	CPPUNIT_ASSERT(ast->matchLeftPattern(1,i));
	CPPUNIT_ASSERT(ast->matchLeftPattern(2,b));
	CPPUNIT_ASSERT(ast->matchLeftPattern(3,c));
	CPPUNIT_ASSERT(ast->matchLeftPattern(4,a));
	CPPUNIT_ASSERT(ast->matchLeftPattern(5,beta));
	CPPUNIT_ASSERT(ast->matchLeftPattern(6,oSCar));
	CPPUNIT_ASSERT(ast->matchLeftPattern(7,tmp));
	CPPUNIT_ASSERT(ast->matchLeftPattern(8,oSCar));
	CPPUNIT_ASSERT(ast->matchLeftPattern(9,x));
	CPPUNIT_ASSERT(ast->matchLeftPattern(10,x));
	CPPUNIT_ASSERT(ast->matchLeftPattern(11,left));
	CPPUNIT_ASSERT(ast->matchLeftPattern(12,right));
	CPPUNIT_ASSERT(ast->matchLeftPattern(13,Romeo));
	CPPUNIT_ASSERT(ast->matchLeftPattern(14,b));
	CPPUNIT_ASSERT(ast->matchLeftPattern(15,c));
	CPPUNIT_ASSERT(ast->matchLeftPattern(16,c));
	CPPUNIT_ASSERT(ast->matchLeftPattern(17,c));
	CPPUNIT_ASSERT(ast->matchLeftPattern(18,x));
	CPPUNIT_ASSERT(ast->matchLeftPattern(19,a));
	CPPUNIT_ASSERT(ast->matchLeftPattern(20,w));

}

//excluding lines with multiplication/division
void ParserPatternTest::testRightPatternStrict() { 
	CPPUNIT_ASSERT(ast->matchRightPattern(1,"1",true));
}

//test pattern that are not strict
void ParserPatternTest::testRightPatternUnStrict(){
	CPPUNIT_ASSERT(ast->matchRightPattern(1,"1",false));
	CPPUNIT_ASSERT(ast->matchRightPattern(2,"200",false));
	CPPUNIT_ASSERT(ast->matchRightPattern(3,"a",false));
}
