#include <iostream>
#include <vector>
#include <variant>

#define USE_UNION

struct Vector {
	float x{}, y{}, z{};
};

class Configurator {
public:
	struct Entry {
		std::string name;
		enum { STRING, INT, FLOAT, VECTOR } type{};

#ifndef USE_UNION
		std::variant<std::string, int, float, Vector> data;
		// ^ using a variant because union fails with strings
#else 
		union {
			char s[32];
			int i;
			float f;
			Vector v;
		};
#endif 

	};

	std::vector<Entry> entries;

	// This adds Add() functions for each of the types:
#ifndef USE_UNION

#define CFG_ADDER(TYPE, TYPE_ENUM) void Add(const std::string& name, TYPE value) { \
	Entry e{ .name = name, .type = Entry::TYPE_ENUM, .data = value }; \
	entries.push_back(e); }

	// Add your own types here:
	CFG_ADDER(int, INT);
	CFG_ADDER(float, FLOAT);
	CFG_ADDER(std::string, STRING);
	CFG_ADDER(Vector, VECTOR);

#else

#define CFG_ADDER(TYPE, TYPE_ENUM, TYPE_VAR) void Add(const std::string& name, TYPE value) { \
	Entry e{ .name = name, .type = Entry::TYPE_ENUM, .TYPE_VAR =value }; \
	entries.push_back(e); }

	// Add your own types here:
	CFG_ADDER(int, INT, i);
	CFG_ADDER(float, FLOAT, f);
	//CFG_ADDER(const char*, STRING, s);
	CFG_ADDER(Vector, VECTOR, v);

	void Add(const std::string& name, const std::string& value) {
		Entry e{ .name = name, .type = Entry::STRING, .s = {} };
		strcpy(e.s, value.c_str());
		entries.push_back(e);
	}

#endif



	void Save() {};
	void Load() {};

	void List() {
		for (auto& e : entries)
		{
			std::cout << e.name << ", " << e.type << ": ";

			switch (e.type)
			{
#ifndef USE_UNION
			case Entry::FLOAT: std::cout << std::get<float>(e.data); break;
			case Entry::INT: std::cout << std::get<int>(e.data); break;
			case Entry::STRING: std::cout << std::get<std::string>(e.data); break;
			case Entry::VECTOR: std::cout << std::get<Vector>(e.data).x; break;
#else
			case Entry::FLOAT: std::cout << e.f; break;
			case Entry::INT: std::cout << e.i; break;
			case Entry::STRING: std::cout << e.s; break;
			case Entry::VECTOR: std::cout << e.v.x; break;
#endif
			}

			std::cout << "\n";
		}

		std::cout << sizeof(std::string) << "\n";
		std::cout << sizeof(Entry) << "\n";
	}
} cfg;


#define CFG_ADD(x) cfg.Add(#x, x);

int main(int argc, char* argv[]) {
	cfg.Load();

	int x = 3;
	int y = 10;
	float smth = 0.435f;
	std::string text = "wow, it works";
	Vector vec;

	CFG_ADD(x);
	CFG_ADD(y);
	CFG_ADD(smth);
	CFG_ADD(text.c_str());
	CFG_ADD(vec);

	cfg.Save();

	cfg.List();

	char chars[32] = "soijo";
	strcpy(chars, text.c_str());

	return EXIT_SUCCESS;
}