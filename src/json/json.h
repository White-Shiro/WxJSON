#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

struct JSON {
private:
	using string = std::string;
public:


	enum class Type {
		Null, Object, Array, String, Number, Boolean
	};

	using JSONArray = std::vector<std::shared_ptr<JSON> >;
	using JSONObject = std::map<string, std::shared_ptr<JSON> >;

	union {
		std::shared_ptr<JSONObject> object_value{};
		std::shared_ptr<JSONArray> array;
		std::shared_ptr<string> string_value;
		double number_value;
		bool boolean_value;
	};

	Type type;

	explicit JSON() : type(Type::Null) {
		number_value = 0;
		boolean_value = false;
	}

	JSON(std::string&& string) {

		string_value = std::make_shared<std::string>(std::move(string));
		type = Type::String;
	}

	JSON(const std::string& string) {
		string_value = std::make_shared<std::string>(string);
		type = Type::String;
	}


	JSON(const float number) {
		number_value = number;
		type = Type::Number;
	}

	JSON(const double number) {
		number_value = number;
		type = Type::Number;
	}

	JSON(const bool boolean) {
		boolean_value = boolean;
		type = Type::Boolean;

	}

	JSON(const JSON& other) {
		type = other.type;
		switch (type) {
			case Type::Null:
				break;
			case Type::Object:
				object_value = other.object_value;
				break;
			case Type::Array:
				array = other.array;
				break;
			case Type::String:
				string_value = other.string_value;
				break;
			case Type::Number:
				number_value = other.number_value;
				break;
			case Type::Boolean:
				boolean_value = other.boolean_value;
				break;
		}
	}

	JSON(JSON&& other) noexcept {
		type = other.type;
		switch (type) {
			case Type::Null:
				break;
			case Type::Object:
				object_value = other.object_value;
				break;
			case Type::Array:
				array = other.array;
				break;
			case Type::String:
				string_value = other.string_value;
				break;
			case Type::Number:
				number_value = other.number_value;
				break;
			case Type::Boolean:
				boolean_value = other.boolean_value;
				break;
		}
	}

	~JSON() {
		if (object_value)
			object_value = nullptr;
		if (array)
			array = nullptr;
		if (string_value)
			string_value = nullptr;
	}

	JSON& operator=(const JSON& rhs) {

		type = rhs.type;
		switch (type) {

			case Type::Null:
				break;
			case Type::Object:
				object_value = rhs.object_value;
				break;
			case Type::Array:
				array = rhs.array;
				break;
			case Type::String:
				string_value = rhs.string_value;
				break;
			case Type::Number:
				number_value = rhs.number_value;
				break;
			case Type::Boolean:
				boolean_value = rhs.boolean_value;
				break;
		}
		return *this;
	}

	JSON& operator=(JSON&& rhs) noexcept {
		type = rhs.type;
		switch (type) {
			case Type::Null:
				break;
			case Type::Object:
				object_value = rhs.object_value;
				break;
			case Type::Array:
				array = rhs.array;
				break;
			case Type::String:
				string_value = rhs.string_value;
				break;
			case Type::Number:
				number_value = rhs.number_value;
				break;
			case Type::Boolean:
				boolean_value = rhs.boolean_value;
				break;
		}
		return *this;
	}


	inline bool isObject() const { return type == Type::Object; }

	inline bool isArray() const { return type == Type::Array; }

	inline bool isString() const { return type == Type::String; }

	inline bool isNumber() const { return type == Type::Number; }

	inline bool isBoolean() const { return type == Type::Boolean; }

	inline JSONObject& asObject() {
		if (type != Type::Object) {
			return *std::make_shared<JSONObject>();
		}

		if (!object_value) {
			object_value = (std::make_shared<JSONObject>());
		}

		return *object_value;
	}

	inline JSONArray& asArray() {
		if (type != Type::Array) {
			return *std::make_shared<JSONArray>();
		}

		if (!array) {
			array = (std::make_shared<JSONArray>());
		}

		return *array;
	}

	inline string asString() {
		if (type != Type::String) {
			return "";
		}
		if (!string_value) {
			string_value = std::make_shared<string>();
		}
		return *string_value;
	}

	inline bool asBoolean() const { return boolean_value; }

	inline float asFloat() const { return static_cast<float>(number_value); }

	inline double asDouble() const { return number_value; }

	inline int asInt() const { return static_cast<int>(number_value); }

	JSON& operator[](const std::string& key) {
		if (type != Type::Object) {
			type = Type::Object;
		}

		if (!object_value) {
			object_value = std::make_shared<JSON::JSONObject>();
		}

		if (object_value->find(key) == object_value->end()) {
			auto newEntry = std::make_shared<JSON>();
			object_value->emplace(key, std::move(newEntry));
		}

		return *((object_value)->at(key));
	}

	JSON& operator[](int idx) {
		switch (type) {
			case Type::Null:
				break;
			case Type::Object:
				break;
			case Type::Array:
				break;
			case Type::String: {
				_convertToArray(std::move(*string_value), idx);
			} break;
			case Type::Number:
				_convertToArray(number_value, idx);
				break;
			case Type::Boolean:
				_convertToArray(boolean_value, idx);
				break;
		}

		type = Type::Array;

		if (!array) {
			array = std::make_shared<JSONArray>(idx + 1);
		}

		if (array->size() <= idx) {
			array->resize(idx + 1);
		}

		if (!array->at(idx)) {
			array->at(idx) = std::make_shared<JSON>();
		}

		return *(array->at(idx));
	}

private:
	template<typename T>
	inline void _convertToArray(T value, int size = 1) {

		switch (type) {
			case Type::Null: break;
			case Type::Object: break;
			case Type::Array: break;
			case Type::String: string_value = nullptr; break;
			case Type::Number: number_value = 0; break;
			case Type::Boolean: boolean_value = false; break;
		}

		type = Type::Array;

		if (!array) {
			array = std::make_shared<JSONArray>(size);
		}

		array->emplace_back(std::make_shared<JSON>(value));
	}
public:
	explicit operator int() const { return asInt(); }

	explicit  operator float() const { return asFloat(); }

	explicit  operator double() const { return asDouble(); }

	explicit  operator bool() const { return asBoolean(); }

	explicit  operator string() { return asString(); }

	void emplace_back(std::shared_ptr<JSON>& value) {

		if (type != Type::Array) {
			type = Type::Array;
		}

		if (!array) {
			array = std::make_shared<JSON::JSONArray>();
		}

		array->emplace_back(std::move(value));
	}

	void emplace_back(JSON&& value) {

		if (type != Type::Array) {
			type = Type::Array;
		}

		if (!array) {
			array = std::make_shared<JSON::JSONArray>();
		}

		array->emplace_back(std::make_shared<JSON>(std::move(value)));
	}

	void emplace_back(JSON& value) {

		if (type != Type::Array) {
			type = Type::Array;
		}

		if (!array) {
			array = std::make_shared<JSON::JSONArray>();
		}

		array->emplace_back(std::make_shared<JSON>(value));
	}

	void emplace_back(std::shared_ptr<JSON>&& value) {

		if (type != Type::Array) {
			type = Type::Array;
		};

		if (!array) {
			array = std::make_shared<JSON::JSONArray>();
		}

		array->emplace_back(std::move(value));
	}
};

