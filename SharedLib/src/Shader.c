#include "pch.h"
#include "Shader.h"
#include "Graphics.h"
#include "Mat.h"

#pragma region INTERNAL
static bool ReadFileFromPath(const char* path, char** buffer)
{
    //char fullPath[1024];
    //_getcwd(fullPath, sizeof(fullPath));
    //LOG_TRACE("Current working directory: %s", fullPath);
    FILE* file;
    fopen_s(&file, path, "rb");

    if (!file)
    {
        LOG_ERROR("Failed to open file: %s", path);
        return false;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buf = ALLOC_ARRAY(char, size + 1);
    ASSERT(buf);

    fread(buf, size, 1, file);
    buf[size] = '\0';
    fclose(file);

	*buffer = buf;

    return true;
}
static bool TestCompileShader(GLuint shader, const char* type)
{
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        LOG_ERROR("Failed to compile %s shader: %s", type, buffer);
        return false;
    }
}
static bool TestLinkProgram(GLuint program)
{
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char buffer[512];
        glGetProgramInfoLog(program, 512, NULL, buffer);
        LOG_ERROR("Failed to link program: %s", buffer);
        return false;
    }
}
#pragma endregion

GLuint DROP_LoadShaders(const char* vsPath, const char* fsPath)
{
    const char* vsSource = NULL;
    if (!ReadFileFromPath(vsPath, &vsSource) || !vsSource)
    {
        LOG_ERROR("Failed to read vertex shader file: %s", vsPath);
		FREE(vsSource);
        return 0;
    }

    const char* fsSource = NULL;
    if (!ReadFileFromPath(fsPath, &fsSource) || !fsSource)
    {
        LOG_ERROR("Failed to read fragment shader file: %s", fsPath);
		FREE(vsSource);
		FREE(fsSource);
        return 0;
    }

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsSource, NULL);
    glShaderSource(fs, 1, &fsSource, NULL);

	FREE(vsSource);
	FREE(fsSource);

    glCompileShader(vs);
    glCompileShader(fs);

    if (!TestCompileShader(vs, "vertex") || !TestCompileShader(fs, "fragment"))
    {
        glDeleteShader(vs);
        glDeleteShader(fs);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    if (!TestLinkProgram(program))
    {
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void DROP_DestroyShader(GLuint shader)
{
	glUseProgram(0);
    glDeleteProgram(shader);
}