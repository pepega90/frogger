// #include <bits/stdc++.h>
#include <raylib.h>
// #include <bits/stdc++.h>
// gunakan package dibawah untuk build
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

int tileSize = 50;
const int WIDTH = 840;
const int HEIGHT = 720;

enum Scene
{
    MENU,
    PLAY
};

struct Player
{
    Vector2 pos;
    Texture2D img;
    Rectangle frameRect;
    Rectangle rect;

    int live = 3;

    int upFrame = 0;
    int upCounter = 0;
    int upSpeed = 10;

    int rightFrame = 6;
    int rightCounter = 0;
    int rightSpeed = 10;

    int leftFrame = 4;
    int leftCounter = 0;
    int leftSpeed = 10;

    int downFrame = 2;
    int downCounter = 0;
    int downSpeed = 10;

    bool up = false;
    bool right = false;
    bool left = false;
    bool down = false;

    bool kenaLog = false;
    bool onFirstLog = false;

    Player(float x, float y, Texture2D img)
    {
        this->pos = Vector2(x, y);
        this->img = img;
    }

    void Animation()
    {
        if (up)
        {
            upCounter++;
            if (upCounter >= (60 / upSpeed))
            {
                upCounter = 0;
                upFrame++;

                if (upFrame > 1)
                {
                    upFrame = 0;
                    up = false;
                }

                frameRect.x = (float)upFrame * (float)img.width / 8;
            }
        }

        if (right)
        {
            rightCounter++;
            if (rightCounter >= (60 / rightSpeed))
            {
                rightCounter = 0;
                rightFrame++;

                if (rightFrame > 7)
                {
                    rightFrame = 6;
                    right = false;
                }

                frameRect.x = (float)rightFrame * (float)img.width / 8;
            }
        }

        if (left)
        {
            leftCounter++;
            if (leftCounter >= (60 / leftSpeed))
            {
                leftCounter = 0;
                leftFrame++;

                if (leftFrame > 5)
                {
                    leftFrame = 4;
                    left = false;
                }

                frameRect.x = (float)leftFrame * (float)img.width / 8;
            }
        }

        if (down)
        {
            downCounter++;
            if (downCounter >= (60 / downSpeed))
            {
                downCounter = 0;
                downFrame++;

                if (downFrame > 3)
                {
                    downFrame = 2;
                    down = false;
                }

                frameRect.x = (float)downFrame * (float)img.width / 8;
            }
        }
    }

    void Draw()
    {
        for (int i = 0; i < live; i++)
        {
            Vector2 posisi = Vector2(i * 50, 640);
            DrawTextureRec(img, Rectangle{0, 0, frameRect.width, frameRect.height}, posisi, WHITE);
        }
        DrawTextureRec(img, Rectangle{frameRect.x, frameRect.y, frameRect.width, frameRect.height}, pos, WHITE);
        // DrawRectangleLinesEx(rect, 2, YELLOW);
    }

    void ResetPosition()
    {
        pos = Vector2(365, 585);
        onFirstLog = false;
    }

    void Update()
    {
        if (IsKeyPressed(KEY_UP))
        {
            pos.y -= kenaLog ? 80 : tileSize;
            up = true;
        }
        if (IsKeyPressed(KEY_DOWN) && pos.y < 550)
        {
            pos.y += (!onFirstLog && kenaLog) ? 80 : tileSize;
            ;
            down = true;
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            pos.x += tileSize;
            right = true;
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            pos.x -= tileSize;
            left = true;
        }

        rect.width = 20;
        rect.height = 20;
        rect.x = pos.x + rect.width;
        rect.y = pos.y + rect.height;

        Animation();
        Draw();
    }

    ~Player() = default;
};

struct Car
{
    Vector2 pos;
    Texture2D img;
    Rectangle frameRect;
    Rectangle rect;

    int speed = 2;
    bool flip = false;
    bool die = false;

    // int car1 = 0;
    // int car2 = 1;
    // int car3 = 2;
    // int car4 = 3;
    int id = 0;

    // Car(float x, float y, Texture2D img, int id)
    // {
    //     this->pos = Vector2(x, y);
    //     this->img = img;
    //     this->id = id;
    // }

    void Draw()
    {
        frameRect.x = (float)id * (float)img.width / 4 - 5;
        DrawTextureRec(img, Rectangle{frameRect.x, frameRect.y, flip ? -frameRect.width : frameRect.width, frameRect.height}, pos, WHITE);
        // DrawRectangleLinesEx(rect, 2, RED);
    }

    void Update()
    {
        pos.x += flip ? -speed : speed;
        rect.x = pos.x;
        rect.y = pos.y;
        rect.width = 60;
        rect.height = 60;
        Draw();
    }
};

struct Log
{
    Vector2 pos;
    Texture2D img;
    Rectangle rect;
    int speed = 2;
    bool die = false;
    int id = 0;

    Log(float x, float y, Texture2D img, int id)
    {
        this->pos = Vector2(x, y);
        this->img = img;
        this->id = id;
    }

    void Draw()
    {
        DrawTextureV(img, pos, WHITE);
        // DrawRectangleLinesEx(rect, 2, RED);
    }

    void Update()
    {
        pos.x += speed;
        rect.x = pos.x;
        rect.y = pos.y;
        rect.width = img.width;
        rect.height = img.height;
        Draw();
    }

    ~Log() = default;
};

struct Finish
{
    Rectangle rect;
    Texture2D img;
    bool show = false;

    void Draw()
    {
        if (show)
        {
            DrawTexture(img, rect.x, rect.y, WHITE);
        }
        // else
        // {
        //     DrawRectangleRec(rect, YELLOW);
        // }
    }
};

int main()
{
    InitWindow(WIDTH, HEIGHT, "Frogger");
    SetTargetFPS(60);

    // load font
    Font f = LoadFont("./assets/Pixellari.ttf");

    Texture2D bg = LoadTexture("./assets/background.png");
    bg.width = 840;
    bg.height = 640;

    Texture2D carSprite = LoadTexture("./assets/cars.png");
    Car b1;
    b1.pos = Vector2(GetScreenWidth(), 530);
    b1.img = carSprite;
    b1.id = 3;
    b1.frameRect = {0, 0, (float)carSprite.width / 4, (float)carSprite.height};

    Car b2;
    b2.pos = Vector2(0, 466);
    b2.img = carSprite;
    b2.id = 2;
    b2.frameRect = {0, 0, (float)carSprite.width / 4, (float)carSprite.height};

    Car b3;
    b3.pos = Vector2(GetScreenWidth(), 400);
    b3.img = carSprite;
    b3.id = 1;
    b3.frameRect = {0, 0, (float)carSprite.width / 4, (float)carSprite.height};

    Car b4;
    b4.pos = Vector2(0, 339);
    b4.img = carSprite;
    b4.id = 0;
    b4.frameRect = {0, 0, (float)carSprite.width / 4, (float)carSprite.height};

    Texture2D playerSprite = LoadTexture("./assets/frog.png");
    Player player = Player(365, 585, playerSprite);
    player.frameRect = {0, 0, (float)playerSprite.width / 8, (float)playerSprite.height};

    int listPosition[4] = {339, 400, 466, 530};
    int cars[4] = {0, 1, 2, 3};
    int rand = GetRandomValue(0, 3);
    std::vector<Car> obs;
    obs.push_back(b1);
    obs.push_back(b2);
    obs.push_back(b3);
    obs.push_back(b4);

    // log
    Texture2D log1Image = LoadTexture("./assets/log1.png");
    Texture2D log2Image = LoadTexture("./assets/log2.png");
    Texture2D log3Image = LoadTexture("./assets/log3.png");
    Log l1 = Log(0, 236, log3Image, 3);
    Log l2 = Log(GetScreenWidth(), 155, log2Image, 2);
    Log l3 = Log(0, 93, log1Image, 1);
    std::vector<Log> logs;
    logs.push_back(l1);
    logs.push_back(l2);
    logs.push_back(l3);
    int logsId[2] = {2, 3};
    int randLog = GetRandomValue(0, 1);

    Texture2D finishImage = LoadTexture("./assets/goal.png");
    std::vector<Finish> finish;

    for (int i = 0; i < 5; i++)
    {
        Finish f;
        f.rect.x = 27 + i * 180;
        f.rect.y = 20;
        f.rect.width = 60;
        f.rect.height = 40;
        f.img = finishImage;
        finish.push_back(f);
    }

    std::vector<Rectangle> loseRect;
    for (int i = 0; i < 5; i++)
    {
        Rectangle f;
        f.x = 105 + i * 180;
        f.y = 10;
        f.width = 80;
        f.height = 40;
        loseRect.push_back(f);
    }

    // countdown untuk car
    float lastUpdate = GetTime();
    int countDown = 3;

    // countdown untuk log/kayu
    float lastUpdateLog = GetTime();
    int countDownLog = 2;

    float lastUpdateLog1 = GetTime();
    int countDownLog1 = 3;

    // other
    int finishCount = 0;
    int score = 0;
    bool dead = false;
    Scene currentScene = Scene::MENU;

    float lastShow = GetTime();
    int showCount = 2;

    while (!WindowShouldClose())
    {

        ClearBackground(BLACK);
        BeginDrawing();
        switch (currentScene)
        {
        case MENU:
        {
            if (GetTime() - lastShow > 0.3 && showCount > 0)
            {
                showCount -= 1;
                lastShow = GetTime();
            }

            if (showCount == 2)
                DrawTextEx(f, "FROGGER", Vector2(100, 150), 100, 40.0, GREEN);

            if (showCount == 0)
                showCount = 2;

            DrawTextEx(f, "Tekan \"SPACE\" untuk start!", Vector2(250, GetScreenHeight() / 2), 30, 0.0, WHITE);
            DrawTextEx(f, "created by aji mustofa @pepega90", Vector2(5, 665), 20, 0.0, WHITE);
            if (IsKeyPressed(KEY_SPACE))
                currentScene = Scene::PLAY;
        }
        break;
        case PLAY:
        {
            DrawTexture(bg, 0, 0, WHITE);

            // countdown untuk car
            if (GetTime() - lastUpdate > 0.5 && countDown > 0)
            {
                countDown -= 1;
                lastUpdate = GetTime();
            }

            if (countDown == 0)
            {
                Car b;
                rand = GetRandomValue(0, 3);
                b.img = carSprite;
                b.id = cars[rand];
                if (b.id == 3 || b.id == 1)
                {
                    b.pos.x = GetScreenWidth();
                    b.flip = true;
                }
                else
                {
                    b.pos.x = -80;
                }
                b.pos.y = listPosition[rand];
                b.die = false;

                b.frameRect = {0, 0, (float)carSprite.width / 4, (float)carSprite.height};
                obs.push_back(b);
                countDown = 3;
            }
            // check jika nyawa player habis
            if (player.live < 1)
            {
                DrawTextEx(f, "Game Over", Vector2(GetScreenWidth() / 2 - 120, 345), 50, 0.0, RED);
                DrawTextEx(f, "Tekan \"R\" untuk restart!", Vector2(GetScreenWidth() / 2 - 190, 420), 35, 0.0, WHITE);
                dead = true;
            }
            else if (finishCount == 5)
            {
                DrawTextEx(f, "Thanks for playing!", Vector2(GetScreenWidth() / 2 - 220, 345), 50, 0.0, GREEN);
                DrawTextEx(f, "Tekan \"R\" untuk restart!", Vector2(GetScreenWidth() / 2 - 190, 420), 35, 0.0, WHITE);
                dead = true;
            }

            if (dead && IsKeyPressed(KEY_R))
            {
                finishCount = 0;
                dead = false;
                player.live = 3;
                score = 0;
                finish.clear();
                for (int i = 0; i < 5; i++)
                {
                    Finish f;
                    f.rect.x = 27 + i * 180;
                    f.rect.y = 20;
                    f.rect.width = 60;
                    f.rect.height = 40;
                    f.show = false;
                    f.img = finishImage;
                    finish.push_back(f);
                }
            }

            for (int i = 0; i < obs.size(); i++)
            {
                obs[i].Update();

                if (obs[i].pos.x > WIDTH || obs[i].pos.x < -80)
                    obs[i].die = true;
            }

            auto removeObs = std::remove_if(
                obs.begin(), obs.end(), [&](Car b)
                { return b.die == true; });

            if (removeObs != obs.end())
                obs.erase(removeObs);

            if (GetTime() - lastUpdateLog > 1. && countDownLog > 0)
            {
                countDownLog -= 1;
                lastUpdateLog = GetTime();
            }

            if (GetTime() - lastUpdateLog1 > 1. && countDownLog1 > 0)
            {
                countDownLog1 -= 1;
                lastUpdateLog1 = GetTime();
            }

            if (countDownLog1 == 0)
            {
                Log newLog = Log(-150, 85, log1Image, 1);
                logs.push_back(newLog);
                countDownLog1 = 3;
            }

            if (countDownLog == 0)
            {
                randLog = GetRandomValue(0, 1);

                if (logsId[randLog] == 2)
                {
                    Log newLog = Log(GetScreenWidth(), 155, log2Image, 2);
                    newLog.speed *= -1;
                    logs.push_back(newLog);
                }
                else if (logsId[randLog] == 3)
                {
                    Log newLog = Log(-270, 236, log3Image, 3);
                    logs.push_back(newLog);
                }
                countDownLog = 2;
            }

            for (int i = 0; i < logs.size(); i++)
            {
                logs[i].Update();

                if (logs[i].pos.x > WIDTH || logs[i].pos.x < -270)
                    logs[i].die = true;
            }

            auto removeLog = std::remove_if(
                logs.begin(), logs.end(), [&](Log b)
                { return b.die == true; });

            if (removeLog != logs.end())
                logs.erase(removeLog);

            // check collision car dengan player
            for (int i = 0; i < obs.size(); i++)
            {
                if (CheckCollisionRecs(obs[i].rect, player.rect))
                {
                    player.live -= 1;
                    player.ResetPosition();
                    player.up = true;
                }
            }

            // check collision log dengan player
            for (int i = 0; i < logs.size(); i++)
            {
                if (CheckCollisionRecs(player.rect, logs[i].rect))
                {
                    if (logs[i].id == 3)
                    {
                        player.onFirstLog = true;
                    }
                }

                if (!CheckCollisionRecs(player.rect, logs[i].rect))
                {
                    player.kenaLog = false;
                    player.onFirstLog = false;
                }

                if (CheckCollisionRecs(player.rect, logs[i].rect))
                {
                    player.kenaLog = true;
                    player.pos.x += logs[i].speed;
                    break;
                }
            }

            // for (auto &l : loseRect)
            //     DrawRectangleRec(l, RED);

            // std::cout << "onFirst: " << player.onFirstLog << std::endl;
            // std::cout << "kenaLog: " << player.kenaLog << std::endl;

            if ((player.pos.y < 283 && player.pos.y > 20) && !player.kenaLog)
            {
                player.live -= 1;
                player.ResetPosition();
            }

            // check collide dengan finish
            for (int i = 0; i < finish.size(); i++)
            {
                if (CheckCollisionRecs(player.rect, finish[i].rect) && !finish[i].show)
                {
                    finish[i].rect.x = finish[i].rect.x - finish[i].img.width / 2 + 26;
                    finish[i].rect.y = finish[i].rect.y - finish[i].img.height / 2 + 9;
                    finish[i].show = true;
                    finishCount++;
                    player.ResetPosition();
                    score += 10;
                }
                if (CheckCollisionRecs(player.rect, loseRect[i]))
                {
                    player.live -= 1;
                    player.ResetPosition();
                }
            }

            // keep player on window
            if (player.pos.x > GetScreenWidth() - player.frameRect.width)
            {
                player.pos.x = GetScreenWidth() - player.frameRect.width;
            }
            else if (player.pos.x < 0)
            {
                player.pos.x = 0;
            }

            // draw finish
            for (auto &f : finish)
            {
                f.Draw();
            }

            // draw and update player
            if (!dead)
                player.Update();

            // draw score
            std::string scoreText = "Score: " + std::to_string(score);
            DrawTextEx(f, scoreText.c_str(), Vector2(700, 655), 30, 0.0, WHITE);
        }
        break;
        }
        // std::string mousePosText = "x = " + std::to_string(GetMousePosition().x) + ", y = " + std::to_string(GetMousePosition().y);
        // DrawText(mousePosText.c_str(), GetMousePosition().x, GetMousePosition().y, 20, WHITE);
        EndDrawing();
    }
    UnloadFont(f);
    UnloadTexture(finishImage);
    UnloadTexture(log1Image);
    UnloadTexture(log2Image);
    UnloadTexture(log3Image);
    UnloadTexture(carSprite);
    UnloadTexture(playerSprite);
    UnloadTexture(bg);
    CloseWindow();

    return 0;
}