#include "QueryPreprocessorCondition.h"
#include "QueryPreprocessor.h"
#include <string>
#include <iostream>
using namespace std;

QueryPreprocessorCondition::QueryPreprocessorCondition(QueryPreprocessorDeclaration* declaration) {
	this->declaration = declaration;
	relation_table = new QueryRelationTable();
	is_valid = true;
}

QNode* QueryPreprocessorCondition::getConditionTree(string condition_string) {
	condition_root = new QNode(CONDITIONLIST, "");
	if (condition_string == "") {
		return condition_root;
	}
	int current_position = 0;
	bool is_end_of_condition = false;

	while (!is_end_of_condition) {
		int next_such_that_position = condition_string.find("such that", current_position + 1);
		int next_with_position = condition_string.find("with", current_position + 1);
		int next_pattern_position = condition_string.find("pattern", current_position + 1);
		int next_min_position = condition_string.length();
		if (next_such_that_position != string::npos) {
			next_min_position = min(next_min_position, next_such_that_position);
		}
		if (next_with_position != string::npos) {
			next_min_position = min(next_min_position, next_with_position);
		}
		if (next_pattern_position != string::npos) {
			next_min_position = min(next_min_position, next_pattern_position);
		}
		if (next_min_position == condition_string.length()) {
			is_end_of_condition = true;
		}
		string conditions_string = QueryPreprocessor::trim(condition_string.substr(current_position, next_min_position - current_position));
		processConditions(conditions_string);
		current_position = next_min_position;
	}
	return condition_root;
}

// parsing a list of conditions separated by "and" into one condition and call the appropriate method for each condition type
void QueryPreprocessorCondition::processConditions(string conditions_string) {
	string condition_type;
	if (conditions_string.find("such that") == 0) {
		condition_type = "such that";
	} else if (conditions_string.find("with") == 0) {
		condition_type = "with";
	} else if (conditions_string.find("pattern") == 0) {
		condition_type = "pattern";
	} else {
		is_valid = false;
		return;
	}
	int current_position = condition_type.length();
	bool is_end_of_such_that = false;
	while (!is_end_of_such_that) {
		int next_and_position = conditions_string.find("and", current_position);
		if (next_and_position == string::npos) {
			is_end_of_such_that = true;
			next_and_position = conditions_string.length();
		}
		string one_condition_string = QueryPreprocessor::trim(conditions_string.substr(current_position, next_and_position - current_position));
		if (condition_type == "such that") {
			processSuchThat(one_condition_string);
		} else if (condition_type == "with") {
			processWith(one_condition_string);
		} else if (condition_type == "pattern") {
			processPattern(one_condition_string);
		}
		current_position = next_and_position + ((string)"and").length();
	}
}

void QueryPreprocessorCondition::processSuchThat(string relation_string) {
	int open_bracket_position = relation_string.find("(");
	if (open_bracket_position == string::npos) {
		is_valid = false;
		return;
	}
	int comma_position = relation_string.find(",", open_bracket_position);
	if (comma_position == string::npos) {
		is_valid = false;
		return;
	}
	int closed_bracket_position = relation_string.find(")", comma_position);
	if (closed_bracket_position == string::npos || closed_bracket_position != relation_string.length() - 1) {
		is_valid = false;
		return;
	}
	
	string relation_name = QueryPreprocessor::trim(relation_string.substr(0, open_bracket_position));
	string relation_left_hand = QueryPreprocessor::trim(relation_string.substr(open_bracket_position + 1, comma_position - (open_bracket_position + 1)));
	string relation_right_hand = QueryPreprocessor::trim(relation_string.substr(comma_position + 1, closed_bracket_position - (comma_position + 1)));
	QNode* left_node = parseRef(relation_left_hand);
	QNode* right_node = parseRef(relation_right_hand);
	if (left_node == NULL || right_node == NULL) {
		is_valid = false;
		return;
	}
	if (relation_table->isValidRule(relation_name, left_node->getQType(), right_node->getQType())) {
		QNode* relation_node = new QNode(RELATION, relation_name);
		condition_root->addChild(relation_node);
		relation_node->addChild(left_node);
		relation_node->addChild(right_node);
	} else {
		is_valid = false;
	}
}

void QueryPreprocessorCondition::processWith(string with_string) {
	// TODO (Sherlin) : Integrate the checking with the table, as well as create the tree
}

void QueryPreprocessorCondition::processPattern(string pattern_string) {
	int open_bracket_position = pattern_string.find("(");
	if (open_bracket_position == string::npos) {
		is_valid = false;
		return;
	}
	int comma_position = pattern_string.find(",", open_bracket_position);
	if (comma_position == string::npos) {
		is_valid = false;
		return;
	}
	
	string pattern_synonym = QueryPreprocessor::trim(pattern_string.substr(0, open_bracket_position));
	string pattern_left_hand = QueryPreprocessor::trim(pattern_string.substr(open_bracket_position + 1, comma_position - (open_bracket_position + 1)));

	if (!declaration->isDeclaredSynonym(pattern_synonym)) {
		is_valid = false;
		return;
	}

	if (declaration->getSynonymType(pattern_synonym) == "if") {
		// TODO : Support the pattern if the synonym is IF synonym
	} else if (declaration->getSynonymType(pattern_synonym) == "while") {
		// TODO : Support the pattern if the synonym is ASSIGN synonym
	} else if (declaration->getSynonymType(pattern_synonym) == "assign") {
		int closed_bracket_position = pattern_string.find(")", comma_position);
		if (closed_bracket_position == string::npos || closed_bracket_position != pattern_string.length() - 1) {
			is_valid = false;
			return;
		}
		string pattern_right_hand = QueryPreprocessor::trim(pattern_string.substr(comma_position + 1, closed_bracket_position - (comma_position + 1)));
		if (!isValidExpression(pattern_right_hand)) {
			is_valid = false;
			return;
		}
		string pattern_right_hand_remove_quote = removeExpressionQuote(pattern_right_hand);
		QNode* pattern_node = new QNode(PATTERN, "");
		QNode* assign_synonym_node = declaration->getSynonymTypeNode(pattern_synonym);
		QNode* left_pattern_node = parseRef(pattern_left_hand);
		QNode* right_pattern_node = new QNode(EXPRESSION, pattern_right_hand_remove_quote);

		if (left_pattern_node->getQType() == VAR || left_pattern_node->getQType() == VARIABLESYNONYM
		    || left_pattern_node->getQType() == ANY) {
			pattern_node->addChild(assign_synonym_node);
			pattern_node->addChild(left_pattern_node);
			pattern_node->addChild(right_pattern_node);
			condition_root->addChild(pattern_node);
		} else {
			is_valid = false;
			return;
		}
	}
}

bool QueryPreprocessorCondition::isValidExpression(string expression) {
	if (expression == "_") {
		return true;
	}
	if (expression.length() < 2) {
		return false;
	}
	if (expression.at(0) == '_' && expression.at(expression.length() - 1) == '_') {
		if (expression.at(1) != '"' || expression.at(expression.length() - 2) != '"') {
			return false;
		}
		for (int i = 2; i < expression.length() - 3; ++i) {
			if (expression.at(i) == '"' || expression.at(i) == '_') {
				return false;
			}
		}
		return true;
	}
	if (expression.at(0) == '"' && expression.at(expression.length() - 1) == '"') {
		for (int i  = 1; i < expression.length() - 1; ++i) {
			if (expression.at(i) == '"' || expression.at(i) == '_') {
				return false;
			}
		}
		return true;
	}
	return false;
}

string QueryPreprocessorCondition::removeExpressionQuote(string expression) {
	string expression_remove_quote = "";
	for (int i = 0; i < expression.length(); ++i) {
		if (expression.at(i) != '"') {
			expression_remove_quote += expression.at(i);
		}
	}
	return expression_remove_quote;
}

QNode* QueryPreprocessorCondition::parseRef(string argument) {
    if (argument == "_") {
        return new QNode(ANY,"");
    } else if (isdigit(argument.at(0))) {
        return new QNode(CONST,argument);
    } else if (argument.at(0) == '"' && argument.at(argument.size() - 1) == '"') {
		return new QNode(VAR,QueryPreprocessor::trim(argument.substr(1, argument.size()-2)));
	} else if (declaration->isDeclaredSynonym(argument)) {
		return declaration->getSynonymTypeNode(argument);
	}
    return NULL;
}

bool QueryPreprocessorCondition::isValidCondition(void) {
	return this->is_valid;
}
