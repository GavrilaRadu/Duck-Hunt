#include "lab_m1/Tema2/Tema2.h"

#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new implementedTema::Camera();
    camera->Set(glm::vec3(-9.311, 1.25f, 1.775) * glm::vec3(4.f, 1.f, 4.f), glm::vec3(0.845f, -0.379f, 0.375f), glm::vec3(0.347f, 0.925f, 0.154f));

    density = 50;
    densityWidth = 10;

    treeCnt = 0;
    treesLeft = 800;
    treesRight = 400;

    enemySpeed = 1;
    speed = 0;
    speedPrev = 0;
    speedMax = 7;
    acceleration = 10;
    rotSpeed = 1.5;

    scaleFactor = 0.01;
    scaleFactorSpeed = 0.01;

    polygonMode = GL_FILL;
    srand(time(NULL));

    // Create race track
    ComputeRaceTrack();

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane50");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("car1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "myModels/Tema2/car1"), "car1.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("car2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "myModels/Tema2/car2"), "car2.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // Create a shader program
    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("TemaShaderTexture");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TextureVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TextureFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    SpawnEnemyCars();


    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    rotCar = atan2(rtVertices[densityWidth].position.z - rtVertices[0].position.z, rtVertices[densityWidth].position.x - rtVertices[0].position.x) + M_PI;
}

void Tema2::ComputeRaceTrack() {
    vector<glm::vec3> auxVertices
    {
        glm::vec3(-9.311, 0, 1.775), // A
        glm::vec3(-8.827, 0, 1.997), // S3
        glm::vec3(-8.415, 0, 2.199), // B
        glm::vec3(-7.935, 0, 2.407), // T3
        glm::vec3(-7.451, 0, 2.643), // C
        glm::vec3(-7.021, 0, 2.828), // U3
        glm::vec3(-6.531, 0, 3.054), // D
        glm::vec3(-6.072, 0, 3.264), // V3
        glm::vec3(-5.670, 0, 3.448), // E
        glm::vec3(-5.271, 0, 3.632), // W3
        glm::vec3(-4.833, 0, 3.841), // F
        glm::vec3(-4.424, 0, 4.022), // Z3
        glm::vec3(-3.951, 0, 4.254), // G
        glm::vec3(-3.203, 0, 4.596), // H
        glm::vec3(-2.398, 0, 4.957), // I
        glm::vec3(-1.656, 0, 5.306), // J
        glm::vec3(-0.933, 0, 5.636), // B4
        glm::vec3(-0.229, 0, 5.953), // C4
        glm::vec3(0.544, 0, 6.321), // D4
        glm::vec3(1.292, 0, 6.657), // E4
        glm::vec3(1.926, 0, 6.948), // F4
        glm::vec3(2.598, 0, 7.259), // G4
        glm::vec3(3.258, 0, 7.563), // H4
        glm::vec3(4.003, 0, 7.897), // I4
        glm::vec3(4.386, 0, 8.080), // A4
        glm::vec3(4.834, 0, 8.269), // K
        glm::vec3(5.075, 0, 8.366), // Z2
        glm::vec3(5.400, 0, 8.420), // L
        glm::vec3(5.705, 0, 8.414), // A3
        glm::vec3(6.000, 0, 8.342), // M
        glm::vec3(6.245, 0, 8.223), // B3
        glm::vec3(6.458, 0, 8.071), // N
        glm::vec3(6.710, 0, 7.794), // O
        glm::vec3(6.904, 0, 7.391), // P
        glm::vec3(6.955, 0, 6.936), // Q
        glm::vec3(6.897, 0, 6.523), // R
        glm::vec3(6.762, 0, 6.213), // S
        glm::vec3(6.532, 0, 5.927), // K4
        glm::vec3(6.151, 0, 5.653), // L4
        glm::vec3(5.684, 0, 5.511), // M4
        glm::vec3(5.298, 0, 5.512), // N4
        glm::vec3(4.813, 0, 5.516), // T
        glm::vec3(4.257, 0, 5.517), // U
        glm::vec3(3.803, 0, 5.517), // V
        glm::vec3(3.442, 0, 5.422), // V2
        glm::vec3(3.146, 0, 5.236), // P4
        glm::vec3(2.865, 0, 4.924), // W
        glm::vec3(2.725, 0, 4.594), // Z
        glm::vec3(2.691, 0, 4.196), // A1
        glm::vec3(2.783, 0, 3.770), // B1
        glm::vec3(3.009, 0, 3.408), // Q4
        glm::vec3(3.318, 0, 3.153), // R4
        glm::vec3(3.631, 0, 3.020), // S4
        glm::vec3(4.033, 0, 2.977), // T4
        glm::vec3(4.470, 0, 2.979), // C1
        glm::vec3(4.933, 0, 3.049), // D1
        glm::vec3(5.322, 0, 3.144), // E1
        glm::vec3(5.692, 0, 3.239), // F1
        glm::vec3(6.195, 0, 3.343), // G1
        glm::vec3(6.561, 0, 3.280), // H1
        glm::vec3(6.964, 0, 3.099), // W2
        glm::vec3(7.135, 0, 2.935), // I1
        glm::vec3(7.363, 0, 2.593), // J1
        glm::vec3(7.467, 0, 2.204), // K1
        glm::vec3(7.429, 0, 1.805), // L1
        glm::vec3(7.326, 0, 1.500), // M1
        glm::vec3(7.184, 0, 1.119), // N1
        glm::vec3(7.046, 0, 0.725), // O1
        glm::vec3(6.909, 0, 0.368), // P1
        glm::vec3(6.822, 0, 0.139), // Q1
        glm::vec3(6.712, 0, -0.167), // R1
        glm::vec3(6.612, 0, -0.475), // S1
        glm::vec3(6.478, 0, -0.784), // T1
        glm::vec3(6.317, 0, -1.057), // U1
        glm::vec3(6.093, 0, -1.380), // V1
        glm::vec3(5.861, 0, -1.625), // W1
        glm::vec3(5.637, 0, -1.856), // Z1
        glm::vec3(5.371, 0, -2.060), // A2
        glm::vec3(5.069, 0, -2.256), // B2
        glm::vec3(4.796, 0, -2.389), // C2
        glm::vec3(4.410, 0, -2.551), // D2
        glm::vec3(4.046, 0, -2.642), // E2
        glm::vec3(3.639, 0, -2.705), // F2
        glm::vec3(3.190, 0, -2.691), // G2
        glm::vec3(2.734, 0, -2.656), // H2
        glm::vec3(2.374, 0, -2.554), // C3
        glm::vec3(1.660, 0, -2.284), // D3
        glm::vec3(0.799, 0, -1.890), // E3
        glm::vec3(0.000, 0, -1.552), // F3
        glm::vec3(-0.625, 0, -1.264), // G3
        glm::vec3(-1.045, 0, -1.080), // H3
        glm::vec3(-1.464, 0, -0.889), // I3
        glm::vec3(-1.825, 0, -0.726), // J3
        glm::vec3(-2.559, 0, -0.498), // K3
        glm::vec3(-3.344, 0, -0.452), // L3
        glm::vec3(-3.911, 0, -0.479), // M3
        glm::vec3(-4.465, 0, -0.547), // N3
        glm::vec3(-5.032, 0, -0.695), // O3
        glm::vec3(-5.666, 0, -0.938), // P3
        glm::vec3(-6.301, 0, -1.235), // Q3
        glm::vec3(-6.814, 0, -1.532), // R3
        glm::vec3(-7.327, 0, -1.816), // J2
        glm::vec3(-8.001, 0, -1.950), // K2
        glm::vec3(-8.577, 0, -1.886), // L2
        glm::vec3(-9.092, 0, -1.677), // M2
        glm::vec3(-9.440, 0, -1.426), // N2
        glm::vec3(-9.735, 0, -1.101), // O2
        glm::vec3(-9.915, 0, -0.807), // P2
        glm::vec3(-10.068, 0, -0.401), // Q2
        glm::vec3(-10.134, 0, 0.075), // R2
        glm::vec3(-10.090, 0, 0.545), // S2
        glm::vec3(-9.993, 0, 0.874), // T2
        glm::vec3(-9.823, 0, 1.214), // U2
        glm::vec3(-9.613, 0, 1.495), // S3
    };

    vector<unsigned int> indices;
    int noPoints, preNoPoints;
    int n = auxVertices.size();
    bool waiting = false;

    for (int i = 0; i < n; i++) {
        int nxt;
        if (i == n - 1)
            nxt = 0;
        else
            nxt = i + 1;

        glm::vec3 d = auxVertices[nxt] - auxVertices[i];

        // Impartim distanta dintre primele 2 puncte in density bucati obtimen o distanta "ideala"
        // pt urmatoarele puncte folosim aceasta distanta "ideala" pentru a creea puncte noi intre 2 puncte scrise in auxVertices
        if (i == 0) {
            idealDist = sqrt(pow(auxVertices[nxt].x - auxVertices[i].x, 2) + pow(auxVertices[nxt].z - auxVertices[i].z, 2) * 1.f) / density;
            noPoints = density;
        }
        else {
            noPoints = sqrt(pow(auxVertices[nxt].x - auxVertices[i].x, 2) + pow(auxVertices[nxt].z - auxVertices[i].z, 2) * 1.f) / idealDist;
        }

        glm::vec3 p = cross(glm::vec3(d.x * 1.f / noPoints, 0, d.z * 1.f / noPoints), glm::vec3(0, 1, 0));

        // We break the road into chunks, if (i % 2 == 0) we duplicate a set of points until we get to the next point
        // if (i % 2 != 0) we connect the points to each other
        if (i % 2 == 0) {
            for (int j = 1; j <= noPoints; j++) {
                glm::vec3 sus = glm::vec3(auxVertices[i] + glm::vec3(j * 1.f / noPoints) * d + p * glm::vec3(density));
                glm::vec3 jos = glm::vec3(auxVertices[i] + glm::vec3(j * 1.f / noPoints) * d - p * glm::vec3(density));
                
                // If we skipped some points to calculate the next one = (i % 2 == 1)
                if (waiting)
                    FillBetweenChunks(sus, jos, preNoPoints, waiting);

                // Continue with (i % 2 == 0)
                FillBetweenPoints(sus, jos);

                // Save tree spots
                treeCnt += 2;
                if (treeCnt > treesLeft) {
                    treePlacements.push_back(auxVertices[i] + glm::vec3(d.x * j / noPoints, 0, d.z * j / noPoints) - glm::vec3(p.x * density * 1.5, 0, p.z * density * 1.5));
                    treesLeft = treeCnt + rand() % (5 * density) + (4 * density);
                }
                if (treeCnt > treesRight) {
                    treePlacements.push_back(auxVertices[i] + glm::vec3(d.x * j / noPoints, 0, d.z * j / noPoints) + glm::vec3(p.x * density * 1.5, 0, p.z * density * 1.5));
                    treesRight = treeCnt + rand() % (5 * density) + (4 * density);
                }
            }
        }
        else {
            preNoPoints = noPoints;
            waiting = true;
            
            if (nxt == 0)
                FillBetweenChunks(rtVertices[0].position, rtVertices[densityWidth - 1].position, preNoPoints, waiting);
        }
    }

    long counter = 0;
    n = rtVertices.size();
    for (int i = 0; i < n; i++, counter++) {
        if ((i == densityWidth - 1) || (counter == densityWidth)) {
            counter = 0;
            continue;
        }

        if (i + densityWidth >= n) {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + densityWidth + 1 - n);

            indices.push_back(i);
            indices.push_back(i + densityWidth + 1 - n);
            indices.push_back(i + densityWidth - n);
        }
        else {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + densityWidth + 1);

            indices.push_back(i);
            indices.push_back(i + densityWidth + 1);
            indices.push_back(i + densityWidth);
        }
    }

    CreateMesh("raceTrack", rtVertices, indices);
}

void Tema2::FillBetweenChunks(glm::vec3& sus, glm::vec3& jos, int preNoPoints, bool& waiting)
{
    glm::vec3 lastSus = rtVertices[rtVertices.size() - densityWidth].position;
    glm::vec3 lastJos = rtVertices[rtVertices.size() - 1].position;

    glm::vec3 d1 = sus - lastSus;
    glm::vec3 d2 = jos - lastJos;

    for (int i = 1; i < preNoPoints; i++)
        FillBetweenPoints(lastSus + glm::vec3(i * 1.f / preNoPoints) * d1, lastJos + glm::vec3(i * 1.f / preNoPoints) * d2);

    waiting = false;
}

void Tema2::FillBetweenPoints(glm::vec3& sus, glm::vec3& jos)
{
    glm::vec3 dw = jos - sus;

    // Vrem sa punem punctele de sus in jos pt a pastra aceeasi logica in tot programul
    for (int i = 0; i <= densityWidth - 1; i++) {
        rtVertices.push_back(VertexFormat(sus + (glm::vec3(i * 1.f / (densityWidth - 1)) * dw), glm::vec3(0, 0, 0)));
    }
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

// If there is no color specified (vec3(0, 0, 0)), we will use v_color
void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    int car_mat = glGetUniformLocation(shader->program, "CarModel");
    glUniformMatrix4fv(car_mat, 1, GL_FALSE, glm::value_ptr(carModelMatrix));

    int scale_factor = glGetUniformLocation(shader->program, "scaleFactor");
    glUniform1f(scale_factor, scaleFactor);

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderTree(glm::vec3 place) {

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, place + glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 2, 0.6));
    RenderSimpleMesh(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(0.4f, 0.2f, 0.1f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, place + glm::vec3(0, 2, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
    RenderSimpleMesh(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0.4f, 0));
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.2f, 0.4f, 0.9f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::PlaceRandomTrees() {
    for (auto x : treePlacements) {
        RenderTree(x * glm::vec3(4));
    }
}

void Tema2::SpawnEnemyCars() {
    // Numarul de masini (vrem o masina o data la 30 de puncte REALE)
    int noCars = rtVertices.size() / (30 * density * densityWidth);
    int pb;

    // Vrem sa eliminam punctele de pe exteriorul pistei (vrem ca masina sa nu arate de parca nu este pe pista)
    for (int i = 0; i < noCars; i++) {
        pb = rand() % (30 * density * densityWidth) + i * (30 * density * densityWidth);

        if (pb % densityWidth == 0)
            pb++;
        else if (pb % densityWidth == densityWidth - 1)
            pb--;

        ecIndex.push_back(pb);
    }
}

void Tema2::MoveEnemyCars(float deltaTimeSeconds) {
    glm::mat4 modelMatrix;
    int n = ecIndex.size();
    int nxt;

    for (int i = 0; i < n; i++) {
        int jump = enemySpeed * deltaTimeSeconds / idealDist;
        nxt = ecIndex[i];

        if (ecIndex[i] - densityWidth * jump < 0)
            nxt = ecIndex[i] - densityWidth * jump + rtVertices.size();
        else
            nxt -= densityWidth * jump;

        float rot = atan2(rtVertices[nxt].position.z - rtVertices[ecIndex[i]].position.z, rtVertices[nxt].position.x - rtVertices[ecIndex[i]].position.x) + M_PI;

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, rtVertices[ecIndex[i]].position * glm::vec3(4.f) + glm::vec3(0, 0.13f, 0));
        modelMatrix = glm::rotate(modelMatrix, -rot, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        RenderSimpleMesh(meshes["car2"], shaders["TemaShaderTexture"], modelMatrix);
        ecIndex[i] = nxt;
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    glm::vec3 position = camera->position;
    glm::vec3 forward = camera->forward;
    glm::vec3 right = camera->right;
    glm::vec3 up = camera->up;

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    RenderScene(deltaTimeSeconds);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    camera->Set(camera->GetTargetPosition() + glm::vec3(0, 6, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1));
    
    glm::ivec2 resolution = window->GetResolution();
    projectionMatrix = glm::ortho(-resolution.x / 100.f, resolution.x / 100.f, -resolution.y / 100.f, resolution.y / 100.f, 0.01f, 300.0f);
    RenderScene(deltaTimeSeconds);

    camera->Set(position, forward, up);
}

void Tema2::RenderScene(float deltaTimeSeconds)
{
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    {
        carModelMatrix = glm::mat4(1);
        carModelMatrix = glm::translate(carModelMatrix, camera->GetTargetPosition());
        carModelMatrix = glm::rotate(carModelMatrix, -rotCar, glm::vec3(0, 1, 0));
        carModelMatrix = glm::scale(carModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        RenderSimpleMesh(meshes["car1"], shaders["TemaShaderTexture"], carModelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
        RenderSimpleMesh(meshes["raceTrack"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix;
        int n = 10;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3((-50.f / n) * (n - 1) + (2 * 50.f / n) * i, 0.1f, (-50.f / n) * (n - 1) + (2 * 50.f / n) * j));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2.f / n, 1, 2.f / n));
                RenderSimpleMesh(meshes["plane50"], shaders["TemaShader"], modelMatrix, glm::vec3(0.32f, 0.72f, 0.1f));
            }
        }
    }
    PlaceRandomTrees();
    MoveEnemyCars(deltaTimeSeconds);
}

void Tema2::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

float Tema2::TriangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    glm::vec3 ab = glm::vec3(b.x - a.x, 0, b.z - a.z);
    glm::vec3 ac = glm::vec3(c.x - a.x, 0, c.z - a.z);
    float cp = ab.x * ac.z - ab.z * ac.x;

    if (cp < 0)
        cp = -cp;

    return (cp / 2);
}

bool Tema2::InsideTriangle(glm::vec3 carPos, int a, int b, int c) {
    float area = TriangleArea(rtVertices[a].position * glm::vec3(4.f), rtVertices[b].position * glm::vec3(4.f), rtVertices[c].position * glm::vec3(4.f));

    float areaTotal = TriangleArea(rtVertices[a].position * glm::vec3(4.f), rtVertices[b].position * glm::vec3(4.f), carPos);
    areaTotal += TriangleArea(rtVertices[a].position * glm::vec3(4.f), rtVertices[c].position * glm::vec3(4.f), carPos);
    areaTotal += TriangleArea(rtVertices[b].position * glm::vec3(4.f), rtVertices[c].position * glm::vec3(4.f), carPos);
    
    float err = 0.0001f;

    if (areaTotal >= area - err && areaTotal <= area + err)
        return true;
    return false;
}

bool Tema2::isOnRaceTrack(glm::vec3 carPos) {
    int n = rtVertices.size();

    for (int i = 0; i < n; i += densityWidth) {
        int nxt = i + densityWidth;

        if (i + densityWidth >= n)
            nxt = 0;

        if (InsideTriangle(carPos, i, i + densityWidth - 1, nxt + densityWidth - 1))
            return true;

        if (InsideTriangle(carPos, i, nxt + densityWidth - 1, nxt))
            return true;
    }

    return false;
}

void Tema2::MoveCar(float deltaTime) {
    if (isOnRaceTrack(camera->GetNextTargetPosition(speed * deltaTime))) {
        camera->MoveForward(speed * deltaTime);
    }
    else {
        speed = 0;
    }
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Inc / Dec scaleFactor
    if (window->KeyHold(GLFW_KEY_Z)) {
        scaleFactor -= scaleFactorSpeed * deltaTime;
        cout << scaleFactor << endl;
    }

    if (window->KeyHold(GLFW_KEY_X)) {
        scaleFactor += scaleFactorSpeed * deltaTime;
        cout << scaleFactor << endl;
    }

    // Block car movement if collision with enemy car
    int n = ecIndex.size();
    glm::vec3 carPos = camera->GetTargetPosition();
    for (int i = 0; i < n; i++) {
        glm::vec3 enemyCarPos = rtVertices[ecIndex[i]].position * glm::vec3(4.f) + glm::vec3(0, 0.5f, 0);
        if (sqrt((carPos.x - enemyCarPos.x) * (carPos.x - enemyCarPos.x) + (carPos.y - enemyCarPos.y) * (carPos.y - enemyCarPos.y) + (carPos.z - enemyCarPos.z) * (carPos.z - enemyCarPos.z)) <= 0.7f) {
            speed = 0;
            return;
        }
    }

    // Car movement
    if (window->KeyHold(GLFW_KEY_W)) {
        speed = speed < speedMax ? speed + acceleration * deltaTime : speed;
        if (isOnRaceTrack(camera->GetNextTargetPosition(speed * deltaTime))) {
            camera->MoveForward(speed * deltaTime);
        }
        else {
            speed = 0;
        }
    }
    else if (window->KeyHold(GLFW_KEY_S)) {
        speed = speed > -speedMax ? speed - acceleration * deltaTime : speed;
    }
    else if (speedPrev == speed) {
        if (speed < 0)
            speed += acceleration * deltaTime;
        else if (speed > 0)
            speed -= acceleration * deltaTime;
    }

    MoveCar(deltaTime);
 
    if (window->KeyHold(GLFW_KEY_A)) {
        rotCar -= rotSpeed * deltaTime;
        camera->RotateThirdPerson_OY(rotSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        rotCar += rotSpeed * deltaTime;
        camera->RotateThirdPerson_OY(-rotSpeed * deltaTime);
    }
    speedPrev = speed;
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F2)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
        camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
