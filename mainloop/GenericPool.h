#pragma once
#include <iostream>
#include <vector>
#include <stack>

template <typename T>
struct GenericPool
{
private:
	size_t pool_size; //Taille de la pool
	
	std::vector<
		std::pair<T*, bool> //On utilise ici une pair de T*, bool, afin d'associer à un objet son etat (Used / Not Used)
	> pool;

	int next;
	
public:
	GenericPool() = delete;
	GenericPool(size_t pool_size) : pool_size(pool_size)
	{
		next = 0;
		pool.reserve(this->pool_size + 1);
	}

	bool InitializePool()
	{
		for (int i = 0; i < pool_size; ++i)
		{
			std::pair<T*, bool> obj{
				new T,
				false
			};
			//obj.first = new T;
			//obj.second = false;

			pool.push_back(obj);
		}
		
		return true;
	}

	T* CreateObject()
	{
		std::pair<T*, bool>& frt = pool[next];
		if (frt.second)
		{
			std::cout << "Wait ! Pool is fully used ! " << std::endl;
			return nullptr;
		}
		
		frt.second = true;
		
		next = GetNextFreeObj();
		
		return frt.first;
	}

	void DestroyObject(T* obj)
	{
		int idx = GetIndexOf(obj);

		if(idx == -1)
		{
			std::cout << "ATTENTION ID -1 l'objet n'est pas dans la pool !!! " << std::endl;
			return;
		}

		pool[idx].second = false;
		next = idx;
	}
	
	bool PurgePool()
	{
		for (int i = 0; i < pool_size; ++i)
		{
			if(pool[i].first != nullptr)
			{
				delete pool[i].first;
				pool[i].first = nullptr;
			}
		}

		pool.clear();
	}

private:
	int GetNextFreeObj()
	{
		int tmp = -1;
		
		for(int i = 0; i < pool_size; ++i)
		{
			if(!pool[i].second)
			{
				tmp = i;
				break;
			}
		}
		
		return tmp;
	}

	int GetIndexOf(T* obj)
	{
		int tmp = -1;

		for (int i = 0; i < pool_size; ++i)
		{
			if (pool[i].first == obj)
			{
				tmp = i;
				break;
			}
		}
		
		return tmp;
	}
};


typedef GenericPool<uint32_t> EntityPool;