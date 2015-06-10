#pragma once

#include "chrono\high_resolution_clock.hpp"

namespace more
{
	namespace chrono
	{
		// 
		class stopwatch
		{
		public:
			typedef typename high_resolution_clock::duration duration;

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
}