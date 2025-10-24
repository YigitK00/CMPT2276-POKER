Poker Game (C++ & SFML)

- A simple poker game prototype built with C++ and SFML 3.0.2.
- The project features a basic poker gameplay system with an SFML-based graphical interface.

How to Build & Run

1. Download and extract SFML 3.0.2

2. Configure Visual Studio
   - Project Properties → C/C++ → General → Include Directories: C:\SFML-3.0.2\include
   - Project Properties → Linker → General → Library Directories: C:\SFML-3.0.2\lib;%(AdditionalLibraryDirectories)
   - Project Properties → Linker → Input → Dependencies: sfml-graphics.lib;sfml-audio.lib;sfml-window.lib;sfml-system.lib;sfml-network.lib;%(AdditionalDependencies)

3. Build the Project with Visual Studio 2022

4. Add SFML DLL Files
   - Copy the following DLL files into the same folder as your .exe file which are located inside your SFML bin directory: 
		- sfml-audio-3.dll
		- sfml-graphics-3.dll
		- sfml-network-3.dll
		- sfml-system-3.dll
		- sfml-window-3.dll

5. Run the Game via Visual Studio 2022