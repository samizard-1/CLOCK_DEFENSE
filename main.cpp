#include "raylib.h"
#include <cmath>  
#include <vector> 

struct Bullet {
    Vector2 pos;
    Vector2 velocity;
    float radius;
    bool active;
};

struct Enemy {
    Vector2 enPos;
    float enSpeed;
    float enRadius;
    bool enActive;
};

struct superEnemy {
    Vector2 superEnPos;
    float superEnSpeed;
    float superEnRadius;
    bool superEnActive;
    bool isSuperEnemyConverted;
    bool superEnemyColor;
};

struct PoofEffect {
    Vector2 pos;
    float timer;
    int currentFrame;
    Color tint;
    bool active;
    float size;
};


void TryReincarnateSuperEnemy(Vector2 deathPos, std::vector<superEnemy>& superEnemies) {
    
    if (GetRandomValue(1, 100) == 1) {
        superEnemy se;
        se.superEnPos = deathPos;
        se.superEnRadius = (float)GetRandomValue(20, 45);
        se.superEnSpeed = (float)GetRandomValue(100, 260);
        se.superEnActive = true;
        se.isSuperEnemyConverted = false;
        se.superEnemyColor = false;

        superEnemies.push_back(se);
    }
}

void DrawGameText(Font font, const char* text, int posX, int posY, int fontSize, Color color) {
    DrawTextEx(font, text, { (float)posX, (float)posY }, (float)fontSize, 2.0f, color);
}

int main() {
    InitWindow(800, 600, "CLOCK DEFENSE");
    SetTargetFPS(60);

    Texture2D cannonTex = LoadTexture("resources/cannon.png");
    Texture2D poofTex = LoadTexture("resources/poof.png");
    Texture2D bulletTex = LoadTexture("resources/cannonball.png");
    Texture2D clockTex = LoadTexture("resources/clock.png");
    Texture2D gearTex = LoadTexture("resources/gear.png");
    Texture2D bgClockTex = LoadTexture("resources/bg.png");
    Font gameFont = LoadFontEx("resources/clockfont.ttf", 64, 0, 250);
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);
    int poofCols = 4;
    int poofRows = 4;
    int poofTotalFrames = poofCols * poofRows;
    float poofFrameSpeed = 0.03f;
    float poofFrameWidth = (float)poofTex.width / poofCols;
    float poofFrameHeight = (float)poofTex.height / poofRows;


    std::vector<PoofEffect> poofs;
    if (poofTex.id == 0) {
        TraceLog(LOG_ERROR, "POOF TEXTURE FAILED TO LOAD!");
    }


    RenderTexture2D target = LoadRenderTexture(800, 600);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    Vector2 rectPos = { 400.0f, 300.0f };
    float rectWidth = 120.0f;
    float rectHeight = 100.0f;
    float gunHitboxRadius = 50.0f; 

    bool winGame = false;
    bool gotUpgrade = false;
    int secretKills = 0;
    bool secretLevelComplete = false;
    bool secretMusicStarted = false;
    int ammo = 10;
    int ammoGetTimer = 60;
    int lives = 10;
    int level = 1;
    bool secretLevel = false;
    bool secretLevelPlaying = false;

    std::vector<Bullet> bullets;
    float bulletSpeed = 600.0f;
    float bulletRadius = 11.0f;

    std::vector<Enemy> enemies;
    std::vector<superEnemy> superEnemies;
    int maxEnemies = 5;

    bool gameOver = false; 
    int enemiesKilled = 0;

    bool L1Complete = false;
    bool L2Complete = false;
    bool L3Complete = false;
    bool L4Complete = false;
    bool L5Complete = false;

    float timeLeft = 3.0f;
    float timeLeft2 = 3.0f;
    float timeLeft3 = 3.0f;
    float timeLeft4 = 3.0f;
    float timeLeftUpgrade = 3.0f;

    bool L1M = false;
    bool L2M = false;
    bool L3M = false;
    bool L4M = false;
    bool L5M = false;
    
    InitAudioDevice();

    Sound enemyDeathSound = LoadSound("resources/enemy_death.wav");
    SetSoundVolume(enemyDeathSound, 0.9f);

    Sound shootSound = LoadSound("resources/shootsound.wav");
    SetSoundVolume(shootSound, 1.0f);

    if (!IsSoundValid(enemyDeathSound)) {
        TraceLog(LOG_ERROR, "Enemy death sound failed to load!");
    }
    if (!IsSoundValid(shootSound)) {
        TraceLog(LOG_ERROR, "Shoot sound failed to load!");
    }

    Music music = LoadMusicStream("resources/L1.mp3");
    Music music2 = LoadMusicStream("resources/L2.wav");
    Music music3 = LoadMusicStream("resources/L3.mp3");
    Music music4 = LoadMusicStream("resources/L4.ogg");
    Music music5 = LoadMusicStream("resources/L5.ogg");
    Music secretMusic = LoadMusicStream("resources/secretmusic.mp3");
    SetMusicVolume(music, 0.6f);
    SetMusicVolume(music2, 0.6f);
    SetMusicVolume(music3, 0.7f);
    SetMusicVolume(music4, 0.6f);
    SetMusicVolume(music5, 0.8f);
    SetMusicVolume(secretMusic, 0.8f);

    Rectangle restartBtn = { 710.0f, 510.0f, 70.0f, 70.0f };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // if (IsKeyPressed(KEY_SPACE)) 
        // {
        //     enemiesKilled += 20;
        // }
        //TESTING PURPOSES ONLY
   
        float scaleX = (float)GetScreenWidth() / 800.0f;
        float scaleY = (float)GetScreenHeight() / 600.0f;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;

        Vector2 rawMouse = GetMousePosition();
        Vector2 cursorPos = {
            (rawMouse.x - (GetScreenWidth() - (800.0f * scale)) * 0.5f) / scale,
            (rawMouse.y - (GetScreenHeight() - (600.0f * scale)) * 0.5f) / scale
        };

        if (gameOver || secretLevelComplete) {
            bool clickedR = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(cursorPos, restartBtn);
            if (IsKeyPressed(KEY_R) || clickedR) {
                
                ammo = 10;
                ammoGetTimer = 60;
                lives = 10;
                level = 1;
                maxEnemies = 5;
                enemiesKilled = 0;
                secretKills = 0;
                bulletRadius = 8.0f;
                
                winGame = false;
                gotUpgrade = false;
                gameOver = false;
                secretLevel = false;
                secretLevelPlaying = false;
                secretLevelComplete = false;
                L1Complete = L2Complete = L3Complete = L4Complete = L5Complete = false;
                
                timeLeft = timeLeft2 = timeLeft3 = timeLeft4 = timeLeftUpgrade = 3.0f;

                enemies.clear();
                superEnemies.clear();
                bullets.clear();

                StopMusicStream(music);
                StopMusicStream(music2);
                StopMusicStream(music3);
                StopMusicStream(music4);
                StopMusicStream(music5);
                StopMusicStream(secretMusic);
                L1M = L2M = L3M = L4M = L5M = false;
                secretMusicStarted = false;
            }
        }


        if (!secretLevel) {
            if (level == 1 && !L1M) {
                PlayMusicStream(music);
                L1M = true;
            } else if (level == 2 && !L2M) {
                StopMusicStream(music);
                PlayMusicStream(music2);
                L2M = true;
            } else if (level == 3 && !L3M) {
                StopMusicStream(music2);
                PlayMusicStream(music3);
                L3M = true;
            } else if (level == 4 && !L4M) {
                StopMusicStream(music3);
                PlayMusicStream(music4);
                L4M = true;
            } else if (level == 5 && !L5M) {
                StopMusicStream(music4);
                PlayMusicStream(music5);
                L5M = true;
            }

            if (level == 1) UpdateMusicStream(music);
            else if (level == 2) UpdateMusicStream(music2);
            else if (level == 3) UpdateMusicStream(music3);
            else if (level == 4) UpdateMusicStream(music4);
            else if (level == 5) UpdateMusicStream(music5);
        } else {
            if (!secretMusicStarted) {
                StopMusicStream(music);
                StopMusicStream(music2);
                StopMusicStream(music3);
                StopMusicStream(music4);
                StopMusicStream(music5);
                PlayMusicStream(secretMusic);
                secretMusicStarted = true;
            }
            UpdateMusicStream(secretMusic);
        }

        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        if (!secretLevel) {
            if (!gameOver) {
                Vector2 rawMouse = GetMousePosition();
                Vector2 cursorPos = {
                    (rawMouse.x - (GetScreenWidth() - (800.0f * scale)) * 0.5f) / scale,
                    (rawMouse.y - (GetScreenHeight() - (600.0f * scale)) * 0.5f) / scale
                };

                float dx = cursorPos.x - rectPos.x;
                float dy = cursorPos.y - rectPos.y;
                float angleRadians = atan2f(dy, dx);
                float angleDegrees = angleRadians * RAD2DEG;
        
                if (enemies.size() < maxEnemies && GetRandomValue(1, 180) == 1 && !L1Complete) {
                    Enemy e;
                    e.enRadius = (float)GetRandomValue(50, 100);
                    e.enSpeed = (float)GetRandomValue(75, 100);

                    int edge = GetRandomValue(0, 3);
                    if (edge == 0) e.enPos = { (float)GetRandomValue(0, 800), -e.enRadius };
                    if (edge == 1) e.enPos = { (float)GetRandomValue(0, 800), 600.0f + e.enRadius };
                    if (edge == 2) e.enPos = { -e.enRadius, (float)GetRandomValue(0, 600) };
                    if (edge == 3) e.enPos = { 800.0f + e.enRadius, (float)GetRandomValue(0, 600) };

                    e.enActive = true;
                    enemies.push_back(e);
                }
                else if (enemies.size() < maxEnemies && GetRandomValue(1, 120) == 1 && L1Complete && !L2Complete) {
                    Enemy e;
                    e.enRadius = (float)GetRandomValue(35, 100);
                    e.enSpeed = (float)GetRandomValue(75, 200);

                    int edge = GetRandomValue(0, 3);
                    if (edge == 0) e.enPos = { (float)GetRandomValue(0, 800), -e.enRadius };
                    if (edge == 1) e.enPos = { (float)GetRandomValue(0, 800), 600.0f + e.enRadius };
                    if (edge == 2) e.enPos = { -e.enRadius, (float)GetRandomValue(0, 600) };
                    if (edge == 3) e.enPos = { 800.0f + e.enRadius, (float)GetRandomValue(0, 600) };

                    e.enActive = true;
                    enemies.push_back(e);
                }
                else if (enemies.size() < maxEnemies && GetRandomValue(1, 80) == 1 && L2Complete && !L3Complete) {
                    Enemy e;
                    e.enRadius = (float)GetRandomValue(20, 90);
                    e.enSpeed = (float)GetRandomValue(100, 300);

                    int edge = GetRandomValue(0, 3);
                    if (edge == 0) e.enPos = { (float)GetRandomValue(0, 800), -e.enRadius };
                    if (edge == 1) e.enPos = { (float)GetRandomValue(0, 800), 600.0f + e.enRadius };
                    if (edge == 2) e.enPos = { -e.enRadius, (float)GetRandomValue(0, 600) };
                    if (edge == 3) e.enPos = { 800.0f + e.enRadius, (float)GetRandomValue(0, 600) };

                    e.enActive = true;
                    enemies.push_back(e);
                }
                else if (enemies.size() < maxEnemies && GetRandomValue(1, 60) == 1 && L3Complete && !L4Complete) {
                    Enemy e;
                    e.enRadius = (float)GetRandomValue(15, 80);
                    e.enSpeed = (float)GetRandomValue(150, 350);

                    int edge = GetRandomValue(0, 3);
                    if (edge == 0) e.enPos = { (float)GetRandomValue(0, 800), -e.enRadius };
                    if (edge == 1) e.enPos = { (float)GetRandomValue(0, 800), 600.0f + e.enRadius };
                    if (edge == 2) e.enPos = { -e.enRadius, (float)GetRandomValue(0, 600) };
                    if (edge == 3) e.enPos = { 800.0f + e.enRadius, (float)GetRandomValue(0, 600) };

                    e.enActive = true;
                    enemies.push_back(e);
                }
                else if (enemies.size() < maxEnemies && GetRandomValue(1, 40) == 1 && L4Complete && !L5Complete) {
                    Enemy e;
                    e.enRadius = (float)GetRandomValue(10, 70);
                    e.enSpeed = (float)GetRandomValue(200, 400);

                    int edge = GetRandomValue(0, 3);
                    if (edge == 0) e.enPos = { (float)GetRandomValue(0, 800), -e.enRadius };
                    if (edge == 1) e.enPos = { (float)GetRandomValue(0, 800), 600.0f + e.enRadius };
                    if (edge == 2) e.enPos = { -e.enRadius, (float)GetRandomValue(0, 600) };
                    if (edge == 3) e.enPos = { 800.0f + e.enRadius, (float)GetRandomValue(0, 600) };

                    e.enActive = true;
                    enemies.push_back(e);
                }
                if (!L5Complete && !secretLevelComplete && !gameOver) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ammo > 0) {
                        Bullet b;
                        float barrelLength = 50.0f;
                        b.pos = {
                            rectPos.x + cosf(angleRadians) * barrelLength,
                            rectPos.y + sinf(angleRadians) * barrelLength
                        };
                        b.velocity = {
                            cosf(angleRadians) * bulletSpeed,
                            sinf(angleRadians) * bulletSpeed
                        };
                        b.radius = bulletRadius;
                        b.active = true;
                        ammo--;

                        PlaySound(shootSound);
                        bullets.push_back(b);
                    }
                }

                for (superEnemy& se : superEnemies) {
                    if (!se.superEnActive) continue;

                    float enemyDx = rectPos.x - se.superEnPos.x;
                    float enemyDy = rectPos.y - se.superEnPos.y;
                    float angleToGun = atan2f(enemyDy, enemyDx);

                    se.superEnPos.x += cosf(angleToGun) * se.superEnSpeed * dt;
                    se.superEnPos.y += sinf(angleToGun) * se.superEnSpeed * dt;

                    if (CheckCollisionCircles(se.superEnPos, se.superEnRadius, rectPos, gunHitboxRadius)) {
                        lives -= 2;
                        se.superEnActive = false;
                        PlaySound(enemyDeathSound);
                    }
                }

                for (Enemy& e : enemies) {
                    if (!e.enActive) continue;

                    float enemyDx = rectPos.x - e.enPos.x;
                    float enemyDy = rectPos.y - e.enPos.y;
                    float angleToGun = atan2f(enemyDy, enemyDx);

                    e.enPos.x += cosf(angleToGun) * e.enSpeed * dt;
                    e.enPos.y += sinf(angleToGun) * e.enSpeed * dt;

                    if (CheckCollisionCircles(e.enPos, e.enRadius, rectPos, gunHitboxRadius)) {
                        lives--;
                        e.enActive = false;
                        PlaySound(enemyDeathSound);
                    }
                }

                if (enemiesKilled >= 50) {
                    L1Complete = true;
                    level = 2;
                }
                if (enemiesKilled >= 100) {
                    L2Complete = true;
                    level = 3;
                }
                if (enemiesKilled >= 150) {
                    L3Complete = true;
                    maxEnemies = 10;
                    level = 4;
                }
                if (enemiesKilled >= 200) {
                    L4Complete = true;
                    maxEnemies = 14;
                    level = 5;
                }
                if (enemiesKilled >= 250) {
                    L5Complete = true;
                    winGame = true;
                }

                for (Bullet& b : bullets) {
                    if (!b.active) continue;

                    b.pos.x += b.velocity.x * dt;
                    b.pos.y += b.velocity.y * dt;

                    if (b.pos.x < 0 || b.pos.x > 800 || b.pos.y < 0 || b.pos.y > 600) {
                        b.active = false;
                        continue;
                    }

                    for (superEnemy& se : superEnemies) {
                        if (se.superEnActive && CheckCollisionCircles(b.pos, b.radius, se.superEnPos, se.superEnRadius)) {
                            b.active = false;
                            se.superEnActive = false;
                            se.isSuperEnemyConverted = true;
                            poofs.push_back({ se.superEnPos, 0.0f, 0, PURPLE, true, se.superEnRadius * 2.5f });
                            Enemy regular;
                            regular.enPos = se.superEnPos;
                            regular.enSpeed = se.superEnSpeed * 1.50f;
                            regular.enRadius = se.superEnRadius * 0.50f;
                            regular.enActive = true;
                            enemies.push_back(regular);
                            PlaySound(enemyDeathSound);
                            break;
                        }
                    }

                    if (!b.active) continue;

                    for (Enemy& e : enemies) {
                        if (e.enActive && CheckCollisionCircles(b.pos, b.radius, e.enPos, e.enRadius)) {
                            b.active = false;
                            e.enActive = false;
                            PlaySound(enemyDeathSound);
                            enemiesKilled++;
                            poofs.push_back({ e.enPos, 0.0f, 0, WHITE, true, e.enRadius * 2.5f});
                            TryReincarnateSuperEnemy(e.enPos, superEnemies);
                            break;
                        }
                    }
                }

                for (int i = bullets.size() - 1; i >= 0; i--) {
                    if (!bullets[i].active) bullets.erase(bullets.begin() + i);
                }
                for (PoofEffect& p : poofs) {
                    if (!p.active) continue;
                    p.timer += dt;
                    if (p.timer >= poofFrameSpeed) {
                        p.timer = 0.0f;
                        p.currentFrame++;
                        if (p.currentFrame >= poofTotalFrames) {
                            p.active = false;
                        }
                    }
                }
                for (int i = poofs.size() - 1; i >= 0; i--) {
                    if (!poofs[i].active) poofs.erase(poofs.begin() + i);
                }
                for (int i = superEnemies.size() - 1; i >= 0; i--) {
                    if (!superEnemies[i].superEnActive) superEnemies.erase(superEnemies.begin() + i);
                }
                for (int i = enemies.size() - 1; i >= 0; i--) {
                    if (!enemies[i].enActive) enemies.erase(enemies.begin() + i);
                }
                if (!L5Complete && !secretLevelComplete) {  
                    ammoGetTimer--;
                    if (ammoGetTimer <= 0) {
                        ammo++;
                        ammoGetTimer = 60;
                    }
                }
                
                if (!gotUpgrade && GetRandomValue(1, 12000) == 1 && enemiesKilled >= 50) {
                    gotUpgrade = true;
                    bulletRadius = 21.0f;
                    //12000
                }

                BeginTextureMode(target);
                    ClearBackground(SKYBLUE);

                    Rectangle bgSrc = { 0.0f, 0.0f, (float)bgClockTex.width, (float)bgClockTex.height };
                    Rectangle bgDest = { 0.0f, 0.0f, 800.0f, 600.0f };
                    DrawTexturePro(bgClockTex, bgSrc, bgDest, { 0.0f, 0.0f }, 0.0f, WHITE);

                    if (L1Complete && timeLeft > 0.0f) {
                        timeLeft -= dt;
                        const char* text = "LEVEL ONE COMPLETE!";
                        int fontSize = 60;
                        int textWidth = MeasureText(text, fontSize);
                        DrawGameText(gameFont, text, (800 - textWidth +120) / 2, (300 - fontSize) / 2, fontSize, GREEN);
                    }

                    if (L2Complete && timeLeft2 > 0.0f) {
                        timeLeft2 -= dt;
                        const char* text = "LEVEL TWO COMPLETE!";
                        int fontSize = 60;
                        int textWidth = MeasureText(text, fontSize);
                        DrawGameText(gameFont, text, (800 - textWidth+120) / 2, (300 - fontSize) / 2, fontSize, GREEN);
                    }

                    if (L3Complete && timeLeft3 > 0.0f) {
                        timeLeft3 -= dt;
                        const char* text = "LEVEL THREE COMPLETE!";
                        int fontSize = 50;
                        int textWidth = MeasureText(text, fontSize);
                        DrawGameText(gameFont, text, (800 - textWidth+120) / 2, (300 - fontSize) / 2, fontSize, GREEN);
                    } 

                    if (L4Complete && timeLeft4 > 0.0f) {
                        timeLeft4 -= dt;
                        const char* text = "LEVEL FOUR COMPLETE!";
                        int fontSize = 50;
                        int textWidth = MeasureText(text, fontSize);
                        DrawGameText(gameFont, text, (800 - textWidth+120) / 2, (300 - fontSize) / 2, fontSize, GREEN);
                    } 
                    
                    if (L5Complete && !secretLevelComplete) {
                        enemies.clear();
                        superEnemies.clear();
                        bullets.clear();
                        const char* text = "LEVEL FIVE COMPLETE!";
                        int fontSize = 50;
                        int textWidth = MeasureText(text, fontSize);
                        DrawGameText(gameFont, text, (800 - textWidth +120) / 2, (300 - fontSize) / 2, fontSize, GREEN);
                        DrawGameText(gameFont, "YOU WIN!", 420, 400, 60, GREEN);
                        DrawGameText(gameFont, "Or have you? Press ENTER to continue...", 135, 560, 30, RED);
                        if (IsKeyPressed(KEY_ENTER)) {
                            secretLevel = true;
                        }
                    } 

                    float spinAngle = (float)GetTime() * 120.0f; 

                    for (const superEnemy& se : superEnemies) {
                        Rectangle src = { 0.0f, 0.0f, (float)clockTex.width, (float)clockTex.height };
                        float diameter = se.superEnRadius * 2.0f;
                        Rectangle dest = { se.superEnPos.x, se.superEnPos.y, diameter, diameter };
                        Vector2 origin = { se.superEnRadius, se.superEnRadius };

                        float tickAngle = sinf((float)GetTime() * 6.0f) * 15.0f;
                        DrawTexturePro(clockTex, src, dest, origin, tickAngle, WHITE);
                    }
                    
                    for (const Enemy& e : enemies) {
                        Rectangle src = { 0.0f, 0.0f, (float)gearTex.width, (float)gearTex.height };
                        float diameter = e.enRadius * 2.0f;
                        Rectangle dest = { e.enPos.x, e.enPos.y, diameter, diameter };
                        Vector2 origin = { e.enRadius, e.enRadius };

                        DrawTexturePro(gearTex, src, dest, origin, spinAngle, WHITE);
                    }

                    for (const PoofEffect& p : poofs) {
                        if (!p.active) continue;

                        int col = p.currentFrame % poofCols;
                        int row = p.currentFrame / poofCols;

                        Rectangle sourceRec = {
                            col * poofFrameWidth,
                            row * poofFrameHeight,
                            poofFrameWidth,
                            poofFrameHeight
                        };

                        Rectangle destRec = {
                            p.pos.x,
                            p.pos.y,
                            p.size,
                            p.size
                        };
                        Vector2 origin = { p.size * 0.5f, p.size * 0.5f };

                        DrawTexturePro(poofTex, sourceRec, destRec, origin, 0.0f, p.tint);
                    }

                    for (const Bullet& b : bullets) {
                        Rectangle bulletSource = { 0.0f, 0.0f, (float)bulletTex.width, (float)bulletTex.height };
                        
                        float diameter = b.radius * 2.0f;
                        Rectangle bulletDest = { b.pos.x, b.pos.y, diameter, diameter };
                        Vector2 bulletOrigin = { b.radius, b.radius };

                        DrawTexturePro(bulletTex, bulletSource, bulletDest, bulletOrigin, 0.0f, WHITE);
                    }

                    DrawGameText(gameFont, TextFormat("Enemies Killed: %d", enemiesKilled), 10, 10, 20, BLACK);
                    DrawGameText(gameFont, TextFormat("Ammo: %d", ammo), 10, 40, 20, BLACK);
                    DrawGameText(gameFont, TextFormat("Lives: %d", lives), 10, 70, 20, BLACK);
                    DrawGameText(gameFont, TextFormat("Level: %d/5", level), 10, 100, 20, BLACK);

                    Rectangle cannonSource = { 0.0f, 0.0f, (float)cannonTex.width, (float)cannonTex.height };
                    Rectangle cannonDest = { rectPos.x, rectPos.y, rectWidth, rectHeight };

                    Vector2 cannonOrigin = { rectWidth * 0.5f, rectHeight * 0.5f };

                    DrawTexturePro(cannonTex, cannonSource, cannonDest, cannonOrigin, angleDegrees, WHITE);


                    if (gotUpgrade && timeLeftUpgrade > 0.0f) {
                        timeLeftUpgrade -= dt;
                        DrawGameText(gameFont, "UPGRADE RECEIVED", 130, 550, 40, GREEN);
                    }
                EndTextureMode();
            } 

            if (lives <= 0) {
                gameOver = true;
                BeginTextureMode(target);
                    ClearBackground(BLACK);

                    const char* text = "YOU LOST!";
                    int fontSize = 60;
                    int textWidth = MeasureText(text, fontSize);
                        
                    DrawRectangleLinesEx(restartBtn, 3, RED);
                    DrawGameText(gameFont, "R", (int)restartBtn.x + 30, (int)restartBtn.y + 5, 65, RED);

                    DrawGameText(gameFont, text, (800 - textWidth + 150) / 2, (600 - fontSize) / 2, fontSize, RED);
                EndTextureMode();
            }

            BeginDrawing();
                ClearBackground(BLACK);
                Rectangle sourceRec = { 0.0f, 0.0f, 800.0f, -600.0f };
                Rectangle destRec = {
                    (GetScreenWidth() - (800.0f * scale)) * 0.5f,
                    (GetScreenHeight() - (600.0f * scale)) * 0.5f,
                    800.0f * scale,
                    600.0f * scale
                };
                DrawTexturePro(target.texture, sourceRec, destRec, { 0.0f, 0.0f }, 0.0f, WHITE);
            EndDrawing();
        }
        else {
            if (!secretLevelPlaying) {
                BeginTextureMode(target);
                    ClearBackground(BLACK);
                    DrawGameText(gameFont, "SECRET LEVEL ACCESSED", 195, 250, 50, PURPLE);
                    DrawGameText(gameFont, "Press ENTER to start.", 250, 350, 30, PURPLE);
                EndTextureMode();

                if (IsKeyPressed(KEY_ENTER)) {
                    secretLevelPlaying = true;
                    enemies.clear();
                    superEnemies.clear();
                    bullets.clear();
                    lives = 10;
                    ammo = 15;
                }
            }
            else if (!gameOver) {
                Vector2 rawMouse = GetMousePosition();
                Vector2 cursorPos = {
                    (rawMouse.x - (GetScreenWidth() - (800.0f * scale)) * 0.5f) / scale,
                    (rawMouse.y - (GetScreenHeight() - (600.0f * scale)) * 0.5f) / scale
                };

                float dx = cursorPos.x - rectPos.x;
                float dy = cursorPos.y - rectPos.y;
                float angleRadians = atan2f(dy, dx);
                float angleDegrees = angleRadians * RAD2DEG;

                if (!secretLevelComplete) {
                    if (superEnemies.size() < maxEnemies && GetRandomValue(1, 140) == 1) {
                        superEnemy se;
                        se.superEnRadius = (float)GetRandomValue(20, 45);
                        se.superEnSpeed = (float)GetRandomValue(100, 260);

                        int edge = GetRandomValue(0, 3);
                        if (edge == 0) se.superEnPos = { (float)GetRandomValue(0, 800), -se.superEnRadius };
                        if (edge == 1) se.superEnPos = { (float)GetRandomValue(0, 800), 600.0f + se.superEnRadius };
                        if (edge == 2) se.superEnPos = { -se.superEnRadius, (float)GetRandomValue(0, 600) };
                        if (edge == 3) se.superEnPos = { 800.0f + se.superEnRadius, (float)GetRandomValue(0, 600) };

                        se.superEnActive = true;
                        se.isSuperEnemyConverted = false;
                        se.superEnemyColor = false;
                        superEnemies.push_back(se);
                    }

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ammo > 0) {
                        Bullet b;
                        float barrelLength = 50.0f;
                        b.pos = {
                            rectPos.x + cosf(angleRadians) * barrelLength,
                            rectPos.y + sinf(angleRadians) * barrelLength
                        };
                        b.velocity = {
                            cosf(angleRadians) * bulletSpeed,
                            sinf(angleRadians) * bulletSpeed
                        };
                        b.radius = bulletRadius;
                        b.active = true;
                        ammo--;

                        PlaySound(shootSound);
                        bullets.push_back(b);
                    }

                    for (superEnemy& se : superEnemies) {
                        if (!se.superEnActive) continue;

                        float enemyDx = rectPos.x - se.superEnPos.x;
                        float enemyDy = rectPos.y - se.superEnPos.y;
                        float angleToGun = atan2f(enemyDy, enemyDx);

                        se.superEnPos.x += cosf(angleToGun) * se.superEnSpeed * dt;
                        se.superEnPos.y += sinf(angleToGun) * se.superEnSpeed * dt;

                        if (CheckCollisionCircles(se.superEnPos, se.superEnRadius, rectPos, gunHitboxRadius)) {
                            lives -= 2;
                            se.superEnActive = false;
                            PlaySound(enemyDeathSound);
                        }
                    }

                    for (Enemy& e : enemies) {
                        if (!e.enActive) continue;

                        float enemyDx = rectPos.x - e.enPos.x;
                        float enemyDy = rectPos.y - e.enPos.y;
                        float angleToGun = atan2f(enemyDy, enemyDx);

                        e.enPos.x += cosf(angleToGun) * e.enSpeed * dt;
                        e.enPos.y += sinf(angleToGun) * e.enSpeed * dt;

                        if (CheckCollisionCircles(e.enPos, e.enRadius, rectPos, gunHitboxRadius)) {
                            lives--;
                            e.enActive = false;
                            PlaySound(enemyDeathSound);
                        }
                    }
                    for (Bullet& b : bullets) {
                        if (!b.active) continue;

                        b.pos.x += b.velocity.x * dt;
                        b.pos.y += b.velocity.y * dt;

                        if (b.pos.x < 0 || b.pos.x > 800 || b.pos.y < 0 || b.pos.y > 600) {
                            b.active = false;
                            continue;
                        }

                        for (superEnemy& se : superEnemies) {
                            if (se.superEnActive && CheckCollisionCircles(b.pos, b.radius, se.superEnPos, se.superEnRadius)) {
                                b.active = false;
                                se.superEnActive = false;
                                se.isSuperEnemyConverted = true;
                                poofs.push_back({ se.superEnPos, 0.0f, 0, PURPLE, true, se.superEnRadius * 2.5f });
                                Enemy regular;
                                regular.enPos = se.superEnPos;
                                regular.enSpeed = se.superEnSpeed * 1.50f;
                                regular.enRadius = se.superEnRadius * 0.50f;
                                regular.enActive = true;
                                enemies.push_back(regular);
                                PlaySound(enemyDeathSound);
                                break;
                            }
                        }

                        if (!b.active) continue;

                        for (Enemy& e : enemies) {
                            if (e.enActive && CheckCollisionCircles(b.pos, b.radius, e.enPos, e.enRadius)) {
                                b.active = false;
                                e.enActive = false;
                                PlaySound(enemyDeathSound);
                                secretKills++;
                                poofs.push_back({ e.enPos, 0.0f, 0, WHITE, true, e.enRadius * 2.5f });
                                TryReincarnateSuperEnemy(e.enPos, superEnemies);
                                break;
                            }
                        }
                    }

                    for (int i = bullets.size() - 1; i >= 0; i--) {
                        if (!bullets[i].active) bullets.erase(bullets.begin() + i);
                    }
                    for (PoofEffect& p : poofs) {
                        if (!p.active) continue;
                        p.timer += dt;
                        if (p.timer >= poofFrameSpeed) {
                            p.timer = 0.0f;
                            p.currentFrame++;
                            if (p.currentFrame >= poofTotalFrames) {
                                p.active = false;
                            }
                        }
                    }

                    for (int i = poofs.size() - 1; i >= 0; i--) {
                        if (!poofs[i].active) poofs.erase(poofs.begin() + i);
                    }
                    for (int i = superEnemies.size() - 1; i >= 0; i--) {
                        if (!superEnemies[i].superEnActive) superEnemies.erase(superEnemies.begin() + i);
                    }
                    for (int i = enemies.size() - 1; i >= 0; i--) {
                        if (!enemies[i].enActive) enemies.erase(enemies.begin() + i);
                    }

                    ammoGetTimer--;
                    if (ammoGetTimer <= 0) {
                        ammo++;
                        ammoGetTimer = 60;
                    }
                    if (secretKills >= 50) {
                        secretLevelComplete = true;
                        enemies.clear();
                        superEnemies.clear();
                        bullets.clear();
                    }
                }

                BeginTextureMode(target);
                    ClearBackground(DARKBROWN);

                    Rectangle bgSrc = { 0.0f, 0.0f, (float)bgClockTex.width, (float)bgClockTex.height };
                    Rectangle bgDest = { 0.0f, 0.0f, 800.0f, 600.0f };
                    DrawTexturePro(bgClockTex, bgSrc, bgDest, { 0.0f, 0.0f }, 0.0f, PURPLE);
                    
                    float spinAngle = (float)GetTime() * 120.0f;

                    for (const superEnemy& se : superEnemies) {
                        Rectangle src = { 0.0f, 0.0f, (float)clockTex.width, (float)clockTex.height };
                        float diameter = se.superEnRadius * 2.0f;
                        Rectangle dest = { se.superEnPos.x, se.superEnPos.y, diameter, diameter };
                        Vector2 origin = { se.superEnRadius, se.superEnRadius };
                        float tickAngle = sinf((float)GetTime() * 6.0f) * 15.0f;
                        DrawTexturePro(clockTex, src, dest, origin, tickAngle, WHITE);
                    }

                    for (const Enemy& e : enemies) {
                        Rectangle src = { 0.0f, 0.0f, (float)gearTex.width, (float)gearTex.height };
                        float diameter = e.enRadius * 2.0f;
                        Rectangle dest = { e.enPos.x, e.enPos.y, diameter, diameter };
                        Vector2 origin = { e.enRadius, e.enRadius };
                        DrawTexturePro(gearTex, src, dest, origin, spinAngle, WHITE);
                    }
                    
                    for (const Bullet& b : bullets) {
                        Rectangle bulletSource = { 0.0f, 0.0f, (float)bulletTex.width, (float)bulletTex.height };
                        float diameter = b.radius * 2.0f;
                        Rectangle bulletDest = { b.pos.x, b.pos.y, diameter, diameter };
                        Vector2 bulletOrigin = { b.radius, b.radius };
                        DrawTexturePro(bulletTex, bulletSource, bulletDest, bulletOrigin, 0.0f, WHITE);
                    }

                    for (const PoofEffect& p : poofs) {
                        if (!p.active) continue;

                        int col = p.currentFrame % poofCols;
                        int row = p.currentFrame / poofCols;

                        Rectangle sourceRec = {
                            col * poofFrameWidth,
                            row * poofFrameHeight,
                            poofFrameWidth,
                            poofFrameHeight
                        };

                        Rectangle destRec = {
                            p.pos.x,
                            p.pos.y,
                            poofFrameWidth,
                            poofFrameHeight
                        };
                        Vector2 origin = { poofFrameWidth * 0.5f, poofFrameHeight * 0.5f };

                        DrawTexturePro(poofTex, sourceRec, destRec, origin, 0.0f, p.tint);
                    }


                    DrawGameText(gameFont, TextFormat("Ammo: %d", ammo), 10, 40, 20, ORANGE);
                    DrawGameText(gameFont, TextFormat("Lives: %d", lives), 10, 70, 20, PINK);
                    DrawGameText(gameFont, "SECRET LEVEL", 10, 100, 20, PURPLE);

                    maxEnemies = 13;

                    Rectangle cannonSource = { 0.0f, 0.0f, (float)cannonTex.width, (float)cannonTex.height };
                    Rectangle cannonDest = { rectPos.x, rectPos.y, rectWidth, rectHeight };
                    Vector2 cannonOrigin = { rectWidth * 0.5f, rectHeight * 0.5f };
                    DrawTexturePro(cannonTex, cannonSource, cannonDest, cannonOrigin, angleDegrees, WHITE);


                    DrawGameText(gameFont, TextFormat("Kills in Secret Level: %d", secretKills), 10, 10, 20, PURPLE);
                    if (secretLevelComplete) {
                        ClearBackground(DARKPURPLE);
                        DrawGameText(gameFont, "SECRET LEVEL COMPLETE!", 110, 220, 45, GOLD);
                        DrawGameText(gameFont, "TRUE ENDING UNLOCKED!", 130, 300, 40, GREEN);
                        DrawRectangleLinesEx(restartBtn, 3, RED);
                        DrawGameText(gameFont, "R", (int)restartBtn.x + 15, (int)restartBtn.y + 5, 65, RED);
                    }
                EndTextureMode();
            }

            BeginTextureMode(target);

                if (lives <= 0) {
                    gameOver = true;
                    ClearBackground(BLACK);
                    const char* text = "YOU LOST!";
                    int fontSize = 60;
                    int textWidth = MeasureText(text, fontSize);

                    DrawRectangleLinesEx(restartBtn, 3, RED);
                    DrawGameText(gameFont, "R", (int)restartBtn.x + 15, (int)restartBtn.y + 5, 65, RED);
                    DrawGameText(gameFont, text, (800 - textWidth+150) / 2, (600 - fontSize) / 2, fontSize, RED);
                }
            EndTextureMode();

            

            BeginDrawing();
                ClearBackground(BLACK);
                Rectangle sourceRec = { 0.0f, 0.0f, 800.0f, -600.0f };
                Rectangle destRec = {
                    (GetScreenWidth() - (800.0f * scale)) * 0.5f,
                    (GetScreenHeight() - (600.0f * scale)) * 0.5f,
                    800.0f * scale,
                    600.0f * scale
                };
                DrawTexturePro(target.texture, sourceRec, destRec, { 0.0f, 0.0f }, 0.0f, WHITE);
            EndDrawing();
        }
    }

    UnloadRenderTexture(target);
    UnloadMusicStream(music);
    UnloadMusicStream(secretMusic);
    UnloadMusicStream(music2);
    UnloadMusicStream(music3);
    UnloadMusicStream(music4);
    UnloadMusicStream(music5);
    UnloadTexture(poofTex);
    UnloadSound(enemyDeathSound);
    UnloadSound(shootSound);
    UnloadTexture(cannonTex);
    UnloadTexture(gearTex);
    UnloadTexture(clockTex);
    UnloadTexture(bgClockTex);
    UnloadFont(gameFont);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
