// IMPORTANT: glad/glad.h MUST be included before GLFW/glfw3.h.
// glad defines platform macros (APIENTRY, etc.) that glfw3.h uses
// conditionally. Reversing the order causes macro redefinition errors.
#include <glad/glad.h>

#include "engine/render/GpuBuffer.h"

#include <utility>

namespace engine {

// ── GpuBuffer ─────────────────────────────────────────────────────────────────

GpuBuffer::~GpuBuffer()
{
    if (m_id != 0) {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }
}

GpuBuffer::GpuBuffer(GpuBuffer&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

GpuBuffer& GpuBuffer::operator=(GpuBuffer&& other) noexcept
{
    if (this != &other) {
        if (m_id != 0) {
            glDeleteBuffers(1, &m_id);
        }
        m_id       = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void GpuBuffer::upload(const void* data, GLsizeiptr sizeBytes, GLenum usage)
{
    // Re-upload: delete the old buffer first.
    if (m_id != 0) {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }

    // DSA: create the buffer object (no bind required).
    glCreateBuffers(1, &m_id);

    // DSA: allocate storage and optionally upload data in one call.
    // glNamedBufferData is the DSA equivalent of glBindBuffer + glBufferData.
    glNamedBufferData(m_id, sizeBytes, data, usage);
}

// ── VertexArray ───────────────────────────────────────────────────────────────

VertexArray::VertexArray()
{
    // DSA: create the VAO (no bind required).
    glCreateVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    if (m_id != 0) {
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other) {
        if (m_id != 0) {
            glDeleteVertexArrays(1, &m_id);
        }
        m_id       = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void VertexArray::addAttrib(GLuint bindingIndex, GLuint attribIndex,
                            GLint components, GLenum type, GLuint offset)
{
    // DSA: enable the attribute slot.
    glEnableVertexArrayAttrib(m_id, attribIndex);

    // DSA: describe the format of the attribute within its binding slot.
    glVertexArrayAttribFormat(m_id, attribIndex, components, type,
                              GL_FALSE, offset);

    // DSA: associate the attribute with a binding slot.
    glVertexArrayAttribBinding(m_id, attribIndex, bindingIndex);
}

void VertexArray::bindVertexBuffer(GLuint bindingIndex, const GpuBuffer& buf,
                                   GLintptr offset, GLsizei stride)
{
    // DSA: associate a buffer with a vertex buffer binding slot.
    // No glBindBuffer or glBindVertexArray needed.
    glVertexArrayVertexBuffer(m_id, bindingIndex, buf.id(), offset, stride);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

} // namespace engine
