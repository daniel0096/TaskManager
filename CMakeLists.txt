cmake_minimum_required(VERSION 3.10)
project(TaskManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
		src/Main.cpp
		src/Log.cpp
		src/Application.cpp
		src/FileSystem.cpp
		src/PythonApplication.cpp
		src/PythonApplicationModule.cpp
		src/Config.cpp
)

set(HEADERS
		src/Log.h
		src/Application.h
		src/FileSystem.h
		src/PythonApplication.h
		src/PythonApplicationModule.h
		src/Config.h
		src/BasicDefines.h
)

set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(INCLUDE_DIR "${DEPENDENCIES_DIR}/include")
set(LIBS_DIR "${DEPENDENCIES_DIR}/libs")

# Executable target
add_executable(taskmanager ${SOURCES} ${HEADERS})

if (WIN32)
	set(PYTHON_DIR "${INCLUDE_DIR}/python")
	set(PYTHON_LIB "${LIBS_DIR}/python313.lib")

	include_directories(${PYTHON_DIR})
	link_directories(${LIBS_DIR})
	target_link_libraries(taskmanager ${PYTHON_LIB})
elseif(UNIX)
	find_package(Python3 3.13 REQUIRED COMPONENTS Interpreter Development)
	include_directories(${Python3_INCLUDE_DIRS})
	target_link_libraries(taskmanager Python3::Python)
endif()
