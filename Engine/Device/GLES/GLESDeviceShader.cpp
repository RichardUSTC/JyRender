#include "GLES/GLESDeviceShader.h"

static bool CompileShader(GLuint shader, std::vector<const char*> sources)
{
    if (sources.empty())
        return false;

    glShaderSource(shader, static_cast<GLsizei>(sources.size()), sources.data(), nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            auto infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            LOG_ERROR("Device", "Failed to compile shader: error {0}\n",
                      infoLog);

            for (auto source : sources)
            {
                LOG_ERROR("Device", "{0}", source);
            }

            delete[]infoLog;
        }
        glDeleteShader(shader);
        return false;
    }
    return true;
}

void GLESGraphicShader::Init(const std::string& vsContent, const std::string& fsContent,
                             const std::string& macroDefines)
{
    if (vsContent.empty())
    {
        LOG_ERROR("Device", "vsContent is emtpy");
        return;
    }

    if (fsContent.empty())
    {
        LOG_ERROR("Device", "fsContent is emtpy");
    }

    mVertex = glCreateShader(GL_VERTEX_SHADER);
    if (!mVertex)
    {
        CHECK_GL_ERROR;
        return;
    }
    mFragment = glCreateShader(GL_FRAGMENT_SHADER);
    if (!mFragment)
    {
        CHECK_GL_ERROR;
        return;
    }

    if (!CompileShader(mVertex, {
                           "#version 300 es\n",
                           ATTRIBUTE_BINDING_DEFINES,
                           macroDefines.c_str(),
                           vsContent.c_str()
                       }))
    {
        mVertex = 0;
        return;
    }

    if (!CompileShader(mFragment, {
                           "#version 300 es\n",
                           macroDefines.c_str(),
                           fsContent.c_str()
                       }))
    {
        mFragment = 0;
        return;
    }

    mProgram = glCreateProgram();
    if (!mProgram)
    {
        CHECK_GL_ERROR;
        return;
    }

    glAttachShader(mProgram, mVertex);
    glAttachShader(mProgram, mFragment);
    glLinkProgram(mProgram);

    GLint linked;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            auto infoLog = new char[infoLen];
            glGetProgramInfoLog(mProgram, infoLen, nullptr, infoLog);
            LOG_ERROR("Device", "Failed to link program: error {0}\nvertex:\n{1}\n{2}\nfragment:\n{1}\n{3}",
                      infoLog, macroDefines, vsContent, fsContent);
            delete[] infoLog;
        }

        glDeleteProgram(mProgram);
        mProgram = 0;
        glDeleteShader(mVertex);
        mVertex = 0;
        glDeleteShader(mFragment);
        mFragment = 0;
    }
}

void GLESGraphicShader::Destroy()
{
    if (mProgram)
    {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }

    if (mVertex)
    {
        glDeleteShader(mVertex);
        mVertex = 0;
    }

    if (mFragment)
    {
        glDeleteShader(mFragment);
        mFragment = 0;
    }
}
