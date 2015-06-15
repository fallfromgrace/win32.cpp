#pragma once

#include "win32\high_resolution_clock.hpp"

namespace win32
{
	// 
	class stopwatch
	{
	public:
		typedef high_resolution_clock::duration duration;

		// 
		stopwatch() :
			start_time(duration::zero()),
			stop_time(duration::zero())
		{

		}

		// 
		inline void start()
		{
			this->start_time = high_resolution_clock::now().time_since_epoch();
		}

		// 
		inline void stop()
		{
			this->stop_time = high_resolution_clock::now().time_since_epoch();
		}

		// 
		inline void reset()
		{
			this->start_time = duration::zero();
			this->stop_time = duration::zero();
		}

		// 
		inline void restart()
		{
			this->reset();
			this->start();
		}

		// 
		inline duration elapsed() const
		{
			return stop_time - start_time;
		}
	private:
		duration start_time;
		duration stop_time;
	};
}