#include "CppUnitTest.h"

#include "win32\event.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace win32
{		
	TEST_CLASS(event_tests)
	{
	public:
		
		TEST_METHOD(auto_reset_event_signal)
		{
			auto_reset_event e;
			e.set();
			e.wait_one(std::chrono::milliseconds(100));
			e.wait_one(std::chrono::milliseconds(100));
		}

		TEST_METHOD(manual_reset_event_signal)
		{
			manual_reset_event e;
			e.set();
			e.wait_one(std::chrono::milliseconds(100));
			e.wait_one(std::chrono::milliseconds(100));
		}

	};
}