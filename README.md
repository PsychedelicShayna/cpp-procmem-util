# Winapi Memory Wrapper
This is a very light wrapper around the Windows API to make it easier to access the memory of other processes. The wrapper comes in three forms, a class, interface, and namespace. The class option is the most outdated, and least recommended. The interface option is in the middle, and the namespace option is the recommended option. 

In the source folder, you'll find three subfolders containing all three versions. If you don't know which one to pick, then pick the namespace version. 

## Building
The wrapper is just a source file / header file combo. The wrapper is light enough as to where you can simply build it along with the rest of your project. If you wish though, you can run the makefile which will generate an object file, and a library file, which you can then link with the rest of your project.

## Documentation
  There is a Wiki that can be found [here](https://github.com/Shayna-x02/MemoryInterface/wiki) however it only covers the old interface version. There are comments in the [namespace version's header](source/Namespace/winmem.hxx) which contains information about what each function does, which I think is enough, as the wrapper is really not too complex.
