#include "Game/Game.h"
#include "OpenGL/GL.h"

#include "Lua/Components/LuaComponentManifestRegistry.h"
#include "Lua/ScriptCommands/CelesteScriptCommands.h"
#include "Lua/LuaState.h"

#include "Settings/GameSettings.h"
#include "Settings/WindowSettings.h"

#include "Debug/Log.h"


namespace Celeste
{
  // Initialise static variables
  observer_ptr<Game> Game::m_current(nullptr);

  //------------------------------------------------------------------------------------------------
  Game::Game() :
    m_resourceManager(Path(Directory::getExecutingAppDirectory(), "Resources")),
    m_screenManager(),
    m_inputManager(m_screenManager.getWindow()->getGLWindow()),
    m_physicsManager(),
    m_renderManager(),
    m_audioManager(),
    m_clock(),
    m_running(false)
  {
    ASSERT(!m_current);
    m_current = this;
  }

  //------------------------------------------------------------------------------------------------
  Game::ResourceManager& Game::getResourceManager()
  {
    return m_current->m_resourceManager;
  }

  //------------------------------------------------------------------------------------------------
  ScreenManager& Game::getScreenManager()
  {
    return m_current->m_screenManager;
  }

  //------------------------------------------------------------------------------------------------
  Game::InputManager& Game::getInputManager()
  {
    return m_current->m_inputManager;
  }

  //------------------------------------------------------------------------------------------------
  Game::PhysicsManager& Game::getPhysicsManager()
  {
    return m_current->m_physicsManager;
  }

  //------------------------------------------------------------------------------------------------
  Game::RenderManager& Game::getRenderManager()
  {
    return m_current->m_renderManager;
  }

  //------------------------------------------------------------------------------------------------
  Game::AudioManager& Game::getAudioManager()
  {
    return m_current->m_audioManager;
  }

  //------------------------------------------------------------------------------------------------
  Clock& Game::getClock()
  {
    return m_current->m_clock;
  }

  //------------------------------------------------------------------------------------------------
  void Game::initialize()
  {
    glfwSetWindowCloseCallback(getWindow()->getGLWindow(), &Game::windowCloseFunc);

    Lua::CelesteScriptCommands::initialize();

    // Call onInitialize first to give derived game classes the chance to register script commands
    // before their lua Game script is run
    applySettings();
    onInitialize();
    executeGameScript();
  }

  //------------------------------------------------------------------------------------------------
  void Game::applySettings() const
  {
    std::unique_ptr<Settings::WindowSettings> windowSettings = ScriptableObject::load<Settings::WindowSettings>(Path(Resources::getResourcesDirectory(), "Data", "Settings", "WindowSettings.asset"));
    if (windowSettings != nullptr)
    {
      windowSettings->apply();
    }

    std::unique_ptr<Settings::GameSettings> gameSettings = ScriptableObject::load<Settings::GameSettings>(Path(Resources::getResourcesDirectory(), "Data", "Settings", "GameSettings.asset"));
    if (gameSettings != nullptr)
    {
      gameSettings->apply();
    }
  }

  //------------------------------------------------------------------------------------------------
  void Game::executeGameScript() const
  {
    Path gameLuaScriptPath(Resources::getResourcesDirectory(), "Scripts", "Game.lua");
    if (File::exists(gameLuaScriptPath))
    {
      Lua::LuaState::scriptFile(gameLuaScriptPath);
    }
  }

  //------------------------------------------------------------------------------------------------
  void Game::onInitialize()
  {
  }

  //------------------------------------------------------------------------------------------------
  void Game::run()
  {
    // Make sure this variable is set before initialize - that way we can exit the game during the
    // Game.lua script if we want to perform some CLI task
    m_running = true;

    // Call awake on the clock to remove any wait time since start up
    initialize();
    m_clock.awake();

    // Game loop:
    // Measure the time since last frame
    // Get the elapsed in game time by using our fixed frame rate & time scale
    // If the time since last frame is greater than our elapsed game time we advance the game forward as many times
    // as necessary before the real time is in the range 0 <= real time <= elapsedGameTime
    // Then we draw the scene
    // The lag now is the amount leftover that the game is actually ahead of the simulation (i.e. realtime - elapsedgametime)
    // We can use that lag to compensate in the rendering by pretending that much extra simulation has occurred
    GLfloat elapsedRealTime = 0;

    while (m_running)
    {
      glfwPollEvents();

      m_clock.update();

      elapsedRealTime = m_clock.getElapsedDeltaTime();

      // If we have spent more than 1 second paused for some reason
      // we should not update the world - it will cause confusion in physics etc.
      if (elapsedRealTime < 1)
      {
        GLfloat targetGameTime = m_clock.getTargetSecondsPerFrame() * m_clock.getTimeScale();

        handleInput();

        //while (elapsedRealTime > targetGameTime)
        {
          // Manage user input
          update(elapsedRealTime);

          //elapsedRealTime -= targetGameTime;
        }

        //update(elapsedRealTime);
        //elapsedRealTime = 0;
      }

      glClear(GL_COLOR_BUFFER_BIT);

      // Render
      render(0);

      glfwSwapBuffers(m_screenManager.getWindow()->getGLWindow());
    }

    onExit();

    // We have exited the game loop, so we cleanup
    m_inputManager.die();
    m_physicsManager.die();
    m_audioManager.die();
    m_renderManager.die();
    m_screenManager.die();  // Kill screen manager after all other cleanup has taken place
    m_resourceManager.die();

    GL::terminate();

    // Finally, make sure we flush the log in case we have buffered output we have not committed to file yet
    Log::getLogger()->flush();
  }

  //------------------------------------------------------------------------------------------------
  void Game::handleInput()
  {
    if (m_inputManager.isActive())
    {
      m_inputManager.handleInput();
    }

    if (m_screenManager.isActive())
    {
      m_screenManager.handleInput();
    }

    if (m_physicsManager.isActive())
    {
      m_physicsManager.handleInput();
    }

    if (m_audioManager.isActive())
    {
      m_audioManager.handleInput();
    }

    if (m_renderManager.isActive())
    {
      m_renderManager.handleInput();
    }

    if (m_inputManager.getKeyboard().isKeyTapped(GLFW_KEY_ESCAPE))
    {
      glfwSetWindowShouldClose(m_screenManager.getWindow()->getGLWindow(), GL_TRUE);
    }
  }

  //------------------------------------------------------------------------------------------------
  void Game::update(GLfloat elapsedGameTime)
  {
    if (m_inputManager.isActive())
    {
      m_inputManager.update(elapsedGameTime);
    }

    if (m_screenManager.isActive())
    {
      m_screenManager.update(elapsedGameTime);
    }

    if (m_physicsManager.isActive())
    {
      m_physicsManager.update(elapsedGameTime);
    }

    if (m_audioManager.isActive())
    {
      m_audioManager.update(elapsedGameTime);
    }

    if (m_renderManager.isActive())
    {
      m_renderManager.update(elapsedGameTime);
    }

    onUpdate(elapsedGameTime);
  }

  //------------------------------------------------------------------------------------------------
  void Game::render(GLfloat lag)
  {
    m_renderManager.render(lag);
  }

  //------------------------------------------------------------------------------------------------
  void Game::exit()
  {
    if (m_current != nullptr)
    {
      m_current->m_running = false;

      if (getWindow() && getWindow()->getGLWindow())
      {
        glfwSetWindowShouldClose(getWindow()->getGLWindow(), GL_TRUE);
      }
    }
  }

  //------------------------------------------------------------------------------------------------
  void Game::windowCloseFunc(GLFWwindow* window)
  {
    Game::exit();
  }
}