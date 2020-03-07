#pragma once

#include "Camera.h"
#include "Events/Event.h"


namespace Celeste
{

class OpenGLWindow
{
  public:
    enum class WindowMode
    {
      kWindowed,
      kFullScreen
    };

    CelesteDllExport OpenGLWindow(WindowMode windowMode = WindowMode::kWindowed, const std::string& windowTitle = "");
    CelesteDllExport OpenGLWindow(
      int windowWidth, 
      int windowHeight,
      WindowMode windowMode = WindowMode::kWindowed,
      const std::string& windowTitle = "");
    CelesteDllExport ~OpenGLWindow();

    CelesteDllExport void handleInput();

    CelesteDllExport glm::vec2 getWindowDimensions() const;
    CelesteDllExport void setWindowDimensions(glm::vec2 windowDimensions);
    void setWindowDimensions(int x, int y) { setWindowDimensions(glm::vec2(x, y)); }

    const glm::vec2& getViewportDimensions() const { return m_viewportDimensions; }
    CelesteDllExport void setViewportDimensions(const glm::vec2& viewportDimensions);
    void setViewportDimensions(int x, int y) { setViewportDimensions(glm::vec2(x, y)); }

    GLFWwindow* getGLWindow() const { return m_window; }

    void enableViewportFlag(GLenum flag) { glEnable(flag); }

    WindowMode getWindowMode() const { return glfwGetWindowMonitor(m_window) ? WindowMode::kFullScreen : WindowMode::kWindowed; }
    CelesteDllExport void setWindowMode(WindowMode windowMode);

    const std::string& getTitle() const { return m_title; }
    CelesteDllExport void setTitle(const std::string& windowTitle);

    CelesteDllExport void setIcon(const std::string& windowTitle);

    const Event<const glm::vec2&>& getViewportDimensionsChangedEvent() const { return m_viewportDimensionsChanged; }

  private:
    void initWindow(WindowMode windowMode);
    void setDimensionsInternal(const glm::vec2& viewportDimensions);

    std::string m_title;

    GLFWwindow* m_window;

    /// Store the values of the window frame size
    int m_left, m_top, m_right, m_bottom;

    glm::vec2 m_viewportDimensions;
    Event<const glm::vec2&> m_viewportDimensionsChanged;
};

};