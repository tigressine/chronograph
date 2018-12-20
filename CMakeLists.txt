cmake_minimum_required(VERSION 3.1)

project(chronograph CXX)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Settings about parallization.
#option (PARCILKPLUS "Use gcc implementation of cilkplus for parallization." ON) 
#option (PAROPENMP "Use OpenMP for parallization for parallization." OFF) 
set (CILK OFF BOOL "Use intel implementation of cilkplus for parallization.") 
set (CILKP ON BOOL "Use gcc implementation of cilkplus for parallization.") 
set (OPENMP OFF BOOL "Use OpenMP for parallization for parallization.") 
set (PHASET ON "TIMINGS FOR EACH PHASE")

add_subdirectory("${PROJECT_SOURCE_DIR}/ext/parallel-divsufsort")
include_directories("${CMAKE_BINARY_DIR}/ext/parallel-divsufsort/include")


# Compiler settings.
# Settings for GCC.
if(CMAKE_COMPILER_IS_GNUCXX)
	if (CILKP)
		append_cxx_compiler_flags("-fcilkplus" "GCC" CMAKE_CXX_FLAGS)
		append_cxx_compiler_flags("-DPARCILKPLUS" "GCC" CMAKE_CXX_FLAGS)
	endif (CILKP)
	if (OPENMP)
		append_cxx_compiler_flags("-fopenmp " "GCC" CMAKE_CXX_FLAGS)
		append_cxx_compiler_flags("-DPAROPENMP" "GCC" CMAKE_CXX_FLAGS)
	endif (OPENMP)
	append_cxx_compiler_flags("-static-libstdc++ -std=c++11 -Wall -Wextra  -DNDEBUG" "GCC" CMAKE_CXX_FLAGS)
	append_cxx_compiler_flags("-O2 -ffast-math -funroll-loops" "GCC" CMAKE_CXX_FLAGS)
else()
	message(SEND_ERROR "cxx compiler no supported.")
endif()


add_executable(eval_pdss source/analyzer.cpp)
include_directories("${PROJECT_SOURCE_DIR}/source")
include_directories("${PROJECT_SOURCE_DIR}/ext/tclap/include")
target_link_libraries (eval_pdss cilkrts divsufsort libprange)

