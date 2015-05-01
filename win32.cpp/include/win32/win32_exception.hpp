#pragma once

#include <stdexcept>
#include <string>

#include <Windows.h>

#include "includes.hpp"

namespace win32
{
	// Use to ensure LocalFree is called for resources allocated by win32 functions.
	// Non-copyable.
	template<typename resource_type>
	class win32_resource_guard
	{
	public:
		// Creates a memory guard around the specified resource.
		win32_resource_guard(resource_type resource) :
			resource(resource)
		{

		}

		// 
		win32_resource_guard(const win32_resource_guard&) = delete;

		// 
		win32_resource_guard& operator=(const win32_resource_guard&) = delete;

		// Releases the resource.  If LocalFree fails, something much worse has probably happened.
		~win32_resource_guard()
		{
			this->resource = static_cast<resource_type>(
				::LocalFree(static_cast<HLOCAL>(this->resource)));
		}

		// Optionally releases the resource.  If LocalFree fails, throws a win32_exception.
		void release()
		{
			this->resource = static_cast<resource_type>(
				::LocalFree(static_cast<HLOCAL>(this->resource)));
			//if (resource != nullptr)
			//	throw get_last_win32_exception();
		}
	private:
		// 
		resource_type resource;
	};


	// Converts an error code created from GetLastError() into an error message.
	std::string get_win32_message(DWORD error_code)
	{
		LPSTR buffer = nullptr;
		DWORD buffer_size = ::FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			error_code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buffer,
			0,
			nullptr);

		if (buffer_size > 0)
		{
			win32_resource_guard<LPSTR> guard(buffer);
			return std::string(buffer, buffer_size);
		}
		else
		{
			return std::string("Unknown Error, ::FormatMessageA failed.");
		}
	}

	// 
	std::string get_last_win32_error_message()
	{
		return get_win32_message(::GetLastError());
	}

	// Defines an exception based on Win32 error codes.
	class win32_exception : public std::runtime_error
	{
	public:
		// Creates a new win32_exception
		win32_exception(DWORD error_code) :
			std::runtime_error(get_win32_message(error_code)),
			error_code(error_code)
		{

		}
	private:
		DWORD error_code;
	};

	// 
	win32_exception get_last_win32_exception()
	{
		return win32_exception(::GetLastError());
	}
}