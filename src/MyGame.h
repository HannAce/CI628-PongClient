#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

#include "SDL.h"


static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;

    std::string score1 = "0";
    std::string score2 = "0";
} game_data;

class MyGame {

    bool isPlayer1 = false;
    bool isPlayer2 = false;
    bool canPickPlayer = true;

private:
    // Rectangles
    SDL_Rect player1 = { 200, 0, 20, 60 };
    SDL_Rect player2 = { 580, 0, 20, 60 };

    SDL_Rect ball = { 0, 0, 20, 20 };

    SDL_Rect scoreP1 = { 500, 70, 110, 120 };
    SDL_Rect scoreP2 = { 200, 70, 110, 120 };

    // Textures
    SDL_Texture* batP1Texture;
    SDL_Texture* batP2Texture;
    SDL_Texture* ballP1Texture;
    SDL_Texture* ballP2Texture;
    SDL_Texture* ballNeutralTexture;

    SDL_Texture* score0Texture;

    // Fonts
    TTF_Font* font1;

    // Colours
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    const SDL_Color blue = { r = 0, g = 0, b = 255, a = 255 };
    const SDL_Color red = { r = 255, g = 0, b = 0, a = 255 };
    const SDL_Color white = { r = 255, g = 255, b = 255, a = 255 };

public:
    // default Constructor
    MyGame(SDL_Renderer* renderer);

        std::vector<std::string> messages;

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);

        // Player
        bool assignPlayer(SDL_Event& event);

        // Textures
        SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string fileName);
        void drawTexture(SDL_Renderer*, SDL_Texture* texture, SDL_Rect* dst, SDL_RendererFlip flip);
        void drawText(SDL_Renderer* renderer, const std::string& text, const int& x, const int& y, TTF_Font* font, SDL_Color colour);
        SDL_Texture* createTextureFromString(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color colour);
    
};

#endif
