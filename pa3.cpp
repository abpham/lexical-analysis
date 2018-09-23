#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include "pa3.h"

infoParser::infoParser() {
}

void infoParser::push(std::string in) {
	//insert new elements on top of the stack
	lineStack.push_back(in);
}

void infoParser::pop() {
	//remove element from the top of the stack
	lineStack.pop_back();
}

int infoParser::nestedLoopCounter() {
	//counts the depth of nested for loops, also detects errors with keywords and parentheses
	int result = 0;
	int leftParen = 0;
	int rightParen = 0;
	int numFors = 0;
	int numBegins = 0;
	int numEnds = 0;

	//check for the number of FOR, BEGIN, and END keywords; also check number of FORs and ENDs to increment nested loop count
	for (unsigned int i = 0; i < keywords.size(); i++) {
		if (keywords[i] == "BEGIN") {
			numBegins++;
		}
		else if (keywords[i] == "FOR") {
			numFors++;
			if (numFors != numEnds) {
				result++;
			}
		}
		else if (keywords[i] == "END") {
			numEnds++;
			if (numEnds == numFors) {
				result--;
			}
		}
	}

	//check for missing END or BEGINs, and push to syntax errors, subtract nested loop count for error loops that don't count
	if (numEnds != numFors) {
		if (numEnds != numFors) {
			syntaxErrors.push_back("END");
		}
		if (numBegins != numFors) {
			syntaxErrors.push_back("BEGIN");
		}
		if (result != 0) {
			result--;
		}
	}

	//count number of left and right parentheses
	for (unsigned int i = 0; i < tmp.size(); i++) {
		if (tmp[i] == '(') {
			leftParen++;
		}
		else if (tmp[i] == ')') {
			rightParen++;
		}
	}

	//check for mismatched parentheses, and if so, push to syntax errors
	if (leftParen != rightParen) {
		if (leftParen > rightParen) {
			syntaxErrors.push_back("(");
		}
		else {
			syntaxErrors.push_back(")");
		}
	}

	//return depth of nested for loops
	return result;
}

void infoParser::duplicateEraser(std::vector<std::string>& v1) {
	//std::unique and std::sort technique learned from https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector

	std::sort(v1.begin(), v1.end());
	v1.erase(std::unique(v1.begin(), v1.end()), v1.end());
}

void infoParser::charChecker() {
	//extract each character from the file into a char vector

	for (unsigned int i = 0; i < lineStack.size(); i++) {
		for (unsigned int j = 0; j < lineStack[i].length(); j++) {
			tmp.push_back(lineStack[i].at(j));
		}
	}
}

void infoParser::classifier() {
	//classifies tokens into different vectors for further checking

	//define keywords and operators
	std::string officialKeywords[3] = { "BEGIN", "FOR", "END" };
	std::string officialOperators[7] = { "+", "-", "*", "/", "++", "=", "--" };

	//separate into keywords, constants, and identifiers
	for (unsigned int i = 0; i < noSymbolTokens.size(); i++) {

		//if uppercase, sort into keywords or misspelled keywords (syntax error)
		if (isupper(noSymbolTokens[i].at(0))) {
			if (noSymbolTokens[i].compare(officialKeywords[0]) == 0 || noSymbolTokens[i].compare(officialKeywords[1]) == 0 || noSymbolTokens[i].compare(officialKeywords[2]) == 0) {
				keywords.push_back(noSymbolTokens[i]);
			}
			else {
				syntaxErrors.push_back(noSymbolTokens[i]);
			}
		}
		else if (islower(noSymbolTokens[i].at(0))) {
			//if lowercase, push to identifiers
			identifiers.push_back(noSymbolTokens[i]);
		}
		else if (isdigit(noSymbolTokens[i].at(0))) {
			//if contains a digit, push to constants
			constants.push_back(noSymbolTokens[i]);
		}

	}

	//identify operators
	for (unsigned int i = 0; i < 7; i++) {
		for (unsigned int j = 0; j < extractedTokens.size(); j++) {
			if (extractedTokens[j].compare(officialOperators[i]) == 0) {
				//if matches the official operators, push into operators vector
				operators.push_back(extractedTokens[j]);
			}
		}
	}

	//identify delimiters
	for (unsigned int i = 0; i < tmp.size(); i++) {
		if (tmp[i] == ';' || tmp[i] == ',') {
			std::string temp(1, tmp[i]);
			delimiters.push_back(temp);
		}
		else if (tmp[i] == '=') {
			std::string temp(1, tmp[i]);
			operators.push_back(temp);
		}
	}
}

void infoParser::tokenExtractor() {
	//adapted from https://stackoverflow.com/questions/7621727/split-a-string-into-words-by-multiple-delimiters

	//extracted tokens with no delimiters or parentheses
	std::string delims = " ,;()";

	for (unsigned int i = 0; i < lineStack.size(); i++) {
		std::size_t start = lineStack[i].find_first_not_of(delims), end = 0;

		while ((end = lineStack[i].find_first_of(delims, start)) != std::string::npos)
		{
			extractedTokens.push_back(lineStack[i].substr(start, end - start));
			start = lineStack[i].find_first_not_of(delims, end);
		}
		if (start != std::string::npos)
			extractedTokens.push_back(lineStack[i].substr(start));
	}

	//no symbol tokens, just words/letters
	delims = " ,;()+-/*=";

	for (unsigned int i = 0; i < lineStack.size(); i++) {
		std::size_t start = lineStack[i].find_first_not_of(delims), end = 0;

		while ((end = lineStack[i].find_first_of(delims, start)) != std::string::npos)
		{
			noSymbolTokens.push_back(lineStack[i].substr(start, end - start));
			start = lineStack[i].find_first_not_of(delims, end);
		}
		if (start != std::string::npos)
			noSymbolTokens.push_back(lineStack[i].substr(start));
	}
}

void infoParser::resultPrinter() {
	//call function to classify tokens
	classifier();

	std::cout << "The depth of nested loop(s) is " << nestedLoopCounter() << std::endl;

	duplicateEraser(keywords);
	duplicateEraser(identifiers);
	duplicateEraser(constants);
	duplicateEraser(operators);
	duplicateEraser(delimiters);

	std::cout << "\nKeywords: ";
	for (unsigned i = 0; i < keywords.size(); i++) {
		std::cout << keywords[i] << " ";
	}
	
	std::cout << "\nIdentifiers: ";
	for (unsigned i = 0; i < identifiers.size(); i++) {
		std::cout << identifiers[i] << " ";
	}

	std::cout << "\nConstants: ";
	for (unsigned i = 0; i < constants.size(); i++) {
		std::cout << constants[i] << " ";
	}

	std::cout << "\nOperators: ";
	for (unsigned i = 0; i < operators.size(); i++) {
		std::cout << operators[i] << " ";
	}

	std::cout << "\nDelimiter: ";
	for (unsigned i = 0; i < delimiters.size(); i++) {
		std::cout << delimiters[i] << " ";
	}

	std::cout << "\n\nSyntax Error(s): ";
	for (unsigned i = 0; i < syntaxErrors.size(); i++) {
		std::cout << syntaxErrors[i] << " ";
	}
	if (syntaxErrors.size() < 1) {
		std::cout << "NA";
	}
}

infoParser* initialize() {
	//takes in the name of the input file
	std::cout << "Please enter the name of the input file: " << std::endl;
	std::string fileName;
	std::cin >> fileName;

	//creates in stream for file
	std::ifstream inFile(fileName.c_str());

	//check that the file is able to be opened
	try {
		if (!inFile) {
			throw 1;
		}
	}
	catch (...) {
		std::cout << "File not found" << std::endl;
	}

	//create stack and information parser
	infoParser* informationParser = new infoParser;

	//push each line to the stack
	std::string temp;
	while (std::getline(inFile, temp)) {
		informationParser->push(temp);
	}

	//close file
	inFile.close();

	return informationParser;
}

int main() {
	//create information parser and analyzer
	infoParser* parser = initialize();

	//extract all tokens and characters
	parser->tokenExtractor();
	parser->charChecker();

	//calculate and print the results
	parser->resultPrinter();

	return 0;
}