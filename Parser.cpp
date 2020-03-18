// Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <list>

#include "Tokenizer.h"

// main program entry point
int main(int argc, char* argv[])
{
	// Check to see that we have at least a input filename
	if (argc < 2) {
		cout << "Invalid command line arguments: need filename" << endl;
		cout << "  parser input_file <output_file>" << endl << endl;
		cout << "If no output file is specified, output will be done to std output." << endl;
		_exit(0);
	}

	string input_filename = argv[1];
	string output_filename = string::basic_string();

	if (argc > 2)
		output_filename = argv[2];

	fstream source;

	// ope the source file
	source.open(input_filename.c_str(), ios_base::in);
	if (source.fail()) {
		cout << "Error occurred during opening " << input_filename << endl;
		_exit(0);
	}

	// Create the token list
	token_parser parser(source);

	// tokenize - lexical analysis
	parser.tokenize();

	// parse - syntax analysis
	parser.parse();

	// output
	parser.print_file(output_filename);
}
