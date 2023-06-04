#include <iostream>
#include <raylib.h>

using namespace std;
int player_score = 0;
int cpu_score = 0;
int frames = 0;

class Ball 
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

void Draw() 
{
    DrawCircle(x, y, 20, WHITE);
}
void Move() 
{
    x += speed_x;
    y += speed_y;

    if(y + radius >= GetScreenHeight() || y - radius <= 0)
    {
        speed_y *= -1;
    }
    if (x + radius >= GetScreenWidth() || x - radius <= 0)
    {
        speed_x *= -1;
    }
    if (x + radius >= GetScreenWidth()) //Cpu scores
    {
        cpu_score++;
        Reset();
    }
    if (x - radius <= 0)  //player scores
    {
        player_score++;
        Reset();
    }
}

void Reset()
{
    x = GetScreenWidth()/2;
    y = GetScreenHeight()/2;

    int speed_dir[2] = {-1,1};
    speed_x *= speed_dir[GetRandomValue(0,1)];
    speed_y *= speed_dir[GetRandomValue(0,1)];
}
};

class Paddle 
{
protected:

void BorderLimit()
{
    if(y <= 0)
    {
        y = 0;
    }
    if (y + height >= GetScreenHeight())
    {
        y = GetScreenHeight() - height;
    }
}

public:
    float x,y;
    float width, height;
    int speed;

void Draw() 
{
    DrawRectangle(x, y, width, height, WHITE);
}

void Move()
{
    if (IsKeyDown(KEY_UP))
    {
        y = y - speed;
    }
    if(IsKeyDown(KEY_DOWN))
    {
        y = y + speed;
    }
    BorderLimit();
}
};


class CpuPaddle: public Paddle 
{
    public:
        int noise;
        
        void Move(int ball_y)
        {
            if (y + height/2 > ball_y)
            {   
                y = y - speed + noise;    //bring paddle up to match ball height   
            }
            if (y + height/2 < ball_y)
            {
                y = y + speed + noise;   //bring paddle down to match ball height
            }
            BorderLimit();
        }  
};


int main() 
{
    cout << "Starting the game" << endl;

    const int screen_width = 1000;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    //class objects and inits
    Ball ball;
    Paddle player;
    CpuPaddle cpu;

    cpu.noise = GetRandomValue(-3, 3);

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 10;
    ball.speed_y = 10;

    player.width = 20;
    player.height = 150;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 10;

    cpu.height = 150;
    cpu.width = 20;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 10;

    //Game start window
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
            
            //cout << MeasureText("PRESS ENTER TO START", 60) << endl; for text width (804)
            //cout << MeasureText("AND ESCAPE TO QUIT", 60) << endl; (690)
            DrawText("PRESS ENTER TO START", screen_width/2 - 804/2, screen_height/2-200, 60, WHITE);
            DrawText("AND ESCAPE TO EXIT", screen_width/2 - 690/2, screen_height/2+200, 60, WHITE);
            if (IsKeyDown(KEY_ENTER))
            {
                break;
            }

        cpu.Draw();
        ball.Draw();
        player.Draw();
        //DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*screen_width/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    // Gameplay window
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        
        // Movement
        ball.Move();
        player.Move();
        cpu.Move(ball.y);
        
        // collision check
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
        }

        
        // Drawing;
        ClearBackground(BLACK);
        cpu.Draw();
        ball.Draw();
        player.Draw();
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*screen_width/4 - 20, 20, 80, WHITE);
        
        if (IsKeyDown(KEY_ESCAPE))
        {
            break;
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}