#include "raylib.h"

typedef struct {
    Vector2 position;
    int width;
    int height;
    float speed;
} Paddle;
  
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float radius; 
} Ball;
  
typedef struct {
    int player1Score;
    int player2Score;
} Score;
  
typedef struct {
    Ball ball;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Score score;
} GameState;

int main(void) {
    // window setup
    InitWindow(800, 600, "first: minimal pong");
    SetTargetFPS(60);

    // declare game state
    GameState state = {
        .ball.position = (Vector2){ 800/2, 600/2 },
        .ball.velocity = (Vector2){ 200, 150 }, // speed, direction 
        .ball.radius = 5,

        .leftPaddle.position = (Vector2){ 5, 600/2 },
        .leftPaddle.width = 10,
        .leftPaddle.height = 10,
        .leftPaddle.speed = 10,

        .rightPaddle.position = (Vector2){ 795, 600/2 },
        .rightPaddle.width = 10,
        .rightPaddle.height = 10,
        .rightPaddle.speed = 10,

        .score = { 0, 0 }
    };



}