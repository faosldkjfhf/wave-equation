#pragma once

#include "types.hpp"

namespace Visualizer {

class Shader {
public:
  Shader(uint type, const char *filename);
  ~Shader();

  void Bind();
  void Unbind();

  uint Id() { return mId; }

private:
  std::string LoadShader(const char *filename);

  uint mType = 0;
  uint mId = 0;
};

} // namespace Visualizer
