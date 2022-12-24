#ifndef NEKO_INTERFACE_USER_ENTRY_HPP
#define NEKO_INTERFACE_USER_ENTRY_HPP

#include <iostream>

namespace neko
{
  extern Engine *_nekoUserInitializeEngine();
  extern void _userEntry();
}

static int protected_main([[maybe_unused]] int argc,
                          [[maybe_unused]] char **argv)
{
  neko::_userEntry();
  auto engine = neko::_nekoUserInitializeEngine();
  engine->start();
  delete engine;
  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  try
  {
    return protected_main(argc, argv);
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Uncaught exception" << std::endl;
  }
  return EXIT_FAILURE;
}

#endif /* NEKO_INTERFACE_USER_ENTRY_HPP */