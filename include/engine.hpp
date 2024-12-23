#pragma once

#include "pipeline.hpp"
#include "types.hpp"

using namespace Eigen;

namespace Visualizer {

class Engine {
public:
  Engine();
  ~Engine();

  Engine(const Engine &) = delete;
  const Engine &operator=(const Engine &) = delete;

  void Run();

private:
  void Init();
  void Input();
  void Update();
  void Render();

  GLFWwindow *mWindow;
  unsigned int mWidth;
  unsigned int mHeight;
  unsigned int mVao = 0;
  unsigned int mVbo = 0;
  unsigned int mIbo = 0;
  unsigned int mTexture = 0;
  unsigned int mDerivativeTexture = 0;
  std::vector<float> mVertices;
  std::vector<unsigned int> mIndices;
  std::shared_ptr<Pipeline> mPipeline;
  std::shared_ptr<Pipeline> mComputePipeline;
  std::shared_ptr<Pipeline> mDerivativeMapPipeline;
  bool mMouseClicked = false;
  Vector2f mMousePosition = Vector2f::Zero();
};

} // namespace Visualizer
