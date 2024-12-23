#include "engine.hpp"
#include "debug.hpp"

using namespace Eigen;
using Color = Vector4f;

namespace Visualizer {

Engine::Engine() : mWidth(600), mHeight(600) {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initialize glfw");
  }

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  mWindow = glfwCreateWindow(mWidth, mHeight, "Wave Equation Visualizer", NULL, NULL);
  if (!mWindow) {
    throw std::runtime_error("failed to create window");
  }

  glfwMakeContextCurrent(mWindow);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSetWindowUserPointer(mWindow, this);
  glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *window, int button, int action, int mods) {
    Engine *e = (Engine *)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      e->mMouseClicked = true;
      double xPos, yPos;
      glfwGetCursorPos(window, &xPos, &yPos);
      e->mMousePosition(0) = static_cast<float>(xPos);
      e->mMousePosition(1) = static_cast<float>(e->mHeight - yPos);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
      e->mMouseClicked = false;
    }
  });
  glfwSetKeyCallback(mWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  });

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GlDebugOutput, nullptr);
    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
  }

  mPipeline = std::make_shared<Pipeline>();
  mComputePipeline = std::make_shared<Pipeline>();
  mDerivativeMapPipeline = std::make_shared<Pipeline>();

  Init();
}

Engine::~Engine() {
  mPipeline.reset();
  mComputePipeline.reset();
  mDerivativeMapPipeline.reset();
  glDeleteTextures(1, &mTexture);
  glDeleteTextures(1, &mDerivativeTexture);
  glDeleteVertexArrays(1, &mVao);
  glDeleteBuffers(1, &mVbo);
  glDeleteBuffers(1, &mIbo);

  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

void Engine::Run() {
  while (!glfwWindowShouldClose(mWindow)) {
    Input();
    Update();
    Render();
  }
}

void Engine::Init() {
  // screen quad vertices and indices
  mVertices = {
      1.f,  -1.f, 0.f, 1.f, 0.f, // 0
      1.f,  1.f,  0.f, 1.f, 1.f, // 1
      -1.f, -1.f, 0.f, 0.f, 0.f, // 2
      -1.f, 1.f,  0.f, 0.f, 1.f, // 3
  };

  mIndices = {0, 1, 2, 2, 1, 3};

  // vertex array object
  glGenVertexArrays(1, &mVao);
  glBindVertexArray(mVao);

  // vertex buffer object
  glGenBuffers(1, &mVbo);
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

  // index buffer object
  glGenBuffers(1, &mIbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);

  // positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);

  // texture coords
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

  // build render pass
  mPipeline->AddShader(GL_VERTEX_SHADER, "../shaders/base/vert.glsl");
  mPipeline->AddShader(GL_FRAGMENT_SHADER, "../shaders/base/frag.glsl");
  mPipeline->Build();

  // build compute pass
  mComputePipeline->AddShader(GL_COMPUTE_SHADER, "../shaders/compute/wave.glsl");
  mComputePipeline->Build();

  mDerivativeMapPipeline->AddShader(GL_COMPUTE_SHADER, "../shaders/compute/derivative.glsl");
  mDerivativeMapPipeline->Build();

  // create the texture to write to
  float black[mWidth * mHeight * 4];
  std::fill_n(black, mWidth * mHeight * 4, 0.0f);

  glGenTextures(1, &mTexture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mWidth, mHeight, 0, GL_RGBA, GL_FLOAT, black);
  glBindImageTexture(0, mTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

  glGenTextures(1, &mDerivativeTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, mDerivativeTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mWidth, mHeight, 0, GL_RGBA, GL_FLOAT, black);
  glBindImageTexture(1, mDerivativeTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void Engine::Input() { glfwPollEvents(); }

void Engine::Update() {}

void Engine::Render() {
  // render pass
  glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mPipeline->Bind();
  mPipeline->SetInt("uTexture", 0);
  mPipeline->SetInt("uDerivativeTexture", 1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, mDerivativeTexture);
  glBindVertexArray(mVao);
  glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);

  // compute shader pass
  mComputePipeline->Bind();
  mComputePipeline->SetInt("uMouseClicked", mMouseClicked);
  mComputePipeline->SetVec2("uMousePosition", mMousePosition);
  glDispatchCompute(static_cast<unsigned int>(mWidth / 8), static_cast<unsigned int>(mHeight / 8), 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  // update derivative map
  mDerivativeMapPipeline->Bind();
  glDispatchCompute(static_cast<unsigned int>(mWidth / 8), static_cast<unsigned int>(mHeight / 8), 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  // swap buffers
  glfwSwapBuffers(mWindow);
}

} // namespace Visualizer
