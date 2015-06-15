#pragma once

#include <chrono>
#include <Windows.h>

#include "more\includes.hpp"
#include "more\log.hpp"
#include "more\string.hpp"

#include "win32\error.hpp"

namespace win32
{
	namespace detail
	{
		// Wraps Win32 events in a more idiomatic C++ RAII style class.
		template<bool_t manual_reset>
		class event
		{
		public:
			// 
			event() : 
				event(false)
			{

			}

			// 
			event(bool_t initial_state) : 
				event_handle(::CreateEvent(
					nullptr,
					static_cast<BOOL>(manual_reset),
					static_cast<BOOL>(initial_state),
					nullptr))
			{
				check_result(this->event_handle);
			}

			// 
			event(const event&) = delete;

			// 
			event& operator=(const event&) = delete;

			// 
			event(event&& other) :
				event_handle(other.event_handle)
			{
				other.event_handle = nullptr;
			}

			// 
			event& operator=(event&& other)
			{
				this->close();
				this->event_handle = other.event_handle;
				other.event_handle = nullptr;
				return *this;
			}

			// 
			~event()
			{
				try
				{
					this->close();
				}
				catch (const std::exception& ex)
				{
					more::error(
						"::CloseHandle(%i) failed in ~event(): %s",
						this->event_handle,
						ex.what());
				}
			}

			// 
			void close()
			{
				// Event handle is already closed or never initialized.
				if (this->event_handle == nullptr)
					return;

				BOOL result = ::CloseHandle(this->event_handle);
				this->event_handle = nullptr;
				event::check_result(result);
			}

			// 
			void set() const
			{
				BOOL result = ::SetEvent(this->event_handle);
				event::check_result(result);
			}

			// 
			void reset() const
			{
				BOOL result = ::ResetEvent(this->event_handle);
				event::check_result(result);
			}

			// 
			void wait() const
			{
				DWORD result = ::WaitForSingleObject(this->event_handle, INFINITE);
				event::check_result(result);
			}

			// 
			template<typename duration>
			void wait(duration timeout) const
			{
				DWORD result = ::WaitForSingleObject(
					this->event_handle,
					static_cast<DWORD>(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count()));
				event::check_result(result);
			}

			// 
			HANDLE handle() const
			{
				return this->event_handle;
			}
		private:
			// 
			static inline void check_result(BOOL result)
			{
				if (result == FALSE)
					throw get_last_error();
			}

			// 
			static inline void check_result(HANDLE result)
			{
				if (result == nullptr)
					throw get_last_error();
			}

			// 
			static inline void check_result(DWORD result)
			{
				switch (result)
				{
				case WAIT_OBJECT_0: break;
				case WAIT_ABANDONED_0: throw std::runtime_error("The wait handle was abandoned.");
				case WAIT_TIMEOUT: throw std::runtime_error("The operation timed out.");
				case WAIT_FAILED: throw get_last_error();
				default: throw std::runtime_error("Unexpected return code.");
				}
			}

			HANDLE event_handle;
		};

		template<typename wait_handle_container>
		DWORD wait_for_multiple_objects(
			BOOL wait_all, 
			DWORD timeout, 
			wait_handle_container&& wait_handles)
		{
			DWORD result = ::WaitForMultipleObjects(
				static_cast<DWORD>(wait_handles.size()),
				wait_handles.data(),
				wait_all,
				timeout);
			if (result == WAIT_TIMEOUT)
				throw std::runtime_error("The operation timed out.");
			else if (result == WAIT_FAILED)
				throw get_last_error();
			else if ((result & WAIT_ABANDONED_0) == WAIT_ABANDONED_0)
				throw std::runtime_error(more::format("Wait handle %i was abandoned.", result - WAIT_ABANDONED));
			else if ((result & WAIT_OBJECT_0) == WAIT_OBJECT_0)
				return result - WAIT_OBJECT_0;
			else
				throw std::runtime_error("Unexpected return code.");
		}

		template<typename... event>
		DWORD wait_for_multiple_objects(BOOL wait_all, DWORD timeout, event&&... events)
		{
			std::array<HANDLE, sizeof...(events)> handles = { events.handle()... };
			return wait_for_multiple_objects(wait_all, timeout, handles);
		}
	}

	// 
	typedef detail::event<false> auto_reset_event;

	// 
	typedef detail::event<true> manual_reset_event;

	template<typename event_container>
	int32_t wait_any0(event_container&& events)
	{
		return static_cast<int32_t>(detail::wait_for_multiple_objects(
			false,
			INFINITE,
			std::forward<event_container>(events)));
	}

	template<typename... event>
	int32_t wait_any(
		event&&... events)
	{
		return static_cast<int32_t>(detail::wait_for_multiple_objects(
			false, 
			INFINITE, 
			std::forward<event>(events)...));
	}

	// MSVC cannot handle template overloads
	template<typename duration, typename... event>
	int32_t wait_any2(
		duration&& timeout,
		event&&... events)
	{
		return static_cast<int32_t>(detail::wait_for_multiple_objects(
			false,
			static_cast<DWORD>(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count()),
			std::forward<event>(events)...));
	}

	template<typename... event>
	void wait_all(
		event&&... events)
	{
		 detail::wait_for_multiple_objects(
			true,
			INFINITE,
			std::forward<event>(events)...);
	}

	// MSVC cannot handle template overloads
	template<typename duration, typename... event>
	void wait_all2(
		duration&& timeout,
		event&&... events)
	{
		detail::wait_for_multiple_objects(
			true,
			static_cast<DWORD>(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count()),
			std::forward<event>(events)...);
	}
}