#include <iostream>
#include "headers/define.h"
#include "headers/display.h"
#include "headers/Game.h"

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }


    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT,
                                          0);

    if(!window)
    {
        std::cout << "Failed to create window\n";
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    display d(*renderer);
    Game g;


    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        return -1;
    }



    bool quit = false;
    SDL_Event e;

    // Main loop
    while (!quit) {
        if(g.fpsCalc()) {
            g.updateVelocity();

            // Handle events on queue
            while (SDL_PollEvent(&e) != 0) {
                // User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if(e.button.button == SDL_BUTTON_LEFT) {
                        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
                        int mouseX = e.button.x;
                        int mouseY = e.button.y;
                    }
                } else if(e.type == SDL_KEYDOWN) {
                    if(e.key.keysym.sym == SDLK_SPACE) {
                        g.jump();
                    }
                }
            }
            g.calcDistance();
            g.updateBird();
            g.checkCandyCollision();

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            g.checkCollision();
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);


            // Clear the screen (black)

            d.drawSpikes(g.getMap());
            d.drawScores(g.getScore(), g.getCandyScore());
            d.drawBird(g.getBirdX(), g.getBirdY(), g.getScore());
            d.drawCandy(g.getCandyX(), g.getCandyY());

            // Update screen
            SDL_RenderPresent(renderer);
        }
    }

    // Free resources and close SDL
    SDL_FreeSurface(window_surface);

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}