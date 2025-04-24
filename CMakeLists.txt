cmake_minimum_required(VERSION 3.20)
project(JoyEngine)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Указываем путь к pybind11 из venv (путь должен быть абсолютный)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/venv/Lib/site-packages/pybind11/share/cmake/pybind11")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/venv/lib/python3.12/site-packages/pybind11/share/cmake/pybind11")
set(PYBIND11_FINDPYTHON ON)

# Указываем вручную путь к GLFW (замени на свой)
# GLFW
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/modules/glfwModules/include")
set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/modules/glfwModules/lib-vc2022/glfw3.lib")


# Vulkan SDK должен быть установлен и переменные окружения заданы автоматически
find_package(Vulkan REQUIRED)
find_package(pybind11 REQUIRED)

# Собрать исходники движка
file(GLOB_RECURSE ENGINE_SOURCES
    src/*.cpp
)

# Основной файл
set(APP_MAIN demo/main.cpp)

# Приложение
add_executable(JoyApp ${APP_MAIN} ${ENGINE_SOURCES})

# Инклуды
target_include_directories(JoyApp PRIVATE
    ${GLFW_INCLUDE_DIR}
    src/
    include/
    src/extern/
    demo/
    external/glm  
)

# Линки
target_link_libraries(JoyApp PRIVATE
    Vulkan::Vulkan
    ${GLFW_LIBRARY}
    pybind11::embed
)

# Python биндинг
add_library(hand_tracker_bind MODULE
    modules/pyModules/bodyTracker/bindings.cpp
    modules/pyModules/bodyTracker/handTracker.cpp
)

# Назовём .so как Python-модуль (на винде будет .pyd)
set_target_properties(hand_tracker_bind PROPERTIES
    PREFIX ""
    SUFFIX ".pyd"
)

# Компиляторные фичи
target_compile_features(hand_tracker_bind PRIVATE cxx_std_20)

# Линки и инклуды
target_link_libraries(hand_tracker_bind PRIVATE pybind11::module)
target_include_directories(hand_tracker_bind PRIVATE
    include/
    src/
    modules/pyModules/bodyTracker/
)
