#include "program.h"
#include "camera.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

Program::Program() {
    camera = Camera();
    world = Chunk();
    window = initialize_window();
    initialize_imgui();
    world.generate_mesh();

    //std::cout << "- Shading Language = " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    //std::cout << "- Version = " << glGetString(GL_VERSION) << std::endl;
    //std::cout << "- Vendor = " << glGetString(GL_VENDOR) << std::endl;
    //std::cout << "- Renderer = " << glGetString(GL_RENDERER) << std::endl << std::endl;

    //GLint numExtensions;
    //glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    //std::cout << "- Extensions" << std::endl;
    //for (GLint i = 0; i < numExtensions; i++)
    //{
    //    std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;
    //}
}

Program::~Program() { cleanup(); }

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

GLFWwindow* Program::initialize_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::abort();
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::abort();
    }

    return window;
}
void Program::initialize_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui_ImplGlfw_InitForOpenGL(window, true);
}

void Program::update(float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(CameraMovement::Forward, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(CameraMovement::Backward, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(CameraMovement::Left, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(CameraMovement::Right, delta_time);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(&show_demo_window);
}

void Program::render() {
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    world.render(camera);

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Program::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Program* program = static_cast<Program*>(glfwGetWindowUserPointer(window));

    glm::vec2 mouse_pos(xpos, ypos);

    if (program->first_mouse) {
        program->last_mouse_pos = mouse_pos;
        program->first_mouse = false;
    }

    glm::vec2 offset = mouse_pos - program->last_mouse_pos;
    program->last_mouse_pos = mouse_pos;

    program->camera.process_mouse_movement(offset);
}
void Program::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Program* program = static_cast<Program*>(glfwGetWindowUserPointer(window));
    program->camera.process_mouse_scroll(yoffset);
}

void Program::cleanup() {
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
