#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
        .ball.velocity = (Vector2){ 200.0f, 150.0f }, // speed, direction 
        .ball.radius = 5,

        .leftPaddle.position = (Vector2){ 5, 600/2 },
        .leftPaddle.width = 10,
        .leftPaddle.height = 50,
        .leftPaddle.speed = 10,

        .rightPaddle.position = (Vector2){ 795, 600/2 },
        .rightPaddle.width = 10,
        .rightPaddle.height = 50,
        .rightPaddle.speed = 10,

        .score = { 0, 0 }
    };

    while(!WindowShouldClose()) {
        float dt = GetFrameTime(); // time (sec) since last frame
        
        // movement
        // note: speed (pix/sec) * dt = pixel delta
        if (IsKeyDown(KEY_W)) state.leftPaddle.position.y -= state.leftPaddle.speed * dt * 75.0f;
        if (IsKeyDown(KEY_S)) state.leftPaddle.position.y += state.leftPaddle.speed * dt * 75.0f;
        
        // clamp paddle
        float halfH = state.leftPaddle.height * 0.5f;
        state.leftPaddle.position.y = fminf(fmaxf(state.leftPaddle.position.y, 0 + halfH), 600.0f - halfH);
        
        // ball movement
        state.ball.position.x += state.ball.velocity.x * dt;
        state.ball.position.y += state.ball.velocity.y * dt;

        // window bounce
        float scaleFactor = 1.2f;
        if (state.ball.position.x > 800) state.ball.velocity.x *= -1 * scaleFactor;
        if (state.ball.position.y > 600) state.ball.velocity.y *= -1 * scaleFactor;
        if (state.ball.position.x < 0) state.ball.velocity.x *= -1 * scaleFactor;
        if (state.ball.position.y < 0) state.ball.velocity.y *= -1 * scaleFactor;

        BeginDrawing();
            ClearBackground(GRAY);

            DrawCircleV(state.ball.position, state.ball.radius, GREEN); 

            DrawRectangleV(
                (Vector2){
                    state.leftPaddle.position.x,
                    state.leftPaddle.position.y - state.leftPaddle.height / 2
                },

                (Vector2){
                    state.leftPaddle.width, state.leftPaddle.height
                },
                GREEN
            );

            DrawRectangleV(
                (Vector2){
                    state.rightPaddle.position.x - state.rightPaddle.width,
                    state.rightPaddle.position.y - state.rightPaddle.height / 2
                },

                (Vector2){
                    state.leftPaddle.width, state.leftPaddle.height
                },
                GREEN
            );


            DrawText(TextFormat("%i", state.score.player1Score), 350, 20, 20, GREEN);
            DrawText(TextFormat("%i", state.score.player2Score), 450, 20, 20, GREEN);

        EndDrawing();

    }

    CloseWindow();

    return 0;

}