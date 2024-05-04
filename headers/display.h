//
// Created by siemb on 4/17/2024.
//

#ifndef AI_TOUCHES_SPIKES_DISPLAY_H
#define AI_TOUCHES_SPIKES_DISPLAY_H
#include <SDL.h>
#include <SDL_image.h>
#include "../headers/define.h"
#include "Game.h"

class display {
    SDL_Texture* birdTxt;
    SDL_Texture* spikeTxt;
    SDL_Renderer& r;
    TTF_Font *gFont = nullptr;
public:
    display(SDL_Renderer &rend);
    void drawBird(int x, int y, int score);
    void drawSpikes(Map m);
    void renderText(const std::string& text, int x, int y, bool isRed);
    void drawCandy(int x, int y);
    void drawScores(int s, int cs);
};


#endif //AI_TOUCHES_SPIKES_DISPLAY_H
