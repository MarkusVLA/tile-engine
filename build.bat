@echo off

git submodule update --init --recursive

REM Check if cmake is installed
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo CMake could not be found. Please install CMake to continue.
    exit /b
)

mkdir build
cd build

cmake ..
cmake --build . --config Release

move Release\app.exe 
.\app.exe