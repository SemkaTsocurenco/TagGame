# Устанавливаем минимальную версию CMake
cmake_minimum_required(VERSION 3.10)

# Указываем название проекта
project(TagGame)

# Устанавливаем стандарт C++ для проекта
set(CMAKE_CXX_STANDARD 17)

# Указываем путь к папке с исходными файлами
set(SOURCES main.cpp  src/a_star.cpp src/get_functions.cpp src/init.cpp src/print_to_console.cpp src/visualizer.cpp
)

# Указываем пути к библиотекам SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

# Создаем исполнимый файл проекта
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключаем библиотеки SFML к проекту
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system ${YAML_CPP_LIBRARIES})
