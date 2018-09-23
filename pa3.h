#ifndef PA3_H
#define PA3_H

class infoParser {
public:
	//functions and constructor
	infoParser();
	void tokenExtractor();
	void resultPrinter();
	void classifier();
	void charChecker();
	void duplicateEraser(std::vector<std::string>& v1);
	int nestedLoopCounter();
	void push(std::string in);
	void pop();
	//token vectors
	std::vector<std::string> lineStack;
	std::vector<std::string> extractedTokens;
	std::vector<std::string> noSymbolTokens;
	std::vector<char> tmp;
	//result vectors
	std::vector<std::string> keywords;
	std::vector<std::string> identifiers;
	std::vector<std::string> constants;
	std::vector<std::string> operators;
	std::vector<std::string> delimiters;
	std::vector<std::string> syntaxErrors;
};

#endif
