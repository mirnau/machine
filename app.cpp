#pragma once
#include <iostream>
#include "app.h"

App::App() {
  std::cout << "Ran Constructor" << std::endl;
}

void App::Run() {
  std::cout << "Running App" << std::endl;  
}

App::~App() {
  std::cout << "Ran Destructor for App" << std::endl;
}
