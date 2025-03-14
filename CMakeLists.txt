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
)

#	base dependency folder
set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")

#	default dependencies folder
set(INCLUDE_DIR "${DEPENDENCIES_DIR}/include")

#	default libraries folder
set(LIBS_DIR "${DEPENDENCIES_DIR}/libs")

#	python include folder
set(PYTHON_DIR "${INCLUDE_DIR}/python")

#	python lib file
set(PYTHON_LIB "${LIBS_DIR}/python313.lib")

include_directories(${PYTHON_DIR})
link_directories(${LIBS_DIR})

#	link Python to executable
add_executable(taskmanager ${SOURCES} ${HEADERS})
target_link_libraries(taskmanager ${PYTHON_LIB})