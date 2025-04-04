/**
 * @file main.cpp
 * @author Bilal Kahraman (kahramannbilal@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <cstdlib>
#include <rasterizer.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <chrono>
#include <frame_buffer.hpp>
#include <memory>
#include <random>
#include <vector3.hpp>

int main(int argc, char *argv[])
{

    if (argc > 1)
    {
        SDL_Log("Usage: %s", argv[0]);
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    uint32_t width = 640;
    uint32_t height = 480;

    SDL_Window *window = SDL_CreateWindow("Hello World", width, height, SDL_WINDOW_MAXIMIZED);

    if (!window)
    {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create a frame buffer
    auto frameBuffer = std::make_unique<cam3d::FrameBuffer>(width, height);
    auto rasterizer = std::make_unique<cam3d::Rasterizer>(width, height);
    // SDL Texture
    SDL_Texture *texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    // generate random color
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    cam3d::ARGB color;
    cam3d::ARGB color2(dis(gen), dis(gen), dis(gen), 255); // Random pixel
    cam3d::ARGB color3(255, 255, 255, 255);                // Random pixel

    std::vector<cam3d::Vector3<float>> points{
        cam3d::Vector3<float>(0.3f, -0.7f, 1.0f),  cam3d::Vector3<float>(1.0f, 1.0f, 0.0f),
        cam3d::Vector3<float>(-1.0f, -1.0f, 1.0f), cam3d::Vector3<float>(0.7f, -0.5f, 0.0f),
        cam3d::Vector3<float>(0.7f, 0.4f, 1.0f),   cam3d::Vector3<float>(0.5f, 0.5f, 0.0f),
        cam3d::Vector3<float>(0.2f, 0.2f, 0.0f),
    };

    std::vector<cam3d::Vector3<float>> pointsv2{
        cam3d::Vector3<float>(150, 38, 43),
        cam3d::Vector3<float>(352, 400, 150),
        cam3d::Vector3<float>(28, 389, 17),
    };

    std::vector<cam3d::Vector3<float>> test_triangle{
        cam3d::Vector3<float>(-1.0f, -0.5f, 0.5f), // Vertex 1
        cam3d::Vector3<float>(0.5f, 1.0f, 0.5f),   // Vertex 2
        cam3d::Vector3<float>(1.0f, -0.5f, 0.9f) // Vertex 3
    };

    cam3d::Vector3<float> start(-50.0f, 120.0f, 0.0f);
    cam3d::Vector3<float> end(900.0f, 200.0f, 0.0f);

    bool running = true;
    SDL_Event event;
    std::chrono::high_resolution_clock::time_point tp;

    tp = std::chrono::high_resolution_clock::now();
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        frameBuffer->clear(color);

        // // Draw triangle with perspective projection
        rasterizer->drawTriangle(rasterizer->projectBasicPerspective(test_triangle[0]),
                                 rasterizer->projectBasicPerspective(test_triangle[1]),
                                 rasterizer->projectBasicPerspective(test_triangle[2]), *frameBuffer, color3);

        SDL_UpdateTexture(texture, NULL, frameBuffer->getBuffer().data(), width * sizeof(cam3d::ARGB));
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}