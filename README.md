# glfw-example

GLFW C++ Example

## GLAD

GLAD comes from the webserver ...

https://glad.dav1d.de


include
    glad
        glad.h
    GLFW
        glfw3.h
        glfw3native.h
    KHR
        khrplatform.h

## linmath

linmath comes from https://github.com/datenwolf/linmath.h

## Building

```bash
g++ -Wall -c glad.c
g++ -Wall -c main.c
g++ -o main main.o glad.o -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
```

## Running

```bash
./main
```