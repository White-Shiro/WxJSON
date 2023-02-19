//
// Created by SHIRO_DESKTOP on 2023/2/5.
//

#include <iostream>
#include "jsonLexer.h"

JsonLexer::JsonLexer(const std::string& input) : _stream(input) {
	_currentChar = static_cast<char>(_stream.get());
	_cursor = 0;
}

void JsonLexer::skipWhitespace() {
	while (isWhiteSpace(_currentChar)) {
		_currentChar = static_cast<char>(_stream.get());
		_cursor++;
	}
}

JsonLexer::Token::Token(Type type, std::string value) : type(type), value(std::move(value)) {}

JsonLexer::Token JsonLexer::getNextToken() {

	using Type = Token::Type;
	skipWhitespace();

	char ch = _currentChar;
	if (ch == '{') {
		_currentChar = _stream.get();
		_cursor++;
		return {Type::LeftBrace, "{"};
	}

	if (ch == '}') {
		_currentChar = _stream.get();
		_cursor++;
		return {Type::RightBrace, "}"};
	}

	if (ch == '[') {
		_currentChar = _stream.get();
		_cursor++;
		return {Type::LeftBracket, "["};
	}

	if (ch == ']') {
		_currentChar = _stream.get();
		_cursor++;
		return {Type::RightBracket, "]"};
	}

	if (ch == ':') {
		_currentChar = _stream.get();
		_cursor++;
		return {Type::Colon, ":"};
	}

	if (ch == '"') {
		_currentChar = _stream.get();
		_cursor++;
		return {Type::QuotationMark, "\""};
	}

	if (ch == ',') {
		_currentChar = _stream.get();
		_cursor++;
		return {Type::Comma, ","};
	}

	if (isNumber(ch)) {
		std::string number;
		do {
			number += ch;
			ch = _stream.get();
			_cursor++;
		} while (isNumber(ch));

		_currentChar = ch;
		return {Type::Number, number};
	}

	if (isCharacter(ch)) {
		std::string str;
		do {
			str += ch;
			ch = _stream.get();
			_cursor++;
		} while (isCharacter(ch));

		_currentChar = ch;

		if (str == "true") { return {Type::True, str}; }
		if (str == "false") { return {Type::False, str}; }
		if (str == "null") { return {Type::Null, str}; }

		return {Type::String, str};
	}

	if (_currentChar == EOF) { return {Type::EndOfFile, "EOF"}; }

	std::string undenfined;
	undenfined += ch;
	return {Type::None,undenfined};
}

std::vector<JsonLexer::Token> JsonLexer::lex(bool print) {
	std::vector<Token> ret;

	while (_currentChar != EOF) { ret.emplace_back(getNextToken()); }
	if (print) printTokenPretty(ret);
	return ret;
}

void JsonLexer::printTokenPretty(const std::vector<Token>& tokens) {

	char* tab = "    "; //4 space tab

	using Token = JsonLexer::Token;

	int indentLevel = 0;
	for (auto& token: tokens) {
		if (token.type == Token::Type::LeftBrace) {
			if (indentLevel <1 ) {
				std::cout << token.value << std::endl << tab;
				indentLevel++;
				continue;
			}

			std::cout << token.value << std::endl;
			indentLevel++;
			for (int i = 0; i < indentLevel; ++i) { std::cout << tab; }
			continue;
		}

		if (token.type == Token::Type::Comma) {
			std::cout << token.value << std::endl;
			for (int i = 0; i < indentLevel; ++i) { std::cout << tab; }
			continue;
		}

		if (token.type == Token::Type::RightBrace) {
			std::cout << std::endl;
			indentLevel--;
			for (int i = 0; i < indentLevel; ++i) { std::cout << tab; }
		}
		std::cout << token.value;
	}
}
