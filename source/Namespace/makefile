builds/winmem.lib: objects/winmem.o
	mkdir -p builds
	llvm-ar rcs builds/winmem.lib objects/winmem.o
    
objects/winmem.o: winmem.cxx
	mkdir -p objects
	clang++ -c winmem.cxx -o objects/winmem.o -std=c++17   