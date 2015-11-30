/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef COLLIDER_H_
#define COLLIDER_H_

template <typename T> 
class Collider {
	public:
		Collider()
		{
			m_pCollisionObject = new T();
		}

		~Collider()
		{
			delete m_pCollisionObject;
			m_pCollisionObject = nullptr;
		}

		T* GetCollisionObject()
		{
			return m_pCollisionObject;
		}

	protected:
		T* m_pCollisionObject;
};

#endif /* COLLIDER_H_ */