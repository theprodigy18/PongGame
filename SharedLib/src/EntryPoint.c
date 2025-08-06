#include "pch.h"
#include "EntryPoint.h"
#include "Window.h"
#include "Graphics.h"
#include "Shader.h"
#include "MeshBuffer.h"
#include "Mat.h"

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

    glBindVertexArray(meshBuffer->vao);
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

    const i32 GAME_WIDTH  = 1280;
    const i32 GAME_HEIGHT = 720;

    Mat4 projectionMat;
    projectionMat.m[0] = Vec4_Create(2.0f / GAME_WIDTH, 0.0f, 0.0f, 0.0f);
    projectionMat.m[1] = Vec4_Create(0.0f, 2.0f / GAME_HEIGHT, 0.0f, 0.0f);
    projectionMat.m[2] = Vec4_Create(0.0f, 0.0f, -1.0f, 0.0f);
    projectionMat.m[3] = Vec4_Create(0.0f, 0.0f, 0.0f, 1.0f);

	Mat4 projT = Mat4_Transpose(&projectionMat);

	glUniformMatrix4fv(basicProjectionLoc, 1, GL_TRUE, (const float*) &projT);

	Mat4 modelMat = Mat4_Identity();
	Mat4 modelT   = Mat4_Transpose(&modelMat);
	glUniformMatrix4fv(basicModelLoc, 1, GL_TRUE, (const float*) &modelT);

    glEnable(GL_FRAMEBUFFER_SRGB);
    glViewport(0, 0, 1280, 720);
    ShowWindow(wndHandle->hwnd, SW_SHOW);

    bool running = true;
    while (running)
    {
        running = DROP_PollEvents();
        glClearColor(0.1f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SwapBuffers(gfxHandle->hdc);
    }

    ShowWindow(wndHandle->hwnd, SW_HIDE);

    DROP_DestroyShader(basicShader);

    DROP_DestroyGraphics(&gfxHandle);
    DROP_DestroyWindow(&wndHandle);

    return 0;
}