//
// Created by siemb on 4/17/2024.
//

#ifndef AI_TOUCHES_SPIKES_GAME_H
#define AI_TOUCHES_SPIKES_GAME_H
#include "../headers/define.h"

struct Candy {
    float x, y;
    bool visible;
    Candy() : x(-200), y(-200), visible(false) {}
};

struct Bird {
    float x, y;
    float acceleration;
    float velocityY;
    float velocityX;
    float distanceY;
};

struct FrameRate {
    std::chrono::steady_clock::time_point fpsA, fpsB;
    std::chrono::duration<double> fpsDelta;
};

struct Gravity {
    std::chrono::steady_clock frameBegin;
    std::chrono::steady_clock frameEnd;
    float timeElapsed;
};

struct Map {
    std::vector<std::vector<bool>> boolSpikes;
};

class Game {
    Bird b;
    Candy c;
    Gravity g;
    FrameRate f;
    Map m;
    int score;
    int candyScore;
public:
    Game();
    int getBirdX() const;
    int getBirdY() const;
    void updateVelocity();
    void updateBird();
    void calcDistance();
    void jump();
    void genSpike();
    Map getMap();
    int getScore() const;


    void genCandy();
    void resetCandy();
    int getCandyScore() const;
    int getCandyX() const;
    int getCandyY() const;
    void checkCandyCollision();

    bool fpsCalc();

    bool checkSpike(int index, bool side);
    bool checkCrossing(bool side, double interX, double interY);
    void checkCollision();
};


#endif //AI_TOUCHES_SPIKES_GAME_H
