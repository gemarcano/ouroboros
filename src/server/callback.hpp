#ifndef _OUROBOROS_CALLBACK_H_
#define _OUROBOROS_CALLBACK_H_

#include <string>

namespace ouroboros
{
	template <typename Item, typename Func>
	class callback
	{
	public:
		
		/**	Constructor.
		 *
		 *	@param [in] aItem Item to execute the callback on.
		 *	@param [in] aFunc Functor to register for the callback. The functor
		 *		must behave as a void(std::string, std::string) functions,
		 *		where the first parameter is the group and the second parameter
		 *		the field that is being observed.
		 */
		callback(Item aItem, Func aFunc);
		
		/**	Function called by others as a callback.
		 *
		 */
		void operator()() const;
		
	private:
		Item mItem;
		Func mFunc;
	};
}

#include "callback.ipp"

#endif//_OUROBOROS_CALLBACK_H_

