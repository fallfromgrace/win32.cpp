#include "CppUnitTest.h"

#include <cstdio>
#include <type_traits>
#include "win32\event.hpp"
#include "win32\event_observable.hpp"
#include "more\string.hpp"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace win32
{
	TEST_CLASS(event_observable_tests)
	{
	public:

		TEST_METHOD(subscribe)
		{
			auto_reset_event e1, e2, e3, e4;

			int32_t set = 0;
			
			auto observable = from_any_event_signalled({ e1.handle(), e2.handle(), e3.handle() });
			observable
				.subscribe_on(rxcpp::observe_on_new_thread())
				.subscribe([&](int32_t index)
			{
				set += (index + 1);
			}, [&]()
			{
				e4.set();
			});

			e1.set();
			std::this_thread::sleep_for(std::chrono::milliseconds(150));

			e2.set();
			e1.set();
			std::this_thread::sleep_for(std::chrono::milliseconds(150));

			e3.set();
			std::this_thread::sleep_for(std::chrono::milliseconds(150));

			try
			{
				e4.wait(std::chrono::milliseconds(1000));
				Assert::IsTrue(set == 4);
			}
			catch (const std::exception& ex)
			{
				Assert::Fail(more::to_wstring(ex.what()).c_str());
			}
		}
	};
}