#pragma once

// glad MUST be included before any GLFW header.
// Consumers who include both must respect this order.
#include <glad/glad.h>

namespace engine {

/// Wraps a single OpenGL buffer object using the DSA (Direct State Access) API.
///
/// All operations go through glCreateBuffers / glNamedBufferData — no legacy
/// glGenBuffers / glBindBuffer calls are made inside this class.
///
/// An OpenGL 4.5 context must be current before any method is called.
class GpuBuffer {
public:
    explicit GpuBuffer() = default;
    ~GpuBuffer();

    // Non-copyable — owns a GL buffer name.
    GpuBuffer(const GpuBuffer&)            = delete;
    GpuBuffer& operator=(const GpuBuffer&) = delete;

    // Movable.
    GpuBuffer(GpuBuffer&& other) noexcept;
    GpuBuffer& operator=(GpuBuffer&& other) noexcept;

    /// Allocate GPU memory and upload data via glCreateBuffers + glNamedBufferData.
    /// Calling upload() again on the same object recreates the buffer storage.
    /// @param data      Pointer to the data to upload (may be nullptr to allocate only).
    /// @param sizeBytes Size in bytes of the data.
    /// @param usage     GL usage hint (e.g. GL_STATIC_DRAW, GL_DYNAMIC_DRAW).
    void upload(const void* data, GLsizeiptr sizeBytes,
                GLenum usage = GL_STATIC_DRAW);

    /// The raw OpenGL buffer object name (0 if not yet uploaded).
    [[nodiscard]] GLuint id() const { return m_id; }

    /// True if a buffer has been successfully created (upload() succeeded).
    [[nodiscard]] bool isValid() const { return m_id != 0; }

private:
    GLuint m_id = 0;
};

// ─────────────────────────────────────────────────────────────────────────────

/// Wraps an OpenGL Vertex Array Object (VAO) using DSA.
///
/// All operations go through glCreateVertexArrays and related DSA entry points.
/// No legacy glGenVertexArrays / glBindVertexArray-then-set calls are made.
class VertexArray {
public:
    explicit VertexArray();
    ~VertexArray();

    // Non-copyable — owns a GL VAO name.
    VertexArray(const VertexArray&)            = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    // Movable.
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    /// Define a vertex attribute format (DSA: glVertexArrayAttribFormat + binding).
    /// @param bindingIndex  The vertex buffer binding slot.
    /// @param attribIndex   The attribute index (layout location in GLSL).
    /// @param components    Number of components (1–4).
    /// @param type          Component type (e.g. GL_FLOAT).
    /// @param offset        Byte offset of this attribute within the vertex.
    void addAttrib(GLuint bindingIndex, GLuint attribIndex,
                   GLint components, GLenum type, GLuint offset);

    /// Bind a GpuBuffer to a vertex buffer binding slot (DSA: glVertexArrayVertexBuffer).
    /// @param bindingIndex  The vertex buffer binding slot.
    /// @param buf           The buffer to bind.
    /// @param offset        Byte offset into the buffer.
    /// @param stride        Stride in bytes between consecutive vertices.
    void bindVertexBuffer(GLuint bindingIndex, const GpuBuffer& buf,
                          GLintptr offset, GLsizei stride);

    /// Activate this VAO for subsequent draw calls (glBindVertexArray).
    void bind() const;

    /// The raw OpenGL VAO name (0 if construction failed).
    [[nodiscard]] GLuint id() const { return m_id; }

    /// True if the VAO was successfully created.
    [[nodiscard]] bool isValid() const { return m_id != 0; }

private:
    GLuint m_id = 0;
};

} // namespace engine
