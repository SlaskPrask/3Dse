#pragma once

#include "platform.h"

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
