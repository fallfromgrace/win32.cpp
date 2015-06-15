#pragma once

#include <stdexcept>
#include <string>
#include <Windows.h>

#include "more\includes.hpp"

#include "win32\alloc_guard.hpp"

namespace win32
{
	namespace detail
	{
		// Converts an error into an error message.
		static std::string get_message(DWORD error_code)
		{
			LPSTR buffer = nullptr;
			// Suppress warning because FormatMessage allocates the buffer, but code analysis does not 
			// understand this.
#pragma warning(suppress: 6387)
			DWORD buffer_size = ::FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				error_code,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&buffer),
				0,
				nullptr);

			if (buffer_size > 0)
			{
				local_alloc_guard guard(buffer);
				return std::string(buffer, buffer_size);
			}
			else
			{
				return std::string("::FormatMessageA failed.");
			}
		}
	}

	// Defines an exception based on Win32 error codes.
	class win32_error : public std::runtime_error
	{
	public:
		// Creates a new win32_exception
		win32_error(DWORD error_code) :
			std::runtime_error(detail::get_message(error_code)),
			error_code(error_code)
		{

		}

		//~win32_error()
		//{
		//	::SetLastError(NO_ERROR);
		//}

		// 
		DWORD code() const
		{
			return this->error_code;
		}
	private:
		const DWORD error_code;
	};

	// 
	static win32_error get_last_error()
	{
		return win32_error(::GetLastError());
	}
}