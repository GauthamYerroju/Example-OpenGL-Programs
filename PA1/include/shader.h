#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType, char **argv);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
    std::string readShaderFile(const char *filePath);

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
