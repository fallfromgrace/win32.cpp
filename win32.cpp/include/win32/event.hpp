#pragma once

#include <memory>

#include <Windows.h>

#include "includes.hpp"
#include "logging\logging.hpp"

#include "exception.hpp"

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
					log_error(
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
				check_result(result);
			}

			// 
			void set() const
			{
				BOOL result = ::SetEvent(this->event_handle);
				check_result(result);
			}

			// 
			void reset() const
			{
				BOOL result = ::ResetEvent(this->event_handle);
				check_result(result);
			}

			// 
			void wait_one() const
			{
				DWORD result = ::WaitForSingleObject(this->event_handle, INFINITE);
				check_result(result);
			}

			// 
			void wait_one(const std::chrono::milliseconds& timeout) const
			{
				DWORD result = ::WaitForSingleObject(
					this->event_handle,
					static_cast<DWORD>(timeout.count()));
				check_result(result);
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
					throw get_last_win32_exception();
			}

			// 
			static inline void check_result(HANDLE result)
			{
				if (result == nullptr)
					throw get_last_win32_exception();
			}

			// 
			static inline void check_result(DWORD result)
			{
				switch (result)
				{
				case WAIT_OBJECT_0: break;
				case WAIT_ABANDONED: break;
				case WAIT_TIMEOUT: break;
				case WAIT_FAILED: throw win32::get_last_win32_exception();
				default: break;
				}
			}

			HANDLE event_handle;
		};
	}

	// 
	typedef detail::event<false> auto_reset_event;

	// 
	typedef detail::event<true> manual_reset_event;

	typedef std::shared_ptr<manual_reset_event> manual_reset_event_ptr;

	manual_reset_event_ptr make_manual_reset_event_ptr()
	{
		return std::make_shared<manual_reset_event>();
	}

	//namespace detail
	//{
	//	template<typename rep, typename period, typename... win32_event_type>
	//	void wait_for_multiple_objects(
	//		BOOL wait_all,
	//		const std::chrono::duration<rep, period>& timeout,
	//		win32_event_type&&... win32_events)
	//	{
	//		std::array<HANDLE, sizeof...(win32_events)> handles = { win32_events.handle()... };
	//		DWORD result = ::WaitForMultipleObjects(
	//			sizeof...(win32_events),
	//			handles.data(),
	//			wait_all,
	//			std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count());
	//		switch (result)
	//		{
	//			case 
	//		}
	//	}
	//	template<typename... win32_event_type>
	//	void wait_for_multiple_objects(
	//		BOOL wait_all,
	//		win32_event_type&&... win32_events)
	//	{
	//		wait_for_multiple_objects(
	//			wait_all,
	//			std::chrono::milliseconds::max()),
	//			std::forward<win32_event_type>(win32_events)...);
	//	}
	//}

	//template<typename... win32_event_type>
	//void wait_any(win32_event_type... win32_events)
	//{
	//	detail::wait_for_multiple_objects(
	//		false, 
	//		std::forward<win32_event_type>(win32_events)...);
	//}

	//template<typename rep, typename period, typename... win32_event_type>
	//void wait_any(
	//	const std::chrono::duration<rep, period>& timeout, 
	//	win32_event_type... win32_events)
	//{
	//	detail::wait_for_multiple_objects(
	//		false,
	//		timeout, 
	//		std::forward<win32_event_type>(win32_events)...);
	//}

	//template<typename... win32_event_type>
	//void wait_all(win32_event_type... win32_events)
	//{
	//	detail::wait_for_multiple_objects(
	//		true, 
	//		std::forward<win32_event_type>(win32_events)...);
	//}
}