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
			::SetLastError(0);
			auto error = get_last_error();
			Assert::IsTrue(error.code() == 0);
			Assert::IsTrue(error.what() != nullptr);
			auto message = std::string(error.what());
			auto compare = message.compare("::FormatMessageA failed.");
			Assert::IsTrue(compare != 0);
		}

		TEST_METHOD(get_last_error_set)
		{
			const DWORD error_code = 6;
			::SetLastError(error_code);
			auto error = get_last_error();
			Assert::IsTrue(error.code() == error_code);
			Assert::IsTrue(error.what() != nullptr);
			auto message = std::string(error.what());
			auto compare = message.compare("::FormatMessageA failed.");
			::SetLastError(0);
			Assert::IsTrue(compare != 0);
		}

	};
}