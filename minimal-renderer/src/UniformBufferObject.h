#pragma once
#include <glad/glad.h>

class UniformBufferObj
{
private:
    GLuint m_renderer_id;
    GLsizeiptr m_size;

public:
    UniformBufferObj(GLsizeiptr size);
    ~UniformBufferObj();

    // Prevent copying
    UniformBufferObj(const UniformBufferObj&) = delete;
    UniformBufferObj& operator=(const UniformBufferObj&) = delete;

    // Allow moving
    UniformBufferObj(UniformBufferObj&& other) noexcept;
    UniformBufferObj& operator=(UniformBufferObj&& other) noexcept;

    void Bind() const;
    void Unbind() const;

    /**
     * Bind a range within a buffer object to an indexed buffer target
     * @param idx - Index of the binding point within the array
     * @param offset - Starting offset in bytes into the buffer object
     * @param size - Amount of data in bytes that can be read from the buffer
     */
    void SetRange(GLuint idx, GLintptr offset, GLsizeiptr size) const;

    /**
     * Store data in the buffer
     * @param offset - Offset into the buffer's data store in bytes
     * @param size - Size in bytes of the data being stored
     * @param data - Pointer to the new data
     */
    void StoreData(GLintptr offset, GLsizeiptr size, const void* data) const;

    /**
     * Convenience template for storing typed data
     * @param data - Reference to the data to store
     * @param offset - Offset into the buffer in bytes (default: 0)
     */
    template<typename T>
    void Store(const T& data, GLintptr offset = 0) const
    {
        StoreData(offset, sizeof(T), &data);
    }

    GLuint GetID() const { return m_renderer_id; }
    GLsizeiptr GetSize() const { return m_size; }
};