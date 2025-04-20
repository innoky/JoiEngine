cmake_minimum_required(VERSION 3.20)
project(JoyEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")


file(GLOB_RECURSE ENGINE_SOURCES
    src/*.cpp
)


set(APP_MAIN demo/main.cpp)


add_executable(JoyApp ${APP_MAIN} ${ENGINE_SOURCES})


find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)


target_include_directories(JoyApp PRIVATE
    src/
    include/
    src/extern/
    demo/        
)

target_link_libraries(JoyApp PRIVATE
    Vulkan::Vulkan
    glfw
)
