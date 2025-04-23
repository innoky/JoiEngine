cmake_minimum_required(VERSION 3.20)
project(JoyEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Указываем путь к pybind11 из venv
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/venv/lib/python3.12/site-packages/pybind11/share/cmake/pybind11")
set(PYBIND11_FINDPYTHON ON)

# Найти Vulkan и GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(pybind11 REQUIRED)

# Собрать исходники движка
file(GLOB_RECURSE ENGINE_SOURCES
    src/*.cpp
)

# Основной файл демо-приложения
set(APP_MAIN demo/main.cpp)

# Добавляем исполняемый файл
add_executable(JoyApp ${APP_MAIN} ${ENGINE_SOURCES})

# Инклуды
target_include_directories(JoyApp PRIVATE
    src/
    include/
    src/extern/
    demo/
)

# Линки
target_link_libraries(JoyApp PRIVATE
    Vulkan::Vulkan
    glfw
    pybind11::embed # для встраивания Python в C++
)

# Биндинги Python
add_library(hand_tracker_bind MODULE
    modules/pyModules/bodyTracker/bindings.cpp
    modules/pyModules/bodyTracker/handTracker.cpp
)

# Назовём .so файл как Python-модуль (убираем "lib" и расширение)
set_target_properties(hand_tracker_bind PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)

# Поддержка C++20 для pybind11
target_compile_features(hand_tracker_bind PRIVATE cxx_std_20)

# Линки
target_link_libraries(hand_tracker_bind PRIVATE pybind11::module)

# Инклуды для биндингов
target_include_directories(hand_tracker_bind PRIVATE
    include/
    src/
    modules/pyModules/bodyTracker/
)
