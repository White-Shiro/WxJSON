#include <iostream>

#include <chrono>
#include "json/jsonParser.h"

JSON testEscapedJSONParse() {

	// parse a Escaped Json String
	std::string escaped = R"(
{
	"glossary": {
		"title": "example glossary",
		"GlossDiv": {
			"title": "S",
			"GlossList": {
				"GlossEntry": {
					"ID": "SGML",
					"SortAs": "SGML",
					"GlossTerm": "Standard Generalized Markup Language",
					"Acronym": "SGML",
					"Abbrev": "ISO 8879:1986",
					"GlossDef": {
						"para": "A meta-markup language, used to create markup languages such as DocBook.",
						"GlossSeeAlso": ["GML", "XML"]
					},
					"GlossSee": "markup"
				}
			}
		}
	}
} 



)";
	return JsonParser::parse(escaped);
}

void testInputJSONFile() {
	std::string path = "D:/Learning/CPP-learn/Learn_JsonParser/input/example.json";
	JsonParser::read(path);
}

int main() {
	auto start = std::chrono::high_resolution_clock::now();
	auto js = testEscapedJSONParse();
	auto stop = std::chrono::high_resolution_clock::now();

	auto ty = js.type;
	auto name = js["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossSee"].asString();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	JSON v3 = true;
	v3[2] = 3.;

	std::cout << "name: " << name << std::endl;
	std::cout << "time used: " << duration.count() << "ms" << std::endl;

	return 0;
}