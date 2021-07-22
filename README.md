# WebBrowser
Uses the OLC Pixel Game Engine by javidx9 to show you the webpage you type in

Needs the httplib library in a folder called httplib to compile.

Run to compile for mac:
```
clang++ -arch x86_64 -std=c++17 -mmacosx-version-min=10.15 -Wall -framework OpenGL -framework GLUT -framework Carbon -lpng main.cpp -o WebBrowser
```
