//
// Created by siemb on 4/17/2024.
//

#ifndef AI_TOUCHES_SPIKES_GAME_H
#define AI_TOUCHES_SPIKES_GAME_H
#include "../headers/define.h"
#include "net.h"

struct State {
    float yVelocity, yPosition, xPosition;
};

struct Data {
    State oldState, newState;
    bool action;
    float reward;

};

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
    bool alive;

    Bird() : x(WIDTH/2), y(HEIGHT/2), acceleration(0), velocityY(0), velocityX(BIRD_X_VELOCITY), distanceY(0), alive(true) {
    }
    int getBirdX() const;
    int getBirdY() const;
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
    std::vector<bool> newSpike;
};

class Game {
    std::vector<Bird> birds;
    Candy c;
    Gravity g;
    FrameRate f;
    Map m;
    int score;
    int candyScore;
public:
    Game();
    int getYvelocity(Bird &b) const;
    void updateVelocity(Bird &b);
    void updateBird(Bird &b);
    void calcDistance(Bird &b);
    void jump(Bird &b);
    void genSpike();
    Map getMap();
    int getScore() const;
    void toggleAlive(Bird &b);
    bool isAlive(Bird &b);
    bool isGoingRight(Bird &b) const;


    void genCandy();
    void resetCandy();
    int getCandyScore() const;
    int getCandyX() const;
    int getCandyY() const;
    void checkCandyCollision(Bird &b);

    bool fpsCalc();

    bool checkSpike(int index, bool side, Bird &b);
    bool checkCrossing(bool side, double interX, double interY, Bird &b);
    void checkCollision(Bird &b);

    bool isPlayerOutOfBounds(Bird &b) const;

    std::string getSpikeBinary(Bird &b);

    void restart(Bird &b);




    double simUpdate(net &n, bool& alive, Bird &b);

    void setSpike(Bird &b);
    bool SIMcheckSpike(int index, bool side, float newX, float newY);
    bool SIMcheckCrossing(bool side, double interX, double interY, float newX, float newY);
    std::vector<Bird>& getBirds();
};


#endif //AI_TOUCHES_SPIKES_GAME_H
