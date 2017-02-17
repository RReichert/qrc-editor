# mimimum cmake requirements
cmake_minimum_required(VERSION 3.6)

# project details
project(qrc-editor)

set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# package configuration
find_package(Boost REQUIRED COMPONENTS filesystem program_options system)

# find external libraries/binaries
set(SOURCE_FILES main.cpp command_line.cpp command_line.h)

# executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

# installation
install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static
)