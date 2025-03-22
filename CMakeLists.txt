cmake_minimum_required(VERSION 3.16)
project(SCP_Roguelike)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    ptsd

    URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip

    # Or alternatively if you want to use a specific git commit instead of a release:
    # GIT_REPOSITORY  https://github.com/ntut-open-source-club/practical-tools-for-simple-design.git
    # GIT_TAG         <git_tag>

    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)

include(files.cmake)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
list(TRANSFORM SRC_FILES PREPEND ${SRC_DIR}/)
list(TRANSFORM INCLUDE_FILES PREPEND ${INCLUDE_DIR}/)

# file(GLOB_RECURSE SRC_FILES src/*)
# file(GLOB_RECURSE HEADER_FILES include/*)

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
 "src/Player.cpp" "src/Hand.cpp" "src/Tilemap.cpp" "src/Bullet.cpp" "src/BulletBox.cpp"  
 "src/LevelManager.cpp" "src/Door.cpp" "src/Item.cpp" "src/UI.cpp" "src/Enemy.cpp" "src/Enemies/SCP610.cpp" "src/Enemies/SCP049_2.cpp" 
 "src/Enemies/SCP743A.cpp" "src/Enemies/SCP743B.cpp" "src/Enemies/SCP1048_B.cpp" "src/Enemies/SCP1048_C.cpp" "src/Enemies/SCP553.cpp" "src/Table.cpp" "src/core/BoxCollider.cpp" "src/core/Actor.cpp" "src/core/Solid.cpp" "src/Core/ColliderManager.cpp" "src/Trap.cpp" "src/UIText.cpp"  "src/Chest.cpp" "src/Menu.cpp"  "src/Core/Camera.cpp" "src/Enemies/SCP049.cpp")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    message(FATAL_ERROR "relative RESOURCE_DIR is WIP, Please use `-DCMAKE_BUILD_TYPE=Debug` build for now.")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    PTSD
)
