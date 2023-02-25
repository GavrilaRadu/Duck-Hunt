#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/TemaCamera.h"
#include <vector>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void ComputeRaceTrack();
        void FillBetweenPoints(glm::vec3& sus, glm::vec3& jos);
        void FillBetweenChunks(glm::vec3& sus, glm::vec3& jos, int preNoPoints, bool& waiting);
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(0, 0, 0));
        void Tema2::RenderTree(glm::vec3 place);

        void FrameStart() override;
        void PlaceRandomTrees();
        void SpawnEnemyCars();
        void MoveEnemyCars(float deltaTimeSeconds);
        float TriangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c);
        bool InsideTriangle(glm::vec3 carPos, int a, int b, int c);
        bool isOnRaceTrack(glm::vec3 carPos);
        void MoveCar(float deltaTime);
        void Update(float deltaTimeSeconds) override;
        void RenderScene(float deltaTimeSeconds);
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        implementedTema::Camera *camera;
        ViewportArea miniViewportArea;
        glm::mat4 projectionMatrix;
        glm::mat4 carModelMatrix;
        GLenum polygonMode;
        std::vector<VertexFormat> rtVertices;
        int density, densityWidth;
        float idealDist;
        std::vector<glm::vec3> treePlacements;
        int treeCnt, treesLeft, treesRight;
        std::vector<int> ecIndex;
        float enemySpeed, speed, speedMax, acceleration, speedPrev, rotSpeed;
        float rotCar;
        float scaleFactor, scaleFactorSpeed;
    };
}   // namespace m1
