/*
 * MIT License
 * * Copyright (c) 2025 Darek
 * * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef GLYPH_GL_H
#define GLYPH_GL_H

// --- Standard Includes ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstddef> // For ptrdiff_t

// --- Platform-Specific GL Includes ---
#ifndef GLYPH_NO_GL_LOADER
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#include <OpenGL/gl.h>
#endif
#elif defined(__linux__) || defined(__unix__)
#include <GL/gl.h>
#endif
#endif

// Define GLYPH_LOG if not defined (placeholder)
// In a real library, this would be defined externally or use a robust logging mechanism.
#ifndef GLYPH_LOG
#define GLYPH_LOG(...) fprintf(stderr, __VA_ARGS__)
#endif

// --- GL Type and Constant Definitions (if GL loader is used) ---
#ifndef GLYPH_NO_GL_LOADER

// Basic types
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

// OpenGL 3.3+ Constants (if not included by GL header)
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
// ... (Include all other `#ifndef ... #define` blocks from the original file here) ...
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH 0x8B84
#endif
#ifndef GL_TEXTURE0
#define GL_TEXTURE0 0x84C0
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifndef GL_RED
#define GL_RED 0x1903
#endif
#ifndef GL_FUNC_ADD
#define GL_FUNC_ADD 0x8006
#endif

// Function Pointer Typedefs
typedef void (*PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
// ... (Include all other PFNGL*PROC typedefs from the original file here) ...
typedef void (*PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
typedef void (*PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (*PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (*PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef GLuint(*PFNGLCREATESHADERPROC)(GLenum type);
typedef void (*PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (*PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (*PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (*PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void (*PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef GLuint(*PFNGLCREATEPROGRAMPROC)(void);
typedef void (*PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (*PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (*PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (*PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void (*PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (*PFNGLUSEPROGRAMPROC)(GLuint program);
typedef GLint(*PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar* name);
typedef GLint(*PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (*PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (*PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (*PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (*PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (*PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (*PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (*PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (*PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (*PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef void (*PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);
typedef void (*PFNGLBINDVERTEXARRAYPROC)(GLuint array);


// Static Function Pointers for loaded GL functions
// These should be defined *once* in the .cpp file, but are often declared static in the header for single-file libraries.
extern PFNGLGENBUFFERSPROC glyph__glGenBuffers;
// ... (Include all other extern declarations for function pointers here) ...
extern PFNGLDELETEBUFFERSPROC glyph__glDeleteBuffers;
extern PFNGLBINDBUFFERPROC glyph__glBindBuffer;
extern PFNGLBUFFERDATAPROC glyph__glBufferData;
extern PFNGLBUFFERSUBDATAPROC glyph__glBufferSubData;
extern PFNGLCREATESHADERPROC glyph__glCreateShader;
extern PFNGLDELETESHADERPROC glyph__glDeleteShader;
extern PFNGLSHADERSOURCEPROC glyph__glShaderSource;
extern PFNGLCOMPILESHADERPROC glyph__glCompileShader;
extern PFNGLGETSHADERIVPROC glyph__glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glyph__glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC glyph__glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glyph__glDeleteProgram;
extern PFNGLATTACHSHADERPROC glyph__glAttachShader;
extern PFNGLLINKPROGRAMPROC glyph__glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glyph__glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glyph__glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glyph__glUseProgram;
extern PFNGLGETATTRIBLOCATIONPROC glyph__glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC glyph__glGetUniformLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC glyph__glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glyph__glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glyph__glDisableVertexAttribArray;
extern PFNGLUNIFORM1IPROC glyph__glUniform1i;
extern PFNGLUNIFORM1FPROC glyph__glUniform1f;
extern PFNGLUNIFORM2FPROC glyph__glUniform2f;
extern PFNGLUNIFORM3FPROC glyph__glUniform3f;
extern PFNGLUNIFORM4FPROC glyph__glUniform4f;
extern PFNGLUNIFORMMATRIX4FVPROC glyph__glUniformMatrix4fv;
extern PFNGLACTIVETEXTUREPROC glyph__glActiveTexture;
extern PFNGLGENVERTEXARRAYSPROC glyph__glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glyph__glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glyph__glBindVertexArray;


#else // GLYPH_NO_GL_LOADER
// Aliases for standard GL functions when not using the custom loader
#define glyph__glGenBuffers glGenBuffers
// ... (Include all other `#define glyph__gl... gl...` aliases here) ...
#define glyph__glDeleteBuffers glDeleteBuffers
#define glyph__glBindBuffer glBindBuffer
#define glyph__glBufferData glBufferData
#define glyph__glBufferSubData glBufferSubData
#define glyph__glCreateShader glCreateShader
#define glyph__glDeleteShader glDeleteShader
#define glyph__glShaderSource glShaderSource
#define glyph__glCompileShader glCompileShader
#define glyph__glGetShaderiv glGetShaderiv
#define glyph__glGetShaderInfoLog glGetShaderInfoLog
#define glyph__glCreateProgram glCreateProgram
#define glyph__glDeleteProgram glDeleteProgram
#define glyph__glAttachShader glAttachShader
#define glyph__glLinkProgram glLinkProgram
#define glyph__glGetProgramiv glGetProgramiv
#define glyph__glGetProgramInfoLog glGetProgramInfoLog
#define glyph__glUseProgram glUseProgram
#define glyph__glGetAttribLocation glGetAttribLocation
#define glyph__glGetUniformLocation glGetUniformLocation
#define glyph__glVertexAttribPointer glVertexAttribPointer
#define glyph__glEnableVertexAttribArray glEnableVertexAttribArray
#define glyph__glDisableVertexAttribArray glDisableVertexAttribArray
#define glyph__glUniform1i glUniform1i
#define glyph__glUniform1f glUniform1f
#define glyph__glUniform2f glUniform2f
#define glyph__glUniform3f glUniform3f
#define glyph__glUniform4f glUniform4f
#define glyph__glUniformMatrix4fv glUniformMatrix4fv
#define glyph__glActiveTexture glActiveTexture
#define glyph__glGenVertexArrays glGenVertexArrays
#define glyph__glDeleteVertexArrays glDeleteVertexArrays
#define glyph__glBindVertexArray glBindVertexArray
#endif // GLYPH_NO_GL_LOADER

// --- Shared Declarations (Implementation is in .cpp) ---

// Static/Internal linkage variables
extern const char* glyph__vertex_shader_source;
extern const char* glyph__fragment_shader_source;

// Function prototypes (definitions in .cpp)
int glyph_gl_load_functions(void);
GLuint glyph__compile_shader(GLenum type, const char* source);
GLuint glyph__create_program(const char* vertex_source, const char* fragment_source);

#endif // GLYPH_GL_H