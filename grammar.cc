#include "grammar.h"


void Grammar::init() {
	for (int i=0; i<rules.size(); i++) {
		symbols[rules[i].lhs].is_terminal = false;
		symbols[rules[i].lhs].is_generating = false;
		symbols[rules[i].lhs].is_reachable = false;
	}
	symbols[rules[0].lhs].is_reachable = true;
}

int Grammar::add_symbol(string name) {
	if (symbol_map.insert(make_pair(name,symbols.size())).second) {
		symbol s;
		s.name = name;
		s.is_terminal = true;
		s.is_generating = true;
		symbols.push_back(s);
	}
	return symbol_map[name];
}

void Grammar::add_rule(rule r) {
	rules.push_back(r);
}

/* ###### 
Task 1 
###### */
void Grammar::task1() {
	print_nts(); print_ts();
}

void Grammar::print_ts() {
	for (int i=1; i<symbols.size(); i++) {
		if (symbols[i].is_terminal) {
			cout << symbols[i].name << " ";
		}
	}
}

void Grammar::print_nts() {
	for (int i=1; i<symbols.size(); i++) {
		if (!symbols[i].is_terminal) {
			cout << symbols[i].name << " ";
		}
	}
}

/* ###### 
Task 2 
###### */
void Grammar::task2() {
	set_generating(); set_reachable(); 
	print_useful();
}

void Grammar::set_generating() {
	bool flag = true;
	while (flag) {
		flag = false;
		for (int i = 0; i < rules.size(); i++) {
			if(!symbols[rules[i].lhs].is_generating){
				if (find_generating(rules[i])) {flag = true;}
			}
		}
	}
}

bool Grammar::find_generating(rule r) {
	bool changed = true;
	bool flag = false;
	for (int i = 0; i < r.rhs.size(); i++) {
		if(!symbols[r.rhs[i]].is_generating) {changed = false;}
	}
	flag = !symbols[r.lhs].is_generating && changed;
	if (changed) {symbols[r.lhs].is_generating = true;}
	return flag;
}

void Grammar::set_reachable() {
	symbols[rules[0].lhs].is_reachable = true;

	bool flag = true;
	while (flag) {
		flag = false;
		for (int i = 0; i < rules.size(); i++) {
			if (symbols[rules[i].lhs].is_reachable 
			&& symbols[rules[i].lhs].is_generating) {
				if(find_reachable(rules[i])) {
					flag = true;
				}
			}
		}
	}
}

bool Grammar::find_reachable(rule r) {
	bool changed = false;
	bool is_generating=true;
	for (int i = 0; i < r.rhs.size(); i++) {
		if(!symbols[r.rhs[i]].is_generating) {is_generating = false;}
	}
	if(is_generating){
		for (int i = 0; i < r.rhs.size(); i++) {
			if(!symbols[r.rhs[i]].is_reachable) {
				symbols[r.rhs[i]].is_reachable = true;
				changed =true;
			}
		}
	}

	return changed;
}

void Grammar::print_useful() {
	for (int i=0; i<rules.size(); i++) {
		// lhs is both generating and reachable
		if (symbols[rules[i].lhs].is_generating && symbols[rules[i].lhs].is_reachable) {
			bool useful = true;
			for (int j=0; j<rules[i].rhs.size(); j++) {
				// rhs is both generating and reachable
				if (!symbols[rules[i].rhs[j]].is_generating || !symbols[rules[i].rhs[j]].is_reachable) {useful = false;}
			}
			// determined useful
			if (useful) {
				cout << symbols[rules[i].lhs].name << " ->";
				for (int j=0; j<rules[i].rhs.size(); j++) {cout << " " << symbols[rules[i].rhs[j]].name;}
				cout << "\n";
			}
		}
	}
}

/* ###### 
Task 3 
###### */
void Grammar::task3() {
	find_firsts(); 
	print_firsts();
}

void Grammar::sort(vector<rule> *sets) {
	for (int i=0; i<sets->size(); i++) {
		::sort(sets->at(i).rhs.begin(), sets->at(i).rhs.end());
	}
}

bool Grammar::find_union(vector<int> *vec, vector<int> *add) {
	bool changed = false;
	bool contains = false;
	
	for (int i=0; i<add->size(); i++) {
		contains = false;
		for (int j=0; j<vec->size(); j++) {
			if(add->at(i)==vec->at(j)) {contains=true;}
		}
		if(!contains && add->at(i)!=0) {
			vec->push_back(add->at(i));
			changed = true;
		}
	}
	return changed;
}

void Grammar::find_firsts() {
	// setup firsts vector
	for (int i=0; i<symbols.size(); i++) {
		rule r; r.lhs = i;
		if (symbols[i].is_terminal) {r.rhs.push_back(i);}
		firsts.push_back(r);
	}

	// used for task 5
	for (int i=0; i<rules.size(); i++) {
		rule r;
		r.lhs = i;
		rule_firsts.push_back(r);
	}

	bool changed = true;
	while (changed) {
		changed=false;
		for (int i = 0; i< rules.size(); i++) {
			if(find_first(rules[i],i)){changed=true;}
		}
	}
}

bool Grammar::find_first(rule r, int r_idx) {
	bool changed = false, flag = true;
	for (int i=0; i<r.rhs.size() && flag; i++) {
		if(find_union(&firsts[r.lhs].rhs, &firsts[r.rhs[i]].rhs)) {changed = true;}
		find_union(&rule_firsts[r_idx].rhs, &firsts[r.rhs[i]].rhs);
		flag = find(firsts[r.rhs[i]].rhs.begin(), firsts[r.rhs[i]].rhs.end(), 0) != firsts[r.rhs[i]].rhs.end();
	}
	if (flag) {
		if (!(find(firsts[r.lhs].rhs.begin(), firsts[r.lhs].rhs.end(), 0) != firsts[r.lhs].rhs.end())) {
			firsts[r.lhs].rhs.push_back(0);
			changed = true;
		} 
		if (!(find(rule_firsts[r_idx].rhs.begin(), rule_firsts[r_idx].rhs.end(), 0) != rule_firsts[r_idx].rhs.end())) {
			rule_firsts[r_idx].rhs.push_back(0);
		}
	}
	return changed;
}

void Grammar::print_firsts() {
	sort(&firsts); // to print in order of grammar
	for (int i=0; i<firsts.size(); i++) {
		if (!symbols[firsts[i].lhs].is_terminal) {
			cout << "FIRST(" << symbols[firsts[i].lhs].name << ") = { ";
			for (int j=0; j<firsts[i].rhs.size(); j++) {
				if(j>=1){cout<<", ";}
				cout << symbols[firsts[i].rhs[j]].name;
	
			}
			cout << " }\n";
		}
	}
}

/* ###### 
Task 4 
###### */
void Grammar::task4() {
	find_firsts(); find_follows(); 
	print_follows();
}

void Grammar::find_follows() {
	// setup follows vector
	for (int i=0; i<symbols.size(); i++) {
		rule r; r.lhs = i;
		follows.push_back(r);
	}
	follows[rules[0].lhs].rhs.push_back(-1);

	// first pass - find union
	for (int i=0; i<rules.size(); i++) {
		for (int j=0; j<rules[i].rhs.size()-1; j++) {
			bool empty = true;
			for (int k=j+1; k<rules[i].rhs.size() && empty; k++) {
				find_union(&follows[rules[i].rhs[j]].rhs, &firsts[rules[i].rhs[k]].rhs);
				empty = find(firsts[rules[i].rhs[k]].rhs.begin(), firsts[rules[i].rhs[k]].rhs.end(), 0) != firsts[rules[i].rhs[k]].rhs.end();
			}
		}
	}

	// Here's where the real stuff happens
	bool changed = true;
	while (changed) {
		changed = false;
		for (int i=0; i<rules.size(); i++) {
			if(find_follow(rules[i])) {changed=true;}
		}
	}
}

bool Grammar::find_follow(rule r) {
	bool changed = false, empty = true;
	for (int i=r.rhs.size()-1; i>=0 && empty; i--) {
		if(find_union(&follows[r.rhs[i]].rhs, &follows[r.lhs].rhs)) {changed=true;}
		empty = find(firsts[r.rhs[i]].rhs.begin(), firsts[r.rhs[i]].rhs.end(), 0) != firsts[r.rhs[i]].rhs.end();
	}
	return changed;
}

void Grammar::print_follows() {
	sort(&follows); // to print in order of grammar
	for (int i=0; i<follows.size(); i++) {
		if (!symbols[follows[i].lhs].is_terminal) {
			cout << "FOLLOW(" << symbols[follows[i].lhs].name << ") = { ";
			for (int j=0; j<follows[i].rhs.size(); j++) {
				if(j>=1){cout<<", ";}
				if(follows[i].rhs[j]==-1) {
					cout << "$";
				} else {
					cout << symbols[follows[i].rhs[j]].name;
				}
			}
			cout << " }\n";
		}
	}
}

/* ###### Task 5 ###### */
void Grammar::task5() {
	set_generating(); set_reachable();
	find_firsts(); find_follows();
	print_is_pparser();

}

void Grammar::print_is_pparser() {
	bool is_pparser = true;
	// useless symbols - all symbols, any not gen/not reach just output no
	for (int i=0; i<symbols.size(); i++) {
		if (!symbols[i].is_generating || !symbols[i].is_reachable) {
			is_pparser = false;
			break;
		}
	}

	if (is_pparser) {
		// now check conditions 1 and 2
	}

	if (is_pparser) {cout << "YES";} else {cout << "NO";}

}

