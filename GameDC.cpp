#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int screenwidth = 640;
const int screenheight = 480;
const int roomminsize = 5;
const int roommaxsize = 20;
const int maxenemies = 10;
const int maxitems = 10;
const int maxtreasures = 5;

enum direction { up, down, left, right };
enum entitytype { player, enemy, item, treasure, exitgame };

struct entity {
    int x, y;
    entitytype type;
};

struct room {
    int x, y;
    int width, height;
};

class healthbar {
public:
    healthbar(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
    void render(SDL_Renderer* renderer, int health) {
        SDL_Rect rect{x, y, width, height};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        rect.w = (int)((float)width * health / 100);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
private:
    int x, y, width, height;
};

class inventory {
public:
    inventory(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
    void render(SDL_Renderer* renderer, vector<entitytype> items) {
        SDL_Rect rect{x, y, width, height};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        for (int i = 0; i < items.size(); i++) {
            SDL_Rect itemrect{x + i * 32, y, 32, 32};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &itemrect);
        }
    }
private:
    int x, y, width, height;
};

class dungeon {
public:
    dungeon() : playerx(screenwidth / 2), playery(screenheight / 2), playerhp(100), 
                healthbar(10, 10, 200, 20), inventory(10, 40, 200, 100) {}

    void init();
    void handleevent(SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer);
    void generatedungeon();
    bool checkcollision(int x1, int y1, int x2, int y2);
    void moveplayer(direction dir);
    
    SDL_Texture* playertexture;
    SDL_Texture* enemytexture;
    SDL_Texture* itemtexture;
    SDL_Texture* treasuretexture;
    SDL_Texture* exittexture;
    SDL_Renderer* renderer;
    SDL_Window* window;
private:
    int playerx, playery;
    int playerhp;
    vector<room> rooms;
    vector<entity> enemies;
    vector<entity> items;
    vector<entity> treasures;
    vector<entitytype> playerinventory;
    entity exitgame;
    healthbar healthbar;
    inventory inventory;
};

void dungeon::init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Dungeon Crawler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenwidth, screenheight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    playertexture = IMG_LoadTexture(renderer, "player.png");
    enemytexture = IMG_LoadTexture(renderer, "enemy.png");
    itemtexture = IMG_LoadTexture(renderer, "item.png");
    treasuretexture = IMG_LoadTexture(renderer, "treasure.png");
    exittexture = IMG_LoadTexture(renderer, "exit.png");

    generatedungeon();
}

void dungeon::handleevent(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                moveplayer(up);
                break;
            case SDLK_DOWN:
                moveplayer(down);
                break;
            case SDLK_LEFT:
                moveplayer(left);
                break;
            case SDLK_RIGHT:
                moveplayer(right);
                break;
        }
    }
}

void dungeon::update() {
    // Update enemies
    for (auto& enemy : enemies) {
        enemy.x += rand() % 3 - 1;
        enemy.y += rand() % 3 - 1;
    }

    // Check for collisions
    for (auto& enemy : enemies) {
        if (checkcollision(playerx, playery, enemy.x, enemy.y)) {
            playerhp -= 10;
        }
    }

    // Check for item pickups
    for (auto& item : items) {
        if (checkcollision(playerx, playery, item.x, item.y)) {
            playerinventory.push_back(item.type);
            items.erase(remove(items.begin(), items.end(), item), items.end());
        }
    }

    // Check for treasure pickups
    for (auto& treasure : treasures) {
        if (checkcollision(playerx, playery, treasure.x, treasure.y)) {
            playerinventory.push_back(treasure.type);
            treasures.erase(remove(treasures.begin(), treasures.end(), treasure), treasures.end());
        }
    }

    // Check for exit
    if (checkcollision(playerx, playery, exitgame.x, exitgame.y)) {
        generatedungeon();
    }
}

void dungeon::render(SDL_Renderer* renderer) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw rooms
    for (auto& room : rooms) {
        SDL_Rect rect{room.x, room.y, room.width, room.height};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Draw player
    SDL_Rect playerrect{playerx, playery, 32, 32};
    SDL_RenderCopy(renderer, playertexture, NULL, &playerrect);

    // Draw enemies
    for (auto& enemy : enemies) {
        SDL_Rect enemyrect{enemy.x, enemy.y, 32, 32};
        SDL_RenderCopy(renderer, enemytexture, NULL, &enemyrect);
    }

    // Draw items
    for (auto& item : items) {
        SDL_Rect itemrect{item.x, item.y, 32, 32};
        SDL_RenderCopy(renderer, itemtexture, NULL, &itemrect);
    }

    // Draw treasures
    for (auto& treasure : treasures) {
        SDL_Rect treasurerect{treasure.x, treasure.y, 32, 32};
        SDL_RenderCopy(renderer, treasuretexture, NULL, &treasurerect);
    }

    // Draw exit
    SDL_Rect exitrect{exitgame.x, exitgame.y, 32, 32};
    SDL_RenderCopy(renderer, exittexture, NULL, &exitrect);

    // Draw health bar
    healthbar.render(renderer, playerhp);

    // Draw inventory
    inventory.render(renderer, playerinventory);

    // Update screen
    SDL_RenderPresent(renderer);
}

void dungeon::generatedungeon() {
    // Clear rooms, enemies, items, treasures, and exit
    rooms.clear();
    enemies.clear();
    items.clear();
    treasures.clear();
    exitgame = entity();

    // Generate rooms
    for (int i = 0; i < 10; i++) {
        room room;
        room.x = rand() % (screenwidth - roommaxsize);
        room.y = rand() % (screenheight - roommaxsize);
        room.width = rand() % (roommaxsize - roomminsize) + roomminsize;
        room.height = rand() % (roommaxsize - roomminsize) + roomminsize;
        rooms.push_back(room);
    }

    // Generate enemies
    for (int i = 0; i < maxenemies; i++) {
        entity enemy;
        enemy.x = rand() % screenwidth;
        enemy.y = rand() % screenheight;
        enemy.type = entitytype::enemy;
        enemies.push_back(enemy);
    }

    // Generate items
    for (int i = 0; i < maxitems; i++) {
        entity item;
        item.x = rand() % screenwidth;
        item.y = rand() % screenheight;
        item.type = entitytype::item;
        items.push_back(item);
    }

    // Generate treasures
    for (int i = 0; i < maxtreasures; i++) {
        entity treasure;
        treasure.x = rand() % screenwidth;
        treasure.y = rand() % screenheight;
        treasure.type = entitytype::treasure;
        treasures.push_back(treasure);
    }

    // Generate exit
    exitgame.x = rand() % screenwidth;
    exitgame.y = rand() % screenheight;
    exitgame.type = entitytype::exitgame;
}

bool dungeon::checkcollision(int x1, int y1, int x2, int y2) {
    return (x1 + 32 > x2 && x1 < x2 + 32 && y1 + 32 > y2 && y1 < y2 + 32);
}

void dungeon::moveplayer(direction dir) {
    switch (dir) {
        case up:
            playery -= 32;
            break;
        case down:
            playery += 32;
            break;
        case left:
            playerx -= 32;
            break;
        case right:
            playerx += 32;
            break;
    }
}

int main(int argc, char* args[]) {
    srand(static_cast<unsigned int>(time(0)));

    dungeon dungeon;
    dungeon.init();

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else {
                dungeon.handleevent(event);
            }
        }

        dungeon.update();
        dungeon.render(dungeon.renderer);
    }

    SDL_DestroyTexture(dungeon.playertexture);
    SDL_DestroyTexture(dungeon.enemytexture);
    SDL_DestroyTexture(dungeon.itemtexture);
    SDL_DestroyTexture(dungeon.treasuretexture);
    SDL_DestroyTexture(dungeon.exittexture);
    SDL_DestroyRenderer(dungeon.renderer);
    SDL_DestroyWindow(dungeon.window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}