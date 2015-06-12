#include <ouroboros/data/subject.h>

namespace ouroboros
{
	template <class Observer>
	void subject::registerObserver(const Observer& aObserver)
	{
		mObservers.emplace_back(aObserver);
	}

	void subject::notify() const
	{
		for (auto obs : mObservers)
		{
			obs();
		}
	}
}
