#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define BUFFER_SIZE 16384

#include "MicroUI.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../WINDOW/Window.h"

struct GLStateBackup {

    GLStateBackup() {

    }

    ~GLStateBackup() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glDepthFunc(GL_LESS);
    }
};

// Layout for our vertex data
typedef struct {
	float pos[2];
	float uv[2];
	unsigned char color[4];
} Vertex;


class ENGINE_API MicroUIRenderer
{
public:
	MicroUIRenderer(const MicroUIRenderer&) = delete;
	MicroUIRenderer& operator=(MicroUIRenderer&) = delete;

	static mu_Context* GetContext() { return ctx; }
	static void InitMicroUIRenderer();
	static void init_gl_objects(void);
	static void r_draw_rect(mu_Rect rect, mu_Color color);
	static void r_draw_text(const char* text, mu_Vec2 pos, mu_Color color);
	static void r_draw_icon(int id, mu_Rect rect, mu_Color color);
	static int r_get_text_width(const char* text, int len);
	static int r_get_text_height(void);
	static void r_set_clip_rect(mu_Rect rect);
	static void r_clear();
	static void r_present(void);
	static void flush();
	static void push_quad(mu_Rect dst, mu_Rect src, mu_Color color);

	static void render_debug_ui();

	static int text_width(mu_Font font, const char* text, int len);
	static int text_height(mu_Font font);

	static int get_key_code(int glfw_key);
	static int get_mouse_code(int glfw_button);

	static void DestroyMicroUIRenderer(){ free(ctx); }
private:
	static mu_Context* ctx;

	static Vertex vert_buf[BUFFER_SIZE * 4];
	static unsigned int index_buf[BUFFER_SIZE * 6];
	static int buf_idx;

	static GLuint program;
	static GLuint vao;
	static GLuint vbo;
	static GLuint ebo;
	static GLuint atlas_tex;
	static GLint u_proj;

	static int win_width;
	static int win_height;
};



