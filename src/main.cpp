#include <iostream>
#include <vector>
#include <variant>

#include "config_list.h"

ConfigList cfg;

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

	mINI::INIFile file("config.ini");

	mINI::INIStructure data;

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

	unsigned char c = 64;
	//cfg.Add("Oops", &c); // Uncommenting this line gives static_assert error

	StructThatHasCFGValues strct;
	strct.SubmitToConfig();

	cfg.Save(data);

	cfg.List();
	std::cout << "\n";

	//file.generate(data, true);

	if (file.read(data))
		cfg.Load(data);

	cfg.List();//

	y = 666;

	cfg.Save(data);
	file.write(data);
	
	return EXIT_SUCCESS;
}