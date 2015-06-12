#ifndef _OUROBOROS_SUBJECT_H_
#define _OUROBOROS_SUBJECT_H_

#include <vector>
#include <functional>

namespace ouroboros
{
	class subject
	{
	public:
		/**	Registers a functor with the subject.
		 *
		 *	@param [in] aObserver Observer functor to call on a notification.
		 *		functor should act like a void() function.
		 */
		template <class Observer>
		void registerObserver(const Observer& aObserver);

		//TODO provide a way to unregister an observer

		/**	Notifies all registered observer by calling their functors.
		 *
		 */
		void notify() const;

	private:
		std::vector<std::function<void()>> mObservers;
	};
}

#endif//_OUROBOROS_SUBJECT_H_
