#pragma once

#include <fstream>
#include "json.h"
#include "jsonLexer.h"

class JsonParser {
public:

	static JSON read(const std::string& path);
	static JSON parse(const std::string& str);

	static void deserialize();
	static void serialize();
private:

	std::vector<JsonLexer::Token> _tokens;
	int _tokenCur = 0;

	JsonLexer::Token* currentToken = nullptr;

	bool parseNextToken(JSON& outJson);
	JSON parse_Object();
	JSON parse_Array();
	inline JSON parse_Number();
	inline std::string parse_String();

	inline JsonLexer::Token peekNextToken() {
		using Token = JsonLexer::Token;
		int peekIdx = _tokenCur;
		return peekIdx >= _tokens.size()? Token(Token::Type::EndOfFile) : _tokens[peekIdx];
	}
};