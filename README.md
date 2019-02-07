# Memory Interface
This is a single header file, containing an interface (abstract class) that should be inherited from in order to use it. The interface contains methods designed to aid in accessing the memory of another process. This makes heavy use of the Windows.h and TlHelp32.h header and does not work on Unix based systems. This is exclusively a Windows interface. 

Examples usages can be found in the examples folder.
