// Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <list>

//shape = {
//	type = "tetrahedron"
//
//		vertices = {
//
//			point = { x = "1" y = "0" z = "0" }
//			point = { x = "0" y = "1" z = "0" }
//			point = { x = "0" y = "0" z = "1" }
//			point = { x = "1" y = "1" z = "1" }
//		}
//
//		color = { r = "0xFF" g = "0x00" b = "0x80" alpha = "0x80" }
//	
//		a = "25"
//}

//(1, 0, shape, )
//(2, 1, type, tetrahedron)
//(3, 1, vertices, )
//(4, 3, point, )
//(5, 4, x, 1)
//(6, 4, y, 0)
//(7, 4, z, 0)
//(8, 3, point, )
//(9, 8, x, 0)
//(10, 8, y, 1)
//(11, 8, z, 0)
//(12, 3, point, )
//(13, 12, x, 0)
//(14, 12, y, 0)
//(15, 12, z, 1)
//(16, 3, point, )
//(17, 16, x, 1)
//(18, 16, y, 1)
//(19, 16, z, 1)
//(20, 1, color, )
//(21, 20, r, 0xFF)
//(22, 20, g, 0x00)
//(23, 20, b, 0x80)
//(24, 20, alpha, 0x80)
//(25, 1, a, 25)

//class token
//{
//public:
//	typedef enum { t_invalid_token = 0,
//	t_symbol,
//	t_integer,
//	t_const_literal,
//	t_punctuation,
//	t_whitespace,
//	t_eol,
//	t_eof
//	} token_t;
//
//	token(token_t _token) : token_type(_token) { };
//	virtual int parse(std::fstream& stream, int input_char) = 0;
//	virtual void print() = 0;
//
//private:
//	token_t token_type;
//};
//
//class token_parser
//{
//private:
//	std::fstream& src_stream;
//	std::list<token*> token_list;
//
//public:
//	token_parser(std::fstream& stream) : src_stream(stream) { };
//	bool parse();
//	void print();
//};
//
//class whitespace_token : public token
//{
//public:
//	whitespace_token() : token(t_whitespace) { };
//	int parse(std::fstream& stream, int input_char);
//	void print();
//};
//
//int whitespace_token::parse(std::fstream& stream, int input_char)
//{
//	while (true)
//	{
//		input_char = stream.get();
//		if (input_char == ' ' || input_char == 0x09 || input_char == 0x0B || input_char == 0x0D)
//		{
//			continue;
//		}
//		return input_char;
//	}
//}
//
//void whitespace_token::print()
//{
//	std::cout << "TOKEN[\"whitespace\" , \" \"]" << std::endl;
//}

#include "Token.h"

// main program entry point
int main(int argc, char* argv[])
{
	// Check to see that we have at least a filename
	if (argc < 2) {
		cout << "Invalid command line arguments: need filename" << endl;
		_exit(0);
	}
	string filename = argv[argc - 1];

	fstream source;

	// ope the source file
	source.open(filename.c_str(), ios_base::in);
	if (source.fail()) {
		cout << "An error has occurred whilst opening " << filename << endl;
		_exit(0);
	}

	// Create the token list
	token_parser parser(source);
	// tokenize - lexical analysis
	parser.tokenize();

	// parse - syntax analysis
	parser.parse();
	parser.print_tokens();
}
