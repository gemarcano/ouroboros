#include "../ouroboros_server.h"
#include "../callback.h"
#include "test.h"
#include <server/plugin.h>
#include <iostream>

using namespace ouroboros;
using namespace std;

void callback(const std::string& aGroup, const std::string& aField)
{
	cout << "we got a callback!!!" << endl;
	cout << "\tHappened at: " << aGroup << '\\' << aField << endl;
}



extern "C" bool plugin_entry(ouroboros_server& aServer)
{
	cout << "Initializing plugin..." << endl;

	aServer.register_callback("", "a_number", ::callback);
	aServer.register_callback("group_1-group_2", "a_string_inside_group_2", ::callback);

	cout << "Done initializing." << endl;
	return true;
}

