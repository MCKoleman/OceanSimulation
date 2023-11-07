#include "main.h"

int main()
{
    // Read Configs
    Config* config = ConfigReader::ReadFile(FileSystem::GetPath(CONFIG_JSON));

    // Create state
    State* state = new State(config);

    // Init window
    Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, APP_NAME, config, state);
    if (window == nullptr)
        return -1;

    // Create scene
    Scene* scene = new Scene(state);

    // Load shaders
    LoadShaders(scene, config->GetConfig("shaders"));

    // Load default scene
    LoadScene(scene, state, config);

    // Add GUIs
    LoadGUIs(window, state, scene);

    // Track time
    double prevFrameTime = glfwGetTime();
    double curFrameTime = glfwGetTime();
    double prevSecond = glfwGetTime();
    float deltaTime = 0.0f;
    int numFrames = 0;

    // Track input
    InputLocks locks = InputLocks();
    int prevX = 0;
    int prevY = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        // Poll events at the start of the loop
        glfwPollEvents();

        // Get deltaTime
        prevFrameTime = curFrameTime;
        curFrameTime = glfwGetTime();
        deltaTime = float(curFrameTime - prevFrameTime);
        CalculateFPS(state, prevSecond, numFrames);

        // Process input
        ProcessInput(window, scene, state, &locks, deltaTime, &prevX, &prevY);

        // Render the scene
        OpenGLDraw(window, state, scene);

        // Draw GUI
        if (state->drawGUI)
        {
            window->DrawGUI();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        // Swap buffers at the end of the loop
        glfwSwapBuffers(window->GetWindow());
    }

    // Clear up dynamic memory usage
    delete scene;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete window;
    return 0;
}

// Draws the current scene
void OpenGLDraw(Window* window, State* state, Scene* scene)
{
    // Clear BG
    glm::vec3 bgColor = scene->GetCamera()->GetBGColor();
    glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);

    // Reset viewport
    glViewport(0, 0, window->GetWidth(), window->GetHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the scene
    scene->Draw(window, scene->GetShader(scene->GetCurShader()));
}

// Opens all defined GUIs
void LoadGUIs(Window* window, State* state, Scene* scene)
{
    window->AddGUI(new GUIDebugToolsWindow(state, scene, true));
    window->AddGUI(new GUILightViewer(state, scene, true));
    window->AddGUI(new GUICameraViewer(state, scene, true));
    window->AddGUI(new GUIWaveViewer(state, scene, true));
}

// Loads all defined shaders
void LoadShaders(Scene* scene, Config* shaderConfig)
{
    double loadStartTime = glfwGetTime();

    std::unordered_map<std::string, Config*> shaders = shaderConfig->GetConfigs();
    for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
    {
        scene->CreateShader(iter->first, iter->second);
    }
    scene->UseShader("default");

    double loadEndTime = glfwGetTime();
    double loadTime = loadEndTime - loadStartTime;
    std::cout << "Shaders loaded in " << loadTime << " seconds." << std::endl;
}

// Loads the scene
void LoadScene(Scene* scene, State* state, Config* config)
{
    // Add light and camera
    scene->SetCamera(config->GetConfig("camera"));
    scene->SetLight(new Light(config->GetConfig("light")));
    scene->SetEntity(new PPlane("Water", config->GetFloat("water.size"), config->GetInt("water.divisions")));
    scene->SetMaterial(new Material(config->GetVec("material.kd"), config->GetVec("material.ka"), config->GetVec("material.ks"), config->GetFloat("material.ns")));
    Config* wave1 = config->GetConfig("waves.wave1");
    Config* wave2 = config->GetConfig("waves.wave2");
    Config* wave3 = config->GetConfig("waves.wave3");
    Config* wave4 = config->GetConfig("waves.wave4");
    scene->AddWave(new Wave(wave1->GetFloat("wavelength"), wave1->GetFloat("amplitude"), wave1->GetFloat("speed"), wave1->GetVec("direction")));
    scene->AddWave(new Wave(wave2->GetFloat("wavelength"), wave2->GetFloat("amplitude"), wave2->GetFloat("speed"), wave2->GetVec("direction")));
    scene->AddWave(new Wave(wave3->GetFloat("wavelength"), wave3->GetFloat("amplitude"), wave3->GetFloat("speed"), wave3->GetVec("direction")));
    scene->AddWave(new Wave(wave4->GetFloat("wavelength"), wave4->GetFloat("amplitude"), wave4->GetFloat("speed"), wave4->GetVec("direction")));
}

// Handles calculating the number of frames per second in state
void CalculateFPS(State* state, double& prevSecond, int& numFrames)
{
    // Calculate FPS
    double currentTime = glfwGetTime();
    numFrames++;

    // Check whether a full second has passed since the last update
    if (currentTime - prevSecond >= 1.0)
    {
        // Calculate FPS and Frame Length in ms
        state->fps = std::to_string(numFrames);
        std::string frameTime = std::to_string(1000.0f / numFrames);
        state->frameTime = frameTime.substr(0, frameTime.find(".") + 3) + "ms";

        // Reset frame times
        numFrames = 0;
        prevSecond = currentTime;
    }
}