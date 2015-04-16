/************************************************************************
							HashTable.h
Author: Johnathan O'Malia © 2015
Description:	Template for storage, management, and deletion of resources.
				Stores texture and provides data necessary for resource manager

TODO:
replace map with custom hash table implementation
figure out interator;

/************************************************************************/

#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

//C++ Headers
#include <map>

template<class Key, class T>
class HashTable
{
public:
	HashTable(){};
	~HashTable(){};

	void Clear();

	T	GetValue(Key key);
	Key GetKey(T value);

	std::pair<Key, T> Find(Key key);
	std::pair<Key, T> Find(T value);

	bool Exists(Key key);
	bool Exists(T value);

	void Insert(Key key, T value);

	void Remove(Key key);
	void Remove(T value);

	bool IsEmpty();
	int GetCount();
	
	std::map<Key, T> getMap(){return dataTable;	};
	

protected:
	std::map<Key, T> dataTable;
};

template<class Key, class T>
void HashTable<Key,T>::Clear()
{
	for (auto& kv : dataTable)
	{
		delete kv.second;
		kv.second = NULL;
	}

	dataTable.clear();
}

template<class Key, class T>
T HashTable<Key, T>::GetValue(Key key)
{
	for (auto& kv : dataTable)
	{
		if (kv.first == key)
		{
			return kv.second;
		}
	}
	return nullptr;
}

template<class Key, class T>
Key HashTable<Key, T>::GetKey(T Value)
{
	for (auto& kv : dataTable)
	{
		if (kv.second == value)
		{
			return kv.first;
		}
	}
	return nullptr;
}

template<class Key, class T>
std::pair<Key, T> HashTable<Key, T>::Find(Key key)
{
	for (auto& kv : dataTable)
	{
		if (kv.first == key)
		{
			return kv;
		}
	}
	return nullptr;
}

template<class Key, class T>
std::pair<Key, T>  HashTable<Key, T>::Find(T Value)
{
	for (auto& kv : dataTable)
	{
		if (kv.second == value)
		{
			return kv;
		}
	}
	return nullptr;
}

template<class Key, class T>
bool HashTable<Key, T>::Exists(Key key)
{
	for (auto& kv : dataTable)
	{
		if (kv.first == key)
		{
			return true;
		}
	}
	return false;
}

template<class Key, class T>
bool HashTable<Key, T>::Exists(T Value)
{
	for (auto& kv : dataTable)
	{
		if (kv.second == value)
		{
			return true;
		}
	}
	return false;
}

template<class Key, class T>
void HashTable<Key, T>::Insert(Key key, T value)
{
	dataTable.insert(std::pair<Key, T>(key, value));
}

template<class Key, class T>
void HashTable<Key, T>::Remove(Key key)
{
	if (this->Exists(key))
	{
		delete dataTable.at(key);
		dataTable.at(key) = NULL;
		dataTable.erase(key);
	}
}

template<class Key, class T>
void HashTable<Key, T>::Remove(T Value)
{
	if (this->Exists(Value))
	{
		dataTable.erase(Value);

		delete Value;
		Value = NULL;		
	}
}

template<class Key, class T>
bool HashTable<Key, T>::IsEmpty()
{
	return dataTable.empty();
}

template<class Key, class T>
int HashTable<Key, T>::GetCount()
{
	return dataTable.count();
}
#endif HASH_TABLE_H_