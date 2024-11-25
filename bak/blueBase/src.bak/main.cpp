// Platform defines must come first
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX

// GLFW
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// BGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <stdio.h>

// Window dimensions (16:9)
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Error callback for GLFW
static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

// Window resize callback
static void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
    bgfx::setViewRect(0, 0, 0, (uint16_t)width, (uint16_t)height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW init failed!\n");
        return -1;
    }

    // Set error callback
    glfwSetErrorCallback(errorCallback);

    // Get primary monitor
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor) {
        printf("Failed to get primary monitor!\n");
        glfwTerminate();
        return -1;
    }

    // Get monitor resolution
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    if (!mode) {
        printf("Failed to get video mode!\n");
        glfwTerminate();
        return -1;
    }

    // Calculate center position
    int xpos = (mode->width - WINDOW_WIDTH) / 2;
    int ypos = (mode->height - WINDOW_HEIGHT) / 2;

    // Create window with GLFW
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT,
        "BGFX Project",
        nullptr,
        nullptr
    );

    if (!window) {
        printf("Failed to create window!\n");
        glfwTerminate();
        return -1;
    }

    // Set window position and callbacks
    glfwSetWindowPos(window, xpos, ypos);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    // Initialize BGFX
    bgfx::PlatformData pd;
    pd.ndt = glfwGetX11Display();
    pd.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
    bgfx::setPlatformData(pd);

    bgfx::Init init;
    init.type = bgfx::RendererType::OpenGL; // Explicitly use OpenGL
    init.resolution.width = WINDOW_WIDTH;
    init.resolution.height = WINDOW_HEIGHT;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = pd;
    
    if (!bgfx::init(init)) {
        printf("Failed to init bgfx!\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Enable debug text
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    // Set view 0 clear state
    bgfx::setViewClear(0,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0x0000ffff, // Blue Background
        1.0f,
        0
    );

    // Set initial view rect
    bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Get current window size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        
        // Update view rect if window size changed
        bgfx::setViewRect(0, 0, 0, (uint16_t)width, (uint16_t)height);

        // Set view transform
        bgfx::touch(0);

        // Use debug font to print information about this example
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "Title: bgfx_project");
        bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");
        bgfx::dbgTextPrintf(0, 3, 0x0f, "Frame: % 7.3f[ms]", 1000.0f * 16.0f);

        // Advance to next frame
        bgfx::frame();
    }

    // Cleanup
    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
