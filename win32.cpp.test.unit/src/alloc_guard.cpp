#include "CppUnitTest.h"

#include "win32\alloc_guard.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace win32
{
	TEST_CLASS(alloc_guard_tests)
	{
	public:

		TEST_METHOD(local_alloc_guard_release)
		{
			auto hlocal = ::LocalAlloc(LMEM_FIXED, 64);
			local_alloc_guard guard(hlocal);
			guard.release();
		}

		TEST_METHOD(local_alloc_guard_destructor)
		{
			auto hlocal = ::LocalAlloc(LMEM_FIXED, 64);
			local_alloc_guard guard(hlocal);
		}
	};
}