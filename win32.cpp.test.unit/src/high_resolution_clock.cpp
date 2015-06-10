#include "CppUnitTest.h"

#include "chrono\high_resolution_clock.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace win32
{
	TEST_CLASS(high_resolution_clock_tests)
	{
	public:

		TEST_METHOD(now)
		{
			auto time = more::chrono::high_resolution_clock::now();
			Assert::IsTrue(time.time_since_epoch().count() > 0);
		}

	};
}