#include "winmem.hxx"

HWND__* Winmem::WindowHandleFromWindowTitle(char* window_title) {
    HWND__* window_handle = FindWindowA(NULL, window_title);
    if(!IsWindow(window_handle)) window_handle = nullptr;
    return window_handle;
}

uint16_t Winmem::ProcessIdFromProcessImage(char* process_image) {
    void* snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    
    if(snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 process_entry;
        process_entry.dwSize = sizeof(PROCESSENTRY32);
        
        if(Process32First(snapshot, &process_entry)) {
            do {
                if(!_stricmp(process_entry.szExeFile, process_image)) {
                    uint16_t process_id = static_cast<uint16_t>(process_entry.th32ProcessID);
                    CloseHandle(snapshot);
                    return process_id;
                }            
            } while(Process32Next(snapshot, &process_entry));
        }
    }
    
    return NULL;
}

uint16_t Winmem::ProcessIdFromWindowHandle(HWND__* window_handle) {
    uint16_t process_id = NULL;
    GetWindowThreadProcessId(window_handle, reinterpret_cast<unsigned long*>(&process_id));
    return process_id;  
}

HANDLE Winmem::ProcessHandleFromProcessId(const uint16_t& process_id) {
    return OpenProcess(PROCESS_ALL_ACCESS, NULL, process_id);
}

address_t Winmem::ModuleAddressFromModuleName(char* module_name, const uint16_t& process_id) {
    void* snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
    
    if(snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 module_entry;
        module_entry.dwSize = sizeof(MODULEENTRY32);
        
        if(Module32First(snapshot, &module_entry)) {
            do {
                if(!_stricmp(module_entry.szModule, module_name)) {
                    address_t module_address = reinterpret_cast<address_t>(module_entry.modBaseAddr);
                    CloseHandle(snapshot);
                    return module_address;
                }
            } while(Module32Next(snapshot, &module_entry));
        }    
    }
    
    return NULL;
}

address_t Winmem::ResolveDynamicAddress(const address_t& static_address, const address_t& static_offset, const std::vector<address_t>& pointer_offsets, HANDLE process_handle) {
    address_t dynamic_address = NULL;
    address_t trailing_offset = pointer_offsets[pointer_offsets.size()-1];
    
    ReadProcessMemory(
        process_handle,
        reinterpret_cast<void*>(static_address + static_offset),
        &dynamic_address,
        sizeof(address_t),
        NULL 
    );
    
    for(std::size_t i=0; i<pointer_offsets.size()-1; ++i) {
        const address_t pointer_offset = pointer_offsets[i];
        
        ReadProcessMemory(
            process_handle,
            reinterpret_cast<void*>(dynamic_address + pointer_offset),
            &dynamic_address,
            sizeof(address_t),
            NULL
        );
    }
    
    if(dynamic_address != NULL) dynamic_address += trailing_offset;
    return dynamic_address;
}

char* Winmem::ReadCStringFromMemory(const address_t& address, HANDLE process_handle) {
    const std::size_t& cstring_length = Winmem::ReadStrlenFromMemory(address, process_handle);
    char* cstring_buffer = new char[cstring_length+1];
    ZeroMemory(cstring_buffer, cstring_length+1);
    
    ReadProcessMemory(
        process_handle,
        reinterpret_cast<void*>(address),
        cstring_buffer,
        cstring_length,
        NULL
    );
    
    return cstring_buffer;
}

std::size_t Winmem::ReadStrlenFromMemory(const address_t& address, HANDLE process_handle) {
    std::size_t string_length = 0;
    char last_read_char = 0x00;
    
    do {
        ReadProcessMemory(
            process_handle,
            reinterpret_cast<void*>(address+string_length),
            &last_read_char,
            sizeof(char),
            NULL
        );
        
        if(last_read_char != 0x00) ++string_length;
        
    } while(last_read_char != 0x00);
    
    return string_length;
}