Written by Ash on 11/19/2022. 

_______________________________________________________
Please make sure that your g++ version is up to date.
g++ versions tested:

$ g++ --version
> g++.exe (Rev4, Built by MSYS2 project) 12.2.0

PS C:\Users\endof\Documents\Cal Poly HomeWork\CSC 448\Project> g++ --version
> g++.exe (MinGW-W64 x86_64-posix-seh, built by Brecht Sanders) 11.2.0

_______________________________________________________
To Build the executable on *Linux* or *MacOS*, enter "make" in the command prompt. 
For example:

endof@DESKTOP-3LG4RQH MINGW64 /C/Users/endof/Documents/Cal Poly HomeWork/CSC 448/Project

> $ make

To build the executable on *Windows*, enter the following command on cmd (**not** PowerShell):

> g++ -Wall -Wextra -Wconversion -static -DONLINE_JUDGE -Wl,--stack=268435456 -O2 -std=c++20 -o digestFragment digestFragment.cpp


_______________________________________________________
To use the executable, enter "digestFragment" for more instruction. For example:

endof@DESKTOP-3LG4RQH MINGW64 /C/Users/endof/Documents/Cal Poly HomeWork/CSC 448/Project

> $ ./digestFragment.exe

