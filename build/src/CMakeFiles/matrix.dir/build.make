# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zsj/SelfFile/SPP_RT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zsj/SelfFile/SPP_RT/build

# Include any dependencies generated for this target.
include src/CMakeFiles/matrix.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/matrix.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/matrix.dir/flags.make

src/CMakeFiles/matrix.dir/Matrix.cpp.o: src/CMakeFiles/matrix.dir/flags.make
src/CMakeFiles/matrix.dir/Matrix.cpp.o: ../src/Matrix.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zsj/SelfFile/SPP_RT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/matrix.dir/Matrix.cpp.o"
	cd /home/zsj/SelfFile/SPP_RT/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matrix.dir/Matrix.cpp.o -c /home/zsj/SelfFile/SPP_RT/src/Matrix.cpp

src/CMakeFiles/matrix.dir/Matrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matrix.dir/Matrix.cpp.i"
	cd /home/zsj/SelfFile/SPP_RT/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zsj/SelfFile/SPP_RT/src/Matrix.cpp > CMakeFiles/matrix.dir/Matrix.cpp.i

src/CMakeFiles/matrix.dir/Matrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matrix.dir/Matrix.cpp.s"
	cd /home/zsj/SelfFile/SPP_RT/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zsj/SelfFile/SPP_RT/src/Matrix.cpp -o CMakeFiles/matrix.dir/Matrix.cpp.s

src/CMakeFiles/matrix.dir/Matrix.cpp.o.requires:

.PHONY : src/CMakeFiles/matrix.dir/Matrix.cpp.o.requires

src/CMakeFiles/matrix.dir/Matrix.cpp.o.provides: src/CMakeFiles/matrix.dir/Matrix.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/matrix.dir/build.make src/CMakeFiles/matrix.dir/Matrix.cpp.o.provides.build
.PHONY : src/CMakeFiles/matrix.dir/Matrix.cpp.o.provides

src/CMakeFiles/matrix.dir/Matrix.cpp.o.provides.build: src/CMakeFiles/matrix.dir/Matrix.cpp.o


# Object files for target matrix
matrix_OBJECTS = \
"CMakeFiles/matrix.dir/Matrix.cpp.o"

# External object files for target matrix
matrix_EXTERNAL_OBJECTS =

../lib/libmatrix.so: src/CMakeFiles/matrix.dir/Matrix.cpp.o
../lib/libmatrix.so: src/CMakeFiles/matrix.dir/build.make
../lib/libmatrix.so: src/CMakeFiles/matrix.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zsj/SelfFile/SPP_RT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../lib/libmatrix.so"
	cd /home/zsj/SelfFile/SPP_RT/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/matrix.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/matrix.dir/build: ../lib/libmatrix.so

.PHONY : src/CMakeFiles/matrix.dir/build

src/CMakeFiles/matrix.dir/requires: src/CMakeFiles/matrix.dir/Matrix.cpp.o.requires

.PHONY : src/CMakeFiles/matrix.dir/requires

src/CMakeFiles/matrix.dir/clean:
	cd /home/zsj/SelfFile/SPP_RT/build/src && $(CMAKE_COMMAND) -P CMakeFiles/matrix.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/matrix.dir/clean

src/CMakeFiles/matrix.dir/depend:
	cd /home/zsj/SelfFile/SPP_RT/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zsj/SelfFile/SPP_RT /home/zsj/SelfFile/SPP_RT/src /home/zsj/SelfFile/SPP_RT/build /home/zsj/SelfFile/SPP_RT/build/src /home/zsj/SelfFile/SPP_RT/build/src/CMakeFiles/matrix.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/matrix.dir/depend

