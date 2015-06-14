#include "../ouroboros_server.h"
#include <ouroboros/plugin.h>
#include <iostream>

using namespace ouroboros;
using namespace std;

void callback(var_field* aField)
{
	cout << "we got a callback!!!" << endl;
	cout << "\tHappened at: " << aField->getTitle() << endl;
}

void func(const std::vector<std::string>& aParams)
{
	std::cout << aParams[0] << " " << aParams[1] << endl;
}

extern "C" bool plugin_entry(ouroboros_server& aServer)
{
	cout << "Initializing plugin..." << endl;

	aServer.register_callback("home", "a_number", ::callback);

	aServer.register_function("func", ::func);
	
	cout << "Done initializing." << endl;
	return true;
}

extern "C" void plugin_exit(ouroboros_server& aServer)
{
	
}

