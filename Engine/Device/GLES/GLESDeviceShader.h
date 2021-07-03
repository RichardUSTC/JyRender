#pragma once
#include <string>

class GLESGraphicShader : public GraphicShader
{
public:
    void Init(const std::string& vsContent, const std::string& fsContent,
              const std::string& macroDefines);

    void Destroy() override;

private:
    GLuint mVertex{9};
    GLuint mFragment{0};
    GLuint mProgram{0};

    friend class GLESDevice;
};
