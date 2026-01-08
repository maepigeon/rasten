cmake_minimum_required(VERSION 3.10)
set(ProjectName 
    rasten)

project($(ProjectName))
# Use the C++23 standard.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL3 REQUIRED)
find_package(glm CONFIG REQUIRED)

# sources variable contains a link to all cpp files to compile
set(SOURCES
    src/Geom.cpp
    src/Render.cpp
    src/main.cpp
)
add_executable(${ProjectName} ${SOURCES})

target_link_libraries(${ProjectName} SDL3::SDL3)
target_link_libraries(${ProjectName} glm::glm)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(${ProjectName}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)