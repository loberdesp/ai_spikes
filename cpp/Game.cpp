//
// Created by siemb on 4/17/2024.
//


#include "../headers/Game.h"


Game::Game() {


    for(int i=0;i<1;i++) {
        Bird t;
        birds.push_back(t);
    }
    //Bird t;
    //birds.push_back(t);

    g.timeElapsed = 0;

    score = 0;
    candyScore = 0;

    m.boolSpikes.resize(2);
    // Resize each inner vector to contain 20 elements
    for (auto& innerVec : m.boolSpikes) {
        innerVec.resize(SPIKE_AMOUNT);
    }
    m.newSpike.resize(SPIKE_AMOUNT);

    //genSpike();
    //setSpike();
}

void Game::updateVelocity(Bird &b) {
    if(g.timeElapsed<1) {
        b.acceleration = G;
        b.velocityY += b.acceleration * g.timeElapsed;
    }
}

void Game::updateBird(Bird &b) {
    b.x+=b.velocityX;
    b.y+=b.distanceY;
    if(b.x>=WIDTH-BIRD_TXT_WIDTH/2 || b.x<=BIRD_TXT_WIDTH/2) {
        b.velocityX*=-1;
        //setSpike();
        score++;
    }
}

void Game::jump(Bird &b) {
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

void Game::calcDistance(Bird &b) {
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
    std::uniform_int_distribution<> amount(1, SPIKE_AMOUNT-4);
    std::uniform_int_distribution<> index(0, SPIKE_AMOUNT);


    int number = amount(gen);
    int randomIndex = index(gen);

    //side = 0 - right side spikes, side = 1 - left side spikes


    std::fill(m.newSpike.begin(), m.newSpike.end(), 0);


//    int locked1 = -1;
//    for(int i=0;i<number;i++) {
//        if(!m.boolSpikes[side][randomIndex]) {
//            m.boolSpikes[side][randomIndex] = true;
//            if(i==0) {
//                if(randomIndex<SPIKE_AMOUNT-3) {
//                    locked1 = randomIndex+1;
//                } else {
//                    locked1 = randomIndex-2;
//                }
//            }
//        } else {
//            while(locked1==randomIndex || locked1+1==randomIndex || m.boolSpikes[side][randomIndex]) {
//                randomIndex = index(gen);
//            }
//            m.boolSpikes[side][randomIndex] = true;
//        }
//    }

    for(int i=0;i<number;i++) {
        if(!m.newSpike[randomIndex]) {
            m.newSpike[randomIndex] = true;
        } else {
            while(m.newSpike[randomIndex]) {
                randomIndex = index(gen);
            }
            m.newSpike[randomIndex] = true;
        }
    }
}

int Game::getScore() const {
    return score;
}


bool Game::isPlayerOutOfBounds(Bird &b) const {

    // Check if the player's y-coordinate is within the window's height
    if (b.y < 0 || b.y > HEIGHT) {
        return true;
    }

    // If the player's coordinates are within the window's dimensions, return true
    return false;
}

//there was so much math involved I just skipped it, simplified everything and just left values without any context
//it just works, don't ask any unnecessary questions
bool Game::checkSpike(int index, bool side, Bird &b) {



    double playerC = side ? b.x + BIRD_TXT_WIDTH / 2 : b.x - BIRD_TXT_WIDTH / 2;
    double SpikeC = index*SPIKE_TXT_DIM*SPIKE_TXT_DIM - 24000*side;

    double x = -playerC;
    double y = (playerC * (-SPIKE_TXT_DIM/2) - SpikeC) / double(SPIKE_TXT_DIM);

    if(checkCrossing(side, x, y, b)) {return true;}

    SpikeC += SPIKE_TXT_DIM*SPIKE_TXT_DIM + side*48000;

    y = (-playerC * (-SPIKE_TXT_DIM/2) - SpikeC) / double(SPIKE_TXT_DIM);

    if(checkCrossing(side, x, y, b)) {return true;}
    return false;
}

//chat wrote it for me, I still don't honestly know what is happening
//I mean I wrote base conditions but told him to simplify them and boom here is our code that works like a charm
bool Game::checkCrossing(bool side, double interX, double interY, Bird &b) {
    int rightBound = side ? b.x + BIRD_TXT_WIDTH / 2 : b.x - BIRD_TXT_WIDTH / 2;

    if ((side && rightBound >= WIDTH - SPIKE_TXT_DIM) ||
        (!side && rightBound <= SPIKE_TXT_DIM)) {
        if ((-interX <= b.x + BIRD_TXT_WIDTH / 2 && -interX >= b.x - BIRD_TXT_WIDTH / 2) &&
            (-interY <= b.y + BIRD_TXT_HEIGHT / 2 && -interY >= b.y - BIRD_TXT_HEIGHT / 2)) {
            return true;
        }
    }
    return false;
}







bool Game::SIMcheckSpike(int index, bool side, float newX, float newY) {



    double playerC = side ? newX + BIRD_TXT_WIDTH / 2 : newX - BIRD_TXT_WIDTH / 2;
    double SpikeC = index*SPIKE_TXT_DIM*SPIKE_TXT_DIM - 24000*side;

    double x = -playerC;
    double y = (playerC * (-SPIKE_TXT_DIM/2) - SpikeC) / double(SPIKE_TXT_DIM);

    if(SIMcheckCrossing(side, x, y, newX, newY)) {return true;}

    SpikeC += SPIKE_TXT_DIM*SPIKE_TXT_DIM + side*48000;

    y = (-playerC * (-SPIKE_TXT_DIM/2) - SpikeC) / double(SPIKE_TXT_DIM);

    if(SIMcheckCrossing(side, x, y, newX, newY)) {return true;}
    return false;
}

//chat wrote it for me, I still don't honestly know what is happening
//I mean I wrote base conditions but told him to simplify them and boom here is our code that works like a charm
bool Game::SIMcheckCrossing(bool side, double interX, double interY, float newX, float newY) {
    int rightBound = side ? newX + BIRD_TXT_WIDTH / 2 : newX - BIRD_TXT_WIDTH / 2;

    if ((side && rightBound >= WIDTH - SPIKE_TXT_DIM) ||
        (!side && rightBound <= SPIKE_TXT_DIM)) {
        if ((-interX <= newX + BIRD_TXT_WIDTH / 2 && -interX >= newX - BIRD_TXT_WIDTH / 2) &&
            (-interY <= newY + BIRD_TXT_HEIGHT / 2 && -interY >= newY - BIRD_TXT_HEIGHT / 2)) {
            return true;
        }
    }
    return false;
}
















void Game::checkCollision(Bird &b) {
    if(isPlayerOutOfBounds(b)) {
        //std::cout << "out of bounds" << std::endl;
        b.alive = false;
    }
    for(int i=0;i<2;i++) {
        for(int j=0;j<SPIKE_AMOUNT;j++) {
            if(m.boolSpikes[i][j]) {
                if(checkSpike(j, i, b)) {
                    b.alive = false;
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

void Game::checkCandyCollision(Bird &b) {
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

std::string Game::getSpikeBinary(Bird &b) {
    std::string s;
    for(auto x : m.boolSpikes[b.velocityX>0 ? 1 : 0]) {
        s+=x ? "1" : "0";
    }
    return s;
}

void Game::toggleAlive(Bird &b) {
    b.alive = !b.alive;
}

bool Game::isAlive(Bird &b) {
    return b.alive;
}

int Game::getYvelocity(Bird &b) const {
    return b.velocityY;
}

void Game::restart(Bird &b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(BIRD_TXT_WIDTH / 2, WIDTH - BIRD_TXT_WIDTH / 2); // bird's x-coordinate
    std::uniform_int_distribution<> disY(BIRD_TXT_HEIGHT / 2, HEIGHT - BIRD_TXT_HEIGHT / 2); // bird's y-coordinate
    std::uniform_int_distribution<> disVelocity(0, 1); // bird's velocity

    b.alive = true;
    b.x = WIDTH/2;//disX(gen); // random x-coordinate within the window
    b.y = HEIGHT/2;//disY(gen)HEIGHT/2; // random y-coordinate within the window
    b.acceleration = 0;
    b.velocityY = 0;
    b.distanceY = 0;
    b.velocityX = BIRD_X_VELOCITY; //* (disVelocity(gen) == 0 ? -1 : 1);

    g.timeElapsed = 0;

    score = 0;
    candyScore = 0;

    for(auto x : m.boolSpikes) {
        std::fill(x.begin(), x.end(), 0);
    }

    //g.genSpike();
    //g.setSpike(b);
}

bool Game::isGoingRight(Bird &b) const {
    if (b.velocityX > 0) {
        return true;
    } else {
        return false;
    }
}

double Game::simUpdate(net &n, bool& alive, Bird &b) {
    //simulate no jump and simulate next position and then pass it to network to calculate q factor
    float noJumpYDistance = 0;
    float noJumpYVelocity = 0;
    float newXvelocity = b.velocityX;
    int newScore = 0;
    bool noJumpAlive = true;
    std::vector<std::vector<bool>> spikes = m.boolSpikes;


    if(g.timeElapsed<1) {
        noJumpYVelocity = b.velocityY + b.acceleration * g.timeElapsed;
        noJumpYDistance = noJumpYVelocity * g.timeElapsed + 0.5 * b.acceleration * pow(g.timeElapsed, 2);
    }
// doing it without candy for simplicity, just for now

    float noJumpX = b.x + newXvelocity;
    float noJumpY = b.y + noJumpYDistance;

//we only do this shit once cause we have no influence and it will change similarly in both jump and no jump cases
    if(noJumpX>=WIDTH-BIRD_TXT_WIDTH/2 || noJumpX<=BIRD_TXT_WIDTH/2) {
        newXvelocity*=-1;
        //spikes[newXvelocity>0] = m.newSpike;
        newScore = score+1;
    }


    if (noJumpY < 0 || noJumpY > HEIGHT) {
        noJumpAlive = false;
    }

    for(int i=0;i<2;i++) {
        for(int j=0;j<SPIKE_AMOUNT;j++) {
            if(spikes[i][j]) {
                if(SIMcheckSpike(j, i, noJumpX, noJumpY)) {
                    noJumpAlive = false;
                }
            }
        }
    }
    //now we have future state after no jump, we can calculate q factor
    //std::cout << "njump velocity: " << noJumpYVelocity << " newY: " << noJumpY << " alive: " << noJumpAlive << std::endl;


    double noJumpQ = n.forwardShow(spikes[newXvelocity>0], float(noJumpYVelocity+600)/2100, float(noJumpY)/HEIGHT, float(noJumpX)/WIDTH)[0];


    //we simulate jump now

    float JumpYDistance = 0;
    float JumpYVelocity = -BIRD_JUMP;
    bool JumpAlive = true;


    JumpYDistance = 0;
    JumpYVelocity = -600;

    if(g.timeElapsed<1) {
        JumpYVelocity = JumpYVelocity + b.acceleration * g.timeElapsed;
        JumpYDistance = JumpYVelocity * g.timeElapsed + 0.5 * b.acceleration * pow(g.timeElapsed, 2);
    }

    float JumpX = b.x + newXvelocity;
    float JumpY = b.y + JumpYDistance;

    if (JumpY < 0 || JumpY > HEIGHT) {
        JumpAlive = false;
    }

    for(int i=0;i<2;i++) {
        for(int j=0;j<SPIKE_AMOUNT;j++) {
            if(spikes[i][j]) {
                if(SIMcheckSpike(j, i, JumpX, JumpY)) {
                    JumpAlive = false;
                }
            }
        }
    }
    //std::cout << " jump velocity: " << JumpYVelocity << " newY: " << JumpY << " alive: " << JumpAlive << std::endl;
    double JumpQ = n.forwardShow(spikes[newXvelocity>0], float(JumpYVelocity+600)/2100, float(JumpY)/HEIGHT, float(JumpX)/WIDTH)[1];


    alive = JumpQ>noJumpQ ? JumpAlive : noJumpAlive;

    return std::max(JumpQ, noJumpQ);
}

void Game::setSpike(Bird &b) {
    bool side = b.velocityX >= 0;
    m.boolSpikes[side] = m.newSpike;
}

std::vector<Bird>& Game::getBirds() {
    return birds;
}

int Bird::getBirdX() const{
    return x;
}

int Bird::getBirdY() const {
    return y;
}