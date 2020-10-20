#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"

static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;
} game_data;

class MyGame {

    bool isPlayer1 = false;
    bool isPlayer2 = false;
    bool canPickPlayer = true;

    private:
        SDL_Rect player1 = { 200, 0, 20, 60 };
        SDL_Rect player2 = { 580, 0, 20, 60 };

        SDL_Rect ball{ 0, 0, 20, 20 };

    public:
        std::vector<std::string> messages;

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);

        bool assignPlayer(SDL_Event& event);
};

#endif
