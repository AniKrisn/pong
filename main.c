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

float randomValueInRange(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

Vector2 randomVector2(float minX, float maxX, float minY, float maxY) {
    Vector2 pos;
    pos.x = minX + ((float)rand() / RAND_MAX) * (maxX - minX);
    pos.y = minY + ((float)rand() / RAND_MAX) * (maxY - minY);
    return pos;
}

Vector2 randomStartingVelocity(void) {
    float speed = 300.0f;
    float theta_max = M_PI / 6.0f; // 30 degrees
    float angle;
    if (rand() % 2 == 0) {
        angle = randomValueInRange(-theta_max, theta_max);
    } else {
        angle = randomValueInRange(M_PI - theta_max, M_PI + theta_max);
    }
    Vector2 val;
    val.x = speed * cosf(angle);
    val.y = speed * sinf(angle);
    return val;
}

bool checkPaddleCollision(Ball ball, Paddle paddle) {
    return
        (ball.position.x - ball.radius < paddle.position.x + paddle.width) &&
        (ball.position.x + ball.radius > paddle.position.x) &&
        (ball.position.y > paddle.position.y - paddle.height / 2) &&
        (ball.position.y < paddle.position.y + paddle.height / 2);
}

void reset(GameState *state) {
    state->ball.position = (Vector2){ 800/2, 600/2 };
    state->ball.velocity = randomStartingVelocity();
}

int main(void) {
    // window setup
    InitWindow(800, 600, "first: minimal pong");
    SetTargetFPS(60);

    // init random seed
    srand(time(NULL));

    // declare game state
    GameState state = {
        .ball.position = (Vector2){ 800/2, 600/2 },
        .ball.velocity = (Vector2){ -200.0f, 300.0f },
        .ball.radius = 5,

        .leftPaddle.position = (Vector2){ 5, 600/2 },
        .leftPaddle.width = 10,
        .leftPaddle.height = 75,
        .leftPaddle.speed = 10,

        .rightPaddle.position = (Vector2){ 795, 600/2 },
        .rightPaddle.width = 10,
        .rightPaddle.height = 75,
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

        // paddle bounce
        float paddleEnd = state.leftPaddle.position.y + state.leftPaddle.height;
        if (state.ball.position.x == state.leftPaddle.position.x && (state.ball.position.y >= state.leftPaddle.position.y && state.ball.position.y <= paddleEnd)) {
            state.ball.velocity.x *= -1;
        }

        // pc control
        if (state.ball.position.y > state.rightPaddle.position.y) {
            state.rightPaddle.position.y += state.rightPaddle.speed * dt * 21.0f;
        }
        if (state.ball.position.y < state.rightPaddle.position.y) {
            state.rightPaddle.position.y -= state.rightPaddle.speed * dt * 21.0f;
        }
        state.rightPaddle.position.y = fminf(fmaxf(state.leftPaddle.position.y, 0 + halfH), 600.0f - halfH);

        
        // ball movement
        state.ball.position.x += state.ball.velocity.x * dt;
        state.ball.position.y += state.ball.velocity.y * dt;

        // top/bottom window bounce
        float scaleFactor = 1.2f;
        if (state.ball.position.y > 600) state.ball.velocity.y *= -1 * scaleFactor;
        if (state.ball.position.y < 0) state.ball.velocity.y *= -1 * scaleFactor;

        // paddle bounce
        if (checkPaddleCollision(state.ball, state.leftPaddle)) state.ball.velocity.x *= -1 * scaleFactor; 
        if (checkPaddleCollision(state.ball, state.rightPaddle)) state.ball.velocity.x *= -1 * scaleFactor;

        // goal: score update, ball position & velocity reset
        if (state.ball.position.x < 0) {
            state.score.player1Score += 1;
            reset(&state);
        }

        if (state.ball.position.x > 800) {
            state.score.player2Score += 1;
            reset(&state);
        }

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