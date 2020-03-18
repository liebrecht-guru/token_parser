#ifndef __TOKEN_DEFINED__
#define __TOKEN_DEFINED__

#pragma once

#include <fstream>
#include <list>

using namespace std;

// All tokens must derive from this token type

class base_token
{
public:
	typedef enum {
		t_invalid_token = 0, t_symbol,
		t_integer, t_literal,
		t_const_literal, t_punctuation,
		t_whitespace, t_eol, t_eof
	} type_of_token;
private:
	type_of_token token_type;
public:
	base_token(type_of_token token) : token_type(token) { };
	type_of_token type() { return token_type; }
	virtual int parse_token(fstream& stream, int input_char) = 0;
	virtual void print_token() = 0;
	virtual string get_value() = 0;
};

// A token that may contain a symbol

class symbol_token : public base_token
{
private:
	string symbol;
public:
	symbol_token() : base_token(t_symbol) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return symbol; }
	void print_token();
};

// A token that represents an integer

class integer_token : public base_token
{
private:
	string integer_string;
public:
	integer_token() : base_token(t_integer) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return integer_string; }
	void print_token();
};

// A token that represents a literal

class literal_token : public base_token
{
private:
	string literal_string;
public:
	literal_token() : base_token(t_literal) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return literal_string; }
	void print_token();
};

// A token that represents a constant literal (e.g. 'A')

class const_literal_token : public base_token
{
private:
	string const_literal_string;
public:
	const_literal_token() : base_token(t_const_literal) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return const_literal_string; }
	void print_token();
};

// A token that represents a punctuation or separator

class punctuation_token : public base_token
{
private:
	string punctuation_string;
public:
	punctuation_token() : base_token(t_punctuation) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return punctuation_string; }
	void print_token();
};

// A token that represents whitespace

class whitespace_token : public base_token
{
public:
	whitespace_token() : base_token(t_whitespace) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return ""; }
	void print_token();
};

// A token that represents an eol

class eol_token : public base_token
{
public:
	eol_token() : base_token(t_eol) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return ""; }
	void print_token();
};

// A token that represents an eof

class eof_token : public base_token
{
public:
	eof_token() : base_token(t_eof) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return ""; }
	void print_token();
};

// A token that represents an illegal character

class invalid_token : public base_token
{
private:
	int invalid_character;
public:
	invalid_token() : base_token(t_invalid_token) { };
	int parse_token(fstream& stream, int input_char);
	string get_value() { return ""; }
	void print_token();
};

class node
{
private:
	int id;
	int child_cnt;
	string name;
	string data;
	node* parent;
	list<node*> children;
public:
	node(int _id) : id(_id), parent(nullptr), child_cnt(0) { };
	int get_id() { return id; }
	void set_name(string s) { name = s; }
	void set_data(string s) { data = s; }
	void add_parent(node* n) { parent = n; }
	void add_child(node* n) { children.push_back(n); }
	node* get_parent() { return parent; }
	list<node*> get_children() { return children; }
};

// The C++ token parser
class token_parser
{
private:
	fstream& source_stream;
	list<base_token*> token_list;
	list<base_token*>::iterator node_iterator;
public:
	token_parser(fstream& stream) : source_stream(stream) { };
	base_token* get_next();
	base_token* peek_next();
	bool tokenize();
	void parse();
	void print_tokens();
};

#endif

