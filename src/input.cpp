#include "input.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
bool ProcessInput(Window* window, Scene* scene, State* state, InputLocks* locks, float deltaTime, int* prevX, int* prevY)
{
    Camera* camera = scene->GetCamera();
    Config* config = state->GetConfig();
    bool didReceiveInput = false;

    // Get window width and height
    window->CalcWindowSize();
    int width = window->GetWidth();
    int height = window->GetHeight();
    GLFWwindow* glfwWindow = window->GetWindow();
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    /* ==================================================== Window Stuff ===================================================== */
    // Exit window
    if (ESC_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, true);
        return true;
    }

    /* ==================================================== Keyboard Input ===================================================== */
    /* =================================== Handle movement input ================================== */
    if (WASDZX_PRESS && !CTRL_PRESS && ALT_PRESS) {
        camera->Translate(deltaTime * config->GetFloat("speeds.camMove") * GetWASDZX(window));
        didReceiveInput = true;
    }

    /* =================================== Handle camera ================================== */
    if (ARROW_PRESS && !CTRL_PRESS && ALT_PRESS) {
        // Rotate camera
        camera->Rotate(GetArrow(window) * config->GetFloat("speeds.camTurn") * deltaTime);
        didReceiveInput = true;
    }

    // Disable UI
    if (SPACE_PRESS)
    {
        if (!locks->lockSpace) {
            state->drawGUI = !state->drawGUI;
            locks->lockSpace = true;
        }
        didReceiveInput = true;
    }

    // Toggle Wireframe
    if (B_PRESS) {
        if (!locks->lockB) {
            scene->GetCamera()->SetWireframe(!scene->GetCamera()->IsWireframe());
            locks->lockB = true;
        }
        didReceiveInput = true;
    }


    /* ==================================================== Mouse Input ===================================================== */
    // Mouse input
    if (MOUSE_PRESS) {
        // Track mouse movement
        double mouseX, mouseY;
        glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
        int xPos = (int)glm::floor(mouseX);
        int yPos = (int)glm::floor(mouseY);
        float u = (xPos + 0.5f) / width;
        float v = (yPos + 0.5f) / height;

        // Reset previous positions
        if (*prevX < 0)
            *prevX = xPos;
        if (*prevY < 0)
            *prevY = yPos;

        float deltaX = float(*prevX - xPos);
        float deltaY = float(*prevY - yPos);

        // Handle moving with mouse
        // Alt + LMB to rotate
        if (LEFT_MOUSE_PRESS && ALT_PRESS) {
            // Apply changes to camera
            camera->Rotate(config->GetFloat("speeds.mouseTurn")* deltaTime * glm::vec3(deltaX, deltaY, 0.0f));
            locks->lockLeftMouse = true;

            // Keep mouse where it was clicked
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetCursorPos(window->GetWindow(), (float)(*prevX), (float)(*prevY));
        }
        // Alt + RMB to move
        else if (RIGHT_MOUSE_PRESS && ALT_PRESS) {
            // Apply changes to camera
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            camera->Translate(config->GetFloat("speeds.mouseMove") * deltaTime * glm::vec3(0.0f, deltaX, deltaY));
            // Keep mouse where it was clicked
            glfwSetCursorPos(window->GetWindow(), (float)(*prevX), (float)(*prevY));
        }

        didReceiveInput = true;
    }
    // Reset previous click location when there 
    else {
        locks->lockLeftMouse = false;
        locks->lockRightMouse = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        *prevX = -1;
        *prevY = -1;
    }

    /* ==================================================== Process Input ===================================================== */
    // If no input was received, clear all input locks
    if (!didReceiveInput) {
        locks->ClearLocks();
    }

    return didReceiveInput;
}

// Returns the combined input vector of arrow keys
glm::vec3 GetArrow(Window* window)
{
    GLFWwindow* glfwWindow = window->GetWindow();
    glm::vec3 output = glm::vec3(0, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
        output += glm::vec3(0, 1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        output += glm::vec3(0, -1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        output += glm::vec3(-1, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        output += glm::vec3(1, 0, 0);

    return output;
}

// Returns the combined input vector of WASDZX
glm::vec3 GetWASDZX(Window* window)
{
    GLFWwindow* glfwWindow = window->GetWindow();
    glm::vec3 output = glm::vec3(0, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        output += glm::vec3(1, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        output += glm::vec3(-1, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        output += glm::vec3(0, 1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        output += glm::vec3(0, -1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_Z) == GLFW_PRESS)
        output += glm::vec3(0, 0, 1);
    if (glfwGetKey(glfwWindow, GLFW_KEY_X) == GLFW_PRESS)
        output += glm::vec3(0, 0, -1);

    return output;
}

// Resets all locks on keys
void InputLocks::ClearLocks()
{
    lockSpace = false;
    lockB = false;
    rerender = false;
}