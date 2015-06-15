#pragma once

#include <chrono>
#include <Windows.h>

#include "more\includes.hpp"

#include "win32\error.hpp"

namespace win32
{
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
		static inline time_point now()
		{
			LARGE_INTEGER count;
			if (::QueryPerformanceCounter(&count) == FALSE)
				throw win32::get_last_error();
			return time_point(duration(
				count.QuadPart * static_cast<rep>(period::den) / high_resolution_clock::frequency()));
		}

		// Gets the frequency of the high_resolution_clock
		static inline uint64_t frequency()
		{
			static uint64_t cached_frequency = 0;
			if (cached_frequency == 0)
				cached_frequency = query_frequency();

			return cached_frequency;
		}

		// Queries the frequency of the high_resolution_clock
		static inline uint64_t query_frequency()
		{
			LARGE_INTEGER frequency;
			if (::QueryPerformanceFrequency(&frequency) == FALSE)
				throw win32::get_last_error();
			return static_cast<uint64_t>(frequency.QuadPart);
		}
	};
}