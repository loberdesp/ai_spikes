//
// Created by siemb on 4/17/2024.
//



#include "../headers/display.h"
#include "../headers/define.h"
#include "../headers/Game.h"


display::display(SDL_Renderer &rend) : r(rend) {
    birdTxt = IMG_LoadTexture(&r,"../assets/bird.jpg");
    spikeTxt = IMG_LoadTexture(&r,"../assets/img.png");

    gFont = TTF_OpenFont("../assets/font.ttf", 60); // Change the path to your font file
    if (gFont == nullptr) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        exit(0);
    }
}

void display::drawBird(int x, int y, int score) {

    SDL_Rect bRect;
    bRect.x = x-BIRD_TXT_WIDTH/2;
    bRect.y = y-BIRD_TXT_HEIGHT/2;
    bRect.w = BIRD_TXT_WIDTH;
    bRect.h = BIRD_TXT_HEIGHT;
    //SDL_RenderCopy(&r, birdTxt, nullptr, &bRect);

    SDL_RenderDrawLine(&r, x - BIRD_TXT_WIDTH / 2, y + BIRD_TXT_WIDTH / 2, x + BIRD_TXT_WIDTH / 2, y + BIRD_TXT_WIDTH / 2);
    SDL_RenderDrawLine(&r, x - BIRD_TXT_WIDTH / 2, y - BIRD_TXT_WIDTH / 2, x + BIRD_TXT_WIDTH / 2, y - BIRD_TXT_WIDTH / 2);
    SDL_RenderDrawLine(&r, x - BIRD_TXT_WIDTH / 2, y - BIRD_TXT_WIDTH / 2, x - BIRD_TXT_WIDTH / 2, y + BIRD_TXT_WIDTH / 2);
    SDL_RenderDrawLine(&r, x + BIRD_TXT_WIDTH / 2, y - BIRD_TXT_WIDTH / 2, x + BIRD_TXT_WIDTH / 2, y + BIRD_TXT_WIDTH / 2);

}

void display::drawScores(int s, int cs) {
    std::ostringstream stream;
    stream << s;
    renderText(stream.str(), 2*WIDTH/5, HEIGHT/8, false);
    std::ostringstream candyStream;
    candyStream << cs;
    renderText(candyStream.str(), 3*WIDTH/5, HEIGHT/8, true);
}



void display::drawSpikes(Map m) {
    SDL_SetRenderDrawColor(&r, 0, 0, 255, 255);
    for(int i=0;i<2;i++) {
        for(int j=0;j<SPIKE_AMOUNT;j++) {
            if(m.boolSpikes[i][j]) {
                if(i) {
                    SDL_RenderDrawLine(&r, WIDTH, (j*SPIKE_TXT_DIM), WIDTH-SPIKE_TXT_DIM, j*SPIKE_TXT_DIM+SPIKE_TXT_DIM/2);
                    SDL_RenderDrawLine(&r, WIDTH, SPIKE_TXT_DIM*(j+1), WIDTH-SPIKE_TXT_DIM, j*SPIKE_TXT_DIM+SPIKE_TXT_DIM/2);
                } else {
                    SDL_RenderDrawLine(&r, 0, (j*SPIKE_TXT_DIM), SPIKE_TXT_DIM, j*SPIKE_TXT_DIM+SPIKE_TXT_DIM/2);
                    SDL_RenderDrawLine(&r, 0, SPIKE_TXT_DIM*(j+1), SPIKE_TXT_DIM, j*SPIKE_TXT_DIM+SPIKE_TXT_DIM/2);
                }
            }
        }
    }
}

void display::renderText(const std::string& text, int x, int y, bool isRed) {
    SDL_Color textColor = {255, 255, 255}; // black color
    if(isRed) {
        textColor = {255, 0, 0}; // red color
    } else {
        textColor = {255, 255, 255}; // white color
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
    if (textSurface == nullptr) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(&r, textSurface);
        if (texture == nullptr) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            SDL_Rect renderQuad = {x-textSurface->w/2, y-textSurface->h/2, textSurface->w, textSurface->h};
            SDL_RenderCopy(&r, texture, nullptr, &renderQuad);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(textSurface);
    }
}

void display::drawCandy(int x, int y) {
    SDL_Rect cRect;
    cRect.x = x-CANDY_TXT_DIM/2;
    cRect.y = y-CANDY_TXT_DIM/2;
    cRect.w = CANDY_TXT_DIM;
    cRect.h = CANDY_TXT_DIM;
    SDL_SetRenderDrawColor(&r, 255, 0, 0, 255);
    SDL_RenderFillRect(&r, &cRect);
}
