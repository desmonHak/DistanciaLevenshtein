cmake_minimum_required(VERSION 3.16)
project(test1 LANGUAGES CXX C)

# Configuración C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuración C (para Levenshtein_c)
set(CMAKE_C_STANDARD 99)

# Crear ejecutable test1
add_executable(test1
        example1.cpp              #  example1.cpp
        Levenshtein.cpp       # Wrapper C++
        Levenshtein_c.c       # Código C original
)

# Headers necesarios
target_include_directories(test1 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Windows: pthread y timing
if(WIN32)
    target_link_libraries(test1 PRIVATE
            pthread
    )
endif()

# Linux/Mac: pthread automático
target_link_libraries(test1 PRIVATE
        pthread
)

# Opcional: Enable warnings
if(MSVC)
    target_compile_options(test1 PRIVATE /W4)
else()
    target_compile_options(test1 PRIVATE -Wall -Wextra -Wpedantic)
endif()
