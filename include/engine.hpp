#pragma once

#include "pipeline.hpp"
#include "types.hpp"

using namespace Eigen;

namespace Visualizer {

/**
 * The Engine for the Visualizer
 */
class Engine {
public:
  /**
   * Default constructor. Initializes everything and sets up GLAD, glfw
   */
  Engine();

  /**
   * Default destructor. Destroys used textures, buffers, and shaders
   */
  ~Engine();

  /**
   * Delete copy constructor
   */
  Engine(const Engine &) = delete;

  /**
   * Delete copy constructor
   */
  const Engine &operator=(const Engine &) = delete;

  /**
   * Runs the Visualizer
   */
  void Run();

private:
  /**
   * Initializes everything needed
   */
  void Init();

  /**
   * Handles input
   */
  void Input();

  /**
   * Handles updates
   */
  void Update();

  /**
   * Handles rendering
   */
  void Render();

  GLFWwindow *mWindow;                              /*!< window */
  unsigned int mWidth;                              /*!< width of window */
  unsigned int mHeight;                             /*!< height of window */
  unsigned int mVao = 0;                            /*!< vertex array object */
  unsigned int mVbo = 0;                            /*!< vertex buffer object */
  unsigned int mIbo = 0;                            /*!< index buffer object */
  unsigned int mTexture = 0;                        /*!< height map */
  unsigned int mDerivativeTexture = 0;              /*!< derivative map */
  std::vector<float> mVertices;                     /*!< vertices for the screen quad */
  std::vector<unsigned int> mIndices;               /*!< indices for the screen quad */
  std::shared_ptr<Pipeline> mPipeline;              /*!< render pass pipeline */
  std::shared_ptr<Pipeline> mComputePipeline;       /*!< compute pass pipeline */
  std::shared_ptr<Pipeline> mDerivativeMapPipeline; /*!< derivative compute pass pipeline */
  bool mMouseClicked = false;                       /*!< has the mouse been clicked or not */
  Vector2f mMousePosition = Vector2f::Zero();       /*!< the position where the mouse was clicked */
};

} // namespace Visualizer
