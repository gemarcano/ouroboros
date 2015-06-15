#ifndef _OUROBOROS_PLUGIN_H_
#define _OUROBOROS_PLUGIN_H_

#include <ouroboros/ouroboros_server.h>

namespace ouroboros
{
	//Defines the type of function that plugins must support.
	typedef bool (*plugin_entry_function)(ouroboros_server& aServer);
	typedef void (*plugin_exit_function)(ouroboros_server& aServer);

	/**	Function called by a plugin manager upon loading the plugin.
	 *
	 *	This function is called by a plugin manager upon finding and loading the
	 *	plugin. Plugin designers are free to execute any code they wish in this
	 *	function, but be advised that it is likely that blocking in this
	 *	function will make the entire program hang. As a result, it is advised
	 *	to make the code that executes here simple, and if necessary, spawn
	 *	external threads to do continuous and/or heavy work.
	 *
	 *	Another aspect to note is that the working directory of the plugins
	 *	will be that of the server, not where they are located relative to the
	 *	server.
	 *
	 *	@param [in] aServer A reference to the server object passed in by the
	 *		plugin manager. All public methods defined by the server are
	 *		accessible.
	 *
	 *	@returns True upon success, false upon failure.
	 */
	extern "C" bool plugin_entry(ouroboros_server& aServer);

	/**	Function called by a plugin manager upon unloading the plugin.
	 *
	 *	This function is called by a plugin manager when unloading the loaded
	 *	plugin. Plugin designers are free to execute any code they wish in this
	 *	function, but be advised that it is likely that blocking in this
	 *	function will make the entire program hang. As a result, it is advised
	 *	to make the code that executes here simple. Any resources registered by
	 *	the plugin MUST be unloaded/unregistered. This includes functions
	 *	registered with the server.
	 *
	 *	Another aspect to note is that the working directory of the plugins
	 *	will be that of the server, not where they are located relative to the
	 *	server.
	 *
	 *	@param [in] aServer A reference to the server object passed in by the
	 *		plugin manager. All public methods defined by the server are
	 *		accessible.
	 */
	extern "C" void plugin_exit(ouroboros_server& aServer);

}

#endif//_OUROBOROS_PLUGIN_H_
