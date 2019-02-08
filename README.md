# Memory Interface
This is a single header file, containing an interface (abstract class) that should be inherited from in order to use it. The interface contains methods designed to aid in accessing the memory of another process. This makes heavy use of the Windows.h and TlHelp32.h header and does not work on Unix based systems. This is exclusively a Windows interface. 

Examples usages can be found in the [examples](https://github.com/Shayna-x02/MemoryInterface/tree/master/source/examples) folder. There is also a small [Wiki](https://github.com/Shayna-x02/MemoryInterface/wiki) at your disposal which contains a reference of all methods, as well as class usage instructions. 
