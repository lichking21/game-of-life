#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <chrono>
#include <thread>
#include <SDL.h>

using namespace std;

const int width = 800;
const int height = 640;

bool paused = false;
int cellSize;
int dencity;
int rows;
int cols;

vector<vector<bool>> field;
vector<vector<bool>> nextField;

void createWindow(SDL_Window*& window, SDL_Renderer*& render) {
    window = SDL_CreateWindow(
        "Game of life",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN
    );

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void startGeneration() {
    srand(time(NULL));

    rows = height / cellSize;
    cols = width / cellSize;

    field.resize(rows, vector<bool>(cols, false));
    nextField.resize(rows, vector<bool>(cols, false));

    for (int x = 0; x < cols; ++x) {
        for (int y = 0; y < rows; ++y) {
            int rnd = rand() % (dencity + 1);
            field[y][x] = rnd == 0;
        }
    }
}

int countNeightbor(int y, int x) {
    int count = 0;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int col = (x + i + cols) % cols;
            int row = (y + j + rows) % rows;

            bool selfCheck = col == x && row == y;
            bool hasLife = field[row][col];

            if (hasLife && !selfCheck)
                count++;
        }
    }

    return count;
}
void nextGen() {
    for (int x = 0; x < cols; ++x) {
        for (int y = 0; y < rows; ++y) {
            int neightborCount = countNeightbor(y, x);
            bool hasLife = field[y][x];

            if (!hasLife && neightborCount == 3)
                nextField[y][x] = true;
            else if (hasLife && (neightborCount < 2 || neightborCount > 3))
                nextField[y][x] = false;
            else
                nextField[y][x] = field[y][x];
        }
    }
    field = nextField;
}

void draw(SDL_Renderer* render) {
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    
    SDL_SetRenderDrawColor(render, 0, 140, 0, 255);

    SDL_Rect rect;
    rect.h = cellSize;
    rect.w = cellSize;

    for (int x = 0; x < cols; ++x) {
        for (int y = 0; y < rows; ++y) {
            if (field[y][x]) {
                rect.x = x * cellSize;
                rect.y = y * cellSize;
                SDL_RenderFillRect(render, &rect);
            }
        }
    }

    SDL_RenderPresent(render);
}


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO); SDL_Window* window = nullptr; SDL_Renderer* render = nullptr;

    cout << "Enter cell size...\t"; cin >> cellSize; cout << endl;
    cout << "Enter dencity...\t"; cin >> dencity; cout << endl;
    if (cellSize > 0 && dencity > 0) {
        createWindow(window, render);
        startGeneration();
    }
    
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            //
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    paused = !paused;
                }

                if (paused) {
                    cout << "Enter cell size...\t"; cin >> cellSize; cout << endl;
                    cout << "Enter dencity...\t"; cin >> dencity; cout << endl;
                    if (cellSize > 0 && dencity > 0) {
                        field.clear();
                        nextField.clear();
                        startGeneration();
                    }
                }
            }
        }
        if (!paused) {
            draw(render);
            nextGen();
        }
        else
            this_thread::sleep_for(chrono::milliseconds(100));

    }
    SDL_Quit();

    cout << endl;
    system("pause");
    return 0;
}

