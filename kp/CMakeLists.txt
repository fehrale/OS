# cmake_minimum_required(VERSION 3.10)

# project(KPProject LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Добавим флаги компиляции
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")

# # Указываем путь к INIReader
# set(INI_READER_DIR "${CMAKE_SOURCE_DIR}/inih/cpp") # Путь к папке с INIReader

# # Подключаем папку include/ и INIReader
# include_directories(
#     "${INI_READER_DIR}"
#     "${CMAKE_SOURCE_DIR}/include"
# )

# # Исходники для "cheker"
# set(SOURCES_CHEKER
#     src/cheker.cpp
# )

# # Исходники для "error"
# set(SOURCES_ERROR
#     src/error.cpp
# )

# # Исходники для "timer_job"
# set(SOURCES_TIMER
#     src/timer_job.cpp
# )

# # Добавляем исходники INIReader
# set(INI_READER_SOURCES
#     "${INI_READER_DIR}/INIReader.cpp"
#     "${INI_CORE_DIR}/ini.c" # Добавляем ini.c
# )

# # Создаём исполняемый файл cheker
# add_executable(cheker ${SOURCES_CHEKER} ${INI_READER_SOURCES})

# # Создаём исполняемый файл error
# add_executable(error ${SOURCES_ERROR})

# # Создаём исполняемый файл timer_job
# add_executable(timer_job ${SOURCES_TIMER})

# # Указываем INIReader как директорию для include
# target_include_directories(cheker PRIVATE "${INI_READER_DIR}")

# # Дополнительная цель "trace" (strace для cheker)
# add_custom_target(trace
#     COMMAND strace -o trace_output.txt ./cheker
#     DEPENDS cheker
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMENT "Running strace on cheker, output saved to trace_output.txt"
# )



# cmake_minimum_required(VERSION 3.10)

# project(KPProject LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Добавим флаги компиляции
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")

# # Указываем путь к INIReader
# set(INI_READER_DIR "${CMAKE_SOURCE_DIR}/inih/cpp") # Путь к папке с INIReader
# set(INI_CORE_DIR "${CMAKE_SOURCE_DIR}/inih")       # Путь к корневой папке inih

# # Подключаем папку include/ и INIReader
# include_directories(
#     "${INI_READER_DIR}"
#     "${INI_CORE_DIR}"
#     "${CMAKE_SOURCE_DIR}/include"
# )

# # Исходники для "cheker"
# set(SOURCES_CHEKER
#     src/cheker.cpp
# )

# # Исходники для "error"
# set(SOURCES_ERROR
#     src/error.cpp
# )

# # Исходники для "timer_job"
# set(SOURCES_TIMER
#     src/timer_job.cpp
# )

# # Добавляем исходники INIReader
# set(INI_READER_SOURCES
#     "${INI_READER_DIR}/INIReader.cpp"
#     "${INI_CORE_DIR}/ini.c" # Исправленный путь к ini.c
# )

# # Создаём исполняемый файл cheker
# add_executable(cheker ${SOURCES_CHEKER} ${INI_READER_SOURCES})

# # Создаём исполняемый файл error
# add_executable(error ${SOURCES_ERROR})

# # Создаём исполняемый файл timer_job
# add_executable(timer_job ${SOURCES_TIMER})

# # Указываем INIReader как директорию для include
# target_include_directories(cheker PRIVATE "${INI_READER_DIR}" "${INI_CORE_DIR}")

# # Дополнительная цель "trace" (strace для cheker)
# add_custom_target(trace
#     COMMAND strace -o trace_output.txt ./cheker
#     DEPENDS cheker
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMENT "Running strace on cheker, output saved to trace_output.txt"
# )


# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(kp)

# Установка C++ стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Пути к исходникам и библиотеке inih
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INIH_DIR "${CMAKE_SOURCE_DIR}/inih")

# Создание статической библиотеки inih
add_library(inih STATIC
    "${INIH_DIR}/ini.c"                # Реализация ini_parse
    "${INIH_DIR}/cpp/INIReader.cpp"    # Реализация INIReader
)

# Подключение include-директорий для библиотеки inih
target_include_directories(inih PUBLIC
    "${INIH_DIR}"                      # Для ini.h
    "${INIH_DIR}/cpp"                  # Для INIReader.h
)

# Добавление исполняемого файла cheker
add_executable(cheker
    src/cheker.cpp
    src/error.cpp
    src/timer_job.cpp
)

target_include_directories(cheker PRIVATE include)

# Линковка cheker с библиотекой inih
target_link_libraries(cheker PRIVATE inih)