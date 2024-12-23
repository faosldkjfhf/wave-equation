#include "shader.hpp"
#include <fstream>

namespace Visualizer {

Shader::Shader(uint type, const char *filename) : mType(type) {
  mId = glCreateShader(type);
  std::string shaderSource = LoadShader(filename);
  const char *src = shaderSource.c_str();

  glShaderSource(mId, 1, &src, NULL);
  glCompileShader(mId);
}

Shader::~Shader() { glDeleteShader(mId); }

std::string Shader::LoadShader(const char *filename) {
  std::string result = "";
  std::ifstream inFile(filename);

  if (!inFile.is_open()) {
    throw std::runtime_error("failed to open " + std::string(filename));
  }

  std::string line;
  while (std::getline(inFile, line)) {
    result += line + "\n";
  }

  inFile.close();
  return result;
}

} // namespace Visualizer
