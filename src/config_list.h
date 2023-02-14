// 
// ConfigList by Nothke
// 
// Follow each of the steps to add your own type
//

#pragma once

#include <iostream>
#include <vector>
#include <variant>

#include "ini.h"

struct Vector {
	float x{}, y{}, z{}, w{};
};

class ConfigList {
public:
	struct Entry {
		std::string name;
		enum Type { NONE, STRING, INT, FLOAT, VECTOR } type{}; // Step 1: Add your enum here
		std::variant<std::string*, int*, float*, Vector*> data; // Step 2: Add your pointer type here
		// ^ using a variant because union fails with strings

		template<typename T>
		static constexpr Type GetTypeEnum()
		{
#define MAP_TYPE(typeName, typeEnum) if constexpr (std::is_same<T, typeName>()) return Entry::Type::typeEnum

			// STEP 3: Map type to enum here:
			MAP_TYPE(std::string, STRING);
			MAP_TYPE(int, INT);
			MAP_TYPE(float, FLOAT);
			MAP_TYPE(Vector, VECTOR);

#undef MAP_TYPE

else return Entry::Type::NONE;
		}
	};


	std::vector<Entry> entries;

	template <typename T>
	void Add(const std::string& name, T* ptr) {

		constexpr Entry::Type type = Entry::GetTypeEnum<T>();

		static_assert(type != Entry::Type::NONE); // Unsupported type

		Entry e{ .name = name, .type = type, .data = ptr };
		entries.push_back(e);
	}

	void Save(mINI::INIStructure& ini) {
		using namespace std;

		for (auto& e : entries)
		{
			std::string segment = "main";

			// Step 4: Add your string conversion for saving here:

			switch (e.type) {
			case Entry::FLOAT:	ini[segment][e.name] = to_string(*get<float*>(e.data));			break;
			case Entry::INT:	ini[segment][e.name] = to_string(*get<int*>(e.data));			break;
			case Entry::STRING: ini[segment][e.name] = *get<string*>(e.data);					break;
			case Entry::VECTOR: ini[segment][e.name] = to_string((*get<Vector*>(e.data)).x);	break;
			}
		}
	};

	void Load(mINI::INIStructure& ini) {
		using namespace std;

		for (auto& e : entries)
		{
			std::string segment = "main";

			// Step 5: Add your conversion from string when loading here:

			switch (e.type) {
			case Entry::FLOAT:	*get<float*>(e.data) = std::stof(ini[segment][e.name]);			break;
			case Entry::INT:	*get<int*>(e.data) = std::stoi(ini[segment][e.name]);			break;
			case Entry::STRING: *get<string*>(e.data) = ini[segment][e.name];					break;
			case Entry::VECTOR: (*get<Vector*>(e.data)).x = std::stof(ini[segment][e.name]);	break;
			}
		}
	};

	void List() {
		for (auto& e : entries)
		{
			std::cout << e.name << ", " << e.type << ": ";

			// Step 6 (optional): Add this for debugging:

			switch (e.type)
			{
			case Entry::FLOAT:	std::cout << *std::get<float*>(e.data); break;
			case Entry::INT:	std::cout << *std::get<int*>(e.data); break;
			case Entry::STRING: std::cout << *std::get<std::string*>(e.data); break;
			case Entry::VECTOR: std::cout << std::get<Vector*>(e.data)->x; break;
			}

			std::cout << "\n";
		}

		std::cout << sizeof(std::string) << "\n";
		std::cout << sizeof(Entry) << "\n";
	}
};