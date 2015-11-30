/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef GLUTCALLBACKS_H_
#define GLUTCALLBACKS_H_

class ICallbacks
{
	public:
		virtual void DisplayCB() = 0;
		virtual void IdleCB() = 0;
		virtual void KeyboardCB(unsigned char key, int x, int y) = 0;
		virtual void KeyboardUpCB(unsigned char key, int x, int y) = 0;
		virtual void MouseCB(int button, int state, int x, int y) = 0;
		virtual void MovingMouseCB(int x, int y) = 0;
		virtual void PassiveMouseCB(int x, int y) = 0;
};

#endif /* GLUTCALLBACKS_H_ */