#include "jsonParser.h"
#include "jsonLexer.h"
#include <iostream>
#include <memory>

JSON JsonParser::parse(const std::string& str) {

	JsonLexer lexer(str);
	JsonParser parser;
	parser._tokens = lexer.lex();

	JSON ret;
	parser.parseNextToken(ret);
	return ret;
}

JSON JsonParser::read(const std::string& path) {
	std::ifstream inFile;
	inFile.open(path);

	if (inFile.is_open()) {
		std::string inputString((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		return parse(inputString);
	}

	return JSON();
}

void JsonParser::deserialize() {

}

void JsonParser::serialize() {

}

JSON JsonParser::parse_Object() {

	using TkType = JsonLexer::Token::Type;
	JSON obj;
	obj.type = JSON::Type::Object;
	obj.object_value = std::make_shared<JSON::JSONObject>();

	while (peekNextToken().type != TkType::RightBrace) {
		JSON key;
		if (parseNextToken(key)) {
			if (key.type != JSON::Type::String) {
				std::cout << "Json Object without Starting String" << std::endl;
				break;
			}

			JSON objValue;
			if (parseNextToken(objValue)) {
				auto shrObjValuePtr = std::make_shared<JSON>(objValue);
				obj.object_value->emplace(std::move(*key.string_value), std::move(shrObjValuePtr));
			} else {
				//Not a Value
				break;
			}
		} else {
			// Not a Key
			break;
		}
	}

	//Consume RightBrace after each { objectParsing } to avoid breaking the loop of the upper level while check
	_tokenCur++;

	return obj;
}

JSON JsonParser::parse_Array() {

	using TkType = JsonLexer::Token::Type;

	JSON arr;

	while (peekNextToken().type != TkType::RightBracket) {
		JSON json;
		if (parseNextToken(json)) {
			auto ptr = std::make_shared<JSON>(json);
			arr.emplace_back(std::move(ptr));
		} else {
			break;
		}
	}

	_tokenCur++; //consume rightBracket ']'

	return arr;
}

JSON JsonParser::parse_Number() {
	if (!currentToken) return JSON();
	double number = std::stof(currentToken->value);
	return JSON(number);
}

std::string JsonParser::parse_String() {
	if (!currentToken) return "";

	using TkType = JsonLexer::Token::Type;

	std::string ret(currentToken->value);

	while (peekNextToken().type != TkType::QuotationMark && peekNextToken().type != TkType::EndOfFile) {
		ret.append(peekNextToken().value);
		_tokenCur++;
	}

	_tokenCur++; //Consume '/"' after each string extraction

	return ret;
}

bool JsonParser::parseNextToken(JSON& outJson) {

	if (_tokenCur >= _tokens.size()) {
		return false;
	}

	currentToken = &_tokens[_tokenCur++];

	using TokenType = JsonLexer::Token::Type;

	switch (currentToken->type) {
		case TokenType::LeftBrace:
			outJson = parse_Object();
			return true;

		case TokenType::LeftBracket: {
			outJson = parse_Array();
			return true;
		}

		case TokenType::String: {
			outJson = parse_String();
			return true;
		}
		case TokenType::Number: {
			outJson = parse_Number();
			return true;
		}
		case TokenType::True: {
			outJson = JSON(true);
			return true;
		}
		case TokenType::False: {
			outJson = JSON(false);
			return true;
		}

		case TokenType::Null: {
			outJson = JSON();
			return true;
		}

		case JsonLexer::Token::Type::RightBrace:
			return parseNextToken(outJson);
		case JsonLexer::Token::Type::RightBracket:
			return parseNextToken(outJson);
		case TokenType::QuotationMark:
			return parseNextToken(outJson);
		case TokenType::Colon:
			return parseNextToken(outJson);
		case TokenType::Comma:
			return parseNextToken(outJson);

		case TokenType::None:
			break;
		case JsonLexer::Token::Type::EndOfFile:
			return false;
	}

	return false;

}
