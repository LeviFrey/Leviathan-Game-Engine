#include <glad/glad.h>
#include "Mesh.h"

/*
 * Strictly Manipulates and Stores instructions (VAO) on how to read rendering data
 * TODO: allow for dynamic addition of data, such as positions for instance rendering
 */
class RenderDataConfig {
    public:
    void init();

    void addAttribute(int dim, size_t offset, GLuint binding);

    unsigned int getVAO() const { return VAO_; }

    void setBindingDivisor(GLuint binding, GLuint divisor);

    void bind() const { glBindVertexArray(VAO_); }
    
    private:
        unsigned int VAO_;
        GLuint location_index_ = 0;
};
