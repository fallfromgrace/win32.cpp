#pragma once

#include <Windows.h>
//#include "win32\error.hpp"

namespace win32
{
	// Use to ensure LocalFree is called for resources allocated by win32 functions.
	// Non-copyable.
	class local_alloc_guard
	{
	public:
		// 
		local_alloc_guard() : 
			resource(nullptr)
		{

		}

		// Creates a memory guard around the specified resource.
		template<typename resource_type>
		local_alloc_guard(resource_type resource) :
			resource(static_cast<HLOCAL>(resource))
		{

		}

		// 
		local_alloc_guard(local_alloc_guard&& other) : 
			resource(other.resource)
		{
			other.resource == nullptr;
		}

		// 
		local_alloc_guard& operator=(local_alloc_guard&& other)
		{
			this->release();
			this->resource = other.resource;
			other.resource == nullptr;
			return *this;
		}

		// 
		local_alloc_guard(const local_alloc_guard&) = delete;

		// 
		local_alloc_guard& operator=(const local_alloc_guard&) = delete;

		// Releases the resource.
		~local_alloc_guard()
		{
			try
			{
				this->release();
			}
			catch (...)
			{

			}
		}

		// Optionally releases the resource.  If LocalFree fails, throws a win32_error.
		void release()
		{
			if (this->resource != nullptr)
			{
				this->resource = ::LocalFree(this->resource);
				//if (resource != nullptr)
				//	throw get_last_error();
			}
		}
	private:
		// 
		HLOCAL resource;
	};

	// Use to ensure LocalFree is called for resources allocated by win32 functions.
	// Non-copyable.
	class global_alloc_guard
	{
	public:
		// Creates a memory guard around the specified resource.
		template<typename resource_type>
		global_alloc_guard(resource_type resource) :
			resource(static_cast<HLOCAL>(resource))
		{

		}

		// 
		global_alloc_guard(global_alloc_guard&& other) :
			resource(other.resource)
		{
			other.resource == nullptr;
		}

		// 
		global_alloc_guard& operator=(global_alloc_guard&& other)
		{
			this->release();
			this->resource = other.resource;
			other.resource == nullptr;
			return *this;
		}

		// 
		global_alloc_guard(const global_alloc_guard&) = delete;

		// 
		global_alloc_guard& operator=(const global_alloc_guard&) = delete;

		// Releases the resource.
		~global_alloc_guard()
		{
			try
			{
				this->release();
			}
			catch (...)
			{

			}
		}

		// Releases the resource.
		void release()
		{
			if (this->resource != nullptr)
			{
				this->resource = ::GlobalFree(this->resource);
				//if (resource != nullptr)
				//	throw get_last_error();
			}
		}
	private:
		// 
		HLOCAL resource;
	};
}