#include "engine.h"
#include "glm/ext.hpp"
#include "mesh.h"
#include "sphere.h"
#include <chrono>

float currentScrollValue = 0;

Engine::Engine(const char* name, int width, int height) : rand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()), urd(60, 70)
{
	m_WINDOW_NAME = name;
	m_WINDOW_WIDTH = width;
	m_WINDOW_HEIGHT = height;

	m_moveLeft = false;
	m_moveRight = false;
	m_swap = false;
	m_FULLSCREEN = false;
	m_running = true;
	m_window = nullptr;
	m_graphics = nullptr;

	oldX = width / 2.0;
	oldY = height / 2.0;

	player = nullptr;
	throttle = 0;
	planetaryObservation = false;
	planetIndex = 0;
	planets.reserve(15);
	yaw = 0;
	pitch = 0;
}


Engine::~Engine()
{
	delete m_window;
	delete m_graphics;
	m_window = nullptr;
	m_graphics = nullptr;
}

bool Engine::Initialize()
{
	// Start a window
	m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
	if (!m_window->Initialize())
	{
		printf("The window failed to initialize.\n");
		return false;
	}

	// Start the graphics
	m_graphics = Graphics::getInstance();
	if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
	{
		printf("The graphics failed to initialize.\n");
		return false;
	}

	// glfwSetCursorPosCallback(m_window->getWindow(), cursor_position_callback);
	glfwSetScrollCallback(m_window->getWindow(), scroll_callback);
	glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	SceneNode* Sun = m_graphics->AddObject("Sun", new Sphere(64, "assets\\2k_sun.jpg", "assets\\2k_moon-n.jpg"));
	Sun->setLocalScale(glm::vec3(20.f, 20.f, 20.f));
	Sphere* sunSphere = (Sphere*)Sun->getObject();
	if (sunSphere != nullptr) {
		sunSphere->setIsEmissive(true);
	}

	SceneNode* Mercury = m_graphics->AddObject("Mercury", nullptr);
	Mercury->addChild(m_graphics->AddObject("Planet", new Sphere(48, "assets\\Mercury.jpg", "assets\\Mercury-n.jpg")));
	Mercury->setLocalScale(glm::vec3(.0361f, .0361f, .0361f));
	Mercury->setLocalPosition(glm::vec3(66.2f, 0.f, 0.f));

	SceneNode* Venus = m_graphics->AddObject("Venus", nullptr);
	Venus->addChild(m_graphics->AddObject("Planet", new Sphere(48, "assets\\Venus.jpg", "assets\\Venus-n.jpg")));
	Venus->setLocalScale(glm::vec3(.0869f, .0869f, .0869f));
	Venus->setLocalPosition(glm::vec3(154.19f, 0.f, 0.f));

	SceneNode* Earth = m_graphics->AddObject("Earth", nullptr);
	Earth->addChild(m_graphics->AddObject("Planet", new Sphere(48, "assets\\2k_earth_daymap.jpg", "assets\\2k_earth_daymap-n.jpg")));
	Earth->setLocalScale(glm::vec3(.0917f, .0917f, .0917f));
	Earth->setLocalPosition(glm::vec3(211.87f, 0.f, 0.f));

	SceneNode* Moon = m_graphics->AddObject("Moon", new Sphere(48, "assets\\2k_moon.jpg", "assets\\2k_moon-n.jpg"));
	Moon->setLocalScale(glm::vec3(.25f, .25f, .25f));
	Moon->setLocalPosition(glm::vec3(5.52f, 0.f, 0.f));

	SceneNode* Mars = m_graphics->AddObject("Mars", nullptr);
	Mars->addChild(m_graphics->AddObject("Planet", new Sphere(48, "assets\\Mars.jpg", "assets\\Mars-n.jpg")));
	Mars->setLocalScale(glm::vec3(.0487f, .0487f, .0487f));
	Mars->setLocalPosition(glm::vec3(323.58f, 0.f, 0.f));

	SceneNode* Jupiter = m_graphics->AddObject("Jupiter", nullptr);
	Jupiter->addChild(m_graphics->AddObject("Planet", new Sphere(48, "assets\\Jupiter.jpg", "assets\\Jupiter-n.jpg")));
	Jupiter->setLocalScale(glm::vec3(1.f, 1.f, 1.f));
	Jupiter->setLocalPosition(glm::vec3(1070.79f, 0.f, 0.f));

	SceneNode* Saturn = m_graphics->AddObject("Saturn", nullptr);
	Saturn->addChild(m_graphics->AddObject("Planet", new Sphere(48, "assets\\Saturn.jpg", "assets\\Jupiter-n.jpg")));
	Saturn->setLocalScale(glm::vec3(.865f, .865f, .865f));
	Saturn->setLocalPosition(glm::vec3(2090.58f, 0.f, 0.f));

	SceneNode* Uranus = m_graphics->AddObject("Uranus", nullptr);
	Uranus->addChild(m_graphics->AddObject("Planet", new Sphere(48, "assets\\Uranus.jpg", "assets\\Uranus-n.jpg")));
	Uranus->setLocalScale(glm::vec3(.3673f, .3673f, .3673f));
	Uranus->setLocalPosition(glm::vec3(4125.8f, 0.f, 0.f));

	SceneNode* Neptune = m_graphics->AddObject("Neptune", nullptr);
	Neptune->addChild(m_graphics->AddObject("Planet", new Sphere(48, "assets\\Neptune.jpg", "assets\\Neptune-n.jpg")));
	Neptune->setLocalScale(glm::vec3(.355f, .355f, .355f));
	Neptune->setLocalPosition(glm::vec3(6455.3f, 0.f, 0.f));

	// Starship
	Mesh* ship = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png", "assets\\SpaceShip-1-n.png");
	ship->setIsEmissive(true);
	player = m_graphics->AddObject("Spaceship", ship);
	player->setLocalScale(glm::vec3(.002f, .002f, .002f));
	player->setLocalPosition(glm::vec3(20.87f, 0.f, 0.f));

	Mesh* box = new Mesh(glm::vec3(0.f, 0.f, 0.f), "assets\\skybox.obj", "assets\\Cubemaps\\Galaxy2.jpg", "");
	skybox = m_graphics->AddObject("Skybox", box);
	skybox->setLocalScale(glm::vec3(10000, 10000, 10000));

	if (box != nullptr) {
		box->setIsEmissive(true);
	}

	Sun->addChild(Mercury);
	Sun->addChild(Venus);
	Sun->addChild(Earth);
	Sun->addChild(Mars);
	Sun->addChild(Jupiter);
	Sun->addChild(Saturn);
	Sun->addChild(Uranus);
	Sun->addChild(Neptune);

	Earth->addChild(Moon);

	planets.push_back(Sun);
	planets.push_back(Mercury);
	planets.push_back(Venus);
	planets.push_back(Earth);
	planets.push_back(Mars);
	planets.push_back(Jupiter);
	planets.push_back(Saturn);

	orbits.push_back(new Orbit(Mercury, 6.62f, 3.972f, 8.8f, 176.f));
	orbits.push_back(new Orbit(Venus, 15.419f, 9.2514f, 22.5f, 243.f));
	orbits.push_back(new Orbit(Earth, 21.187f, 12.7122f, 36.5f, 1.f));
	orbits.push_back(new Orbit(Mars, 32.358f, 19.4148f, 68.7f, 1.025694f));
	orbits.push_back(new Orbit(Jupiter, 107.079f, 64.2474f, 433.3f, 0.41389f));
	orbits.push_back(new Orbit(Saturn, 209.058f, 125.4348f, 1075.6f, 0.440278f));
	orbits.push_back(new Orbit(Uranus, 412.58f, 2475.48f, 3068.7f, 0.718056f));
	orbits.push_back(new Orbit(Neptune, 645.53f, 387.318f, 5979.9f, 0.670833f));

	orbits.push_back(new Orbit(Moon, 5, 5, 27, 29.5f));

	Sphere* roid = new Sphere(48, "assets\\Haumea.jpg", "assets\\Haumea-n.jpg");
	roid->setIsEmissive(true);

	SceneNode* Asteroid = m_graphics->AddObject("Asteroid", nullptr);
	for (int i = 0; i < 2000; ++i) {
		SceneNode* piece = m_graphics->AddObject("Asteroid", roid);
		piece->setLocalScale(glm::vec3(.03f, .03f, .03f));

		float scaleA = urd(rand);
		float progress = ((urd(rand) - 60) / 10.f) * glm::two_pi<float>();

		piece->setLocalPosition(glm::vec3(scaleA * cos(progress), 0.f, scaleA * sin(progress)));

		Asteroid->addChild(piece);
	}

	for (int i = 0; i < 3000; ++i) {
		SceneNode* piece = m_graphics->AddObject("Asteroid", roid);
		piece->setLocalScale(glm::vec3(.12f, .12f, .12f));

		float scaleA = urd(rand) * 7;
		float progress = ((urd(rand) - 60) / 10.f) * glm::two_pi<float>();
		piece->setLocalPosition(glm::vec3(scaleA * cos(progress), 0.f, scaleA * sin(progress)));

		Asteroid->addChild(piece);
	}

	Sun->addChild(Asteroid);

	orbits.push_back(new Orbit(Asteroid, 0, 0, 1000, 29.5f));

	for (int i = 0; i < 1000; ++i) {
		SceneNode* ring = m_graphics->AddObject("Ring", roid);
		ring->setLocalScale(glm::vec3(.03f, .03f, .03f));

		float distance = (i / 20) * 0.1f + 1.7f;
		float progress = ((urd(rand) - 60) / 10.f) * glm::two_pi<float>();
		ring->setLocalPosition(glm::vec3(distance * cos(progress), 0, distance * sin(progress)));
		Saturn->addChild(ring);
	}
	planets.push_back(Uranus);
	planets.push_back(Neptune);

	// No errors
	return true;
}

void Engine::Run()
{
	m_running = true;

	while (!glfwWindowShouldClose(m_window->getWindow()))
	{
		ProcessInput();
		Display(m_window->getWindow(), glfwGetTime());
		glfwPollEvents();
	}
	m_running = false;

}

void Engine::ProcessInput()
{
	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window->getWindow(), true);

	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_O) == GLFW_PRESS) {
		if (!m_swap) {
			m_swap = true;
			planetaryObservation = !planetaryObservation;
		}
	}
	else
	{
		m_swap = false;
	}

	double mouseX, mouseY;

	glfwGetCursorPos(m_window->getWindow(), &mouseX, &mouseY);

	float deltaX = (mouseX - oldX) * 0.0005f;
	float deltaY = (mouseY - oldY) * -0.0005f;
	float deltaZ = 0;

	/* Set values, and instead of zeroing out values, we set values independently together
	so you can now move in 2 directions at the same time.
	*/

	glfwSetCursorPos(m_window->getWindow(), oldX, oldY);

	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
		deltaZ = .1;
	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
		deltaZ = -.1;

	// Use exploration mode
	if (!planetaryObservation) {
		double x = 0;
		double y = 0;
		double z = 0;

		if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
			throttle -= .01;
		if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
			throttle += .01;
		if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
			x = .1;
		if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
			x = -.1;
		if (glfwGetKey(m_window->getWindow(), GLFW_KEY_R) == GLFW_PRESS)
			z = .1;
		if (glfwGetKey(m_window->getWindow(), GLFW_KEY_F) == GLFW_PRESS)
			z = -.1;

		throttle = std::max(std::min(1.f, throttle), -.5f);

		if (std::abs(throttle) < 0.04f) {
			throttle *= 0.8f;
		}

		float currentThrottle = throttle;
		if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			currentThrottle *= 50;

		player->setLocalPosition(
			player->getLocalPosition() +
			player->getForwardVector() * currentThrottle +
			player->getRightVector() * x +
			player->getUpVector() * z);

		player->setLocalRotation(player->getLocalRotation() * glm::quat(glm::vec3(-deltaY, -deltaX, deltaZ)));

		cameraPosition = player->getForwardVector() + player->getUpVector() * 0.12f;
		cameraTarget = player->getPosition();
		cameraUp = player->getUpVector();
		m_graphics->getCamera()->Follow(player);
		m_graphics->getCamera()->Zoom(-currentScrollValue / 10);
		skybox->setLocalPosition(player->getLocalPosition());
		currentScrollValue = 0;
		return;
	}

	// Use Planetary Observation mode
	// Select target planet

	// Rotate around the planet
	pitch = std::max(std::min(glm::half_pi<float>(), pitch - deltaY), -glm::half_pi<float>());
	yaw -= deltaX;

	glm::vec3 forward = glm::rotate(glm::toQuat(glm::eulerAngleY(yaw) * glm::eulerAngleX(pitch)), glm::vec3(0, 0, -1));
	float scale = planets[planetIndex]->getScale().x * 4;
	cameraPosition = forward * -scale;
	cameraUp = glm::vec3(0, 1, 0);
	m_graphics->getCamera()->Follow(planets[planetIndex]);
	m_graphics->getCamera()->Zoom(-currentScrollValue / 10);

	skybox->setLocalPosition(planets[planetIndex]->getPosition());
	currentScrollValue = 0;

	// Use keys to select another planet
	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_RELEASE) {
		if (!m_moveLeft) {
			m_moveLeft = true;
			planetIndex = (planetIndex + planets.size() - 1) % planets.size();
		}
	}
	else
	{
		m_moveLeft = false;
	}

	if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_RELEASE) {
		if (!m_moveRight) {
			m_moveRight = true;
			planetIndex = (planetIndex + 1) % planets.size();
		}
	}
	else
	{
		m_moveRight = false;
	}
}

void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void Engine::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	currentScrollValue += yOffset;
}

void Engine::updateDelta()
{
	deltaTime = glfwGetTime() - oldTime;
	oldTime = glfwGetTime();
}

double Engine::getDT()
{
	//long long TimeNowMillis = GetCurrentTimeMillis();
	//assert(TimeNowMillis >= m_currentTimeMillis);
	//unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
	//m_currentTimeMillis = TimeNowMillis;
	//return DeltaTimeMillis;
	return deltaTime;
}

long long Engine::GetCurrentTimeMillis()
{
	//timeval t;
	//gettimeofday(&t, NULL);
	//long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	//return ret;
	return (long long)glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

	updateDelta();

	double dt = getDT();
	for (Orbit* orbit : orbits) {
		orbit->Update(dt);
	}
	m_graphics->getCamera()->UpdateVectors(cameraPosition);
	m_graphics->getCamera()->Update(dt);

	m_graphics->Render();
	m_window->Swap();
}
