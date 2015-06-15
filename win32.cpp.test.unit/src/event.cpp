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
			e.wait();
			Assert::ExpectException<std::runtime_error>([&]()
			{
				e.wait(std::chrono::milliseconds(75));
			});
		}

		TEST_METHOD(manual_reset_event_signal)
		{
			manual_reset_event e;
			e.set();
			e.wait();
			e.wait(std::chrono::milliseconds(75));
			e.reset();
			Assert::ExpectException<std::runtime_error>([&]()
			{
				e.wait(std::chrono::milliseconds(75));
			});
		}

		TEST_METHOD(wait_any_events)
		{
			manual_reset_event e1, e2, e4;
			auto_reset_event e3;
			std::thread test([&]()
			{
				e2.set();
				e4.set();
				std::this_thread::sleep_for(std::chrono::milliseconds(75));
				e3.set();
				std::this_thread::sleep_for(std::chrono::milliseconds(75));
				e1.set();
			});

			auto index = wait_any(e1, e2, e3, e4);
			Assert::IsTrue(index == 1);
			test.join();
		}

		TEST_METHOD(wait_all_events)
		{
			manual_reset_event e1, e2, e4;
			auto_reset_event e3;
			std::thread test([&]()
			{
				e2.set();
				std::this_thread::sleep_for(std::chrono::milliseconds(75));
				e3.set();
				e4.set();
				std::this_thread::sleep_for(std::chrono::milliseconds(75));
				e1.set();
			});

			wait_all(e1, e2, e3, e4);
			test.join();
		}
	};
}