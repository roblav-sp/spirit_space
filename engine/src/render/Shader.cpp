// IMPORTANT: glad/glad.h MUST be included before GLFW/glfw3.h.
// glad defines platform macros (APIENTRY, etc.) that glfw3.h uses
// conditionally. Reversing the order causes macro redefinition errors.
#include <glad/glad.h>

#include "engine/render/Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

namespace engine {

// ── Helpers ───────────────────────────────────────────────────────────────────

namespace {

/// Read an entire text file into a string. Returns empty string on error.
std::string readFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Shader] Cannot open file: " << path << '\n';
        return {};
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

/// Retrieve the info log from a shader or program object.
std::string getInfoLog(GLuint obj, bool isProgram)
{
    GLint len = 0;
    if (isProgram) {
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &len);
    } else {
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &len);
    }
    if (len <= 0) return {};
    std::string log(static_cast<std::size_t>(len), '\0');
    if (isProgram) {
        glGetProgramInfoLog(obj, len, nullptr, log.data());
    } else {
        glGetShaderInfoLog(obj, len, nullptr, log.data());
    }
    return log;
}

} // namespace

// ── compileStage ──────────────────────────────────────────────────────────────

// static
GLuint Shader::compileStage(GLenum type, const std::string& src)
{
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        std::cerr << "[Shader] glCreateShader failed for type "
                  << type << '\n';
        return 0;
    }

    const char* srcPtr = src.c_str();
    glShaderSource(shader, 1, &srcPtr, nullptr);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE) {
        std::string log = getInfoLog(shader, false);
        const char* stageStr = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        std::cerr << "[Shader] " << stageStr << " compile error:\n"
                  << log << '\n';
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// ── loadFromFiles ─────────────────────────────────────────────────────────────

bool Shader::loadFromFiles(const std::string& vertPath,
                           const std::string& fragPath)
{
    // Clean up any previously linked program.
    if (m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
    }

    const std::string vertSrc = readFile(vertPath);
    if (vertSrc.empty()) {
        return false;
    }

    const std::string fragSrc = readFile(fragPath);
    if (fragSrc.empty()) {
        return false;
    }

    const GLuint vert = compileStage(GL_VERTEX_SHADER, vertSrc);
    if (vert == 0) return false;

    const GLuint frag = compileStage(GL_FRAGMENT_SHADER, fragSrc);
    if (frag == 0) {
        glDeleteShader(vert);
        return false;
    }

    const GLuint prog = glCreateProgram();
    if (prog == 0) {
        std::cerr << "[Shader] glCreateProgram failed\n";
        glDeleteShader(vert);
        glDeleteShader(frag);
        return false;
    }

    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    // Shaders are compiled into the program; detach and free immediately.
    glDetachShader(prog, vert);
    glDetachShader(prog, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    GLint ok = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (ok == GL_FALSE) {
        std::string log = getInfoLog(prog, true);
        std::cerr << "[Shader] link error:\n" << log << '\n';
        glDeleteProgram(prog);
        return false;
    }

    m_program = prog;
    return true;
}

// ── Destructor / move ─────────────────────────────────────────────────────────

Shader::~Shader()
{
    if (m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

Shader::Shader(Shader&& other) noexcept
    : m_program(other.m_program)
{
    other.m_program = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) {
        if (m_program != 0) {
            glDeleteProgram(m_program);
        }
        m_program       = other.m_program;
        other.m_program = 0;
    }
    return *this;
}

// ── bind / unbind ─────────────────────────────────────────────────────────────

void Shader::bind() const
{
    glUseProgram(m_program);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

// ── Uniform setters ───────────────────────────────────────────────────────────

void Shader::setUniform(const std::string& name, float v)
{
    const GLint loc = glGetUniformLocation(m_program, name.c_str());
    if (loc == -1) {
        std::cerr << "[Shader] Uniform not found: " << name << '\n';
        return;
    }
    glProgramUniform1f(m_program, loc, v);
}

void Shader::setUniform(const std::string& name, int v)
{
    const GLint loc = glGetUniformLocation(m_program, name.c_str());
    if (loc == -1) {
        std::cerr << "[Shader] Uniform not found: " << name << '\n';
        return;
    }
    glProgramUniform1i(m_program, loc, v);
}

void Shader::setUniform(const std::string& name, const float* mat4)
{
    const GLint loc = glGetUniformLocation(m_program, name.c_str());
    if (loc == -1) {
        std::cerr << "[Shader] Uniform not found: " << name << '\n';
        return;
    }
    glProgramUniformMatrix4fv(m_program, loc, 1, GL_FALSE, mat4);
}

} // namespace engine
