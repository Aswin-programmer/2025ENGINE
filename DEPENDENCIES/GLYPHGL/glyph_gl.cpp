#include "glyph_gl.h"

// Define GLYPH_LOG here if it wasn't defined in the header (or rely on the header's definition)
#ifndef GLYPH_LOG
#define GLYPH_LOG(...) fprintf(stderr, __VA_ARGS__)
#endif

// --- GL Function Pointers Definitions (Only when GLYPH_NO_GL_LOADER is NOT defined) ---
#ifndef GLYPH_NO_GL_LOADER

PFNGLGENBUFFERSPROC glyph__glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glyph__glDeleteBuffers = NULL;
PFNGLBINDBUFFERPROC glyph__glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glyph__glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glyph__glBufferSubData = NULL;
PFNGLCREATESHADERPROC glyph__glCreateShader = NULL;
PFNGLDELETESHADERPROC glyph__glDeleteShader = NULL;
PFNGLSHADERSOURCEPROC glyph__glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glyph__glCompileShader = NULL;
PFNGLGETSHADERIVPROC glyph__glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glyph__glGetShaderInfoLog = NULL;
PFNGLCREATEPROGRAMPROC glyph__glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC glyph__glDeleteProgram = NULL;
PFNGLATTACHSHADERPROC glyph__glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glyph__glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glyph__glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glyph__glGetProgramInfoLog = NULL;
PFNGLUSEPROGRAMPROC glyph__glUseProgram = NULL;
PFNGLGETATTRIBLOCATIONPROC glyph__glGetAttribLocation = NULL;
PFNGLGETUNIFORMLOCATIONPROC glyph__glGetUniformLocation = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glyph__glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glyph__glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glyph__glDisableVertexAttribArray = NULL;
PFNGLUNIFORM1IPROC glyph__glUniform1i = NULL;
PFNGLUNIFORM1FPROC glyph__glUniform1f = NULL;
PFNGLUNIFORM2FPROC glyph__glUniform2f = NULL;
PFNGLUNIFORM3FPROC glyph__glUniform3f = NULL;
PFNGLUNIFORM4FPROC glyph__glUniform4f = NULL;
PFNGLUNIFORMMATRIX4FVPROC glyph__glUniformMatrix4fv = NULL;
PFNGLACTIVETEXTUREPROC glyph__glActiveTexture = NULL;
PFNGLGENVERTEXARRAYSPROC glyph__glGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC glyph__glDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glyph__glBindVertexArray = NULL;

#endif // GLYPH_NO_GL_LOADER

// --- Static Shader Sources Definitions ---
const char* glyph__vertex_shader_source =
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   gl_Position = projection * vec4(aPos, 0.0, 1.0);\n"
"   TexCoord = aTexCoord;\n"
"}\n";

const char* glyph__fragment_shader_source =
"#version 330 core\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D textTexture;\n"
"uniform vec3 textColor;\n"
"uniform int effects;\n"
"void main() {\n"
"   float alpha;\n"
"   if (TexCoord.x == -1.0 && TexCoord.y == -1.0 && (effects & 4) != 0) {\n"
"       alpha = 1.0;\n"
"   } else {\n"
"       alpha = texture(textTexture, TexCoord).r;\n"
"   }\n"
"   FragColor = vec4(textColor, alpha);\n"
"   //FragColor = vec4(1.0f);\n"
"}\n";


// --- Function Definitions (Including GL Loader and Shader Utilities) ---

static GLuint glyph__compile_shader(GLenum type, const char* source) {
    GLuint shader = glyph__glCreateShader(type);
    glyph__glShaderSource(shader, 1, &source, NULL);
    glyph__glCompileShader(shader);

    GLint success;
    glyph__glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glyph__glGetShaderInfoLog(shader, 512, NULL, info_log);
        GLYPH_LOG("Shader compilation failed: %s\n", info_log);
        glyph__glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint glyph__create_program(const char* vertex_source, const char* fragment_source) {
    GLuint vertex_shader = glyph__compile_shader(GL_VERTEX_SHADER, vertex_source);
    if (!vertex_shader) return 0;

    GLuint fragment_shader = glyph__compile_shader(GL_FRAGMENT_SHADER, fragment_source);
    if (!fragment_shader) {
        glyph__glDeleteShader(vertex_shader);
        return 0;
    }

    GLuint program = glyph__glCreateProgram();
    glyph__glAttachShader(program, vertex_shader);
    glyph__glAttachShader(program, fragment_shader);
    glyph__glLinkProgram(program);

    GLint success;
    glyph__glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glyph__glGetProgramInfoLog(program, 512, NULL, info_log);
        GLYPH_LOG("Program linking failed: %s\n", info_log);
        glyph__glDeleteProgram(program);
        glyph__glDeleteShader(vertex_shader);
        glyph__glDeleteShader(fragment_shader);
        return 0;
    }

    glyph__glDeleteShader(vertex_shader);
    glyph__glDeleteShader(fragment_shader);

    return program;
}

#ifndef GLYPH_NO_GL_LOADER

// Helper Macro Definitions (Only needed for the function loading implementation)
#if defined(_WIN32) || defined(_WIN64)
    // Windows specific function pointer loading definition
#define GLYPH_GL_LOAD_PROC(type, name) \
        glyph__##name = (type)wglGetProcAddress(#name); \
        if (!glyph__##name) { \
            GLYPH_LOG("Failed to load OpenGL function: %s\n", #name); \
            return 0; \
        }
#elif defined(__APPLE__)
#include <dlfcn.h>
// macOS specific function pointer loading definition
#define GLYPH_GL_LOAD_PROC(type, name) \
        glyph__##name = (type)dlsym(RTLD_DEFAULT, #name); \
        if (!glyph__##name) { \
            GLYPH_LOG("Failed to load OpenGL function: %s\n", #name); \
            return 0; \
        }
#elif defined(__linux__) || defined(__unix__)
#include <dlfcn.h>
static void* glyph__libgl_handle = NULL;
// Linux/Unix specific function pointer loading definition
#define GLYPH_GL_LOAD_PROC(type, name) \
        if (!glyph__libgl_handle) { \
            glyph__libgl_handle = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL); \
            if (!glyph__libgl_handle) glyph__libgl_handle = dlopen("libGL.so", RTLD_LAZY | RTLD_GLOBAL); \
        } \
        if (glyph__libgl_handle) { \
            typedef void* (*glXGetProcAddressARB_t)(const GLubyte*); \
            glXGetProcAddressARB_t glXGetProcAddressARB = (glXGetProcAddressARB_t)dlsym(glyph__libgl_handle, "glXGetProcAddressARB"); \
            if (glXGetProcAddressARB) { \
                glyph__##name = (type)glXGetProcAddressARB((const GLubyte*)#name); \
            } \
            if (!glyph__##name) glyph__##name = (type)dlsym(glyph__libgl_handle, #name); \
        } \
        if (!glyph__##name) { \
            GLYPH_LOG("Failed to load OpenGL function: %s\n", #name); \
            return 0; \
        }
#endif

// OpenGL Function Loading Implementation
int glyph_gl_load_functions(void) {
    GLYPH_GL_LOAD_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
    GLYPH_GL_LOAD_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
    GLYPH_GL_LOAD_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
    GLYPH_GL_LOAD_PROC(PFNGLBUFFERDATAPROC, glBufferData);
    GLYPH_GL_LOAD_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
    GLYPH_GL_LOAD_PROC(PFNGLCREATESHADERPROC, glCreateShader);
    GLYPH_GL_LOAD_PROC(PFNGLDELETESHADERPROC, glDeleteShader);
    GLYPH_GL_LOAD_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
    GLYPH_GL_LOAD_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
    GLYPH_GL_LOAD_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
    GLYPH_GL_LOAD_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    GLYPH_GL_LOAD_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    GLYPH_GL_LOAD_PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    GLYPH_GL_LOAD_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
    GLYPH_GL_LOAD_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
    GLYPH_GL_LOAD_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    GLYPH_GL_LOAD_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    GLYPH_GL_LOAD_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);
    GLYPH_GL_LOAD_PROC(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
    GLYPH_GL_LOAD_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
    GLYPH_GL_LOAD_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    GLYPH_GL_LOAD_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    GLYPH_GL_LOAD_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
    GLYPH_GL_LOAD_PROC(PFNGLUNIFORM1IPROC, glUniform1i);
    GLYPH_GL_LOAD_PROC(PFNGLUNIFORM1FPROC, glUniform1f);
    GLYPH_GL_LOAD_PROC(PFNGLUNIFORM2FPROC, glUniform2f);
    GLYPH_GL_LOAD_PROC(PFNGLUNIFORM3FPROC, glUniform3f);
    GLYPH_GL_LOAD_PROC(PFNGLUNIFORM4FPROC, glUniform4f);
    GLYPH_GL_LOAD_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
    GLYPH_GL_LOAD_PROC(PFNGLACTIVETEXTUREPROC, glActiveTexture);
    GLYPH_GL_LOAD_PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
    GLYPH_GL_LOAD_PROC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
    GLYPH_GL_LOAD_PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);

    return 1;
}

#else // GLYPH_NO_GL_LOADER

// Fallback Implementation for glyph_gl_load_functions when using standard GL calls
int glyph_gl_load_functions(void) {
    return 1;
}

#endif // GLYPH_NO_GL_LOADER