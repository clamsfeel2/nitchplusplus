cmake_minimum_required(VERSION 3.20)
project(nitch++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual")
# Enable building compile_commands.json for help with ycm
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch and include tomlplusplus if not found
include(FetchContent)
find_package(tomlplusplus QUIET)
if(NOT tomlplusplus_FOUND)
    FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(tomlplusplus)
endif()
file(GLOB SRC_FILES 
    "src/main.cpp" 
    "src/colors.hpp" 
    "src/logos.cpp" "src/logos.h"
	"src/icons.cpp" "src/icons.h"
    "src/system_info.cpp" "src/system_info.h"	
    "src/configuration.cpp" "src/configuration.h"
	"src/cli_processor.cpp" "src/cli_processor.h"
	"src/printer.cpp" "src/printer.h"
)

include_directories(src)
add_executable(${PROJECT_NAME} ${SRC_FILES})
# Add include directories for the target
target_include_directories(${PROJECT_NAME} PRIVATE src)
# Link libraries to the release executable
target_link_libraries(${PROJECT_NAME} PRIVATE tomlplusplus::tomlplusplus)

# Copy ascii-art.txt to $HOME/.config/nitch++/.ascii.txt
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$ENV{HOME}/.config/nitch++/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/.assets/ascii.txt" "$ENV{HOME}/.config/nitch++/.ascii.txt"
)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -E remove /usr/bin/nitch++)
