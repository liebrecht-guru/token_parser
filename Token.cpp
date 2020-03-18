/*
	The following code parses C/C++ source and prints out a list of tokens.
	The code is intended to be used as an example of parsing, and nothing
	else. Having said that, it must be pointed out that whilst this parser
	does correctly identify and process C/C++ tokens, there are tokens
	parsed that would not exist when a true C++ compiler tokenised it's
	input. Preprocessor tokenization is implemented in phase III of the
		compiler front-end and such tokens as #, ## would not be seen (i.e. they
		would have been processed in phase III). Furthermore, tokens enclosed in
		single quotes would be converted directly into their integer equivalents.
		Also, unicode characters and unicode sequences are not processed by this
	example.

	I have endeavoured to put a few checks in the parser to ensure that
	double quoted literals and block comments are terminated before the end
	of file, but this code assumes that the source provided is actually
	compilable!!

	The code has been compiled and tested under the Microsoft VC++ version
	10, B2 Release and no guarantee can be made as to it's compatibility with
	either other versions of VC++, or any other C++ compiler.

	If you have any problems, with this code please do not hesitate to ask.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stack>
#include <cctype>

using namespace std;

#include "Token.h"


// parse the rest of a symbol
int symbol_token::parse_token(fstream& stream, int input_char) {
	symbol = input_char;
	while (true) {
		input_char = stream.get();
		if (isalpha(input_char) || isdigit(input_char) || input_char == '_') {
			symbol += input_char;
			continue;
		}
		return input_char;
	}
}

// print the token to cout
void symbol_token::print_token() {
	cout << "TOKEN[\"symbol\" , \"" << symbol << "\"]" << endl;
}

// parse the rest of an integer
int integer_token::parse_token(fstream& stream, int input_char) {
	integer_string = input_char;
	if (input_char == '0')
	{
		input_char = stream.peek();
		if (input_char == 'X' || input_char == 'x') {
			integer_string += input_char;
			input_char = stream.get();
			while (true) {
				input_char = stream.get();
				if (isxdigit(input_char)) {
					integer_string += input_char;
					continue;
				}
				return input_char;
			}
		}
	}
	while (true) {
		input_char = stream.get();
		if (isdigit(input_char)) {
			integer_string += input_char;
			continue;
		}
		return input_char;
	}
}

// print the token to cout
void integer_token::print_token() {
	cout << "TOKEN[\"integer\" , " << integer_string << "]" << endl;
}

// parse the rest of a literal
int literal_token::parse_token(fstream& stream, int input_char) {
	literal_string.clear();
	while (true) {
		input_char = stream.get();
		if (input_char == '\\') {
			input_char = stream.peek();
			if (input_char == '\"' || input_char == '\\') {
				literal_string += '\\';
				input_char = stream.get();
				literal_string += input_char;
				continue;
			}
			if (input_char == 0x0A) {
				cout << "error: EOL encountered before closing literal quotes" << endl;
				exit(0);
			}
			if (input_char == -1) {
				cout << "error: EOF encountered before closing literal quotes" << endl;
				exit(0);
			}
			literal_string += input_char;
			continue;
		}
		if (input_char != '\"' && input_char != -1) {
			literal_string += input_char;
			continue;
		}
		if (input_char == -1) {
			cout << "error: EOF encountered before closing literal quotes" << endl;
			exit(0);
		}
		input_char = stream.get();
		return input_char;
	}
}

// print the token to cout
void literal_token::print_token() {
	cout << "TOKEN[\"literal\" , \"" << literal_string << "\"]" << endl;
}

// parse the rest of a literal
int const_literal_token::parse_token(fstream& stream, int input_char) {
	const_literal_string.clear();
	while (true) {
		input_char = stream.get();
		if (input_char == '\\') {
			input_char = stream.peek();
			if (input_char == '\'' || input_char == '\\') {
				const_literal_string += '\\';
				input_char = stream.get();
				const_literal_string += input_char;
				continue;
			}
			const_literal_string += input_char;
			continue;
		}
		if (input_char != '\'') {
			const_literal_string += input_char;
			continue;
		}
		input_char = stream.get();
		return input_char;
	}
}

// print the token to cout
void const_literal_token::print_token() {
	cout << "TOKEN[\"constant literal\" , \"" << const_literal_string << "\"]" << endl;
}

// parse the rest of a punctuation sequence - this consists of
// ending up with either one, two or three characters in the
// punctuation string. NB: The sequence .. is accepted as a 
// punctuation token, but must be rejected by the compiler at
// some later stage.
int punctuation_token::parse_token(fstream& stream, int input_char) {
	punctuation_string = input_char;
	switch (input_char) {
	case '!': // Looking for either ! or !=
		input_char = stream.peek();
		if (input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // != token
		}
		break;
	case '#': // Looking for either # or ##
		input_char = stream.peek();
		if (input_char == '#') {
			input_char = stream.get();
			punctuation_string += input_char; // ## token
		}
		break;
	case '%': // Looking for either % or %=
		input_char = stream.peek();
		if (input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // %= token
		}
		break;
	case '&': // Looking for either &, && or &=
		input_char = stream.peek();
		if (input_char == '&' || input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // && token
		}
		break;
	case '*': // Looking for either * or *=
		input_char = stream.peek();
		if (input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // *= token
		}
		break;
	case '+': // Looking for either +, ++, or +=
		input_char = stream.peek();
		if (input_char == '+' || input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // ++ or += token
		}
		break;
	case '-': // Looking for either -, --, -=, ->, ->*
		input_char = stream.peek();
		if (input_char == '-' || input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // -- or -= token
		}
		if (input_char == '>') {
			input_char = stream.get();
			punctuation_string += input_char; // -> token
			input_char = stream.peek();
			if (input_char == '*') {
				input_char = stream.get();
				punctuation_string += input_char; // ->* token
			}
		}
		break;
	case '.': // Looking for either ., .. or ...
		input_char = stream.peek();
		if (input_char == '.') {
			input_char = stream.get();
			punctuation_string += input_char; // .. token (illegal!)
			input_char = stream.peek();
			if (input_char == '.') {
				input_char = stream.get();
				punctuation_string += input_char; // ... token
			}
		}
		break;
	case '/': // Looking for either / or /=
		input_char = stream.peek();
		if (input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // /= token
		}
		break;
	case ':': // Looking for either : or ::
		input_char = stream.peek();
		if (input_char == ':') {
			input_char = stream.get();
			punctuation_string += input_char; // :: token
		}
		break;
	case '<': // Looking for either < or <=, <<, or <<=
		input_char = stream.peek();
		if (input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // <= token
			break;
		}
		if (input_char == '<') {
			input_char = stream.get();
			punctuation_string += input_char; // << token
			input_char = stream.peek();
			if (input_char == '=') {
				input_char = stream.get();
				punctuation_string += input_char; // <<= token
			}
		}
		break;
	case '=': // Looking for either = or ==
		input_char = stream.peek();
		if (input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // == token
		}
		break;
	case '>': // Looking for either >, >=, >>, or >>=
		input_char = stream.peek();
		if (input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // >= token
			break;
		}
		if (input_char == '>') {
			input_char = stream.get();
			punctuation_string += input_char; // >> token
			input_char = stream.peek();
			if (input_char == '=') {
				input_char = stream.get();
				punctuation_string += input_char; // >>= token
			}
		}
		break;
	case '|': // Looking for either |, |=, or || 
		input_char = stream.peek();
		if (input_char == '|' || input_char == '=') {
			input_char = stream.get();
			punctuation_string += input_char; // || or |= token
		}
		break;
	}
	input_char = stream.get();
	return input_char;
}

// print the token to cout
void punctuation_token::print_token() {
	cout << "TOKEN[\"punctuation\" , \"" << punctuation_string << "\"]" << endl;
}

// parse the whitespace characters
int whitespace_token::parse_token(fstream& stream, int input_char) {
	while (true) {
		input_char = stream.get();
		if (input_char == ' ' || input_char == 0x09 || input_char == 0x0B || input_char == 0x0D) {
			continue;
		}
		return input_char;
	}
}

// print the token to cout
void whitespace_token::print_token() {
	cout << "TOKEN[\"whitespace\" , \" \"]" << endl;
}

// parse the eol character
int eol_token::parse_token(fstream& stream, int input_char) {
	while (true) {
		input_char = stream.get();
		return input_char;
	}
}

// print the token to cout
void eol_token::print_token() {
	cout << "TOKEN[\"EOL\"]" << endl;
}

// parse the eof character
int eof_token::parse_token(fstream& stream, int input_char) {
	return 0;
}

// print the token to cout
void eof_token::print_token(void) {
	cout << "TOKEN[\"EOF\"]" << endl;
}

// parse the invalid character
int invalid_token::parse_token(fstream& stream, int input_char) {
	invalid_character = input_char;
	input_char = stream.get();
	return input_char;
}

// print the token to cout
void invalid_token::print_token(void) {
	cout << "TOKEN[\"INVALID\"" << invalid_character << endl;
}

// parse the input source
bool token_parser::tokenize() {
	base_token* token = nullptr;

	while (!source_stream.eof()) {
		int input_char = source_stream.get();

		// Determine what the leading character is of the sequence,
		// create an appropriate token and get the actual token
		// class to parse the rest of it (if any)

		while (!source_stream.eof()) {
			// The following do loop is there only because I hate seeing
			// if () ... else if () ... else if () ... code!!!
			// Hence it's a do ... while ( false ) - single shot
			do
			{
				// Remove any comments from the source
				if (input_char == '/') {
					int peek_character = source_stream.peek();
					if (peek_character == '/') {
						// Remove the line comment
						while (peek_character != 0x0A && !source_stream.eof()) {
							peek_character = source_stream.get();
						}
						token = new(nothrow) eol_token;
						break;
					}
					if (peek_character == '*') {
						// Remove a block comment
						while (true) {
							peek_character = source_stream.get();
							if (peek_character == -1) {
								cout << "error: block comment not terminated before EOF" << endl;
								exit(0);
							}
							if (peek_character == 0x0A) {
								token = new(nothrow) eol_token;
								// Add the token to the end of the list
								token_list.push_back(token);
								continue;
							}
							if (peek_character == '*') {
								peek_character = source_stream.get();
								if (peek_character == -1) {
									cout << "error: block comment not terminated before EOF" << endl;
									exit(0);
								}
								if (peek_character == '/') {
									// We need to ensure that a whitespace token
									// is created to ensure /* */ in the middle
									// of a source line is processed correctly.
									input_char = source_stream.get();
									input_char = ' ';
									token = new(nothrow) whitespace_token;
									break;
								}
							}
						}
					}
				}
				if (isalpha(input_char) || input_char == '_') {
					// Start of a symbol sequence
					token = new(nothrow) symbol_token;
					break;
				}
				if (input_char == 0x0A) {
					// EOL
					token = new(nothrow) eol_token;
					break;
				}
				if (isspace(input_char)) {
					// Start of whitespace sequence
					token = new(nothrow) whitespace_token;
					break;
				}
				if (input_char == '\"') {
					// Start of literal sequence
					token = new(nothrow) literal_token;
					break;
				}
				if (input_char == '\'') {
					// Start of constant literal sequence
					token = new(nothrow) const_literal_token;
					break;
				}
				if (isdigit(input_char)) {
					// Start of number sequence
					token = new(nothrow) integer_token;
					break;
				}
				if (ispunct(input_char)) {
					// Start of punctuation sequence
					token = new(nothrow) punctuation_token;
					break;
				}
			} while (false);
			if (token == NULL) return false;
			input_char = token->parse_token(source_stream, input_char);
			// Add the token to the end of the list
			if ((token->type() != base_token::t_whitespace)
				&& (token->type() != base_token::t_eol))
				token_list.push_back(token);
			continue;
		}
	}
	// Add the EOF token to the end of the list
	token = new(nothrow) eof_token;
	token_list.push_back(token);

	node_iterator = token_list.begin();
	return true;
}


//t_invalid_token = 0, t_symbol,
//t_integer, t_literal,
//t_const_literal, t_punctuation,
//t_whitespace, t_eol, t_eof

void token_parser::parse()
{
	list<base_token*>::iterator iter;
	iter = token_list.begin();
	int id = 1;

	list<node*> node_list;
	stack<node*> node_stack;

	node* root = new node(id);

	node_list.push_back(root);
	node* tmp_node = root;
	node* parent_node = nullptr;

	base_token* tok;
	base_token* next;
	auto node_lst_it = node_list.begin();	// first node = root

	while (((tok = get_next()) != nullptr) && tok->type() != base_token::t_eof)
	{
		string val = tok->get_value();
		next = peek_next();
		switch (tok->type())
		{
			case base_token::t_symbol:
				if (next == nullptr)
				{
					cout << "next = null. exit" << endl;
					_exit(-1);
				}

				tmp_node->set_name(val);

				if (next->type() == base_token::t_punctuation)
				{
					if (next->get_value().compare("=") == 0)
					{
						continue;
					}
					else
					{
						// not a valid token
						cout << "no valid token 1" << endl;
					}
				}
				else
				{
					// not a valid token
					cout << "no valid token 2" << endl;
				}
				
				continue;

			case base_token::t_punctuation:
				if (val.compare("{") == 0)
				{
					if (next->type() == base_token::t_symbol)
					{
						node* new_elem = new node(++id);
						new_elem->add_parent(tmp_node);
						//new_elem->set_name(val);
						tmp_node->add_child(new_elem);
						node_list.push_back(new_elem);
						parent_node = tmp_node;
						tmp_node = new_elem;
						continue;
					}
					else
					{
						cout << "no valid token 3" << endl;
					}
				}
				else if (val.compare("}") == 0)
				{
					// go back to parent node
					tmp_node = parent_node->get_parent();
					if (next->type() == base_token::t_symbol)
					{
						node* new_elem = new node(++id);
						new_elem->add_parent(tmp_node);
						//new_elem->set_name(val);
						tmp_node->add_child(new_elem);
						node_list.push_back(new_elem);
						tmp_node = new_elem;
					}
					else
					{
						parent_node = parent_node->get_parent();
					}

				}
				else if (val.compare("=") == 0)
				{
					if (next->type() == base_token::t_punctuation)
					{
						//tmp_node->set_name(val);
						if (next->get_value().compare("{") == 0)
						{
							// there will be a list
							continue;
						}
						else
						{
							// not a valid token
							cout << "no valid token 1" << endl;
						}
					}
					else if (next->type() == base_token::t_literal || next->type() == base_token::t_const_literal || next->type() == base_token::t_integer)
					{
						// there will be a value, nothing to do
						continue;
					}
					else 
					{
						// not a valid token
						cout << "no valid token 2" << endl;
					}
				}
				break;

			case base_token::t_literal:
			case base_token::t_integer:
			case base_token::t_const_literal:
				tmp_node->set_data(val);
				// if next symbol, create new elem
				if (next->type() == base_token::t_symbol)
				{
					node* new_elem = new node(++id);
					new_elem->add_parent(parent_node);
					//new_elem->set_name(val);
					parent_node->add_child(new_elem);
					node_list.push_back(new_elem);
					tmp_node = new_elem;
					continue;
				}
				break;

			default:
				break;
		}
	}
}

base_token* token_parser::get_next()
{
	if (node_iterator != token_list.end())
		return (*node_iterator++);
	else
		return nullptr;
}

base_token* token_parser::peek_next()
{
	if ((*node_iterator) != nullptr && std::next(node_iterator, 0) != token_list.end())
		return (*std::next(node_iterator, 0));
	else
		return nullptr;
}

// Simply iterate through the list of tokens and print them to cout
// Of course, get the token object to print itself :o)
void token_parser::print_tokens() {
	list<base_token*>::iterator iterator;
	iterator = token_list.begin();
	while (iterator != token_list.end()) {
		(*iterator)->print_token();
		++iterator;
	}
}
