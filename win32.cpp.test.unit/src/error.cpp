#include "CppUnitTest.h"

#include "win32\error.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace win32
{
	TEST_CLASS(error_tests)
	{
	public:

		TEST_METHOD(get_last_error_none)
		{
			auto error = get_last_error();
			Assert::IsTrue(error.code() == 0);
			Assert::IsTrue(error.what() != nullptr);
		}

		TEST_METHOD(get_last_error_set)
		{
			::SetLastError(-1);
			auto error = get_last_error();
			Assert::IsTrue(error.code() == -1);
			Assert::IsTrue(error.what() != nullptr);
		}

	};
}