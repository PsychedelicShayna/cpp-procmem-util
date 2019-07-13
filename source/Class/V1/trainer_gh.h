#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

class Trainer {
protected:
	unsigned __int64	m_base_address;
	char*				m_window_title;
	char*				m_module_title;
	void*				m_process_handle;
	HWND__*				m_process_window;
	unsigned __int16	m_process_id;

	unsigned __int64 discover_base_address_(char* module_title, unsigned __int16 process_id) const {
		unsigned __int64 base_address = 0;

		void* handle_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
		if (handle_snapshot == INVALID_HANDLE_VALUE) return EXIT_FAILURE;

		MODULEENTRY32 module_entry_vector;
		module_entry_vector.dwSize = sizeof(module_entry_vector);
		if (!Module32First(handle_snapshot, &module_entry_vector)) return EXIT_FAILURE;
		do {
			if (!_stricmp(module_entry_vector.szModule, module_title)) {
				base_address = (unsigned __int64)module_entry_vector.modBaseAddr;
				break;
			}
		} while (Module32Next(handle_snapshot, &module_entry_vector));
		CloseHandle(handle_snapshot);

		return base_address;
	}

	unsigned __int64 discover_dynamic_address_(unsigned __int64 static_address, std::vector<unsigned __int64> offsets, unsigned __int64 base_address = 0) const {
		unsigned __int64 pointer_buffer = 0;
		unsigned __int64 trailing_offset = offsets.at(offsets.size() - 1);

		ReadProcessMemory(m_process_handle, (void*)((base_address ? base_address : m_base_address) + static_address), &pointer_buffer, sizeof(pointer_buffer), 0);

		for (std::size_t i = 0; i < offsets.size() - 1; i++) {
			const unsigned __int64& current_offset = offsets.at(i);
			ReadProcessMemory(m_process_handle, (void*)(pointer_buffer + current_offset), &pointer_buffer, sizeof(pointer_buffer), 0);
		}

		pointer_buffer = pointer_buffer == 0 ? 0 : pointer_buffer + trailing_offset;
		return pointer_buffer;
	}

	HWND__* fetch_process_window_(char* window_title) const {
		HWND__* process_window = nullptr;
		process_window = FindWindowA(0, window_title);
		if (!IsWindow(process_window) || process_window == nullptr) {
			throw std::exception("Invalid window. (No window under that title exists?)");
			return nullptr;
		}

		return process_window;
	}

	unsigned __int16 fetch_process_id_(HWND__* process_window) const {
		unsigned __int16 process_id = NULL;
		GetWindowThreadProcessId(m_process_window, (unsigned long*)&process_id);
		if (process_id == NULL)
			throw std::exception("Unable to resolve process ID. Remained NULL.");

		return process_id;
	}

	void* fetch_process_handle_(unsigned __int16 process_id) const {
		void* process_handle = nullptr;
		process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, process_id);
		if (process_handle == nullptr)
			throw std::exception("Failed to get process handle. Handle remained nullptr.");

		return process_handle;
	}

	void trainer_setup_() {
		m_process_window = fetch_process_window_(m_window_title);
		m_process_id = fetch_process_id_(m_process_window);
		m_process_handle = fetch_process_handle_(m_process_id);
	}

public:
	template <typename DataType>
	DataType read_address_data(unsigned __int64 address) const {
		DataType address_data;
		ReadProcessMemory(m_process_handle, (void*)address, &address_data, sizeof(address_data), 0);
		return address_data;
	}

	template <typename DataType>
	void write_address_data(unsigned __int64 address, DataType data) const {
		WriteProcessMemory(m_process_handle, (void*)address, &data, sizeof(data), 0);
	}
};