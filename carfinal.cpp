#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define OBSTACLE_COLOR YELLOW
#define CAR_COLOR RED
#define NITRO_COLOR MAGENTA
#define BOOST_COLOR BLUE

#define MAX_LIVES 2
#define NUM_OBSTACLES 3
#define NUM_NITRO 1

int speed = 5, baseSpeed;
bool boostActive = false;
int boostTimer = 0;
int score = 0;
int lives = MAX_LIVES;
int carX = 200, carY = 400;
bool paused = false;
int highScore = 0;

// Function declarations
void showStartScreen();
void showMenu();
int getLevelSelection();
void setDifficulty(int choice);
void drawCar(int x, int y);
void drawBoostBar(int x, int y);
void drawNitroItems(int nitroX[], int nitroY[]);
void drawLaneBoundaries();
void showGameOver();
void drawHighScore();
void drawMovingRoad();
void loadHighScore();
void saveHighScore();
void drawObstacleCar(int x, int y);
void drawInstructions(); // Added

void showStartScreen() {
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    setcolor(LIGHTCYAN);
    outtextxy(90, 150, (char*)"CAR RACING GAME");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(100, 250, (char*)"Press any key to Start...");
    Beep(700, 400);
    getch();
}

void showMenu() {
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    setcolor(WHITE);
    outtextxy(100, 150, (char*)"Select Difficulty:");
    outtextxy(100, 200, (char*)"1. Easy");
    outtextxy(100, 240, (char*)"2. Medium");
    outtextxy(100, 280, (char*)"3. Hard");
    outtextxy(100, 320, (char*)"Press Q to Quit");
}

int getLevelSelection() {
    while (true) {
        if (GetAsyncKeyState('1') & 0x8000) return 1;
        if (GetAsyncKeyState('2') & 0x8000) return 2;
        if (GetAsyncKeyState('3') & 0x8000) return 3;
        if (GetAsyncKeyState('Q') & 0x8000) return -1;
        delay(100);
    }
}

void setDifficulty(int choice) {
    if (choice == 1) speed = 3;
    else if (choice == 2) speed = 5;
    else if (choice == 3) speed = 8;
}

void drawCar(int x, int y) {
    setcolor(CAR_COLOR);
    setfillstyle(SOLID_FILL, CAR_COLOR);
    bar(x, y, x + 20, y + 40);

    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + 5, y + 10, x + 15, y + 30);

    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x + 3, y + 5, 2, 5);
    fillellipse(x + 17, y + 5, 2, 5);
    fillellipse(x + 3, y + 35, 2, 5);
    fillellipse(x + 17, y + 35, 2, 5);

    if (boostActive) {
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(x + 10, y + 45, 4, 8);
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(x + 10, y + 53, 2, 4);
    }
}

void drawObstacleCar(int x, int y) {
    setcolor(OBSTACLE_COLOR);
    setfillstyle(SOLID_FILL, OBSTACLE_COLOR);
    bar(x, y, x + 20, y + 40);

    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + 5, y + 10, x + 15, y + 30);

    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x + 3, y + 5, 2, 5);
    fillellipse(x + 17, y + 5, 2, 5);
    fillellipse(x + 3, y + 35, 2, 5);
    fillellipse(x + 17, y + 35, 2, 5);
}

void drawBoostBar(int x, int y) {
    setcolor(BOOST_COLOR);
    rectangle(x, y, x + 100, y + 20);
    if (boostActive) {
        setfillstyle(SOLID_FILL, BOOST_COLOR);
        bar(x + 1, y + 1, x + 1 + (100 * boostTimer / 50), y + 19);
    }
}

void drawNitroItems(int nitroX[], int nitroY[]) {
    for (int i = 0; i < NUM_NITRO; i++) {
        setcolor(NITRO_COLOR);
        setfillstyle(SOLID_FILL, NITRO_COLOR);
        bar(nitroX[i], nitroY[i], nitroX[i] + 20, nitroY[i] + 20);
        setcolor(WHITE);
        outtextxy(nitroX[i] + 5, nitroY[i] + 3, (char*)"N");
    }
}

void drawLaneBoundaries() {
    setcolor(WHITE);
    line(150, 0, 150, 480);
    line(200, 0, 200, 480);
    line(250, 0, 250, 480);
    line(300, 0, 300, 480);

    for (int i = 0; i < 480; i += 40) {
        line(225, i, 225, i + 20);
    }
}

void drawMovingRoad() {
    setcolor(WHITE);
    for (int i = 0; i < 480; i += 40) {
        line(175, i, 175, i + 20);
        line(225, i, 225, i + 20);
        line(275, i, 275, i + 20);
    }
}

void showGameOver() {
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    setcolor(RED);
    outtextxy(100, 200, (char*)"Game Over!");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    char buffer[50];
    sprintf(buffer, "Score: %d", score);
    outtextxy(120, 260, buffer);

    sprintf(buffer, "High Score: %d", highScore);
    outtextxy(120, 300, buffer);

    Beep(500, 700);
    getch();
}

void drawHighScore() {
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char buffer[50];
    sprintf(buffer, "High Score: %d", highScore);
    outtextxy(320, 10, buffer);
}

void drawInstructions() {
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(320, 50, (char*)"Instructions:");
    outtextxy(320, 70, (char*)"Arrow Left/Right: Move Car");
    outtextxy(320, 90, (char*)"N: Collect Nitro (Boost)");
    outtextxy(320, 110, (char*)"P: Pause/Resume");
    outtextxy(320, 130, (char*)"Q: Quit Game");
    outtextxy(320, 150, (char*)"Boost Bar Shows Boost Time");
}

void loadHighScore() {
    FILE *file = fopen("highscore.dat", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    } else {
        highScore = 0;
    }
}

void saveHighScore() {
    FILE *file = fopen("highscore.dat", "w");
    if (file != NULL) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

int main() {
    int gd = DETECT, gm;
    int obsX[NUM_OBSTACLES], obsY[NUM_OBSTACLES];
    int obsSpeed[NUM_OBSTACLES];
    int nitroX[NUM_NITRO], nitroY[NUM_NITRO];
    bool quitGame = false;

    initgraph(&gd, &gm, (char*)"");
    srand(time(0));

    loadHighScore();

    showStartScreen();
    showMenu();
    int choice = getLevelSelection();
    if (choice == -1) {
        closegraph();
        return 0;
    }
    setDifficulty(choice);
    baseSpeed = speed;

    for (int i = 0; i < NUM_OBSTACLES; i++) {
        obsX[i] = 150 + (rand() % 3) * 50;
        obsY[i] = -i * 150;
        obsSpeed[i] = speed + (rand() % 3);
    }

    for (int i = 0; i < NUM_NITRO; i++) {
        nitroX[i] = 150 + (rand() % 3) * 50;
        nitroY[i] = -i * 200 - 100;
    }

    while (!quitGame) {
        if (!paused) {
            cleardevice();
            drawMovingRoad();
            drawLaneBoundaries();
            drawCar(carX, carY);

            for (int i = 0; i < NUM_OBSTACLES; i++) {
                drawObstacleCar(obsX[i], obsY[i]);
                obsY[i] += obsSpeed[i];

                if (obsY[i] > 480) {
                    obsY[i] = 0;
                    obsX[i] = 150 + (rand() % 3) * 50;
                    obsSpeed[i] = speed + (rand() % 3);
                    score++;
                }

                if (carX < obsX[i] + 20 && carX + 20 > obsX[i] &&
                    carY < obsY[i] + 40 && carY + 40 > obsY[i]) {
                    if (!boostActive) {
                        lives--;
                        if (lives <= 0) {
                            if (score > highScore) highScore = score;
                            saveHighScore();
                            showGameOver();
                            quitGame = true;
                            break;
                        }
                    }
                    obsY[i] = 0;
                    obsX[i] = 150 + (rand() % 3) * 50;
                }
            }

            drawNitroItems(nitroX, nitroY);

            for (int i = 0; i < NUM_NITRO; i++) {
                nitroY[i] += speed;
                if (nitroY[i] > 480) {
                    nitroY[i] = 0;
                    nitroX[i] = 150 + (rand() % 3) * 50;
                }

                if (carX < nitroX[i] + 20 && carX + 20 > nitroX[i] &&
                    carY < nitroY[i] + 20 && carY + 20 > nitroY[i]) {
                    boostActive = true;
                    boostTimer = 50;
                    nitroX[i] = 150 + (rand() % 3) * 50;
                    nitroY[i] = -100;
                }
            }

            if (boostActive) {
                boostTimer--;
                if (boostTimer <= 0) {
                    boostActive = false;
                }
            }

            if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && carX > 150)
                carX -= 10;
            if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && carX < 280)
                carX += 10;

            if ((GetAsyncKeyState(VK_UP) & 0x8000))
                speed = baseSpeed + 3;
            else if ((GetAsyncKeyState(VK_DOWN) & 0x8000))
                speed = baseSpeed - 2;
            else if (!boostActive)
                speed = baseSpeed;

            setcolor(GREEN);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            char buffer[50];
            sprintf(buffer, "Score: %d", score);
            outtextxy(10, 10, buffer);

            sprintf(buffer, "Lives: %d", lives);
            outtextxy(10, 40, buffer);

            drawHighScore();
            drawBoostBar(10, 70);
            drawInstructions(); // ✅ Show instructions under boost bar

            Sleep(50);
        }

        if ((GetAsyncKeyState('P') & 0x8000)) {
            paused = !paused;
            Sleep(300);
        }
        if ((GetAsyncKeyState('Q') & 0x8000)) {
            quitGame = true;
        }
    }

    closegraph();
    return 0;
}
