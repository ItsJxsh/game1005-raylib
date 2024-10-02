#include "raylib.h"
#include "Math.h"

constexpr float SCREEN_WIDTH = 1200.0f;
constexpr float SCREEN_HEIGHT = 800.0f;
constexpr Vector2 CENTER{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };

// Ball can move half the screen width per-second
constexpr float BALL_SPEED = SCREEN_WIDTH * 0.5f;
constexpr float BALL_SIZE = 40.0f;

// Paddles can move half the screen height per-second
constexpr float PADDLE_SPEED = SCREEN_HEIGHT * 0.5f;
constexpr float PADDLE_WIDTH = 40.0f;
constexpr float PADDLE_HEIGHT = 160.0f;


int p1Score = 0;
int p2Score = 0;

//save an array of colours, which are selected at random to switch the colour of the ball whenever it collides with something.
int i = 0;
Color colors[14] = { WHITE, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK,
                         YELLOW, GREEN, SKYBLUE, PURPLE, ORANGE };


struct Box
{
    float xMin;
    float xMax;
    float yMin;
    float yMax;
};

bool BoxOverlap(Box box1, Box box2)
{
    bool x = box1.xMax >= box2.xMin && box1.xMin <= box2.xMax;
    bool y = box1.yMax >= box2.yMin && box1.yMin <= box2.yMax;
    return x && y;
}

Rectangle BoxToRec(Box box)
{
    Rectangle rec;
    rec.x = box.xMin;
    rec.y = box.yMin;
    rec.width = box.xMax - box.xMin;
    rec.height = box.yMax - box.yMin;
    return rec;
}

Box BallBox(Vector2 position)
{
    Box box;
    box.xMin = position.x - BALL_SIZE * 0.5f;
    box.xMax = position.x + BALL_SIZE * 0.5f;
    box.yMin = position.y - BALL_SIZE * 0.5f;
    box.yMax = position.y + BALL_SIZE * 0.5f;
    return box;
}

Box PaddleBox(Vector2 position)
{
    Box box;
    box.xMin = position.x - PADDLE_WIDTH * 0.5f;
    box.xMax = position.x + PADDLE_WIDTH * 0.5f;
    box.yMin = position.y - PADDLE_HEIGHT * 0.5f;
    box.yMax = position.y + PADDLE_HEIGHT * 0.5f;
    return box;
}

void ResetBall(Vector2& position, Vector2& direction)
{
    position = CENTER;
    direction.x = rand() % 2 == 0 ? -1.0f : 1.0f;
    direction.y = 0.0f;
    direction = Rotate(direction, Random(-60.0f, 60.0f) * DEG2RAD);
}

void DrawBall(Vector2 position, Color color)
{
    Box ballBox = BallBox(position);
    DrawRectangleRec(BoxToRec(ballBox), color);
}

void DrawPaddle(Vector2 position, Color color)

{
    Box paddleBox = PaddleBox(position);
    DrawRectangleRec(BoxToRec(paddleBox), color);
}

int main()
{
    InitAudioDevice();
    Sound Wall = LoadSound("src/WallBounce.mp3");
    Sound Paddle = LoadSound("src/PaddleBounce.mp3");
    Sound Score = LoadSound("src/Score.mp3");


    Vector2 ballPosition;
    Vector2 ballDirection;
    ResetBall(ballPosition, ballDirection);

    Vector2 paddle1Position, paddle2Position;
    paddle1Position.x = SCREEN_WIDTH * 0.05f;
    paddle2Position.x = SCREEN_WIDTH * 0.95f;
    paddle1Position.y = paddle2Position.y = CENTER.y;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        float ballDelta = BALL_SPEED * dt;
        float paddleDelta = PADDLE_SPEED * dt;

        // Move paddle with key input
        if (IsKeyDown(KEY_W))
            paddle1Position.y -= paddleDelta;
        if (IsKeyDown(KEY_S))
            paddle1Position.y += paddleDelta;

        float phh = PADDLE_HEIGHT * 0.5f;
        paddle1Position.y = Clamp(paddle1Position.y, phh, SCREEN_HEIGHT - phh);
        paddle2Position.y = Clamp(paddle2Position.y, phh, SCREEN_HEIGHT - phh);

        // Mirror paddle 1 for now
        if (IsKeyDown(KEY_UP))
            paddle2Position.y -= paddleDelta;
        if (IsKeyDown(KEY_DOWN))
            paddle2Position.y += paddleDelta;

        DrawText(TextFormat("%01i", p1Score), 200, 80, 200, RED);
        DrawText(TextFormat("%01i", p2Score), 900, 80, 200, BLUE);

        if (p1Score == 5) 
        {
            DrawText("Player 1 Wins!", 250, 500, 100, RED);
            ballDelta = 0;
        }
        else if (p2Score == 5)
        {
            DrawText("Player 2 Wins!", 250, 500, 100, BLUE);
            ballDelta = 0;
        }

        // Change the ball's direction on-collision
        Vector2 ballPositionNext = ballPosition + ballDirection * ballDelta;
        Box ballBox = BallBox(ballPositionNext);
        Box paddle1Box = PaddleBox(paddle1Position);
        Box paddle2Box = PaddleBox(paddle2Position);

        if (ballBox.xMin < 0.0f)
        {
            ResetBall(ballPosition, ballDirection);
            p2Score++;
            i = 0;
            PlaySound(Score);
        }

        if (ballBox.xMax > SCREEN_WIDTH)
        {
            ResetBall(ballPosition, ballDirection);
            p1Score++;
            i = 0;
            PlaySound(Score);
        }

        if (ballBox.yMin < 0.0f || ballBox.yMax > SCREEN_HEIGHT)
        {
            i = Random(0, 13);
            ballDirection.y *= -1.0f;
            PlaySound(Wall);
        }
        if (BoxOverlap(ballBox, paddle1Box) || BoxOverlap(ballBox, paddle2Box)) 
        {
            i = Random(0, 13);
            ballDirection.x *= -1.0f;
            PlaySound(Paddle);
        }

        // Update ball position after collision resolution, then render
        ballPosition = ballPosition + ballDirection * ballDelta;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawBall(ballPosition, colors[i]);
        DrawPaddle(paddle1Position, WHITE);
        DrawPaddle(paddle2Position, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
