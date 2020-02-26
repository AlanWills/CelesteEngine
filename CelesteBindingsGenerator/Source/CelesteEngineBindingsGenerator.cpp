// CelesteBindingsGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Project.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace Celeste;
using namespace BindingsGenerator;


int main()
{
  Path projectPath(Directory::getExecutingAppDirectory(), UPDIR_STRING, "CelesteBindings");
  Project project(projectPath, "CelesteBindings", "Celeste");
  project.generateBindings();

  std::this_thread::sleep_for(std::chrono::seconds(2));

  return 0;
}