#pragma once

#include "shader.hpp"
#include "types.hpp"

using namespace Eigen;

namespace Visualizer {

/**
 * Represents an OpenGL Shader Program
 */
class Pipeline {
public:
  /**
   * Default constructor
   */
  Pipeline();

  /**
   * Default destructor
   */
  ~Pipeline();

  /**
   * Compiles the program and links it with the shaders added
   */
  void Build();

  /**
   * Binds the program for use
   */
  void Bind();

  /**
   * Unbinds the program
   */
  void Unbind();

  /**
   * Adds a shader to be built with. Meant for use before building
   *
   * @param type The type of the shader
   * @param filename The file location of the shader
   */
  void AddShader(uint type, const char *filename);

  /**
   * Binds a uniform float
   *
   * @param name The name to use
   * @param value The value to set
   */
  void SetFloat(const char *name, const float &value);

  /**
   * Binds a uniform int
   *
   * @param name The name to use
   * @param value The value to set
   */
  void SetInt(const char *name, const int &value);

  /**
   * Binds a uniform mat4
   *
   * @param name The name to use
   * @param values The values to set
   */
  void SetMatrix4f(const char *name, const Matrix4f &values);

  /**
   * Binds a uniform vec2
   *
   * @param name The name to use
   * @param values The values to set
   */
  void SetVec2(const char *name, const Vector2f &values);

private:
  std::vector<std::shared_ptr<Shader>> mShaders; /*!< shaders to be built with */
  unsigned int mId = 0;                          /*!< the program id */
};

} // namespace Visualizer
