#include "Window.h"

// Initialize static members
unsigned int Window::m_width = 0;
unsigned int Window::m_height = 0;
const char* Window::m_title = nullptr;
GLFWwindow* Window::m_window = nullptr;
float Window::beginTime = 0.0f;
float Window::endTime = 0.0f;
float Window::dt = 0.0f;

// ? Define the exported color variables
ENGINE_API float Window_r = 0.0f;
ENGINE_API float Window_g = 0.0f;
ENGINE_API float Window_b = 0.0f;
ENGINE_API float Window_a = 1.0f;

bool Window::init(const char* title) {
    PROFILE_SCOPE_N("Window::Init");

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_width = 640;
    m_height = 480;
    m_title = title;

    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);

    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glViewport(0, 0, m_width, m_height);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;

    GLint combinedUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedUnits);
    std::cout << "Max combined texture image units: " << combinedUnits << std::endl;

    GLint depthEnabled = 0;
    glGetIntegerv(GL_DEPTH_TEST, &depthEnabled);
    std::cout << "Depth test enabled? " << depthEnabled << std::endl;

    int depthBits = 0;
    glGetIntegerv(GL_DEPTH_BITS, &depthBits);
    std::cout << "Actual depth buffer bits = " << depthBits << std::endl;

    beginTime = Time::getTime();
    endTime = beginTime;
    dt = 0.0f;

    return true;
}

void Window::processInput() {
    // Example: ESC to close window
    // if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //     glfwSetWindowShouldClose(m_window, true);
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(m_window);
}

void Window::update() {
    PROFILE_SCOPE_N("Window::Update");
    endTime = Time::getTime();
    dt = endTime - beginTime;
    beginTime = endTime;

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::clearScreen() {
    PROFILE_SCOPE_N("Window::ClearScreen");
    glClearColor(Window_r, Window_g, Window_b, Window_a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* Window::getGLFWWindow() {
    return m_window;
}

void Window::getFPS() {
    static float elapsed = 0.0f;
    static int frames = 0;

    frames++;
    elapsed += dt;

    if (elapsed >= 0.25f) {
        float fps = frames / elapsed;
        std::cout << "FPS: " << fps << std::endl;
        frames = 0;
        elapsed = 0.0f;
    }
}

float Window::GetFPSValue() {
    static float timer = 0.0f;
    static int frames = 0;
    static float fps = 0.0f;

    frames++;
    timer += dt;

    if (timer >= 1.0f) {
        fps = frames / timer;
        frames = 0;
        timer = 0.0f;
    }
    return fps;
}

void Window::cleanup() {
    PROFILE_SCOPE_N("Window::CleanUp");
    glfwTerminate();
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}
