//
// Created by siemb on 4/17/2024.
//


#include "../headers/Game.h"

Game::Game() {
    b.x = WIDTH/2;
    b.y = 0;
    b.acceleration = 0;
    b.velocityY = 0;
    b.distanceY = 0;
    b.velocityX = BIRD_X_VELOCITY;

    g.timeElapsed = 0;

    score = 0;
    candyScore = 0;

    m.boolSpikes.resize(2);
    // Resize each inner vector to contain 20 elements
    for (auto& innerVec : m.boolSpikes) {
        innerVec.resize(SPIKE_AMOUNT);
    }

    genSpike();
}

int Game::getBirdX() const {
    return b.x;
}

int Game::getBirdY() const {
    return b.y;
}

void Game::updateVelocity() {
    if(g.timeElapsed<1) {
        b.acceleration = G;
        b.velocityY += b.acceleration * g.timeElapsed;
    }
}

void Game::updateBird() {
    if(b.x>=WIDTH-BIRD_TXT_WIDTH/2 || b.x<=BIRD_TXT_WIDTH/2) {
        b.velocityX*=-1;
        genSpike();
        score++;
    }
    b.x+=b.velocityX;
    b.y+=b.distanceY;
}

void Game::jump() {
    b.velocityY = -BIRD_JUMP;
}

bool Game::fpsCalc() {
    f.fpsA = std::chrono::steady_clock::now();
    f.fpsDelta = f.fpsA - f.fpsB;

    if (f.fpsDelta >= std::chrono::duration<double>(1.0 / FPS))
    {
        // FPS count
        // 1 / fpsDelta.count()
        g.timeElapsed = f.fpsDelta.count();
        f.fpsB = f.fpsA;
        return true;
    }
    else
    {
        return false;
    }
}

void Game::calcDistance() {
    if(g.timeElapsed<1) {
        b.distanceY = b.velocityY * g.timeElapsed + 0.5 * b.acceleration * pow(g.timeElapsed, 2);
    }
}

void Game::genSpike() {

    if(!c.visible) {
        genCandy();
    }


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> amount(2, SPIKE_AMOUNT-2);
    std::uniform_int_distribution<> index(0, SPIKE_AMOUNT);


    int number = amount(gen);
    int randomIndex = index(gen);

    //side = 0 - right side spikes, side = 1 - left side spikes
    bool side = b.velocityX >= 0;

    std::fill(m.boolSpikes[side].begin(), m.boolSpikes[side].end(), 0);



    int locked1 = -1;
    for(int i=0;i<number;i++) {
        if(!m.boolSpikes[side][randomIndex]) {
            m.boolSpikes[side][randomIndex] = true;
            if(i==0) {
                if(randomIndex<SPIKE_AMOUNT-3) {
                    locked1 = randomIndex+1;
                } else {
                    locked1 = randomIndex-2;
                }
            }
        } else {
            while(locked1==randomIndex || locked1+1==randomIndex || m.boolSpikes[side][randomIndex]) {
                randomIndex = index(gen);
            }
            m.boolSpikes[side][randomIndex] = true;
        }
    }
}

int Game::getScore() const {
    return score;
}

//there was so much math involved I just skipped it, simplified everything and just left values without any context
//it just works, don't ask any unnecessary questions
bool Game::checkSpike(int index, bool side) {

    double playerC = side ? b.x + BIRD_TXT_WIDTH / 2 : b.x - BIRD_TXT_WIDTH / 2;
    double SpikeC = index*SPIKE_TXT_DIM*SPIKE_TXT_DIM - 24000*side;

    double x = -playerC;
    double y = (playerC * (-SPIKE_TXT_DIM/2) - SpikeC) / double(SPIKE_TXT_DIM);

    if(checkCrossing(side, x, y)) {return true;}

    SpikeC += SPIKE_TXT_DIM*SPIKE_TXT_DIM + side*48000;

    y = (-playerC * (-SPIKE_TXT_DIM/2) - SpikeC) / double(SPIKE_TXT_DIM);

    if(checkCrossing(side, x, y)) {return true;}
    return false;
}

//chat wrote it for me, I still don't honestly know what is happening
//I mean I wrote base conditions but told him to simplify them and boom here is our code that works like a charm
bool Game::checkCrossing(bool side, double interX, double interY) {
    int rightBound = side ? b.x + BIRD_TXT_WIDTH / 2 : b.x - BIRD_TXT_WIDTH / 2;

    if ((side && rightBound >= WIDTH - SPIKE_TXT_DIM) ||
        (!side && rightBound <= SPIKE_TXT_DIM)) {
        if ((-interX <= b.x + BIRD_TXT_WIDTH / 2 && -interX >= b.x - BIRD_TXT_WIDTH / 2) &&
            (-interY <= b.y + BIRD_TXT_HEIGHT / 2 && -interY >= b.y - BIRD_TXT_HEIGHT / 2)) {
            std::cout << "Lines are crossing at point (" << -interX << ", " << -interY << ")." << std::endl;
            return true;
        }
    }
    return false;
}

void Game::checkCollision() {
    for(int i=0;i<2;i++) {
        for(int j=0;j<SPIKE_AMOUNT;j++) {
            if(m.boolSpikes[i][j]) {
                if(checkSpike(j, i)) {
                    std::cout << "kolizja!" << std::endl;
                }
            }
        }
    }
}

Map Game::getMap() {
    return m;
}

void Game::genCandy() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(HEIGHT/6, 5*HEIGHT/6); // middle of the screen

    c.x = WIDTH / 2; // middle of the screen in x-axis
    c.y = dis(gen); // random y-coordinate in the middle of the screen
    c.visible = true; // make the candy visible
}

void Game::resetCandy() {
    c.x = -200;
    c.y = -200;
    c.visible = false; // make the candy invisible
}

int Game::getCandyX() const {
    return c.x;
}

int Game::getCandyY() const {
    return c.y;
}

void Game::checkCandyCollision() {
    if (b.x + BIRD_TXT_WIDTH / 2 >= c.x - CANDY_TXT_DIM / 2 &&
        b.x - BIRD_TXT_WIDTH / 2 <= c.x + CANDY_TXT_DIM / 2 &&
        b.y + BIRD_TXT_HEIGHT / 2 >= c.y - CANDY_TXT_DIM / 2 &&
        b.y - BIRD_TXT_HEIGHT / 2 <= c.y + CANDY_TXT_DIM / 2) {
        candyScore++;
        resetCandy();
    }
}

int Game::getCandyScore() const {
    return candyScore;
}
