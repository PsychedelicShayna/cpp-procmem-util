#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

class Trainer {
private:  virtual void __interface__() = 0;
protected:
	unsigned __int64	m_base_address;
	char*				m_window_title;
	char*				m_process_title;
	void*				m_process_handle;
	HWND__*				m_process_window;
	unsigned __int16	m_process_id;

	unsigned __int64 discover_base_address_(char* process_title, unsigned __int16 process_id) const {
		unsigned __int64 base_address = 0;

		void* handle_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
		if (handle_snapshot == INVALID_HANDLE_VALUE) return EXIT_FAILURE;

		MODULEENTRY32 module_entry_vector;
		module_entry_vector.dwSize = sizeof(module_entry_vector);
		if (!Module32First(handle_snapshot, &module_entry_vector)) return EXIT_FAILURE;
		do {
			if (!_stricmp(module_entry_vector.szModule, process_title)) {
				base_address = (unsigned __int64)module_entry_vector.modBaseAddr;
				break;
			}
		} while (Module32Next(handle_snapshot, &module_entry_vector));
		CloseHandle(handle_snapshot);

		return base_address;
	}

	unsigned __int64 discover_dynamic_address_(unsigned __int64 static_address, std::vector<unsigned __int64> offsets) const {
		unsigned __int64 pointer_buffer = 0;
		unsigned __int64 trailing_offset = offsets.at(offsets.size() - 1);

		ReadProcessMemory(m_process_handle, (void*)(m_base_address + static_address), &pointer_buffer, sizeof(pointer_buffer), 0);

		for (std::size_t i = 0; i < offsets.size() - 1; i++) {
			const unsigned __int64& current_offset = offsets.at(i);
			ReadProcessMemory(m_process_handle, (void*)(pointer_buffer + current_offset), &pointer_buffer, sizeof(pointer_buffer), 0);
		}

		pointer_buffer = pointer_buffer == 0 ? 0 : pointer_buffer + trailing_offset;
		return pointer_buffer;
	}

	__int64 read_address_data_(unsigned __int64 address) const {
		__int64 address_data = 0;
		ReadProcessMemory(m_process_handle, (void*)address, &address_data, sizeof(address_data), 0);
		return address_data;
	}

	void write_address_value_(unsigned __int64 address, __int64 data) const {
		WriteProcessMemory(m_process_handle, (void*)address, &data, sizeof(data), 0);
	}

	void trainer_setup_() {
		m_process_window = FindWindowA(0, m_window_title);
		GetWindowThreadProcessId(m_process_window, (unsigned long*)&m_process_id);
		m_process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, m_process_id);
		m_base_address = discover_base_address_(m_process_title, m_process_id);
	}
};