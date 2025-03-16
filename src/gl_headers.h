#ifndef _GL_HEADERS_H_
#define _GL_HEADERS_H_
#define GL_GLEXT_PROTOTYPES
#ifndef _WIN32
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES3/gl3.h>
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include "glad/glad.h"
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#ifndef _WIN32
#include <SDL2/SDL_opengles2.h>
#endif
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#endif
#endif // _GL_HEADERS_H_