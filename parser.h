#pragma once
#include "lexer.h"
#include "grammar.h"
#include <vector>

using namespace std;

class Parser {
	private:
		LexicalAnalyzer lexer;
		Grammar g;
		Token peek();
		void parse_rule();
		void parse_rules();
		vector<int> parse_ids();
		vector<int> parse_rhs();
	public:
		Parser();
		Grammar get_g();
};