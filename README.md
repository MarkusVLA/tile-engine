# Tile based game engine using SFML library


## Usage Instructions

### Prerequisites
- CMake (Minimum version 3.10)
- A C++17 compatible compiler

### Building the Project
1. **Clone the Repository and Initialize Submodules**: 
   ```bash
   git clone https://github.com/MarkusVLA/tile-engine.git
   cd tile-engine
   git submodule update --init --recursive
   ```

2. **Build the Project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the Application**:
   After building, run the application from the `build` directory:
   ```bash
   ./app
   ```

### Windows-Specific build Instructions


For building on Windows, ensure Visual Studio with C++ support is installed. Then, use the command line to configure and generate Visual Studio project files:

1. Open a command prompt or PowerShell in the project directory.
2. Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build
   ```
3. Run CMake to generate Visual Studio project files:
   ```bash
   cmake .. -G "Visual Studio 16 2019" # Adjust for your Visual Studio version
   ```
4. Build the project using CMake:
   ```bash
   cmake --build . --config Release
   ```
