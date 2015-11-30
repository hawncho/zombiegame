/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef ACTIONDATA_H_
#define ACTIONDATA_H_

class ActionData
{
	public:
		ActionData(int ticksPerAction = 64)
		{
			m_TicksPerAction = ticksPerAction;
			m_PreviousActionTick = -m_TicksPerAction;
			m_IsPerformingAction = false;
		}

		void PerformAction(int currentTick)
		{
			m_PreviousActionTick = currentTick;
			m_IsPerformingAction = true;
		}

		void UpdateAction(int currentTick)
		{
			m_IsPerformingAction = false;
		}

		bool IsActionCompleted(int currentTick)
		{
			if (currentTick - m_PreviousActionTick > m_TicksPerAction)
			{
				return true;
			}

			return false;
		}

		void SetActionSpeed(int ticksPerAction)
		{
			m_TicksPerAction = ticksPerAction;
			m_PreviousActionTick = -m_TicksPerAction;
		}

		void Reset()
		{
			m_IsPerformingAction = false;
			m_PreviousActionTick = -m_TicksPerAction;
		}

		bool IsPerformingAction()
		{
			return m_IsPerformingAction;
		}

	private:
		int m_TicksPerAction;
		int m_PreviousActionTick;
		bool m_IsPerformingAction;
};

/*
class ActionData
{
	public:
		ActionData(int ticksPerAction = 64)
		{
			m_TicksPerAction = ticksPerAction;
			m_PreviousActionTick = -m_TicksPerAction;
			m_IsPerformingAction = false;
		}

		bool CanPerformAction(int currentTick)
		{
			if (currentTick - m_PreviousActionTick > m_TicksPerAction)
			{
				return true;
			}

			return false;
		}

		void PerformAction(int currentTick)
		{
			if (CanPerformAction(currentTick))
			{
				m_PreviousActionTick = currentTick;

				m_IsPerformingAction = true;
			}
			else
			{
				StopAction();
			}
		}

		void StopAction()
		{
			m_IsPerformingAction = false;
		}

		void Reset()
		{
			m_IsPerformingAction = false;
			m_PreviousActionTick = -m_TicksPerAction;
		}

		bool IsPerformingAction()
		{
			return m_IsPerformingAction;
		}

		void SetActionSpeed(int ticksPerAction)
		{
			m_TicksPerAction = ticksPerAction;
			m_PreviousActionTick = -m_TicksPerAction;
		}

	private:
		int m_TicksPerAction;
		int m_PreviousActionTick;
		bool m_IsPerformingAction;
};
*/

#endif /* ACTIONDATA_H_ */