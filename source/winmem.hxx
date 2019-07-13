#ifndef WINMEM_HXX 
#define WINMEM_HXX 

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>

#ifndef __WIN64
#define address_t uint32_t
#else
#define address_t uint64_t
#endif

namespace Winmem {
    /* Finds a window with a matching title, and returns a new handle to it.
    * Nullptr is returned if the handle is invalid. */
    HWND__* WindowHandleFromWindowTitle(char*);

    /* Finds the process ID of a process using the name of its executable.
    * If the process ID could not be found, 0 is returned. */ 
    uint16_t ProcessIdFromProcessImage(char*);
    
    /* Finds the process ID of a process using a handle to one of its windows.
    * If the process Id could not be found, 0 is returned. */
    uint16_t ProcessIdFromWindowHandle(HWND__*);
    
    /* Find the process with a matching process ID, and returns a new handle to it.
    * The handle will be a nullptr if there is an error. */
    HANDLE ProcessHandleFromProcessId(const uint16_t&);    
    
    /* Returns the internal address of a module within the running process using
    * the name of the module. 0 Is returned if there was an error. The second
    * argument is the process ID of the process to look through. */   
    address_t ModuleAddressFromModuleName(char*, const uint16_t&);
    
    /* Resolves a dynamic address using a static starting point, a static module address, and a vector
    * of static offsets used to find the pointers that lead to the dynamic address. Returns 0 if there
    * was an error. Warning: A non-zero return value does not necessarily indicate success. */
    address_t ResolveDynamicAddress(const address_t&, const address_t&, const std::vector<address_t>&, HANDLE);

    /* Reads a null terminated c-string of an unknown size from the memory of another process. */
    char* ReadCStringFromMemory(const address_t&, HANDLE);
    
    /* Reads the length of a null terminated c-string from the memory of another process. */
    std::size_t ReadStrlenFromMemory(const address_t&, HANDLE);
    
    /* Reads a primitive from a memory address of another process. The primitive
    * type is the template parameter. */
    template<typename T>
    T ReadPrimitiveFromMemory(const address_t& address, HANDLE process_handle) {
        T address_data_buffer;
        
        ReadProcessMemory(
            process_handle,
            reinterpret_cast<void*>(address), 
            &address_data_buffer, 
            sizeof(address_data_buffer), 
            NULL
        );
        
        return address_data_buffer;
    }
    
    /* Writes a primitive to a memory address of another process. The primitive
    * type is the template parameter. */
    template<typename T>
    void WritePrimitiveToMemory(const address_t& address, HANDLE process_handle, T value) {
        WriteProcessMemory(
            process_handle,
            reinterpret_cast<void*>(address),
            &value,
            sizeof(value),
            NULL
        );
    }
    
    /* Reads an array of primitives from the memory address of another process. The
    * the primitive type is the template parameter. */
    template<typename T>
    void ReadMultiplePrimitives(const address_t& address, HANDLE process_handle, const std::size_t& amount, T* output) {
        ReadProcessMemory(
            process_handle,
            reinterpret_cast<T*>(address),
            output,
            sizeof(T)*amount,
            NULL
        );
    }    
    
    /* Writes an array of primitives to the memory address of another process. The
    * the primitive type is the template parameter. */
    template<typename T>
    void WriteMultiplePrimitives(const address_t& address, HANDLE process_handle, T* data, const std::size_t& amount) {
        WriteProcessMemory(
            process_handle,
            reinterpret_cast<T*>(address),
            data,
            sizeof(T)*amount,
            NULL
        );
    }
};

#endif