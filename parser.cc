#include "parser.h"

Parser::Parser() {
    g.add_symbol("#");
    parse_rules();
    g.init();
}

Token Parser::peek() {
    Token t = lexer.GetToken(); lexer.UngetToken(t);
    return t;
}

void Parser::parse_rule() {
    rule r;
    Token t = lexer.GetToken();
    r.lhs = g.add_symbol(t.lexeme);
    lexer.GetToken(); // arrow
    r.rhs = parse_rhs();
    lexer.GetToken(); // hash
    g.add_rule(r);
}

void Parser::parse_rules() {
    parse_rule();
    Token t = peek();
    if (t.token_type==ID) {parse_rules();}
}

vector<int> Parser::parse_ids() {
    vector<int> ids; 
    Token t = peek();

    if (t.token_type==HASH) {
        ids.push_back(g.add_symbol("#"));
    }

    while (t.token_type!=HASH) {
        t = lexer.GetToken();
        ids.push_back(g.add_symbol(t.lexeme));
        t = peek();
    }
    return ids;
}

vector<int> Parser::parse_rhs() {return parse_ids();}
Grammar Parser::get_g() {return g;}