# mkdir dist\win
# cd dist
# cmake -G "MinGW Makefiles" --fresh -S .. -B ..\dist\win -DSDL3_DIR=E:\libs\sdl3-desktop-prefix\lib\cmake\SDL3 -Dbox2d_DIR=E:\libs\box2d-2.4.2-prefix\lib\cmake\box2d
# cd win
# mingw32-make
# app

cmake_minimum_required(VERSION 3.20)

project(print_gravity_box2d)
add_executable(app) # WIN32

target_include_directories(app PRIVATE E:/libs/glad-2.0.6/include)

target_sources(app
PRIVATE
    E:/libs/glad-2.0.6/src/glad.c
    main.cpp
)

# This code copies the "SDL3.dll" file to the build directory
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:app> $<TARGET_FILE_DIR:app>
)

find_package(SDL3)
find_package(box2d)
target_link_libraries(app PRIVATE box2d::box2d SDL3::SDL3)
target_compile_definitions(app PRIVATE SDL_MAIN_USE_CALLBACKS)

target_link_options(app PRIVATE -static-libgcc -static-libstdc++)
