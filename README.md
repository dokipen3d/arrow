arrow
=====

An OpenGL based cross platform GUI framework.


> git clone https://github.com/microsoft/vcpkg

> .\vcpkg\bootstrap-vcpkg.bat

> .\vcpkg\vcpkg install freetype

.\vcpkg\vcpkg integrate install

create a build folder

run 'cmake ..' (by default, it build in debug mode but optionally you can run 'cmake -DCMAKE_BUILD_TYPE=Release' or if on windows, release is specified as a build option shown in step 3 below )

run 'cmake --build .' (or on windows ''cmake --build . --config Release')

On windows the executable will be '.\Debug\cpplox.exe' if you have built debug or '.\Release\cpplox.exe' for release


