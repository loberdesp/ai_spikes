#include <chrono>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <bitset>

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

#define G 2000

#define LEARNING_RATE 0.1f
#define CLIPPED_RATIO 0.9f
#define LEARNING_RATE_AI 0.3f
#define DISCOUNT_FACTOR 1