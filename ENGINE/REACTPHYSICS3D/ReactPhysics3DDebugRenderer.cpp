#include "ReactPhysics3DDebugRenderer.h"

ReactPhysicsDebugRenderer::ReactPhysicsDebugRenderer()
{
}

ReactPhysicsDebugRenderer::ReactPhysicsDebugRenderer(
    std::string vertPath, 
    std::string fragPath, 
    std::shared_ptr<ReactPhysics3DCore> reactPhysics3DCore
)
    : 
    reactPhysics3DCore{ reactPhysics3DCore },
    debugShader(std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str(), nullptr, nullptr, nullptr))
{
    // Create VAO
    glCreateVertexArrays(1, &debugVAO);

    // Create VBO (interleaved position + color)
    glCreateBuffers(1, &debugVBO);
    glNamedBufferStorage(
        debugVBO,
        (sizeof(rp3d::Vector3) + sizeof(uint32_t)) * MAX_VERTICES,
        nullptr,
        GL_DYNAMIC_STORAGE_BIT
    );

    // ----- BINDING POINT 0 -----
    // One vertex = vec3 (12 bytes) + uint32 color (4 bytes) = 16 bytes
    const GLsizei stride = sizeof(rp3d::Vector3) + sizeof(uint32_t);

    glVertexArrayVertexBuffer(
        debugVAO,
        0,               // binding index
        debugVBO,
        0,               // offset
        stride           // stride
    );

    // ----- POSITION ATTRIBUTE (location = 0) -----
    glVertexArrayAttribFormat(
        debugVAO,
        0,               // attribute index
        3,               // vec3
        GL_FLOAT,
        GL_FALSE,
        0                // offset in vertex struct
    );
    glVertexArrayAttribBinding(debugVAO, 0, 0); // attribute 0 → binding 0
    glEnableVertexArrayAttrib(debugVAO, 0);

    // ----- COLOR ATTRIBUTE (location = 1) -----
    glVertexArrayAttribIFormat(
        debugVAO,
        1,               // attribute index
        1,               // uint
        GL_UNSIGNED_INT,
        sizeof(rp3d::Vector3)  // offset = 12 bytes
    );
    glVertexArrayAttribBinding(debugVAO, 1, 0); // attribute 1 → binding 0
    glEnableVertexArrayAttrib(debugVAO, 1);
}

ReactPhysicsDebugRenderer::~ReactPhysicsDebugRenderer()
{
	if (debugVBO) glDeleteBuffers(1, &debugVBO);
	if (debugVAO) glDeleteVertexArrays(1, &debugVAO);
}

void ReactPhysicsDebugRenderer::ReactPhysicsRendererRender(glm::mat4 view, glm::mat4 proj)
{
    rp3d::DebugRenderer& debugRenderer = reactPhysics3DCore->GetPhysicsWorld()->getDebugRenderer();

    if (reactPhysics3DCore->IsDebugEnabled())
    {
        // -- Lines -- //
        const unsigned int numLines = debugRenderer.getNbLines();
        size_t vertexSize = sizeof(rp3d::Vector3) + sizeof(uint32_t);

        if (numLines > 0)
        {
            glNamedBufferSubData(
                debugVBO, 
                0, 
                static_cast<unsigned long long>(numLines) * 2 * vertexSize,
                debugRenderer.getLinesArray()
            );
        }

        const unsigned int numTriangles = debugRenderer.getNbTriangles();
        if (numTriangles > 0)
        {
            glNamedBufferSubData(
                debugVBO, 
                static_cast<unsigned long long>(numLines) * 2 * vertexSize,
                static_cast<unsigned long long>(numTriangles) * 3 * vertexSize,
                debugRenderer.getTrianglesArray()
            );
        }

        debugShader->use();
        debugShader->setMat4("view", view);
        debugShader->setMat4("projection", proj);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(debugVAO);
        glDrawArrays(GL_LINES, 0, numLines * 2);
        glDrawArrays(GL_TRIANGLES, numLines * 2, numTriangles * 3);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } 

}
