#ifndef TRAINER
#define TRAINER

#include <Windows.h>    // The Windows OS API.
#include <TlHelp32.h>	// Additional Windows API functions.
#include <cstdint>   	// Alternative integer definitions.
#include <string>		// std::string type.
#include <vector>		// std::vector type.

#ifndef __WIN64
#define address_t uint32_t
#else
#define address_t uint64_t
#endif

class MemoryInterface {
protected:
    virtual void ABSTRACT() const = 0;

public:
    HWND__* hwnd_fromtitle(char* title) const {
        HWND__* return_buffer = nullptr; 
        
        return_buffer = FindWindowA(0, title); 
        
        if(!IsWindow(return_buffer)) {
            return_buffer = nullptr;
        }
        
        return return_buffer;
    }

    uint16_t processid_fromimage(char* target_image) const {
        uint16_t return_buffer = 0;
        
        void* snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);        
        if(snapshot == INVALID_HANDLE_VALUE) return return_buffer;
        
        PROCESSENTRY32 process; process.dwSize = sizeof(process);
        
        if(!Process32First(snapshot, &process)) {
            return return_buffer; 
        }
        
		do {
            if(!_stricmp(process.szExeFile, target_image)) {
                return_buffer = (uint16_t)process.th32ProcessID;
                break;
            }
        } while(Process32Next(snapshot, &process)); 
        
        CloseHandle(snapshot); 
        
        return return_buffer;
    }

    uint16_t processid_fromhwnd(HWND__* window_handle) const {
        uint16_t return_buffer = 0;        
        GetWindowThreadProcessId(window_handle, (unsigned long*)&return_buffer);
        
        return return_buffer;        
    }
    
	address_t module_address(char* target_module, uint16_t process_id) const {
		address_t return_buffer = 0; 
		
		void* snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);		
		if(snapshot == INVALID_HANDLE_VALUE) return return_buffer;
		
		MODULEENTRY32 module; module.dwSize = sizeof(module);
		
		if(!Module32First(snapshot, &module)) {
            return return_buffer; 
		}
		
		do {
            if(!_stricmp(module.szModule, target_module)) {
                return_buffer = (address_t)module.modBaseAddr; 
                break;
            }
        } while(Module32Next(snapshot, &module)); 
		
		CloseHandle(snapshot); 
		
		return return_buffer;
	}
	
	void* processhandle_frompid(uint16_t process_id, uint32_t access=PROCESS_ALL_ACCESS) const {
	    void* return_buffer = nullptr;   
	    return_buffer = OpenProcess(access, 0, process_id);
	    return return_buffer;
	}	
	
	address_t pointer_chain_resolve(address_t starting_address, address_t module_offset, std::vector<address_t> offsets, void* process_handle) const {
	    address_t return_buffer = 0;
	    address_t trailing_offset = offsets.at(offsets.size()-1);
	    
	    ReadProcessMemory(process_handle, (void*)(module_offset + starting_address), &return_buffer, sizeof(return_buffer), 0);
	    for(std::size_t i=0; i<offsets.size()-1; ++i) {
	        const address_t offset = offsets.at(i);
            ReadProcessMemory(process_handle, (void*)(return_buffer + offset), &return_buffer, sizeof(return_buffer), 0);
	    }
	    
	    if(return_buffer != 0) return_buffer += trailing_offset;
        
        return return_buffer;
	}

	template<typename T>
	T read_address_integral(address_t address, void* process_handle) const {
	    T data_buffer = 0;
	    ReadProcessMemory(process_handle, (void*)address, &data_buffer, sizeof(data_buffer), 0);
	    return data_buffer;
	}
	
	template<typename T>
	void write_address_integral(address_t address, T data, void* process_handle) const {
	    WriteProcessMemory(process_handle, (void*)address, &data, sizeof(data), 0);
	}
	
	std::string read_address_cstring(address_t address, void* process_handle) const {
        std::string return_buffer;
        
        address_t offset = 0;

        while(true) {
            char character = read_address_integral<char>(address+offset, process_handle);
            if(character == '\0') break;
            return_buffer.push_back(character);        
            ++offset;
        }
        
        return return_buffer;        
	}
	
	void write_address_cstring(address_t address, std::string string, void* process_handle) const {
        for(std::size_t i=0; i<string.size(); ++i) {
            char character = string.at(i);
            write_address_integral<char>(address+i, character, process_handle);
        }
    }
};

#endif
