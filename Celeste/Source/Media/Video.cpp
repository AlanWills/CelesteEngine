#include "Media/Video.h"
#include "System/Process.h"

#include <thread>
#include <future>


namespace Celeste::Media
{
  Path Video::m_ffplayExecutablePath = Path("Celeste", "3rdParty", "Resources", "ffplay", "ffplay.exe");

  //------------------------------------------------------------------------------------------------
  void Video::play(const std::string& videoPath, const VideoSettings& videoSettings)
  {
    std::string commandLine(m_ffplayExecutablePath.as_string());
    commandLine.push_back(' ');
    commandLine.append(videoPath);

    if (videoSettings.m_autoExit)
    {
      commandLine.append(" -autoexit");
    }

    commandLine.append(" -volume ");
    commandLine.append(std::to_string(videoSettings.m_volume * 100.0f));
    commandLine.push_back('\0');

    System::Process::start(commandLine, videoSettings.m_videoCompleteCallback);
  }

  //------------------------------------------------------------------------------------------------
  void Video::playAsync(const std::string& videoPath, const VideoSettings& videoSettings)
  {
    std::thread t(&Video::play, videoPath, videoSettings);
    t.detach();
  }
}