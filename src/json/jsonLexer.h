#pragma once

#include <string>
#include <sstream>
#include <utility>
#include <vector>

class JsonLexer {
public:
	JsonLexer(const std::string& input);

	class Token {
	public:
		enum class Type : uint8_t {
			None,
			LeftBrace,
			RightBrace,
			LeftBracket,
			RightBracket,
			Colon,
			QuotationMark,
			Comma,
			String,
			Number,
			True,
			False,
			Null,
			EndOfFile
		};

		Type type;
		std::string value;

		Token(Type type, std::string value = "");
	};

	Token getNextToken();

	std::vector<Token> lex(bool print = false);

private:
	void skipWhitespace();

	//Token _prevToken;
	int _cursor = 0;
	char _currentChar;
	std::stringstream _stream;

	inline static bool isWhiteSpace(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\0'; }

	inline static bool isCharacter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

	inline static bool isNumber(char c) { return (c >= '0' && c <= '9') || (c == '-') || (c== '.'); }

	void static printTokenPretty(const std::vector<Token>& tokens);

};
