#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <algorithm>

#include "lab_m1/Tema1/transform2DT.h"
#include "lab_m1/Tema1/object2DT.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    timer = -1;
    timeToEscape = 10;

    glm::vec3 corner = glm::vec3(0, 0, 0);
    bodyBase = 100;
    wingBase = 60;
    ciocBase = 25;
    headRadius = 40;

    maxx = bodyBase + headRadius / 2 + 2 * ciocBase; // jumatate de corp + razaCap / 2 
    minx = -bodyBase;                                // (pt ca doar atat e de la finalul corpului pana incepe ciocul) + cioc
    maxy = max(bodyBase / 2, wingBase * 2);
    miny = -max(bodyBase / 2, wingBase * 2);

    duckSpeedBasic = 300;
    duckSpeed = duckSpeedBasic;
    wingSpeed = 3;
    wingMoveStep = 0;
    wingMoveBack = true;

    showHitbox = false;
    
    isShot = false;
    isEscaping = false;
    noBullets = 3;
    tempBullets = noBullets;
    noLives = 3;
    tempLives = noLives;

    maxScore = 30;
    tempScore = 0;
    ducksShot = 0;
    ducksShotSinceMiss = 0;
    bonusSpeed = 1;
    bonusKillSpree = 1;
    killSpreeMoveStep = 0;
    
    grassHeight = 150;
    cloud1Radius = 30;
    cloud2Radius = 40;
    cloud3Radius = 30;
    cloudSpeed = 400;
    txCloud1 = 0;
    txCloud2 = 0;
    txCloud3 = 0;

    glLineWidth(3.0f);

    RandomDuckSpawn();

    Mesh* bodyWings = object2DT::CreateTriangle("bodyWings", corner, 1, glm::vec3(0.4f, 0.2f, 0.1f), true);
    AddMeshToList(bodyWings);

    Mesh* cioc = object2DT::CreateTriangle("cioc", corner, ciocBase, glm::vec3(1, 0.6f, 0.2f), true);
    AddMeshToList(cioc);

    Mesh* head = object2DT::CreateCircle("head", corner, headRadius, glm::vec3(0, 0.5f, 0));
    AddMeshToList(head);

    Mesh* life = object2DT::CreateCircle("life", corner, 30, glm::vec3(1, 0, 0));
    AddMeshToList(life);

    Mesh* cloud = object2DT::CreateCircle("cloud", corner, 1, glm::vec3(1, 1, 1));
    AddMeshToList(cloud);

    Mesh* bullet = object2DT::CreateSquare("bullet", corner, 1, glm::vec3(0, 0, 1));
    AddMeshToList(bullet);

    Mesh* score = object2DT::CreateSquare("score", corner, 1, glm::vec3(0.3f, 0.3f, 0.3f));
    AddMeshToList(score);

    Mesh* scoreBar = object2DT::CreateSquare("scoreBar", corner, 1, glm::vec3(0.2f, 0.2f, 0.2f), true);
    AddMeshToList(scoreBar);

    Mesh* grass = object2DT::CreateSquare("grass", corner, 1, glm::vec3(0.32f, 0.72f, 0.1f), true);
    AddMeshToList(grass);

    Mesh* dirt = object2DT::CreateSquare("dirt", corner, 1, glm::vec3(0.8f, 0.4f, 0.2f), true);
    AddMeshToList(dirt);

    Mesh* killSpree = object2DT::CreateSquare("killSpree", corner, 60, glm::vec3(1, 0, 0), true);
    AddMeshToList(killSpree);
}

void Tema1::RandomDuckSpawn() {
    srand(time(NULL));
    tx = rand() % (resolution.x - (3 * (int)headRadius + 3)) + headRadius;
    ty = 0;

    duckAngle = rand() % ((int)(100 * (M_PI - 0.52))) / 100 + 0.26; // Random angle from 0.26 to 2.88 (15 degrees to 165 degrees)
    
    while ((duckAngle > M_PI / 2 - 0.13) && (duckAngle < M_PI / 2 + 0.13)) // Recalculate angle is it's too close to M_PI / 2
        duckAngle = rand() % ((int)(100 * (M_PI - 0.52))) / 100 + 0.26;
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.4f, 0.6f, 0.9f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema1::ComputeGrass() {
    // Dirt
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DT::Scale(resolution.x, (int)(grassHeight - grassHeight / 3));
    RenderMesh2D(meshes["dirt"], shaders["VertexColor"], modelMatrix);

    // Grass
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DT::Scale(resolution.x, grassHeight);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);
}

void Tema1::ComputeCloud(int noCloud) {
    int radius = 0;
    float tyCloud = 0;
    float txCloud = 0;

    switch (noCloud) {
    case 1:
        radius = cloud1Radius;
        cloudSpeed = 500;
        tyCloud = resolution.y * 4 / 5;
        txCloud = txCloud1;
        if (txCloud * cloudSpeed >= resolution.x + radius * 3.4)
            txCloud1 = 0;
        break;

    case 2:
        radius = cloud2Radius;
        cloudSpeed = 300;
        tyCloud = resolution.y * 3 / 5;
        txCloud = (resolution.x + radius * 1.7) / cloudSpeed - txCloud2;
        if (txCloud2 * cloudSpeed >= resolution.x + radius * 3.4)
            txCloud2 = 0;
        break;

    case 3:
        radius = cloud3Radius;
        cloudSpeed = 400;
        tyCloud = resolution.y * 2 / 5;
        txCloud = txCloud3;
        if (txCloud * cloudSpeed >= resolution.x + radius * 3.4)
            txCloud3 = 0;
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DT::Translate(resolution.x + radius * 1.7 - cloudSpeed * txCloud, tyCloud);
    cpModelMatrix = modelMatrix;
    cpModelMatrix *= transform2DT::Translate(-radius, -radius);
    cpModelMatrix *= transform2DT::Scale(radius, radius);
    cpModelMatrix *= transform2DT::Translate(1, 1);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], cpModelMatrix);

    cpModelMatrix = modelMatrix;
    // Translate(-radius * 0.7 - radius * 0.9, -radius * 0.7 - radius / 2)
    cpModelMatrix *= transform2DT::Translate(-radius * 1.6, -radius * 1.2);
    cpModelMatrix *= transform2DT::Scale(radius * 0.7, radius * 0.7);
    cpModelMatrix *= transform2DT::Translate(1, 1);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], cpModelMatrix);

    cpModelMatrix = modelMatrix;
    // Translate(-radius * 0.7 + radius * 0.9, -radius * 0.7 - radius / 2)
    cpModelMatrix *= transform2DT::Translate(radius * 0.2, -radius * 1.2);
    cpModelMatrix *= transform2DT::Scale(radius * 0.7, radius * 0.7);
    cpModelMatrix *= transform2DT::Translate(1, 1);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], cpModelMatrix);
}

void Tema1::ComputeHud() {
    // Bullets
    for (int i = 0; i < tempBullets; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2DT::Translate(resolution.x - 50 * (noBullets - i), resolution.y - 100);
        modelMatrix *= transform2DT::Scale(30, 70);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }

    // Lives
    for (int i = 0; i < tempLives; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2DT::Translate(resolution.x - 50 * noBullets + 20 - 70 * (noLives - i), resolution.y - 65);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }

    // KillSpree
    for (int i = 0; i < ((bonusKillSpree - 1) / 0.5); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2DT::Translate(50 + i * 80, resolution.y - 65);
        modelMatrix *= transform2DT::Rotate(-killSpreeMoveStep);
        modelMatrix *= transform2DT::Translate(-30, -30);
        RenderMesh2D(meshes["killSpree"], shaders["VertexColor"], modelMatrix);
    }

    // Score
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DT::Translate(resolution.x - 50 * noBullets - 70 * noLives - 10, resolution.y - 170);
    modelMatrix *= transform2DT::Scale(50 * noBullets + 70 * noLives - 10, 50);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DT::Translate(resolution.x - 50 * noBullets - 70 * noLives - 10, resolution.y - 170);
    modelMatrix *= transform2DT::Scale((50 * noBullets + 70 * noLives - 10) * tempScore / maxScore, 50);
    RenderMesh2D(meshes["scoreBar"], shaders["VertexColor"], modelMatrix);
}

void Tema1::ComputeDuck() {
    modelMatrix = glm::mat3(1);

    // Head
    modelMatrix *= transform2DT::Translate(tx, ty);
    modelMatrix *= transform2DT::Rotate(duckAngle);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);

    // Cioc
    cpModelMatrix = modelMatrix;
    cpModelMatrix *= transform2DT::Translate(headRadius / 2, -headRadius / 2);
    RenderMesh2D(meshes["cioc"], shaders["VertexColor"], cpModelMatrix);

    // Body
    modelMatrix *= transform2DT::Translate(-bodyBase * 2, -bodyBase / 2); // Connect body to head
    cpModelMatrix = modelMatrix;
    cpModelMatrix *= transform2DT::Scale(bodyBase, bodyBase);
    RenderMesh2D(meshes["bodyWings"], shaders["VertexColor"], cpModelMatrix);

    // Left Wing
    modelMatrix *= transform2DT::Translate(bodyBase, bodyBase / 2);
    cpModelMatrix = modelMatrix;
    cpModelMatrix *= transform2DT::Rotate(M_PI / 2 + wingMoveStep);
    cpModelMatrix *= transform2DT::Translate(0, -wingBase / 2);
    cpModelMatrix *= transform2DT::Scale(wingBase, wingBase);
    RenderMesh2D(meshes["bodyWings"], shaders["VertexColor"], cpModelMatrix);

    // Right Wing
    cpModelMatrix = modelMatrix;
    cpModelMatrix *= transform2DT::Rotate(-M_PI / 2 - wingMoveStep);
    cpModelMatrix *= transform2DT::Translate(0, -wingBase / 2);
    cpModelMatrix *= transform2DT::Scale(wingBase, wingBase);
    RenderMesh2D(meshes["bodyWings"], shaders["VertexColor"], cpModelMatrix);

    // Hitbox
    if (showHitbox) {
        cpModelMatrix = modelMatrix;
        cpModelMatrix *= transform2DT::Translate(minx, miny);
        cpModelMatrix *= transform2DT::Scale(maxx - minx, maxy - miny);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], cpModelMatrix);
    }
}

void Tema1::Respawn() {
    RandomDuckSpawn();

    if (!isShot) {
        tempLives--;
        bonusKillSpree = 1;
        ducksShotSinceMiss = 0;
    }
    else if (tempScore < maxScore - bonusSpeed * bonusKillSpree) {
        duckSpeed /= 2;     // Delete duck speedup (gained from falling)
        tempScore += bonusSpeed * bonusKillSpree;
        ducksShot++;
        ducksShotSinceMiss++;

        if (ducksShot % 5 == 0) {
            duckSpeed += duckSpeed / 3;
            bonusSpeed += 0.2;
        }

        if (ducksShotSinceMiss % 3 == 0 && bonusKillSpree < 2.5) {
            bonusKillSpree += 0.5;
        }
    }
    else {
        tempLives = noLives;
        tempScore = 0;
        ducksShot = 0;
        ducksShotSinceMiss = 0;
        bonusKillSpree = 1;
        bonusSpeed = 1;
        duckSpeed = duckSpeedBasic;
    }

    if (tempLives <= 0) {
        tempLives = noLives;
        tempScore = 0;
        ducksShot = 0;
        ducksShotSinceMiss = 0;
        bonusKillSpree = 1;
        bonusSpeed = 1;
        duckSpeed = duckSpeedBasic;
    }

    isShot = false;
    isEscaping = false;
    tempBullets = noBullets;

    // Cooldown
    timer = -1;
}

void Tema1::ManageWallCollision() {
    // Down Left -> Right
    if (duckAngle >= (3 * M_PI / 2) && ty - headRadius <= 0)
        duckAngle += (2 * M_PI - duckAngle) * 2 - 2 * M_PI;

    // Down Right -> Left
    if (duckAngle >= M_PI && duckAngle < (3 * M_PI / 2) && ty - headRadius <= 0)
        duckAngle -= (duckAngle - M_PI) * 2;

    // Up Left -> Right
    if (duckAngle <= M_PI / 2 && ty + headRadius >= resolution.y)
        duckAngle = 2 * M_PI - duckAngle;

    // Up Right -> Left
    if (duckAngle < M_PI && duckAngle > M_PI / 2 && ty + headRadius >= resolution.y)
        duckAngle += (M_PI - duckAngle) * 2;

    // Right Down -> Up
    if (duckAngle < M_PI / 2 && tx + headRadius >= resolution.x)
        duckAngle += (M_PI / 2 - duckAngle) * 2;

    // Right Up -> Down
    if (duckAngle > (3 * M_PI / 2) && tx + headRadius >= resolution.x)
        duckAngle -= (duckAngle - (3 * M_PI / 2)) * 2;

    // Left Down -> Up
    if (duckAngle < M_PI && duckAngle > M_PI / 2 && tx - headRadius <= 0)
        duckAngle = M_PI - duckAngle;

    // Left Up -> Down
    if (duckAngle > M_PI && duckAngle < (3 * M_PI / 2) && tx - headRadius <= 0)
        duckAngle += ((3 * M_PI / 2) - duckAngle) * 2;
}

bool Tema1::isDuckOutOfBounds() {
    // 0.1 is used to make sure the duck's angle is exactly M_PI / 2 and 3 * M_PI / 2

    if (isEscaping && (ty - (bodyBase * 2) >= resolution.y))    // Duck is escaping and the body is out of the screen
            return true;

    if (isShot && (ty + (bodyBase * 2) <= 0))   // Duck is dead and the body is out of the screen
            return true;

    return false;
}

void Tema1::Update(float deltaTimeSeconds)
{
    ComputeHud();
    ComputeGrass();

    if (timer < 0)
        timer += deltaTimeSeconds;
    else {
        ComputeDuck();

        // Move wings
        if ((((wingMoveStep >= M_PI / 4) && wingMoveBack) || ((wingMoveStep <= -M_PI / 6) && !wingMoveBack)) && !isShot)
            wingMoveBack = !wingMoveBack;

        if (wingMoveBack && !isShot)
            wingMoveStep += wingSpeed * deltaTimeSeconds;
        else if (!isShot)
            wingMoveStep -= wingSpeed * deltaTimeSeconds;
        else
            wingMoveStep = M_PI / 4;

        // Move Duck
        tx += duckSpeed * cos(duckAngle) * deltaTimeSeconds;
        ty += duckSpeed * sin(duckAngle) * deltaTimeSeconds;


        // Escape, Collision or Shot
        if (!isShot) {
            timer += deltaTimeSeconds;
            if (timer >= timeToEscape) {
                duckAngle = M_PI / 2;
                isEscaping = true;
            }
            else if (!isEscaping)
                ManageWallCollision();
        }
        else
            duckAngle = 3 * M_PI / 2;

        // Check is duck is out of bounds
        if (isDuckOutOfBounds())
            Respawn();
    }
    // Rotate killSpree Squares
    if (killSpreeMoveStep > 2 * M_PI)
        killSpreeMoveStep -= 2 * M_PI;
    killSpreeMoveStep += wingSpeed * deltaTimeSeconds;

    // Move clouds
    txCloud1 += deltaTimeSeconds;
    txCloud2 += deltaTimeSeconds;
    txCloud3 += deltaTimeSeconds;

    ComputeCloud(1);
    ComputeCloud(2);
    ComputeCloud(3);
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_H))
        showHitbox = true;
    else
        showHitbox = false;
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1 && timer >= 0) {
        float x = mouseX;
        float y = resolution.y - mouseY;

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2DT::Translate(bodyBase, 0);
        modelMatrix *= transform2DT::Rotate(-duckAngle);
        modelMatrix *= transform2DT::Translate(-tx, -ty);
        
        glm::vec3 newCoord = modelMatrix * glm::vec3(x, y, 1);

        // [0, 0] is now in the center of the duck's body

        if (newCoord.x <= maxx && newCoord.x >= minx && tempBullets > 0)
            if (newCoord.y <= maxy && newCoord.y >= miny && y >= grassHeight) {
                isShot = true;
                duckSpeed *= 2;     // Duck speedup when falling
            }

        tempBullets--;

        // Escape?
        if (!isShot && (tempBullets <= 0))
            timer = timeToEscape;
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    ClearScreen(glm::vec3(0, 0, 0));
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->Update();
}
