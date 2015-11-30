/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef EXPIRABLE_H_
#define EXPIRABLE_H_

#define SPAWN_TICK_NOT_SET				-1

class Expirable
{
	public: 
		Expirable()
		{
			m_SpawnTick = SPAWN_TICK_NOT_SET;
			m_LifespanInTicks = 1;
		}

	protected:
		int m_LifespanInTicks;

		void UpdateExpirationTimer(int currentTick)
		{
			if (m_SpawnTick == SPAWN_TICK_NOT_SET)
			{
				m_SpawnTick = currentTick;
			}
		}

		bool IsExpired(int currentTick)
		{
			return currentTick - m_SpawnTick > m_LifespanInTicks;
		}

		void ResetExpirationTimer()
		{
			m_SpawnTick = SPAWN_TICK_NOT_SET;
		}

	private:
		int m_SpawnTick;
};

#endif /* EXPIRABLE_H_ */