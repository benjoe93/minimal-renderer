#pragma once
#include <glad/glad.h>
#include <vector>

class UniformBufferObj
{
private:
    unsigned int m_renderer_id;

public:
    UniformBufferObj(GLsizeiptr size);
    ~UniformBufferObj();

    void Bind() const;
    void Unbind() const;

    /*Bind a range within a buffer object to an indexed buffer target
    * @param idx - Specify the index of the binding point within the array specified by target.
    * @param offset - The starting offset in basic machine units into the buffer object buffer.
    * @param size - The amount of data in machine units that can be read from the buffet object while used as an indexed target.
    */
    void SetRange(unsigned int idx, GLintptr offset, GLsizeiptr size);

    /*Store data
    @param offset - Specifies the offset into the buffer object's data store where data replacement will begin, measured in bytes.
    @param size - Specifies the size in bytes of the data store region being replaced.
    @param data - Specifies a pointer to the new data that will be copied into the data store.
    */
    void StoreData(GLintptr offset, GLsizeiptr size, const void* data);
};