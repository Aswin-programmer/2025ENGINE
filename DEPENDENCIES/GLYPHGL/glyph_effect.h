#ifndef __GLYPH_EFFECT_H
#define __GLYPH_EFFECT_H

#include "glyph_gl.h"

typedef enum {
    GLYPH_NONE,
    GLYPH_GLOW,
    GLYPH_RAINBOW,
    GLYPH_OUTLINE,
    GLYPH_SHADOW,
    GLYPH_WAVE,
    GLYPH_GRADIENT,
    GLYPH_NEON
} glyph_effect_type_t;

typedef struct {
    glyph_effect_type_t type;
    const char* vertex_shader;
    const char* fragment_shader;
} glyph_effect_t;

/*
 * ================== INTEGRATED SHADERS ==================
 *
 * Glow Effect:
 * | Relatively simple and kind of messy implementation of a glowing text effect.
 * | Simulates bloom by sampling surrounding pixels and blending weighted intensity.
 * | - Uniforms: (float) glowIntensity — controls how bright the glow appears.
 *
 * Rainbow Effect:
 * | Animated color cycling effect based on screen coordinates and time.
 * | Useful for flashy UI elements or debug overlays.
 * | - Uniforms: (float) time — drives hue shifting animation.
 *
 * Outline Effect:
 * | Generates a basic black outline around glyphs using neighboring alpha samples.
 * | - Uniforms: (vec3) outlineColor — defines outline color.
 *
 * Shadow Effect:
 * | Renders a soft shadow by offsetting glyph sampling and blending underneath text.
 * | - Uniforms:
 * |   (vec2) shadowOffset — pixel offset of the shadow.
 * |   (vec3) shadowColor  — color of the shadow.
 *
 * Wave Effect:
 * | Wavy distortion effect using a sine function along the X-axis.
 * | Great for playful, animated text.
 * | - Uniforms:
 * |   (float) time          — drives wave motion.
 * |   (float) waveAmplitude — vertical distortion strength.
 *
 * Gradient Effect:
 * | Smooth color blend from top to bottom using two color uniforms.
 * | - Uniforms:
 * |   (vec3) gradientStart — starting color at the top.
 * |   (vec3) gradientEnd   — ending color at the bottom.
 *
 * Neon Effect:
 * | Pulsating glow animation simulating neon lighting.
 * | - Uniforms:
 * |   (float) time — controls the pulsing brightness over time.
 *
 * =========================================================
 */

glyph_effect_t glyph_effect_create_glow();

glyph_effect_t glyph_effect_create_rainbow();

glyph_effect_t glyph_effect_create_outline();

glyph_effect_t glyph_effect_create_shadow();

glyph_effect_t glyph_effect_create_wave();

glyph_effect_t glyph_effect_create_gradient();

glyph_effect_t glyph_effect_create_neon();

#endif