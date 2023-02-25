#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void RandomDuckSpawn();
        void ComputeDuck();
        void ComputeCloud(int noCloud);
        void ComputeGrass();
        void ComputeHud();
        void Respawn();
        void ManageWallCollision();
        bool isDuckOutOfBounds();
        void Update(float deltaTimeSeconds) override;
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
        glm::mat3 modelMatrix;
        glm::mat3 cpModelMatrix;
        glm::ivec2 resolution;
        float tx, ty;
        float txCloud1, txCloud2, txCloud3;
        float cloud1Radius, cloud2Radius, cloud3Radius;
        float cloudSpeed;
        float bodyBase, wingBase, ciocBase;
        float headRadius;
        float maxx, minx, maxy, miny;
        int grassHeight;
        bool showHitbox;
        float wingSpeed;
        float wingMoveStep;
        bool wingMoveBack;
        float timer;
        int timeToEscape;
        float duckAngle, duckSpeed, duckSpeedBasic;
        bool isShot, isEscaping;
        int noBullets, tempBullets;
        int noLives, tempLives;
        float maxScore, tempScore;
        int ducksShot, ducksShotSinceMiss;
        float bonusSpeed, bonusKillSpree;
        float killSpreeMoveStep;
    };
}   // namespace m1
