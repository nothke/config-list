#include <iostream>
#include <vector>
#include <variant>

struct Vector {
	float x{}, y{}, z{};
};

class Configurator {
public:
	struct Entry {
		std::string name;
		enum Type { NONE, STRING, INT, FLOAT, VECTOR } type{}; // Step 1: Add your enum here
		std::variant<std::string*, int*, float*, Vector*> data; // Step 2: Add your pointer type here
		// ^ using a variant because union fails with strings

		template<typename T>
		static constexpr Type GetTypeEnum() {

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

		//static_assert(type != Entry::Type::NONE);


		Entry e{ .name = name, .type = type, .data = ptr };
		entries.push_back(e);
	}

	void Save() {};
	void Load() {};

	void List() {
		for (auto& e : entries)
		{
			std::cout << e.name << ", " << e.type << ": ";

			switch (e.type)
			{
			case Entry::FLOAT: std::cout << *std::get<float*>(e.data); break;
			case Entry::INT: std::cout << *std::get<int*>(e.data); break;
			case Entry::STRING: std::cout << *std::get<std::string*>(e.data); break;
			case Entry::VECTOR: std::cout << std::get<Vector*>(e.data)->x; break;
			}

			std::cout << "\n";
		}

		std::cout << sizeof(std::string) << "\n";
		std::cout << sizeof(Entry) << "\n";
	}
} cfg;


#define CFG_ADD(x) cfg.Add(#x, &x);

struct StructThatHasCFGValues {

	int years{ 33 };
	float multiplier{ 1.f };
	int hash{ 2138210984 };

	void SubmitToConfig()
	{
		CFG_ADD(years);
		CFG_ADD(multiplier);
		CFG_ADD(hash);
	}
};

int main(int argc, char* argv[]) {
	cfg.Load();

	int x = 3;
	int y = 10;
	float smth = 0.435f;
	std::string text = "wow, it works";
	Vector vec;

	cfg.Add("x", &x);

	CFG_ADD(x);
	CFG_ADD(y);
	CFG_ADD(smth);
	CFG_ADD(text);
	CFG_ADD(vec);

	StructThatHasCFGValues strct;
	strct.SubmitToConfig();

	cfg.Save();

	cfg.List();

	char chars[32] = "soijo";
	strcpy(chars, text.c_str());

	return EXIT_SUCCESS;
}