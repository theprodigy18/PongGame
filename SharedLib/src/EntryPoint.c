#include "pch.h"
#include "EntryPoint.h"
#include "Window.h"
#include "Graphics.h"
#include "Shader.h"
#include "MeshBuffer.h"
#include "Mat.h"

#include <math.h>

#include <crtdbg.h>

#define WORLD_WIDTH 20.0f
#define WORLD_HEIGHT 11.25f
#define WORLD_LEFT -10.0f
#define WORLD_RIGHT 10.0f
#define WORLD_TOP 5.625f
#define WORLD_BOTTOM -5.625f

#define PADDLE_CLAMP 5.0f
#define PADDLE_SPEED 15.0f

#define AI_SPEED 12.0f
#define AI_DEADZONE 1.5f

#define BALL_SPEED 10.0f

#define CIRCLE_SEGMENTS 32
#define PI 3.1415926535897932384626433832795f

static Mat4 ballModelMat;
static f32  ballVelocityX = 0.0f;
static f32  ballVelocityY = 0.0f;

static MeshBuffer* numbers        = NULL;
static GLint*      numbersIndices = NULL;

static Mat4 playerScoreUnitMat;
static Mat4 playerScoreTenMat;

static Mat4 aiScoreUnitMat;
static Mat4 aiScoreTenMat;

static void InitializeNumbersMeshBuffer()
{
    // Shared VBO.
    Vertex vertices[] = {
        {.pos = {-0.5f, 1.5f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {0.5f, 1.5f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {0.5f, -1.5f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {-0.5f, -1.5f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {-0.2f, -0.25f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {0.2f, -0.25f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {0.2f, -1.2f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {-0.2f, -1.2f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {-0.2f, 1.2f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {0.2f, 1.2f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {0.2f, 0.25f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {-0.2f, 0.25f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {-0.5f, 0.25f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {0.5f, 0.25f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {-0.5f, -0.25f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
        {.pos = {0.5f, -0.25f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}}};

    GLint indices0[] = {
        0, 8, 9,
        9, 1, 0,
        1, 9, 6,
        6, 2, 1,
        2, 6, 7,
        7, 3, 2,
        3, 7, 8,
        8, 0, 3};
    GLint indices1[] = {
        1, 9, 6,
        6, 2, 1};
    GLint indices2[] = {
        0, 8, 9,
        9, 1, 0,
        1, 9, 10,
        10, 15, 1,
        10, 12, 4,
        4, 15, 10,
        12, 3, 7,
        7, 4, 12,
        7, 3, 2,
        2, 6, 7};
    GLint indices3[] = {
        0, 8, 9,
        9, 1, 0,
        1, 9, 6,
        6, 2, 1,
        7, 3, 2,
        2, 6, 7,
        11, 4, 5,
        5, 10, 11};
    GLint indices4[] = {
        0, 14, 4,
        4, 8, 0,
        11, 4, 5,
        5, 10, 11,
        1, 9, 6,
        6, 2, 1};
    GLint indices5[] = {
        1, 0, 8,
        8, 9, 1,
        0, 14, 11,
        11, 8, 0,
        11, 14, 5,
        5, 13, 11,
        13, 5, 6,
        6, 2, 13,
        2, 6, 7,
        7, 3, 2};
    GLint indices6[] = {
        1, 0, 8,
        8, 9, 1,
        0, 3, 7,
        7, 8, 0,
        2, 6, 7,
        7, 3, 2,
        13, 5, 6,
        6, 2, 13,
        11, 4, 5,
        5, 13, 11};
    GLint indices7[] = {
        0, 8, 9,
        9, 1, 0,
        1, 9, 6,
        6, 2, 1};
    GLint indices8[] = {
        0, 8, 9,
        9, 1, 0,
        1, 9, 6,
        6, 2, 1,
        2, 6, 7,
        7, 3, 2,
        3, 7, 8,
        8, 0, 3,
        11, 4, 5,
        5, 10, 11};
    GLint indices9[] = {
        0, 8, 9,
        9, 1, 0,
        1, 9, 6,
        6, 2, 1,
        2, 6, 7,
        7, 3, 2,
        0, 14, 11,
        11, 8, 0,
        11, 14, 5,
        5, 10, 11};

    MeshBuffer buffer0 = NULL;
    MeshBuffer buffer1 = NULL;
    MeshBuffer buffer2 = NULL;
    MeshBuffer buffer3 = NULL;
    MeshBuffer buffer4 = NULL;
    MeshBuffer buffer5 = NULL;
    MeshBuffer buffer6 = NULL;
    MeshBuffer buffer7 = NULL;
    MeshBuffer buffer8 = NULL;
    MeshBuffer buffer9 = NULL;

    // Bind the first VBO.
    DROP_CreateMeshBuffer(vertices, indices0, ARRAY_COUNT(vertices), ARRAY_COUNT(indices0), &buffer0);
    // Use the shared VBO.
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices1, ARRAY_COUNT(indices1), &buffer1);
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices2, ARRAY_COUNT(indices2), &buffer2);
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices3, ARRAY_COUNT(indices3), &buffer3);
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices4, ARRAY_COUNT(indices4), &buffer4);
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices5, ARRAY_COUNT(indices5), &buffer5);
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices6, ARRAY_COUNT(indices6), &buffer6);
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices7, ARRAY_COUNT(indices7), &buffer7);
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices8, ARRAY_COUNT(indices8), &buffer8);
    DROP_CreateMeshBufferWithSharedVBO(buffer0->vbo, indices9, ARRAY_COUNT(indices9), &buffer9);

    if (!buffer0 || !buffer1 || !buffer2 || !buffer3 || !buffer4 || !buffer5 || !buffer6 || !buffer7 || !buffer8 || !buffer9)
    {
        ASSERT_MSG(false, "Failed to create mesh buffers.");
        return;
    }

    numbers = ALLOC_ARRAY(MeshBuffer, 10);
    ASSERT(numbers);
    numbers[0] = buffer0;
    numbers[1] = buffer1;
    numbers[2] = buffer2;
    numbers[3] = buffer3;
    numbers[4] = buffer4;
    numbers[5] = buffer5;
    numbers[6] = buffer6;
    numbers[7] = buffer7;
    numbers[8] = buffer8;
    numbers[9] = buffer9;

    numbersIndices = ALLOC_ARRAY(GLint, 10);
    ASSERT(numbersIndices);
    numbersIndices[0] = ARRAY_COUNT(indices0);
    numbersIndices[1] = ARRAY_COUNT(indices1);
    numbersIndices[2] = ARRAY_COUNT(indices2);
    numbersIndices[3] = ARRAY_COUNT(indices3);
    numbersIndices[4] = ARRAY_COUNT(indices4);
    numbersIndices[5] = ARRAY_COUNT(indices5);
    numbersIndices[6] = ARRAY_COUNT(indices6);
    numbersIndices[7] = ARRAY_COUNT(indices7);
    numbersIndices[8] = ARRAY_COUNT(indices8);
    numbersIndices[9] = ARRAY_COUNT(indices9);
}

static void DestroyNumbersMeshBuffer()
{
    for (i32 i = 0; i < 9; ++i)
    {
        DROP_DestroyMeshBuffer(&numbers[i], true);
    }

    DROP_DestroyMeshBuffer(&numbers[9], false);

    FREE(numbers);
    numbers = NULL;

    FREE(numbersIndices);
    numbersIndices = NULL;
}

static void PrintMatrix(Mat4* mat, const char* name)
{
    const f32* m = (const f32*) mat;
    printf("%s:\n", name);
    printf("%f %f %f %f\n", m[0], m[4], m[8], m[12]);
    printf("%f %f %f %f\n", m[1], m[5], m[9], m[13]);
    printf("%f %f %f %f\n", m[2], m[6], m[10], m[14]);
    printf("%f %f %f %f\n", m[3], m[7], m[11], m[15]);
    printf("\n");
}

static f64 GetCurrentTimeMs()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (f64) (counter.QuadPart * 1000.0) / frequency.QuadPart;
}

static void InitializeBallDir()
{
    ballModelMat.m[3].x = 0.0f;
    ballModelMat.m[3].y = 0.0f;

    f32 ballDirX = (rand() % 2 == 0) ? 1.0f : -1.0f;
    f32 ballDirY = ((f32) rand() / RAND_MAX - 0.5f) * 2.0f * 0.5f;

    ballVelocityX = ballDirX * BALL_SPEED;
    ballVelocityY = ballDirY * BALL_SPEED;
}

static void DrawPlayerScore(i32 score, GLint modelLoc)
{
    i32 ten  = score / 10;
    i32 unit = score % 10;

    glBindVertexArray(numbers[ten]->vao);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &playerScoreTenMat);
    glDrawElements(GL_TRIANGLES, numbersIndices[ten], GL_UNSIGNED_INT, 0);

    glBindVertexArray(numbers[unit]->vao);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &playerScoreUnitMat);
    glDrawElements(GL_TRIANGLES, numbersIndices[unit], GL_UNSIGNED_INT, 0);
};

static void DrawAIScore(i32 score, GLint modelLoc)
{
    i32 ten  = score / 10;
    i32 unit = score % 10;

    glBindVertexArray(numbers[ten]->vao);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &aiScoreTenMat);
    glDrawElements(GL_TRIANGLES, numbersIndices[ten], GL_UNSIGNED_INT, 0);

    glBindVertexArray(numbers[unit]->vao);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &aiScoreUnitMat);
    glDrawElements(GL_TRIANGLES, numbersIndices[unit], GL_UNSIGNED_INT, 0);
};

int EntryPoint()
{
    SetProcessDPIAware();

    WndInitProps wndProps =
        {
            .title  = L"Pong Game",
            .width  = 1280,
            .height = 720};

    WndHandle wndHandle = NULL;

    if (!DROP_CreateWindow(&wndProps, &wndHandle) || !wndHandle)
    {
        LOG_ERROR("Failed to create window.");
        return -1;
    }

    GfxInitProps gfxProps =
        {
            .wndHandle = wndHandle};

    GfxHandle gfxHandle = NULL;
    if (!DROP_CreateGraphics(&gfxProps, &gfxHandle) || !gfxHandle)
    {
        LOG_ERROR("Failed to create graphics.");
        DROP_DestroyWindow(&wndHandle);
        return -1;
    }

    GLuint basicShader = DROP_LoadShaders("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    if (!basicShader)
    {
        LOG_ERROR("Failed to load shaders.");
        DROP_DestroyGraphics(&gfxHandle);
        DROP_DestroyWindow(&wndHandle);
        return -1;
    }

    Vertex vertices[] =
        {
            {.pos = {0.5f, -0.5f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
            {.pos = {0.5f, 0.5f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
            {.pos = {-0.5f, 0.5f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}},
            {.pos = {-0.5f, -0.5f, 0.0f}, .color = {1.0f, 1.0f, 1.0f}}};

    GLint indices[] = {0, 1, 2, 2, 3, 0};

    MeshBuffer meshBuffer = NULL;
    if (!DROP_CreateMeshBuffer(vertices, indices, ARRAY_COUNT(vertices), ARRAY_COUNT(indices), &meshBuffer) || !meshBuffer)
    {
        LOG_ERROR("Failed to create mesh buffer.");
        DROP_DestroyShader(basicShader);
        DROP_DestroyGraphics(&gfxHandle);
        DROP_DestroyWindow(&wndHandle);
        return -1;
    }

    Vertex ballVertices[CIRCLE_SEGMENTS + 1];
    GLint  ballIndices[CIRCLE_SEGMENTS * 3];

    f32 angle = 2.0f * PI / CIRCLE_SEGMENTS;

    for (i32 i = 0; i <= CIRCLE_SEGMENTS; ++i)
    {
        f32 rad               = i * angle;
        ballVertices[i].pos.x = cosf(rad) * 0.5f;
        ballVertices[i].pos.y = sinf(rad) * 0.5f;
        ballVertices[i].pos.z = 0.0f;

        ballVertices[i].color.x = 1.0f;
        ballVertices[i].color.y = 0.0f;
        ballVertices[i].color.z = 0.0f;
    }

    for (i32 i = 0; i < CIRCLE_SEGMENTS; ++i)
    {
        ballIndices[i * 3 + 0] = 0;
        ballIndices[i * 3 + 1] = i + 1;
        ballIndices[i * 3 + 2] = i + 2;
    }

    MeshBuffer ballBuffer = NULL;
    if (!DROP_CreateMeshBuffer(ballVertices, ballIndices, ARRAY_COUNT(ballVertices), ARRAY_COUNT(ballIndices), &ballBuffer) || !ballBuffer)
    {
        LOG_ERROR("Failed to create mesh buffer.");
        DROP_DestroyMeshBuffer(&meshBuffer, false);
        DROP_DestroyShader(basicShader);
        DROP_DestroyGraphics(&gfxHandle);
        DROP_DestroyWindow(&wndHandle);
        return -1;
    }

    glUseProgram(basicShader);

    GLint basicProjectionLoc = glGetUniformLocation(basicShader, "projection");
    GLint basicModelLoc      = glGetUniformLocation(basicShader, "model");
    if (basicProjectionLoc == -1 || basicModelLoc == -1)
    {
        LOG_ERROR("Failed to get uniform locations.");
        DROP_DestroyMeshBuffer(&meshBuffer, false);
        DROP_DestroyShader(basicShader);
        DROP_DestroyGraphics(&gfxHandle);
        DROP_DestroyWindow(&wndHandle);
        return -1;
    }

    Mat4 projectionMat;
    projectionMat.m[0] = Vec4_Create(2.0f / WORLD_WIDTH, 0.0f, 0.0f, 0.0f);
    projectionMat.m[1] = Vec4_Create(0.0f, 2.0f / WORLD_HEIGHT, 0.0f, 0.0f);
    projectionMat.m[2] = Vec4_Create(0.0f, 0.0f, -1.0f, 0.0f);
    projectionMat.m[3] = Vec4_Create(0.0f, 0.0f, 0.0f, 1.0f);

    Mat4 projT = Mat4_Transpose(&projectionMat);
    glUniformMatrix4fv(basicProjectionLoc, 1, GL_FALSE, (const float*) &projT);

    Vec3 pos      = {-9.5f, 0.0f, 0.0f};
    Vec3 scale    = {0.5f, 3.0f, 1.0f};
    Mat4 posMat   = Mat4_Translate(&pos);
    Mat4 scaleMat = Mat4_Scale(&scale);
    Mat4 modelMat = Mat4_MultiplyMat4(&scaleMat, &posMat);
    PrintMatrix(&posMat, "posMat");
    PrintMatrix(&scaleMat, "scaleMat");
    PrintMatrix(&modelMat, "modelMat");

    Vec3 enemyPos      = {9.5f, 0.0f, 0.0f};
    Mat4 enemyPosMat   = Mat4_Translate(&enemyPos);
    Mat4 enemyModelMat = Mat4_MultiplyMat4(&scaleMat, &enemyPosMat);
    PrintMatrix(&enemyPosMat, "enemyPosMat");
    PrintMatrix(&enemyModelMat, "enemyModelMat");

    Vec3 ballPos      = {0.0f, 0.0f, 0.0f};
    Vec3 ballScale    = {0.5f, 0.5f, 1.0f};
    Mat4 ballPosMat   = Mat4_Translate(&ballPos);
    Mat4 ballScaleMat = Mat4_Scale(&ballScale);
    ballModelMat      = Mat4_MultiplyMat4(&ballScaleMat, &ballPosMat);
    PrintMatrix(&ballPosMat, "ballPosMat");
    PrintMatrix(&ballScaleMat, "ballScaleMat");
    PrintMatrix(&ballModelMat, "ballModelMat");

    Mat4 identity = Mat4_Identity();

    Vec3 numberScale    = {1.25f, 1.0f, 1.0f};
    Mat4 numberScaleMat = Mat4_Scale(&numberScale);

    Vec3 playerScoreTenPos  = {-7.0f, 4.0f, 0.0f};
    Vec3 playerScoreUnitPos = {-5.0f, 4.0f, 0.0f};
    Vec3 aiScoreTenPos      = {5.0f, 4.0f, 0.0f};
    Vec3 aiScoreUnitPos     = {7.0f, 4.0f, 0.0f};

    playerScoreUnitMat = Mat4_Translate(&playerScoreUnitPos);
    playerScoreTenMat  = Mat4_Translate(&playerScoreTenPos);
    aiScoreUnitMat     = Mat4_Translate(&aiScoreUnitPos);
    aiScoreTenMat      = Mat4_Translate(&aiScoreTenPos);

    playerScoreUnitMat = Mat4_MultiplyMat4(&numberScaleMat, &playerScoreUnitMat);
    playerScoreTenMat  = Mat4_MultiplyMat4(&numberScaleMat, &playerScoreTenMat);
    aiScoreUnitMat     = Mat4_MultiplyMat4(&numberScaleMat, &aiScoreUnitMat);
    aiScoreTenMat      = Mat4_MultiplyMat4(&numberScaleMat, &aiScoreTenMat);

    Vertex rectVertices[] =
        {
            {.pos = {0.5f, -0.5f, 0.0f}, .color = {0.0f, 0.0f, 0.0f}},
            {.pos = {0.5f, 0.5f, 0.0f}, .color = {0.0f, 0.0f, 0.0f}},
            {.pos = {-0.5f, 0.5f, 0.0f}, .color = {0.0f, 0.0f, 0.0f}},
            {.pos = {-0.5f, -0.5f, 0.0f}, .color = {0.0f, 0.0f, 0.0f}}};

    GLint      rectIndices[] = {0, 1, 2, 2, 3, 0};
    MeshBuffer rectBuffer    = NULL;
    DROP_CreateMeshBuffer(rectVertices, rectIndices, ARRAY_COUNT(rectVertices), ARRAY_COUNT(rectIndices), &rectBuffer);

    Vec3 rectPos      = {0.0f, 4.2f, 0.0f};
    Vec3 rectScale    = {20.0f, 3.0f, 1.0f};
    Mat4 rectScaleMat = Mat4_Scale(&rectScale);
    Mat4 rectPosMat   = Mat4_Translate(&rectPos);
    rectPosMat        = Mat4_MultiplyMat4(&rectScaleMat, &rectPosMat);

    // Random ball direction.
    srand(time(NULL));
    InitializeBallDir();

    // Initialize all UI meshes.
    InitializeNumbersMeshBuffer();

    glEnable(GL_FRAMEBUFFER_SRGB);
    glViewport(0, 0, 1280, 720);
    ShowWindow(wndHandle->hwnd, SW_SHOW);

    static f64 lastTime    = 0;
    f64        currentTime = GetCurrentTimeMs(); // Initialize first time
    lastTime               = currentTime;

    // Scoring.
    i32 playerScore = 0;
    i32 aiScore     = 0;

    f32 speedMultiplier = 1.05f;
    f32 maxSpeed        = BALL_SPEED * 3.0f;

    f32 paddleHalfHeight = 1.5f;
    f32 ballRadius       = 0.25f;

    bool running = true;
    while (running)
    {
        // Delta time calculations.
        currentTime   = GetCurrentTimeMs();
        f32 deltaTime = (f32) (currentTime - lastTime) / 1000.0f;

        running = DROP_PollEvents();

        // Player movement.
        if (GetKeyState('W') & 0x8000)
        {
            modelMat.m[3].y += PADDLE_SPEED * deltaTime;
            if (modelMat.m[3].y > PADDLE_CLAMP)
                modelMat.m[3].y = PADDLE_CLAMP;
        }
        if (GetKeyState('S') & 0x8000)
        {
            modelMat.m[3].y -= PADDLE_SPEED * deltaTime;
            if (modelMat.m[3].y < -PADDLE_CLAMP)
                modelMat.m[3].y = -PADDLE_CLAMP;
        }

        // AI movement.
        f32 currentY     = enemyModelMat.m[3].y;
        f32 currentBallY = ballModelMat.m[3].y;

        if (ballVelocityX > 0.0f)
        {
            f32 targetY = currentBallY;

            // Add error.
            f32 aiError = ((f32) rand() / RAND_MAX - 0.5f) * 2.0f * 0.35f;
            targetY += aiError;

            if (targetY > currentY + AI_DEADZONE)
                enemyModelMat.m[3].y += AI_SPEED * deltaTime;
            if (targetY < currentY - AI_DEADZONE)
                enemyModelMat.m[3].y -= AI_SPEED * deltaTime;
        }

        if (currentY > PADDLE_CLAMP)
            enemyModelMat.m[3].y = PADDLE_CLAMP;
        if (currentY < -PADDLE_CLAMP)
            enemyModelMat.m[3].y = -PADDLE_CLAMP;

        // Ball movement.
        ballModelMat.m[3].x += ballVelocityX * deltaTime;
        ballModelMat.m[3].y += ballVelocityY * deltaTime;

        // Ball colission with top and bottom wall.
        if (ballModelMat.m[3].y > WORLD_TOP - 0.25f)
        {
            ballModelMat.m[3].y = WORLD_TOP - 0.25f;
            ballVelocityY       = -ballVelocityY;
        }

        if (ballModelMat.m[3].y < WORLD_BOTTOM + 0.25f)
        {
            ballModelMat.m[3].y = WORLD_BOTTOM + 0.25f;
            ballVelocityY       = -ballVelocityY;
        }

        // Ball collision with paddle.
        // Player paddle.
        if (ballModelMat.m[3].x <= -9.0f && ballVelocityX < 0.0f)
        {
            if (ballModelMat.m[3].y + ballRadius >= modelMat.m[3].y - paddleHalfHeight &&
                ballModelMat.m[3].y - ballRadius <= modelMat.m[3].y + paddleHalfHeight)
            {
                f32 hitPos = (ballModelMat.m[3].y - modelMat.m[3].y) / 1.5f;

                ballVelocityX       = -ballVelocityX;
                ballVelocityY       = hitPos * BALL_SPEED * 0.8f;
                ballModelMat.m[3].x = -9.0f;

                ballVelocityX *= speedMultiplier;
                ballVelocityY *= speedMultiplier;
                if (fabs(ballVelocityX) > maxSpeed)
                    ballVelocityX = (ballVelocityX > 0.0f) ? maxSpeed : -maxSpeed;
                if (fabs(ballVelocityY) > maxSpeed)
                    ballVelocityY = (ballVelocityY > 0.0f) ? maxSpeed : -maxSpeed;
            }
        }
        // AI paddle.
        if (ballModelMat.m[3].x >= 9.0f && ballVelocityX > 0.0f)
        {
            if (ballModelMat.m[3].y + ballRadius >= enemyModelMat.m[3].y - paddleHalfHeight &&
                ballModelMat.m[3].y - ballRadius <= enemyModelMat.m[3].y + paddleHalfHeight)
            {
                f32 hitPos = (ballModelMat.m[3].y - enemyModelMat.m[3].y) / 1.5f;

                ballVelocityX       = -ballVelocityX;
                ballVelocityY       = hitPos * BALL_SPEED * 0.8f;
                ballModelMat.m[3].x = 9.0f;

                ballVelocityX *= speedMultiplier;
                ballVelocityY *= speedMultiplier;
                if (fabs(ballVelocityX) > maxSpeed)
                    ballVelocityX = (ballVelocityX > 0.0f) ? maxSpeed : -maxSpeed;
                if (fabs(ballVelocityY) > maxSpeed)
                    ballVelocityY = (ballVelocityY > 0.0f) ? maxSpeed : -maxSpeed;
            }
        }

        // Check for score.
        if (ballModelMat.m[3].x < WORLD_LEFT)
        {
            ++aiScore;
            LOG_TRACE("AI score: %d, Player score: %d", aiScore, playerScore);
            InitializeBallDir();
        }

        if (ballModelMat.m[3].x > WORLD_RIGHT)
        {
            ++playerScore;
            LOG_TRACE("AI score: %d, Player score: %d", aiScore, playerScore);
            InitializeBallDir();
        }

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(meshBuffer->vao);
        glUniformMatrix4fv(basicModelLoc, 1, GL_FALSE, (const float*) &modelMat);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUniformMatrix4fv(basicModelLoc, 1, GL_FALSE, (const float*) &enemyModelMat);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(ballBuffer->vao);
        glUniformMatrix4fv(basicModelLoc, 1, GL_FALSE, (const float*) &ballModelMat);
        glDrawElements(GL_TRIANGLES, CIRCLE_SEGMENTS * 3, GL_UNSIGNED_INT, 0);

        // Test draw UI.
        glBindVertexArray(rectBuffer->vao);
        glUniformMatrix4fv(basicModelLoc, 1, GL_FALSE, (const float*) &rectPosMat);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        DrawPlayerScore(playerScore, basicModelLoc);
        DrawAIScore(aiScore, basicModelLoc);

        SwapBuffers(gfxHandle->hdc);

        lastTime = currentTime;
    }

    ShowWindow(wndHandle->hwnd, SW_HIDE);

    // Destroy all objects.
    DROP_DestroyMeshBuffer(&meshBuffer, false);
    DROP_DestroyMeshBuffer(&ballBuffer, false);

    DestroyNumbersMeshBuffer();

    DROP_DestroyShader(basicShader);

    DROP_DestroyGraphics(&gfxHandle);
    DROP_DestroyWindow(&wndHandle);

    _CrtDumpMemoryLeaks();

    return 0;
}