// Platform defines must come first
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX

// GLFW
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// BGFX
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <bgfx/embedded_shader.h>

#include <stdio.h>
#include <stdlib.h>

// Window dimensions (16:9)
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Uniform handle for time
bgfx::UniformHandle u_time;

// Vertex structure
struct PosTexVertex
{
    float x;
    float y;
    float z;
    float u;
    float v;
    
    static void init()
    {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
    
    static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosTexVertex::ms_layout;

// Plane vertices - made larger
static PosTexVertex s_planeVertices[] =
{
    {-2.0f,  0.0f,  2.0f, 0.0f, 0.0f},
    { 2.0f,  0.0f,  2.0f, 1.0f, 0.0f},
    {-2.0f,  0.0f, -2.0f, 0.0f, 1.0f},
    { 2.0f,  0.0f, -2.0f, 1.0f, 1.0f},
};

// Plane indices
static const uint16_t s_planeIndices[] =
{
    0, 1, 2,
    1, 3, 2,
};

// Shader loading function
static bgfx::ShaderHandle loadShader(const char* filename)
{
    const char* shaderPath = filename;
    
    FILE* file = fopen(shaderPath, "rb");
    if (!file) {
        printf("Failed to open shader file: %s\n", filename);
        return BGFX_INVALID_HANDLE;
    }
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    const int shaderSize = (int)fileSize;
    unsigned char* mem = (unsigned char*)malloc(shaderSize + 1);
    fread(mem, 1, shaderSize, file);
    mem[shaderSize] = '\0';
    fclose(file);
    
    const bgfx::Memory* shaderMem = bgfx::copy(mem, shaderSize + 1);
    free(mem);
    
    bgfx::ShaderHandle handle = bgfx::createShader(shaderMem);
    if (bgfx::isValid(handle)) {
        bgfx::setName(handle, filename);
    }
    
    return handle;
}

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

    // Set view 0 clear state with darker background
    bgfx::setViewClear(0,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0x0000ffff, // Blue background
        1.0f,
        0
    );

    // Set initial view rect
    bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialize liquid shader setup
    PosTexVertex::init();

    // Create vertex and index buffers
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_planeVertices, sizeof(s_planeVertices)),
        PosTexVertex::ms_layout
    );

    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_planeIndices, sizeof(s_planeIndices))
    );

    // Create uniform for time
    u_time = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);

    // Load shaders
    bgfx::ShaderHandle vsh = loadShader("shaders/liquid/compiled/vs_liquid.bin");
    bgfx::ShaderHandle fsh = loadShader("shaders/liquid/compiled/fs_liquid.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Get current window size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        
        // Update view rect if window size changed
        bgfx::setViewRect(0, 0, 0, (uint16_t)width, (uint16_t)height);

        // Update time uniform
        float time = (float)glfwGetTime();
        float temp[4] = { time, 0.0f, 0.0f, 0.0f };
        bgfx::setUniform(u_time, temp);

        // Set up view matrix
        float view[16];
        bx::mtxLookAt(view, 
            { 0.0f, 2.0f, -3.0f },  // Camera position
            { 0.0f, 0.0f,  0.0f }   // Look at point
        );

        // Set up projection matrix
        float proj[16];
        bx::mtxProj(proj, 
            60.0f,                                          // FOV
            float(width)/float(height),                     // Aspect ratio
            0.1f,                                          // Near
            100.0f,                                        // Far
            bgfx::getCaps()->homogeneousDepth             // Depth type
        );

        bgfx::setViewTransform(0, view, proj);

        // Set model matrix
        float mtx[16];
        bx::mtxIdentity(mtx);
        bx::mtxRotateX(mtx, bx::toRad(15.0f));  // Slight rotation to see waves better
        bgfx::setTransform(mtx);

        // Set vertex and index buffers
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        // Set render states
        bgfx::setState(BGFX_STATE_WRITE_RGB 
                     | BGFX_STATE_WRITE_A 
                     | BGFX_STATE_WRITE_Z 
                     | BGFX_STATE_DEPTH_TEST_LESS 
                     | BGFX_STATE_MSAA
                     | BGFX_STATE_BLEND_ALPHA);

        // Submit primitive for rendering
        bgfx::submit(0, program);

        // Use debug font to print information about this example
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "Title: bgfx_project - blueBase - Basic Liquid Shader");
        bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Liquid shader demonstration");
        bgfx::dbgTextPrintf(0, 3, 0x0f, "Frame: % 7.3f[ms]", 1000.0f * 16.0f);

        // Advance to next frame
        bgfx::frame();
    }

    // Cleanup
    bgfx::destroy(vbh);
    bgfx::destroy(ibh);
    bgfx::destroy(program);
    bgfx::destroy(u_time);
    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}