#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include <random>
#include "window.h"
#include "graphics.h"
#include "orbit.h"

class Engine
{
  public:
    Engine(const char*  name, int width, int height);
    
    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    double getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

  
  private:

    void updateDelta();

    // Window related variables
    Window *m_window;    
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN, m_swap, m_moveLeft, m_moveRight;
    double oldX, oldY, oldTime;
    double deltaTime;

    int planetIndex;

    bool planetaryObservation;

    float throttle;
    float yaw, pitch;

    Graphics *m_graphics;
    SceneNode* player;
    std::vector<SceneNode*> planets;
    std::vector<Orbit*> orbits;
    SceneNode* skybox;

    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;

    std::default_random_engine rand;
    std::uniform_real_distribution<float> urd;

    bool m_running;
};

#endif // ENGINE_H
