/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef OBJECTPOOL_H_
#define OBJECTPOOL_H_

#include <vector>

template <typename T> class ObjectPool {
	public:
		ObjectPool(int poolSize = 1, const char* label = "Object")
		{
			m_Objects.resize(poolSize);
			for (int i = m_Objects.size() - 1; i >= 0; --i)
			{
				m_InactiveObjects.push_back(&(m_Objects[i]));
			}

			m_Label = label;
		}

		~ObjectPool()
		{
			for (unsigned int i = 0; i < m_InactiveObjects.size(); ++i)
			{
				delete m_InactiveObjects[i];
				m_InactiveObjects[i] = nullptr;
			}
			m_InactiveObjects.clear();

			m_Objects.clear();
		}

		T* GetNewObject()
		{
			if (m_InactiveObjects.size() > 0)
			{
				T* inactiveObject = m_InactiveObjects.back();

				m_InactiveObjects.pop_back();

				return inactiveObject;
			}
			else
			{
				fprintf(stderr, "Ran out of %ss in %s pool.\n", m_Label, m_Label);

				system("pause");
				exit(34);
			}
		}

		void RemoveObject(T* inactiveObject)
		{
			m_InactiveObjects.push_back(inactiveObject);
		}

	private:
		std::vector<T> m_Objects;
		std::vector<T*> m_InactiveObjects;

		const char* m_Label;
};

#endif /* OBJECTPOOL_H_ */