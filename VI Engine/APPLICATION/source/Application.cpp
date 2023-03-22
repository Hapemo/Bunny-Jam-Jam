/*!*****************************************************************************
\file Application.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Main application that gets called in the main loop. It handles the creation and
start up of window and game system, also runs their update functions.
*******************************************************************************/
#include "Application.h"
//#include "Windows.h"
#include "Helper.h"
#include "Input.h"
#include "GameStateManager.h"
#include "ECSManager.h"
#include "Editor\EditorManager.h"
#include "PerformanceVisualiser.h"
#include "ResourceManager.h"
#include "ScriptManager.h"
#include "FilePathManager.h"
#include "NetworkSerialization.h"

#define NETWORKSERIALISETEST 0

#include <winsock2.h>
#include "ClientManager.h"
#include "ServerManager.h"

// Static variables
int Application::window_width{};
int Application::window_height{};
std::string Application::title{ "gam200" };
GLFWwindow* Application::ptr_window;
std::string Application::mCurrGameStateName{ "" };
bool Application::mLoadAllResources{ true };

void Application::startup() {
    loadConfig("../config.txt");
    FilePathManager::Init("../filePaths.txt");
    GLFWStartUp();
    Input::Init(ptr_window);
    Helper::Init(ptr_window);
    GlewStartUp();
    ECSManager::ECS_init();
    //GameStateManager::GetInstance()->Init();
}

void Application::SystemInit() {
#ifdef _EDITOR
    editorManager->Load(ptr_window, &window_width, &window_height);
#endif
    audioManager->Init();
    //aiManager->weatherAIinit();
    movementAIManager->Init();
    renderManager->Init(&window_width, &window_height);
    buttonManager->Init(&window_width, &window_height);
    //playerManager->Init(window_width, window_height);
#ifdef _EDITOR
    renderManager->RenderToFrameBuffer();
#else
    renderManager->RenderToScreen();
#endif
    // For render debug
    renderManager->SetVectorLengthModifier(5.f);

    // Collision database initialization
    collision2DManager->SetupCollisionDatabase();

    // Activate logic system & Mono
    logicSystem->Activate();

#ifdef _EDITOR
#ifdef _DEBUG
    if (Application::mLoadAllResources) // TODO: This should be removed during game launch.
#endif
        ResourceManager::GetInstance()->LoadAllResources();
#endif

#ifdef _EDITOR
    editorManager->Init(); //need loaded resources
#endif

      // Part 1.1 (Networking init)
#ifdef _CLIENT
    ////std::string tempServerAdd = "172.28.136.35";
    //std::string tempServerAdd = "172.28.137.119";
    //u_short tempServerPort = 5051;
    //std::string tempClientName = "Client1";
    //if(!ClientManager::GetInstance()->clientInit(tempServerAdd, tempServerPort, tempClientName)) {
    //  std::cout << "ERROR: ---------- SOME FUNKY BUSINESS IS GOING ON IN CLIENT INIT\n";
    //}
#endif

#ifdef _SERVER
  u_short tempServerPort = 5051;
  
  if (!ServerManager::GetInstance()->serverInit(tempServerPort)) {
      std::cout << "ERROR: ---------- SOME FUNKY BUSINESS IS GOING ON IN SERVER INIT\n";
  }
#endif
  
  // Set fullscreen for .exe build
  //Helper::SetFullScreen(true);
}

void Application::SystemUpdate() {
  buttonManager->Update();
  // AI
  TRACK_PERFORMANCE("AI");
  aiManager->UpdateAI();
  movementAIManager->Update();
  END_TRACK("AI");

  //Scripting
  TRACK_PERFORMANCE("Scripting");
  logicSystem->EarlyUpdate();
  logicSystem->Update();
  logicSystem->FixedUpdate();
  logicSystem->LateUpdate();
  END_TRACK("Scripting");

  // Physics
  TRACK_PERFORMANCE("Physics");
  //try {
  //    physics2DManager->mPhysicsStepLock.lock();
  //    std::thread phyThread{ [] {physics2DManager->Update(FPSManager::dt); } };
  //    phyThread.join();
  //    physics2DManager->mPhysicsStepLock.unlock();
  //}
  //catch (...) {
  //    if (physics2DManager->mPhysicsStepLock.try_lock())
  //        physics2DManager->mPhysicsStepLock.unlock();
  //}
  physics2DManager->Update(FPSManager::dt);
  END_TRACK("Physics");

  TRACK_PERFORMANCE("Collision");
  collision2DManager->Update(FPSManager::dt);
  END_TRACK("Collision");

  // Particles
  TRACK_PERFORMANCE("Particle");
  particleManager->Update();
  END_TRACK("Particle");

  // Layer
  TRACK_PERFORMANCE("Layer");
  layerManager->Update(FPSManager::dt);
  END_TRACK("Layer");

  // Animator
  TRACK_PERFORMANCE("Animation");
  animator->Animate();
  sheetAnimator->Animate();
  END_TRACK("Animation");

  //// Audio
  //TRACK_PERFORMANCE("Audio");   //shifted to update in editor
  //audioManager->UpdateSound();
  //END_TRACK("Audio");
}

#if NETWORKSERIALISETEST
Entity globalE{};
Entity globalE2{};
Entity globalE3{};
#endif

void Application::init() {
  // Part 1
  startup();
  SystemInit();

  GameStateManager::GetInstance()->Init();
  // Set up quadtree after scene entities have been loaded
  collision2DManager->SetupQuadTree();

#if NETWORKSERIALISETEST
  {
    globalE = ECS::CreateEntity();

    globalE.AddComponent<General>(General());
    globalE.GetComponent<General>().name = "testingName";
    globalE.GetComponent<General>().isActive = true;
    globalE.GetComponent<General>().isPaused = false;

    globalE.AddComponent<Transform>(Transform());
    globalE.GetComponent<Transform>().rotation = 12.3f;
    globalE.GetComponent<Transform>().scale = Math::Vec2{ 2.23f, 3.23f };
    globalE.GetComponent<Transform>().translation = Math::Vec2{ 4.23f, 5.23f };

    globalE.AddComponent<Physics2D>(Physics2D());
    globalE.GetComponent<Physics2D>().dynamicsEnabled = true;
    globalE.GetComponent<Physics2D>().mass = 1.123f;
    globalE.GetComponent<Physics2D>().inertia = 2.123f;
    globalE.GetComponent<Physics2D>().restitution = 3.123f;
    globalE.GetComponent<Physics2D>().friction = 4.123f;
    globalE.GetComponent<Physics2D>().damping = 5.123f;
    globalE.GetComponent<Physics2D>().accumulatedForce = Math::Vec2{ 10.123f, 11.123f };
    globalE.GetComponent<Physics2D>().velocity = Math::Vec2{ 12.123f, 13.123f };
    globalE.GetComponent<Physics2D>().acceleration = Math::Vec2{ 14.123f, 15.123f };
    globalE.GetComponent<Physics2D>().angularVelocity = 6.123f;
    globalE.GetComponent<Physics2D>().angularTorque = 7.123f;

    std::array<Force, 5> forces;
    int count{};
    for (auto& force : forces) {
      if (++count == 3) break;
      force.lifetimeLimit = 1.23;
      force.age = 2.23;
      force.isActive = true;
      force.forceID = 2;
      force.dragForce.directionalDrag = 10.23f;
      force.dragForce.rotationalDrag = 11.23f;
      globalE.GetComponent<Physics2D>().forceList.push_back(force);
    }

    count = 0;
    for (auto& force : forces) {
      if (++count < 3) continue;
      force.lifetimeLimit = 1.23;
      force.age = 2.23;
      force.isActive = true;
      force.forceID = 0;
      force.linearForce.unitDirection = Math::Vec2{ 10.23f,11.23f };
      force.linearForce.magnitude = 12.23f;
      globalE.GetComponent<Physics2D>().forceList.push_back(force);
    }

    globalE.AddComponent<RectCollider>(RectCollider());
    globalE.GetComponent<RectCollider>().centerOffset = Math::Vec2{ 20.123f, 21.123f };
    globalE.GetComponent<RectCollider>().scaleOffset = Math::Vec2{ 22.123f, 23.123f };
    globalE.GetComponent<RectCollider>().isTrigger = true;
    globalE.GetComponent<RectCollider>().renderFlag = false;

    globalE.AddComponent<CircleCollider>(CircleCollider());
    globalE.GetComponent<CircleCollider>().centerOffset = Math::Vec2{ 30.123f, 31.123f };
    globalE.GetComponent<CircleCollider>().scaleOffset = 32.123f;
    globalE.GetComponent<CircleCollider>().isTrigger = true;
    globalE.GetComponent<CircleCollider>().renderFlag = false;
  }

  {
    globalE2 = ECS::CreateEntity();

    globalE2.AddComponent<General>(General());
    globalE2.GetComponent<General>().name = "testingName2";
    globalE2.GetComponent<General>().isActive = true;
    globalE2.GetComponent<General>().isPaused = false;

    globalE2.AddComponent<Transform>(Transform());
    globalE2.GetComponent<Transform>().rotation = 72.3f;
    globalE2.GetComponent<Transform>().scale = Math::Vec2{ 2.23f, 3.23f };
    globalE2.GetComponent<Transform>().translation = Math::Vec2{ 4.23f, 5.23f };

    globalE2.AddComponent<Physics2D>(Physics2D());
    globalE2.GetComponent<Physics2D>().dynamicsEnabled = true;
    globalE2.GetComponent<Physics2D>().mass = 7.723f;
    globalE2.GetComponent<Physics2D>().inertia = 2.723f;
    globalE2.GetComponent<Physics2D>().restitution = 3.723f;
    globalE2.GetComponent<Physics2D>().friction = 4.723f;
    globalE2.GetComponent<Physics2D>().damping = 5.723f;
    globalE2.GetComponent<Physics2D>().accumulatedForce = Math::Vec2{ 70.723f, 77.723f };
    globalE2.GetComponent<Physics2D>().velocity = Math::Vec2{ 72.723f, 73.723f };
    globalE2.GetComponent<Physics2D>().acceleration = Math::Vec2{ 74.723f, 75.723f };
    globalE2.GetComponent<Physics2D>().angularVelocity = 6.723f;
    globalE2.GetComponent<Physics2D>().angularTorque = 7.723f;

    std::array<Force, 5> forces;
    int count{};
    for (auto& force : forces) {
      if (++count == 3) break;
      force.lifetimeLimit = 7.23;
      force.age = 2.23;
      force.isActive = true;
      force.forceID = 2;
      force.dragForce.directionalDrag = 70.23f;
      force.dragForce.rotationalDrag = 77.23f;
      globalE2.GetComponent<Physics2D>().forceList.push_back(force);
    }

    count = 0;
    for (auto& force : forces) {
      if (++count < 3) continue;
      force.lifetimeLimit = 7.23;
      force.age = 2.23;
      force.isActive = true;
      force.forceID = 0;
      force.linearForce.unitDirection = Math::Vec2{ 70.23f,77.23f };
      force.linearForce.magnitude = 72.23f;
      globalE2.GetComponent<Physics2D>().forceList.push_back(force);
    }

    globalE2.AddComponent<RectCollider>(RectCollider());
    globalE2.GetComponent<RectCollider>().centerOffset = Math::Vec2{ 20.723f, 27.723f };
    globalE2.GetComponent<RectCollider>().scaleOffset = Math::Vec2{ 22.723f, 23.723f };
    globalE2.GetComponent<RectCollider>().isTrigger = true;
    globalE2.GetComponent<RectCollider>().renderFlag = false;

    globalE2.AddComponent<CircleCollider>(CircleCollider());
    globalE2.GetComponent<CircleCollider>().centerOffset = Math::Vec2{ 30.723f, 37.723f };
    globalE2.GetComponent<CircleCollider>().scaleOffset = 32.723f;
    globalE2.GetComponent<CircleCollider>().isTrigger = true;
    globalE2.GetComponent<CircleCollider>().renderFlag = false;
}

  {
    globalE3 = ECS::CreateEntity();

    globalE3.AddComponent<General>(General());
    globalE3.GetComponent<General>().name = "testingName3";
    globalE3.GetComponent<General>().isActive = true;
    globalE3.GetComponent<General>().isPaused = false;

    globalE3.AddComponent<Transform>(Transform());
    globalE3.GetComponent<Transform>().rotation = 12.3f;
    globalE3.GetComponent<Transform>().scale = Math::Vec2{ 2.23f, 3.23f };
    globalE3.GetComponent<Transform>().translation = Math::Vec2{ 4.23f, 5.23f };

    globalE3.AddComponent<Physics2D>(Physics2D());
    globalE3.GetComponent<Physics2D>().dynamicsEnabled = true;
    globalE3.GetComponent<Physics2D>().mass = 1.123f;
    globalE3.GetComponent<Physics2D>().inertia = 2.123f;
    globalE3.GetComponent<Physics2D>().restitution = 3.123f;
    globalE3.GetComponent<Physics2D>().friction = 4.123f;
    globalE3.GetComponent<Physics2D>().damping = 5.123f;
    globalE3.GetComponent<Physics2D>().accumulatedForce = Math::Vec2{ 10.123f, 11.123f };
    globalE3.GetComponent<Physics2D>().velocity = Math::Vec2{ 12.123f, 13.123f };
    globalE3.GetComponent<Physics2D>().acceleration = Math::Vec2{ 14.123f, 15.123f };
    globalE3.GetComponent<Physics2D>().angularVelocity = 6.123f;
    globalE3.GetComponent<Physics2D>().angularTorque = 7.123f;

    std::array<Force, 5> forces;
    int count{};
    for (auto& force : forces) {
      if (++count == 3) break;
      force.lifetimeLimit = 1.23;
      force.age = 2.23;
      force.isActive = true;
      force.forceID = 2;
      force.dragForce.directionalDrag = 10.23f;
      force.dragForce.rotationalDrag = 11.23f;
      globalE3.GetComponent<Physics2D>().forceList.push_back(force);
    }

    count = 0;
    for (auto& force : forces) {
      if (++count < 3) continue;
      force.lifetimeLimit = 1.23;
      force.age = 2.23;
      force.isActive = true;
      force.forceID = 0;
      force.linearForce.unitDirection = Math::Vec2{ 10.23f,11.23f };
      force.linearForce.magnitude = 12.23f;
      globalE3.GetComponent<Physics2D>().forceList.push_back(force);
    }

    globalE3.AddComponent<RectCollider>(RectCollider());
    globalE3.GetComponent<RectCollider>().centerOffset = Math::Vec2{ 20.123f, 21.123f };
    globalE3.GetComponent<RectCollider>().scaleOffset = Math::Vec2{ 22.123f, 23.123f };
    globalE3.GetComponent<RectCollider>().isTrigger = true;
    globalE3.GetComponent<RectCollider>().renderFlag = false;

    globalE3.AddComponent<CircleCollider>(CircleCollider());
    globalE3.GetComponent<CircleCollider>().centerOffset = Math::Vec2{ 30.123f, 31.123f };
    globalE3.GetComponent<CircleCollider>().scaleOffset = 32.123f;
    globalE3.GetComponent<CircleCollider>().isTrigger = true;
    globalE3.GetComponent<CircleCollider>().renderFlag = false;
  }
#endif

#ifdef NDEBUG
#ifdef _EDITOR
#else
  renderManager->Render();
  Helper::SetFullScreen(true);
#endif
#endif
}

bool Application::FirstUpdate() {
  FPSManager::mPrevTime = glfwGetTime();

  // Part 1
  glfwPollEvents();

#ifdef _CLIENT
  NetworkSerializationManager::GetInstance()->SerialiseAndSend(NetworkSerializationManager::NETWORKDATATYPE::C2SPlayerControls);
#endif


#ifdef _SERVER
  if (Input::CheckKey(PRESS, SPACE))
    NetworkSerializationManager::GetInstance()->SerialiseAndSend(NetworkSerializationManager::NETWORKDATATYPE::S2CNumOfClientConnected);
#endif


#if NETWORKSERIALISETEST
  //NetworkSerializationManager::GetInstance()->SerialisePlayerControls();
  GameStateManager::GetInstance()->mCurrentGameState->mScenes[0].mEntities.insert(globalE);
  GameStateManager::GetInstance()->mCurrentGameState->mScenes[0].mEntities.insert(globalE2);
  GameStateManager::GetInstance()->mCurrentGameState->mScenes[0].mEntities.insert(globalE3);
  std::set<Entity> globalEs;
  globalEs.insert(globalE);
  globalEs.insert(globalE2);
  globalEs.insert(globalE3);
  int size = NetworkSerializationManager::GetInstance()->SerialiseMultipleEntities(globalEs);
  std::cout << "size: " << size << '\n';
#endif

  // Part 2
  FPSManager::CalcFPS(0);
  return !Helper::GetWindowMinimized();
}

void Application::SecondUpdate() {
  PrintTitleBar(0.3);

#ifndef _SERVER
  // Close the window if the close flag is triggered
  if (glfwWindowShouldClose(Application::getWindow())) GameStateManager::mGSMState = GameStateManager::E_GSMSTATE::EXIT;
  /////audioManager->UpdateSound();
#endif

#if NETWORKSERIALISETEST
  NetworkSerializationManager::GetInstance()->TestTransferData();
  //NetworkSerializationManager::GetInstance()->DeserialisePlayerControls();
  NetworkSerializationManager::GetInstance()->DeserialiseMultipleEntities();
#endif 

  // Reset input
  Input::UpdatePrevKeyStates();
  buttonManager->ResetAllButtons();

#ifndef _SERVER
  // Part 2: swap buffers: front <-> back
  glfwSwapBuffers(Application::getWindow());
#endif

  FPSManager::LimitFPS();
  FPSManager::CalcDeltaTime();
}

void Application::MainUpdate() {
  // Update gamestate and loop it. 
  // Stop when exit or restart game state.
  // Stop when change game state. 

  // Structure is this,
  // 
  // Application and controls first update
  // Editor update
  // Logic & Systems update
  // Graphics update
  // Application ending update

  while (GameStateManager::mGSMState != GameStateManager::E_GSMSTATE::EXIT) {
    if (!FirstUpdate()) {
      audioManager->SetALLVolume(0.f);   //need pause all the audio... and resume properly
      continue;
    }
    TRACK_PERFORMANCE("MainLoop");
#ifdef _EDITOR
    TRACK_PERFORMANCE("Editor");
    editorManager->Update();
    END_TRACK("Editor");
    if (!editorManager->IsScenePaused()) {
      GameStateManager::GetInstance()->Update(); // Game logic
      SystemUpdate(); // Should be called after logic
    }
#else
    GameStateManager::GetInstance()->Update(); // Game logic
    shadowManager->Update();
    SystemUpdate();

#endif
    static bool toggle{ false };
    if (Input::CheckKey(HOLD, LEFT_CONTROL) && Input::CheckKey(PRESS, F)) Helper::SetFullScreen(toggle = !toggle);

    TRACK_PERFORMANCE("Shadow");
    //shadowManager->Update();
    END_TRACK("Shadow");


#ifndef _SERVER
    TRACK_PERFORMANCE("Graphics");
    //--------------------- Drawing and rendering ---------------------
    renderManager->Render();
    //-----------------------------------------------------------------
    END_TRACK("Graphics");

    // Audio
    //TRACK_PERFORMANCE("Audio");
    audioManager->UpdateSound(); 
    //END_TRACK("Audio");
#endif

    // If it changes, it should've came from when updaing game logic
    //if (Input::CheckKey(PRESS, ESCAPE)) GameStateManager::GetInstance()->GameStateExit();
    GameStateManager::GetInstance()->UpdateNextGSMState();

    SecondUpdate(); // This should always be the last
    END_TRACK("MainLoop");
  }
  glfwSetWindowShouldClose(ptr_window, 1);
}

void Application::exit() {
    //collision2DManager->Cleanup();
  GameStateManager::GetInstance()->Unload();
  ECS::DestroyAllEntities();
#ifdef _EDITOR
  editorManager->Unload();
#endif
  audioManager->Unload();
  spriteManager->FreeTextures();
  logicSystem->Close(); // Close Mono
  ScriptManager<ScriptComponent>::GetInstance()->UnloadScripts();
  ResourceManager::GetInstance()->UnloadAllResources();
  SingletonManager::destroyAllSingletons();
  // Part 2
  glfwTerminate();
}

// Private helper functions
void Application::loadConfig(std::string path) {
  // Opening file
  std::fstream file;
  file.open(path, std::ios_base::in);
  if (!file.is_open()) {
    LOG_WARN("File " + path + " not found.\n");
    return;
  }
  
  std::map<std::string, std::string> config = Util::TextFileToMap(file);

  // Applying configurations
#ifdef _DEBUG
  std::cout << "config file\n-----------\n";
#endif
  for (auto [key, value] : config) {
#ifdef _DEBUG
    std::cout << key << " | " << value << '\n';
#endif
    ASSERT(value.length() < 1, "Config error: " + key + " not found!\n");

    if (key == "window_width") window_width = stoi(value);
    else if (key == "window_height") window_height = stoi(value);
    else if (key == "title") title = value;
    else if (key == "fps_limit") FPSManager::mLimitFPS = static_cast<double>(stoi(value));
    //else if (key == "starting_gamestate") GameStateManager::GetInstance()->SetStartingGS(static_cast<E_GS>(stoi(value)));
    else if (key == "load_all_resources") Application::mLoadAllResources = stoi(value);
    else if (key == "new_starting_gamestate") GameStateManager::GetInstance()->SetNextGSPath(value);
  }
#ifdef _DEBUG
  std::cout << "-----------\n";
#endif
}

// _s, time interval in updating titlebar, in seconds
void Application::PrintTitleBar(double _s) {
  static bool printDebug{ true };
  if (Input::CheckKey(HOLD, LEFT_ALT) && Input::CheckKey(HOLD, LEFT_SHIFT) && Input::CheckKey(PRESS, S)) printDebug = !printDebug;

  static double timeElapsed{};
  timeElapsed += FPSManager::dt;
  if (timeElapsed > _s) {
    timeElapsed = 0;

    // write window title with current fps ...
    std::stringstream sstr;

#if _DEBUG
    sstr << std::fixed << std::setprecision(3) << Application::getTitle() << " | " 
                                               << "GameState: " << mCurrGameStateName << " | "
                                               << "fps: " << FPSManager::fps << " | "
                                               << "dt: " << FPSManager::dt << " | "
                                               << "Entity Count: " << Coordinator::GetInstance()->GetEntityCount() << " | ";
    if (printDebug) sstr << GET_SYSTEMS_PERFORMANCES();
#else
    sstr << Application::getTitle();
#endif
    glfwSetWindowTitle(Application::getWindow(), sstr.str().c_str());
  }
}

void Application::GLFWStartUp() {
  // Part 1
  ASSERT(!glfwInit(), "GLFW init has failed - abort program!!!\n");
  
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  if (!window_width) window_width = mode->width;
  if (!window_height) window_height = mode->height;

  // In case a GLFW function fails, an error is reported to callback function
  glfwSetErrorCallback(error_cb);
  
   //Before asking GLFW to create an OpenGL context, we specify the minimum constraints
  // in that context:
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // window dimensions are not static

  ptr_window = glfwCreateWindow(window_width, window_height, title.c_str(), NULL, NULL);
  glfwSetWindowAspectRatio(ptr_window, window_width, window_height);
  glfwSetWindowSizeCallback(ptr_window, [](GLFWwindow* window, int width, int height)
      {
          (void)window;
          window_width = width;
          window_height = height;
      });
  if (!ptr_window) {
    glfwTerminate();
    ASSERT(!ptr_window, "GLFW unable to create OpenGL context - abort program");
  }

  glfwMakeContextCurrent(ptr_window);

  glfwSetFramebufferSizeCallback(ptr_window, fbsize_cb);
  glfwSwapInterval(0);
}

void Application::GlewStartUp() {
  // Part 2: Initialize entry points to OpenGL functions and extensions
  GLenum err = glewInit();

  std::stringstream string;
  string << "Unable to initialize GLEW - error " << glewGetErrorString(err) << " abort program\n";
  ASSERT(GLEW_OK, string.str());
}

void Application::error_cb(int error, char const* description) {
    (void)error;
    (void)description; // This function should only be called for debug mode
#ifdef _DEBUG
  std::cerr << "GLFW error " << error << ": " << description << std::endl;
#endif
}

void Application::fbsize_cb(GLFWwindow* ptr_win, int width, int height) {
#ifdef _DEBUG
  std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
  // use the entire framebuffer as drawing region
  glViewport(0, 0, width, height);
  (void)ptr_win;
  // later, if working in 3D, we'll have to set the projection matrix here ...
}
