#include "object2DT.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2DT::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2DT::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, length, 0), color),
        VertexFormat(corner + glm::vec3(length * 2, length / 2, 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2DT::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color)
{
    float rad = radius;
    std::vector<VertexFormat> vertices;
    int nrVert = 40;

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(center, color));
    indices.push_back(0);
    for (int i = 0; i < nrVert; i++) {
        vertices.push_back(VertexFormat(center + glm::vec3(rad * cos(i * 2 * M_PI / nrVert), rad * sin(i * 2 * M_PI / nrVert), 0), color));
        indices.push_back(i + 1);
    }
    indices.push_back(1);

    circle->SetDrawMode(GL_TRIANGLE_FAN);

    circle->InitFromData(vertices, indices);
    return circle;
}