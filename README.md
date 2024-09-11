[Setting up SDL3 for Windows using CMake and MinGW-w64](https://8observer8.github.io/tutorials/setting-up/sdl3-for-windows/public/index.html)

> mkdir dist\win

> cd dist

> cmake -G "MinGW Makefiles" --fresh -S .. -B ..\dist\win -DSDL3_DIR=E:\libs\sdl3-desktop-prefix\lib\cmake\SDL3 -Dbox2d_DIR=E:\libs\box2d-2.4.2-prefix\lib\cmake\box2d

> cd win

> mingw32-make

> app

[EXE for Windows 10, 64-bit](https://www.dropbox.com/scl/fi/t9dgohhqwe7f26n0u3ko4/print-gravity-box2d-opengles20-sdl3-cpp-exe.zip?rlkey=jg0mjrz3ujro10abapd9f9jty&st=787y3yli&raw=1) - 2.66 MB, unzipped - 4.68 MB

![alt text](https://www.dropbox.com/scl/fi/z0ge1ovcveugc1ct4995f/print-gravity-box2d-opengles20-sdl3-cpp.png?rlkey=suyqvtpgxzgikfu2nkj68vbab&st=kd00fc3k&raw=1)
