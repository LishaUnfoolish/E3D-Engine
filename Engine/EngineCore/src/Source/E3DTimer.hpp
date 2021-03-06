//
//  E3DTimer.hpp
//
//  Created by 潘庆云 on 2017/5/9.
//

#ifndef __E3D_TIMER_HPP__
#define __E3D_TIMER_HPP__

#include <stdio.h>
#include "Interface.h"
#include "E3DDelegate.h"

namespace E3DEngine
{
	class Time : public IObject
	{
	public:
		static float deltaTime;
	};

	struct stTimer
	{
		stTimer()
		{
			param = nullptr;
		}
		~stTimer()
		{
			timerCallBack.Cleanup();
		}
		int ID;
		Event::EventDelegates timerCallBack;
		bool isRepeat;
		float timeInterval;
		float tickTime;
		bool isStop;
		IObject * handleClass;
		EventArgs * param;
	};
	
	class Timer : public IObject
	{
	private:
		static Timer & getInstance()
		{
			static Timer timer;
			return timer;
		}
	public:
		static unsigned int	AddTimer(float timeInSec, EventHandleFunction * timerTickCallBack,EventArgs * param, bool isRepeat = false);
		static void StartTimer(unsigned int timerID);
		static void StopAllTimer();
		static void PauseTimer(unsigned int timerID);
		static void StopTimer(unsigned int timerID);
		static void Update(float deltaTime);
		static void Init();
		static void Destory()
		{
			StopAllTimer();
		}

	private:
		std::vector<std::list<stTimer*>> mTimerList;
		std::map<unsigned int, stTimer*> mTimerMap;
		unsigned int m_nTimerID;
		float deltaTime;
		int mIndex;
	};
}

#endif /* E3DTimer_hpp */
