#include "engine.hpp"

int main() {
  try {
    Visualizer::Engine engine;
    engine.Run();

    return 0;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;

    return 1;
  }
}
