#include <chrono>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 600
#define BIRD_X_VELOCITY 3
#define FPS 144
#define BIRD_JUMP 600

#define BIRD_TXT_WIDTH 50
#define BIRD_TXT_HEIGHT 50

#define CANDY_TXT_DIM 30

#define SPIKE_AMOUNT 8

#define SPIKE_TXT_DIM HEIGHT/SPIKE_AMOUNT

#define GAMEWIDTH 200
#define GAMEHEIGHT 600

#define G 2000