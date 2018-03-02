#pragma once
#include <GL/glew.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

GLuint loadImage(const char *imagePath);

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath);