cmake_minimum_required(VERSION 3.27)

project(MYGAME VERSION 1.0)

# Encuentra la biblioteca LUA
find_package(Lua REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/Game/game.cpp src/Game/game.h)

# Incluye los directorios de encabezados de LUA
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})

target_link_libraries(
    ${PROJECT_NAME}
    SDL2
    SDL2_image
    SDL2main
    ${LUA_LIBRARIES}
)

