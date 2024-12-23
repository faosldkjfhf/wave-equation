#pragma once

#include "shader.hpp"
#include "types.hpp"

using namespace Eigen;

namespace Visualizer {

class Pipeline {
public:
  Pipeline();
  ~Pipeline();

  void Build();
  void Bind();
  void Unbind();
  void AddShader(uint type, const char *filename);

  void SetFloat(const char *name, const float &value);
  void SetInt(const char *name, const int &value);
  void SetMatrix4f(const char *name, const Matrix4f &values);
  void SetVec2(const char *name, const Vector2f &values);

private:
  std::vector<std::shared_ptr<Shader>> mShaders;
  unsigned int mId = 0;
};

} // namespace Visualizer
