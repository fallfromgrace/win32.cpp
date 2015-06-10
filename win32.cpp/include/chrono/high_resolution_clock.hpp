#pragma once

#include <chrono>
#include <Windows.h>

#include "includes.hpp"

#include "win32\error.hpp"

namespace more
{
	namespace chrono
	{
		namespace detail
		{
			static uint64_t cached_frequency = 0;

			static inline void query_frequency()
			{
				LARGE_INTEGER frequency;
				if (::QueryPerformanceFrequency(&frequency) == FALSE)
					throw win32::get_last_error();
				cached_frequency = static_cast<uint64_t>(frequency.QuadPart);
			}

			// 
			static inline uint64_t frequency()
			{
				if (cached_frequency == 0)
					query_frequency();

				return cached_frequency;
			}
		}

		// 
		class high_resolution_clock
		{
		public:
			typedef uint64_t rep;
			typedef std::nano period;
			typedef std::chrono::duration<rep, period> duration;
			typedef std::chrono::time_point<high_resolution_clock> time_point;
			static const bool_t is_steady = true;

			// 
			static time_point now()
			{
				LARGE_INTEGER count;
				if (::QueryPerformanceCounter(&count) == FALSE)
					throw win32::get_last_error();
				return time_point(duration(
					count.QuadPart * static_cast<rep>(period::den) / detail::frequency()));
			}
		};
	}
}