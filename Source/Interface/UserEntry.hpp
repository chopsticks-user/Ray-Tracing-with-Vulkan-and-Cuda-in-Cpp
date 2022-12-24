#ifndef NEKO_INTERFACE_USER_ENTRY_HPP
#define NEKO_INTERFACE_USER_ENTRY_HPP

#include <iostream>

namespace Neko {
  class Engine;
  extern Engine* _nekoUserInitializeEngine();
  extern void _userEntry();
  extern void createImGUI();
} /* namespace Neko */

static int protectedMain(int, char**) {
  Neko::_userEntry();

  auto engine = Neko::_nekoUserInitializeEngine();
  engine->start();
  delete engine;

  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
  try {
    return protectedMain(argc, argv);
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
  }
  return EXIT_FAILURE;
}

#endif /* NEKO_INTERFACE_USER_ENTRY_HPP */