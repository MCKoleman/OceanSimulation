#include "main.h"

int main()
{
    // Read Configs
    Config* config = ConfigReader::ReadFile(FileSystem::GetPath(CONFIG_JSON));

    // Init window
    Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, APP_NAME, config);
    if (window == nullptr)
        return -1;

    // Load shaders
    std::vector<Shader*> shaderList;
    LoadShaders(shaderList, config->GetConfig("shaders"));
    if (shaderList.size() > 0)
        shaderList[0]->Use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        // Poll events at the start of the loop
        glfwPollEvents();

        // TODO: Process input

        // Render the scene
        glClearColor(0.3f, 0.4f, 0.4f, 1.0f);
        glViewport(0, 0, window->GetWidth(), window->GetHeight());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw GUI
        window->DrawGUI();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers at the end of the loop
        glfwSwapBuffers(window->GetWindow());
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete window;
    return 0;
}

// Loads all defined shaders
// --------------------------------------------------
void LoadShaders(std::vector<Shader*>& shaderList, Config* shaderConfig)
{
    double loadStartTime = glfwGetTime();

    std::unordered_map<std::string, Config*> shaders = shaderConfig->GetConfigs();
    for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
    {
        shaderList.push_back(new Shader(iter->second->GetString("file"), iter->second->GetBool("hasGeomShader")));
    }

    double loadEndTime = glfwGetTime();
    double loadTime = loadEndTime - loadStartTime;
    std::cout << "Shaders loaded in " << loadTime << " seconds." << std::endl;
}