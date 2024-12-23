#include "pipeline.hpp"

namespace Visualizer {

Pipeline::Pipeline() { mId = glCreateProgram(); }

Pipeline::~Pipeline() { glDeleteProgram(mId); }

void Pipeline::Build() {
  for (std::shared_ptr<Shader> shader : mShaders) {
    glAttachShader(mId, shader->Id());
  }

  glLinkProgram(mId);
  glValidateProgram(mId);
  glUseProgram(mId);
}

void Pipeline::Bind() { glUseProgram(mId); }

void Pipeline::Unbind() { glUseProgram(0); }

void Pipeline::AddShader(uint type, const char *filename) {
  mShaders.emplace_back(std::make_shared<Shader>(type, filename));
}

void Pipeline::SetFloat(const char *name, const float &value) { glUniform1f(glGetUniformLocation(mId, name), value); }

void Pipeline::SetInt(const char *name, const int &value) { glUniform1i(glGetUniformLocation(mId, name), value); }

void Pipeline::SetMatrix4f(const char *name, const Matrix4f &values) {
  glUniformMatrix4fv(glGetUniformLocation(mId, name), 1, GL_FALSE, values.data());
}

void Pipeline::SetVec2(const char *name, const Vector2f &values) {
  glUniform2fv(glGetUniformLocation(mId, name), 1, values.data());
}

} // namespace Visualizer
