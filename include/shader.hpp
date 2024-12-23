#pragma once

#include "types.hpp"

namespace Visualizer {

/**
 * Loads a shader of any type from the given filename
 */
class Shader {
public:
  /**
   * Create a new Shader
   *
   * @param type The type of shader
   * @param filename The filename containing the shader code
   *
   */
  Shader(uint type, const char *filename);

  /**
   * Default destructor, deletes the shader
   */
  ~Shader();

  /**
   * Getter for the shader id
   *
   * @return the id of the shader
   */
  uint Id() { return mId; }

private:
  /**
   * Loads the shader from the given file
   *
   * @param filename The file to load from
   * @return The contents of the file as a string
   */
  std::string LoadShader(const char *filename);

  uint mType = 0; /*!< type of shader*/
  uint mId = 0;   /*!< shader id */
};

} // namespace Visualizer
