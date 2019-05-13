# glfw-example

GLFW C++ Example

## Resources

https://learnopengl.com/

## GLFW

* Download the source distribution (or clone the repo).
* Generate the Makefiles using the cmake utility
* make and then make install.

```bash
git clone ...
cd ...
cmake .
make
make install
```

Headers are in /usr/local/include/GLFW
Static library in /usr/local/lib/libglfw3.a

## GLAD

GLAD comes from the webserver ...

https://glad.dav1d.de


- include
    - glad
        - glad.h
    - GLFW
        - glfw3.h
        - glfw3native.h
    - KHR
        - khrplatform.h

## linmath

linmath comes from https://github.com/datenwolf/linmath.h

## Building

When using glad ...

```bash
g++ -Wall -c glad.c
g++ -Wall -c main.cpp
g++ -o main.out main.o glad.o -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
```

When using gl_lite ...

```bash
g++ -Wall -c main.cpp
g++ -o main.out main.o -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
```

See http://apoorvaj.io/loading-opengl-without-glew.html

See e.g. https://www.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

## Running

```bash
./main.out
```