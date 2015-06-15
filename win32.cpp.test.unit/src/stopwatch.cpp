#include "CppUnitTest.h"

#include "win32\stopwatch.hpp"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace win32
{
	TEST_CLASS(stopwatch_tests)
	{
	public:

		TEST_METHOD(elapsed)
		{
			auto duration = std::chrono::milliseconds(150);
			auto duration_max = duration + std::chrono::milliseconds(16);
			auto duration_min = duration - std::chrono::milliseconds(16);

			win32::stopwatch sw;
			sw.start();
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
			sw.stop();

			Assert::IsTrue(sw.elapsed() < duration_max && sw.elapsed() > duration_min);
		}

		TEST_METHOD(reset)
		{
			auto duration = std::chrono::milliseconds(150);
			auto duration_required = std::chrono::milliseconds(0);

			win32::stopwatch sw;
			sw.start();
			std::this_thread::sleep_for(duration);
			sw.stop();
			sw.reset();

			Assert::IsTrue(sw.elapsed() == duration_required);
		}

		TEST_METHOD(restart)
		{
			auto duration = std::chrono::milliseconds(150);
			auto duration_max = duration + std::chrono::milliseconds(16);
			auto duration_min = duration - std::chrono::milliseconds(16);

			win32::stopwatch sw;
			sw.start();
			std::this_thread::sleep_for(duration);
			sw.stop();
			sw.restart();
			std::this_thread::sleep_for(duration);
			sw.stop();

			Assert::IsTrue(sw.elapsed() < duration_max && sw.elapsed() > duration_min);
		}

	};
}