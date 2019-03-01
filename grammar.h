#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

typedef struct {
	int lhs;
	vector<int> rhs;
} rule;

typedef struct {
	string name;
	bool is_terminal;
	bool is_reachable, is_generating;
} symbol;

class Grammar {
	private:
		map<string, int> symbol_map;
		vector<symbol> symbols;
		vector<rule> rules, firsts, rule_firsts, follows;

		// T1
		void print_nts(); void print_ts();

		// T2
		void set_generating(); 
		void set_reachable();
		bool find_generating(rule r); 
		bool find_reachable(rule r);
		void print_useful();
		// T3
		bool find_union(vector<int> *vec, vector<int> *add);
		void sort(vector<rule> *sets);
		bool find_first(rule r, int pizza);
		void find_firsts(); 
		void print_firsts();
		// T4
		void follows_init();
		bool find_follows(rule r); 
		void follow_firstpass();
		void print_follows();
		// T5
		string pparser_rules();

	public:
		int add_symbol(string name);
		void add_rule(rule r);

		void init();
		void task1();
		void task2();
		void task3();
		void task4();
		void task5();
};