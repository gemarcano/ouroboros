#include <ouroboros/data/misc.h>

namespace ouroboros
{
	template<class T>
	group<T>::group(const std::string& aTitle, const std::string& aDescription)
	:base_field(aTitle, aDescription)
	{}

	template<class T>
	void group<T>::add(T *apField)
	{
		if (apField)
		{
			std::string lookupTitle =
				detail::normalize_name(apField->getTitle());
			mItems[lookupTitle] = apField;

			mInsertOrder.push_back(lookupTitle);
		}
	}

	template<class T>
	void group<T>::add(group<T> *apGroup)
	{
		if (apGroup)
		{
			std::string lookupTitle =
				detail::normalize_name(apGroup->getTitle());
			mGroups[lookupTitle] = apGroup;

			mInsertOrder.push_back(lookupTitle);
		}
	}

	template<class T>
	T *group<T>::removeItem(const std::string& aName)
	{
		T *result = NULL;
		if (mItems.count(aName))
		{
			result = mItems.at(aName);
			mItems.erase(aName);
			mInsertOrder.remove(aName);
		}
		return result;
	}

	template<class T>
	group<T> *group<T>::removeGroup(const std::string& aName)
	{
		group<T> *result = NULL;
		if (mGroups.count(aName))
		{
			result = mGroups.at(aName);
			mGroups.erase(aName);
			mInsertOrder.remove(aName);
		}
		return result;
	}

	template<class T>
	T *group<T>::findItem(const std::string& aName)
	{
		return const_cast<T *>(
			static_cast<const group&>(*this).findItem(aName));
	}

	template<class T>
	group<T>* group<T>::findGroup(const std::string& aName)
	{
		return const_cast<group<T> *>(
			static_cast<const group&>(*this).findGroup(aName));
	}

	template<class T>
	const T *group<T>::findItem(const std::string& aName) const
	{
		if (mItems.count(aName))
		{
			return mItems.at(aName);
		}
		return NULL;
	}

	template<class T>
	const group<T>* group<T>::findGroup(const std::string& aName) const
	{
		if (mGroups.count(aName))
		{
			return mGroups.at(aName);
		}
		return NULL;
	}

	template<class T>
	std::size_t group<T>::getSize() const
	{
		return mItems.size() + mGroups.size();
	}

	template<class T>
	std::string group<T>::getJSON() const
	{
		std::string result = "{ ";

		result += "\"type\" : \"group\"";

		std::string base = base_field::getJSON();
		base.erase(base.find_first_of('{'), 1);
		base.erase(base.find_last_of('}'), 1);

		result += ", " + base + ", \"contents\" : {";
		if (!(mGroups.empty() && mItems.empty()))
		{
			typedef std::list<std::string>::const_iterator l_iter;

			for (l_iter itr = mInsertOrder.begin();
				itr != mInsertOrder.end();
				++itr)
			{
				if (mGroups.count(*itr))
				{
					result += " \"" + *itr + "\" : "
						+ mGroups.find(*itr)->second->getJSON() + ",";
				}
				if (mItems.count(*itr))
				{
					result +=  "\"" + *itr + "\" : "
						+ mItems.find(*itr)->second->getJSON() + ",";
				}
			}

			if (!(mGroups.empty() && mItems.empty()))
			{
				result.erase(result.end()-1);
			}
		}

		result += " } }";
		return result;
	}
	
	template<class T>
	std::vector<T*> group<T>::getFields() const
	{
		std::vector<T*> result;
		typedef typename std::map<std::string, T*>::const_iterator iter;
		for (iter itr = mItems.begin(); itr != mItems.end(); ++itr)
		{
			result.push_back(itr->second);
		}
		return result;
	}

	template<class T>
	std::vector<group<T>*> group<T>::getGroups() const
	{
		std::vector<group<T>*> result;
		typedef typename std::map<std::string, group<T>*>::const_iterator iter;
		for (iter itr = mGroups.begin(); itr != mGroups.end(); ++itr)
		{
			result.push_back(itr->second);
		}
		return result;
	}

}
