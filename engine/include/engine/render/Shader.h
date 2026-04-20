#pragma once

#include <string>

// Include glad for GL types. glad must be included before any GLFW header.
// Consumers who also use GLFW must include glad/glad.h before GLFW/glfw3.h.
#include <glad/glad.h>

namespace engine {

/// Compiles and links a GLSL vertex + fragment shader program.
///
/// Usage:
///   Shader s;
///   if (!s.loadFromFiles("shader.vert", "shader.frag")) { /* handle error */ }
///   s.bind();
///   s.setUniform("uTime", 1.0f);
///   s.unbind();
///
/// Compile/link errors are written to std::cerr with a "[Shader]" prefix.
/// An OpenGL 4.5 context must be current before calling loadFromFiles().
class Shader {
public:
    explicit Shader() = default;
    ~Shader();

    // Non-copyable — owns a GL program handle.
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    // Movable.
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    /// Read vertex and fragment shader source from disk, compile, and link.
    /// Returns true on success; false on any I/O or GL error.
    /// On failure, isValid() remains false and errors are logged to std::cerr.
    bool loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    /// True if the program was linked successfully and is ready to use.
    [[nodiscard]] bool isValid() const { return m_program != 0; }

    /// Activate this program for subsequent draw calls.
    void bind() const;

    /// Deactivate — bind the null program.
    void unbind() const;

    /// The raw OpenGL program object name (0 if invalid).
    [[nodiscard]] GLuint programId() const { return m_program; }

    // ── Uniform setters ───────────────────────────────────────────────────────

    void setUniform(const std::string& name, float v);
    void setUniform(const std::string& name, int v);

    /// Upload a column-major 4×4 matrix (pointer to 16 floats).
    void setUniform(const std::string& name, const float* mat4);

private:
    /// Compile a single shader stage. Returns 0 on failure.
    static GLuint compileStage(GLenum type, const std::string& src);

    GLuint m_program = 0;
};

} // namespace engine
