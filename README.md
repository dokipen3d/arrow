arrow
=====

An OpenGL based cross platform GUI framework.


> git clone https://github.com/microsoft/vcpkg

> .\vcpkg\bootstrap-vcpkg.bat

> .\vcpkg\vcpkg install freetype

> ./vcpkg.exe install freetype:x64-windows (i had to do this on my machine otherwisw it wasn't found)

> ./vcpkg.exe install glfw3:x64-windows

> ./vcpkg.exe install glew:x64-windows

> ./vcpkg.exe install glm:x64-windows

.\vcpkg\vcpkg integrate install

create a build folder

run 'cmake ..' (by default, it build in debug mode but optionally you can run 'cmake -DCMAKE_BUILD_TYPE=Release' or if on windows, release is specified as a build option shown in step 3 below )

-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake

run 'cmake --build .' (or on windows ''cmake --build . --config Release')

On windows the executable will be '.\Debug\cpplox.exe' if you have built debug or '.\Release\cpplox.exe' for release


