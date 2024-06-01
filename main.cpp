#include <iostream>
#include "headers/define.h"
#include "headers/display.h"
#include "headers/Game.h"
#include "headers/neuron.h"
#include "headers/net.h"







    int main(int argc, char* argv[])
{

    net qNetwork({3,7,2});
    net targetNetwork = qNetwork;

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
                                          WIDTH*2, HEIGHT,
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
    float epsilon = 0.1;
    bool action = false;
    float distance = 0;
    State oldState, newState;
    std::vector<Data> data;


    // Main loop
    while (!quit) {
        if(g.fpsCalc()) {
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
                        //g.jump();
                    } else if(e.key.keysym.sym == SDLK_KP_ENTER) {
                    }
                }
            }

            //define state 1
            oldState = {float((g.getYvelocity(g.getBirds()[0])+BIRD_JUMP))/(1500+BIRD_JUMP),float(g.getBirds()[0].getBirdY())/HEIGHT,float(g.getBirds()[0].getBirdX())/WIDTH};

            //calculate and pick best action
            qNetwork.forwardTrain(float((g.getYvelocity(g.getBirds()[0])+600))/2100, float(g.getBirds()[0].getBirdY())/HEIGHT, float(g.getBirds()[0].getBirdX())/WIDTH, action);

            //take action
            if(action) {
                g.jump(g.getBirds()[0]);
            }

            //update environment
            g.updateVelocity(g.getBirds()[0]);
            g.calcDistance(g.getBirds()[0]);
            g.updateBird(g.getBirds()[0]);
            g.checkCandyCollision(g.getBirds()[0]);
            g.checkCollision(g.getBirds()[0]);

            //reward (log for not overloading weights when good run happened)
            float reward = -0.01;
            distance += BIRD_X_VELOCITY;
            reward += log(distance/3600+1)/5;
            if(!g.getBirds()[0].alive) {
                reward += -1;
            }

            newState = {float((g.getYvelocity(g.getBirds()[0])+BIRD_JUMP))/(1500+BIRD_JUMP),float(g.getBirds()[0].getBirdY())/HEIGHT,float(g.getBirds()[0].getBirdX())/WIDTH};
            //pass to data vector for learning
            data.push_back({oldState,newState,action,reward});

            //if bird is dead learn
            if(!g.getBirds()[0].alive) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, 10);
                double avgE = 0;

                for(int i=0;i<data.size();i++) {
                    bool newAction = false;

                    double oldQ = qNetwork.getQ(data[i].oldState.yVelocity,data[i].oldState.yPosition,data[i].oldState.xPosition, data[i].action);
                    double newQ = targetNetwork.forwardTrain(data[i].newState.yVelocity,data[i].newState.yPosition,data[i].newState.xPosition,newAction);
                    newQ+=data[i].reward;
                    double error = newQ-oldQ;
                    avgE+=error;
                    qNetwork.backProp(error, data[i].action);
                }
                if(dis(gen) > 8) {
                    targetNetwork = qNetwork;
                }
                data.clear();
                //std::cout << "Bird traveled " <<  distance << " units and received bonus " << log(distance/3600+1)/5 << " reward!" << std::endl;
                distance = 0;
                g.restart(g.getBirds()[0]);
            }



                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                // Clear the screen (black)

                d.drawSpikes(g.getMap());
                d.drawScores(g.getScore(), g.getCandyScore());
                d.drawBird(g.getBirds()[0].getBirdX(), g.getBirds()[0].getBirdY(), g.getScore());
                d.drawCandy(g.getCandyX(), g.getCandyY());

                // Draw net
                d.drawNet(qNetwork.getNet(), qNetwork.getWeights());

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