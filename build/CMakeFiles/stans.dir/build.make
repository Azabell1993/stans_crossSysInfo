# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.30.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.30.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/build"

# Include any dependencies generated for this target.
include CMakeFiles/stans.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/stans.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/stans.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stans.dir/flags.make

CMakeFiles/stans.dir/src/main.cpp.o: CMakeFiles/stans.dir/flags.make
CMakeFiles/stans.dir/src/main.cpp.o: /Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/src/main.cpp
CMakeFiles/stans.dir/src/main.cpp.o: CMakeFiles/stans.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/stans.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/stans.dir/src/main.cpp.o -MF CMakeFiles/stans.dir/src/main.cpp.o.d -o CMakeFiles/stans.dir/src/main.cpp.o -c "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/src/main.cpp"

CMakeFiles/stans.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/stans.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/src/main.cpp" > CMakeFiles/stans.dir/src/main.cpp.i

CMakeFiles/stans.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/stans.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/src/main.cpp" -o CMakeFiles/stans.dir/src/main.cpp.s

# Object files for target stans
stans_OBJECTS = \
"CMakeFiles/stans.dir/src/main.cpp.o"

# External object files for target stans
stans_EXTERNAL_OBJECTS =

stans: CMakeFiles/stans.dir/src/main.cpp.o
stans: CMakeFiles/stans.dir/build.make
stans: libedge_client.a
stans: /usr/local/lib/libboost_system.dylib
stans: /usr/local/lib/libboost_thread.dylib
stans: CMakeFiles/stans.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable stans"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stans.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stans.dir/build: stans
.PHONY : CMakeFiles/stans.dir/build

CMakeFiles/stans.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/stans.dir/cmake_clean.cmake
.PHONY : CMakeFiles/stans.dir/clean

CMakeFiles/stans.dir/depend:
	cd "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)" "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)" "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/build" "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/build" "/Users/azabell/Desktop/workspace/stans/client(request_crosscompile)/build/CMakeFiles/stans.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/stans.dir/depend

