# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "C:/Program Files (x86)/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files (x86)/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Workspace/DiskAccess-VSCode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Workspace/DiskAccess-VSCode/build

# Include any dependencies generated for this target.
include App/CMakeFiles/DiskAccess.dir/depend.make

# Include the progress variables for this target.
include App/CMakeFiles/DiskAccess.dir/progress.make

# Include the compile flags for this target's objects.
include App/CMakeFiles/DiskAccess.dir/flags.make

App/CMakeFiles/DiskAccess.dir/Src/main.cpp.obj: App/CMakeFiles/DiskAccess.dir/flags.make
App/CMakeFiles/DiskAccess.dir/Src/main.cpp.obj: App/CMakeFiles/DiskAccess.dir/includes_CXX.rsp
App/CMakeFiles/DiskAccess.dir/Src/main.cpp.obj: ../App/Src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Workspace/DiskAccess-VSCode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object App/CMakeFiles/DiskAccess.dir/Src/main.cpp.obj"
	cd C:/Workspace/DiskAccess-VSCode/build/App && C:/msys64/mingw32/bin/clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DiskAccess.dir/Src/main.cpp.obj -c C:/Workspace/DiskAccess-VSCode/App/Src/main.cpp

App/CMakeFiles/DiskAccess.dir/Src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DiskAccess.dir/Src/main.cpp.i"
	cd C:/Workspace/DiskAccess-VSCode/build/App && C:/msys64/mingw32/bin/clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Workspace/DiskAccess-VSCode/App/Src/main.cpp > CMakeFiles/DiskAccess.dir/Src/main.cpp.i

App/CMakeFiles/DiskAccess.dir/Src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DiskAccess.dir/Src/main.cpp.s"
	cd C:/Workspace/DiskAccess-VSCode/build/App && C:/msys64/mingw32/bin/clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Workspace/DiskAccess-VSCode/App/Src/main.cpp -o CMakeFiles/DiskAccess.dir/Src/main.cpp.s

# Object files for target DiskAccess
DiskAccess_OBJECTS = \
"CMakeFiles/DiskAccess.dir/Src/main.cpp.obj"

# External object files for target DiskAccess
DiskAccess_EXTERNAL_OBJECTS =

../bin/App/DiskAccess.exe: App/CMakeFiles/DiskAccess.dir/Src/main.cpp.obj
../bin/App/DiskAccess.exe: App/CMakeFiles/DiskAccess.dir/build.make
../bin/App/DiskAccess.exe: ../bin/Lib/libDisk.dll.a
../bin/App/DiskAccess.exe: App/CMakeFiles/DiskAccess.dir/linklibs.rsp
../bin/App/DiskAccess.exe: App/CMakeFiles/DiskAccess.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Workspace/DiskAccess-VSCode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/App/DiskAccess.exe"
	cd C:/Workspace/DiskAccess-VSCode/build/App && "C:/Program Files (x86)/CMake/bin/cmake.exe" -E rm -f CMakeFiles/DiskAccess.dir/objects.a
	cd C:/Workspace/DiskAccess-VSCode/build/App && C:/msys64/mingw32/bin/llvm-ar.exe cr CMakeFiles/DiskAccess.dir/objects.a @CMakeFiles/DiskAccess.dir/objects1.rsp
	cd C:/Workspace/DiskAccess-VSCode/build/App && C:/msys64/mingw32/bin/clang++.exe -g   -Wl,--whole-archive CMakeFiles/DiskAccess.dir/objects.a -Wl,--no-whole-archive  -o ../../bin/App/DiskAccess.exe -Wl,--out-implib,../../bin/Lib/libDiskAccess.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/DiskAccess.dir/linklibs.rsp

# Rule to build all files generated by this target.
App/CMakeFiles/DiskAccess.dir/build: ../bin/App/DiskAccess.exe

.PHONY : App/CMakeFiles/DiskAccess.dir/build

App/CMakeFiles/DiskAccess.dir/clean:
	cd C:/Workspace/DiskAccess-VSCode/build/App && $(CMAKE_COMMAND) -P CMakeFiles/DiskAccess.dir/cmake_clean.cmake
.PHONY : App/CMakeFiles/DiskAccess.dir/clean

App/CMakeFiles/DiskAccess.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Workspace/DiskAccess-VSCode C:/Workspace/DiskAccess-VSCode/App C:/Workspace/DiskAccess-VSCode/build C:/Workspace/DiskAccess-VSCode/build/App C:/Workspace/DiskAccess-VSCode/build/App/CMakeFiles/DiskAccess.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : App/CMakeFiles/DiskAccess.dir/depend

