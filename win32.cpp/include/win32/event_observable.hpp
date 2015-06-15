#pragma once

#include "rx.hpp"

#include "win32\error.hpp"

namespace win32
{
	// Creates an observable sequence from the specified wait handles that signals when any
	// handle is signalled.  The last handle in the sequence completes the sequence.
	rxcpp::observable<int32_t> from_any_event_signalled(const std::vector<HANDLE>& wait_handles)
	{
		return rxcpp::observable<>::create<int32_t>([=](const rxcpp::subscriber<int32_t>& o)
		{
			for (;;)
			{
				DWORD wait_result = ::WaitForMultipleObjects(
					static_cast<DWORD>(wait_handles.size()),
					wait_handles.data(),
					false, INFINITE);

				if (wait_result == WAIT_FAILED)
				{
					o.on_error(std::make_exception_ptr(get_last_error()));
					return;
				}
				else if (wait_result == WAIT_TIMEOUT)
				{
					o.on_error(std::make_exception_ptr(get_last_error()));
					return;
				}
				else if ((wait_result & WAIT_ABANDONED_0) == WAIT_ABANDONED_0)
				{
					o.on_error(std::make_exception_ptr(get_last_error()));
					return;
				}
				else if ((wait_result & WAIT_OBJECT_0) == WAIT_OBJECT_0)
				{
					if (wait_result - WAIT_OBJECT_0 == wait_handles.size() - 1)
					{
						o.on_completed();
						return;
					}
					else
					{
						o.on_next(wait_result - WAIT_OBJECT_0);
					}
				}
				else
				{
					o.on_error(std::make_exception_ptr(get_last_error()));
					return;
				}
			}
		});
		//.subscribe_on(rxcpp::observe_on_new_thread());
	}
}