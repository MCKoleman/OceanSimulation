#include "window.h"


// Opens a OpenGL window with the given name
// -----------------------------------------
Window::Window(int width, int height, const std::string& name, Config* config)
{
    // Set class variables
    // ---------------------------
    mWidth = width;
    mHeight = height;
    mName = name;
    Config* styleConfig = config->GetConfig("style");
    Config* qualityConfig = config->GetConfig("quality");

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    const char* windowName = mName.c_str();
    mWindow = glfwCreateWindow(mWidth, mHeight, windowName, NULL, NULL);
    if (mWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Set icon
    GLFWimage images[1];
    images[0].pixels = stbi_load(FileSystem::GetPath(ICON).c_str(), &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(mWindow, 1, images);
    stbi_image_free(images[0].pixels);

    // TODO: Change to import setting
    stbi_set_flip_vertically_on_load(true);

    // // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    CalcWindowSize();

    // Dear imGUI
    // -----------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& tempIO = ImGui::GetIO();
    tempIO.Fonts->AddFontFromFileTTF(FileSystem::GetPath(FONT_DIR + styleConfig->GetString("font")).c_str(), styleConfig->GetFloat("fontSize"));
    tempIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    mIO = &tempIO;
    SetupImGuiStyle(styleConfig->GetBool("darkTheme"), styleConfig->GetFloat("windowOpacity"));

    // Dear ImGui Docking
    mIsDockInitialized = false;
    mIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

// Sets up the ImGui Style
inline void SetupImGuiStyle(bool isDarkStyle, float alphaThreshold)
{
    //Use a ternary operator
    isDarkStyle ? ImGui::StyleColorsDark() : ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();

    // Adjusts the alpha values of the ImGui colors based on the alpha threshold.
    for (int i = 0; i < ImGuiCol_COUNT; i++)
    {
        const auto color_id = static_cast<ImGuiCol>(i);
        auto& color = style.Colors[i];
        if (color.w < alphaThreshold || color_id == ImGuiCol_FrameBg || color_id == ImGuiCol_WindowBg || color_id == ImGuiCol_ChildBg)
        {
            color.w *= alphaThreshold;
        }
    }

    // Sets the border sizes and rounding.
    style.ChildBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameRounding = 3.0f;
    style.Alpha = 1.0f;
}

// Draws all GUIs
void Window::DrawGUI()
{
    // Start the Dear ImGUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

     // Check if the docking system is enabled
    if (mIO->ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        // Create a docking space
        if (!mIsDockInitialized)
        {
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("DockSpace", nullptr, dockspace_flags);
            ImGui::PopStyleVar(3);

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

            mIsDockInitialized = true;
            ImGui::End();
        }
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}