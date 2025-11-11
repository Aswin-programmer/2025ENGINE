# ENGINE/DEPENDENCIES/GLYPHGL/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(GLYPHGL LANGUAGES C CXX)

# Add as a header-only library
add_library(GlyphGL "glyph.cpp" "glyph_atlas.cpp" "glyph_effect.cpp" "glyph_gl.cpp" "glyph_truetype.cpp"  "glyph_image.cpp")

# Tell CMake where to find the headers
target_include_directories(GlyphGL PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    opengl32
)
