#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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


class Particle {
public:
    double x;
    double y;

    int size;

    SDL_Color colour;

    // Default constructor
    Particle(int x, int y, int size, SDL_Color colour);

    // Deconstructor
    ~Particle();

    //void drawParticle(SDL_Renderer* renderer);
    
};


class MyGame {

    bool isPlayer1 = false;
    bool isPlayer2 = false;
    bool canPickPlayer = true;
    bool player1Win = false;
    bool player2Win = false;

private:
    // Rectangles
    SDL_Rect player1 = { 200, 0, 20, 100 };
    SDL_Rect player2 = { 580, 0, 20, 100 };

    SDL_Rect ball = { 0, 0, 20, 20 };

    SDL_Rect background{ 0, 0, 800, 600 };

    // Textures
    SDL_Texture* backgroundP1Texture;
    SDL_Texture* backgroundP2Texture;
    SDL_Texture* backgroundNeutralTexture;

    SDL_Texture* batP1Texture;
    SDL_Texture* batP2Texture;

    SDL_Texture* ballP1Texture;
    SDL_Texture* ballP2Texture;
    SDL_Texture* ballNeutralTexture;

    SDL_Texture* textures[8] = { backgroundP1Texture, backgroundP2Texture, backgroundNeutralTexture, batP1Texture, batP2Texture,
        ballP1Texture, ballP2Texture, ballNeutralTexture };

    // Particles
    std::vector<Particle*> particles;

    // Fonts
    TTF_Font* font1;

    // Audio
    Mix_Chunk* pingSFX;
    Mix_Chunk* pongSFX;
    Mix_Chunk* victorySFX;

    Mix_Chunk* soundFX[3] = { pingSFX, pongSFX, victorySFX };

    // Colours
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    const SDL_Color black = { r = 0, g = 0, b = 0, a = 255 };
    const SDL_Color blue = { r = 0, g = 190, b = 255, a = 255 };
    const SDL_Color red = { r = 255, g = 85, b = 50, a = 255 };
    const SDL_Color white = { r = 255, g = 255, b = 255, a = 255 };

public:
    // default Constructor
    MyGame(SDL_Renderer* renderer);

    // deconstructor
    ~MyGame();

        std::vector<std::string> messages;

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);

        // Player
        bool assignPlayer(SDL_Event& event);
        bool playerWin();

        // Textures
        // Texture and Text functions written with reference to Almas Baimagambetov, https://github.com/AlmasB/xcube2d, under the GNU General Public License v2.0
        SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string fileName);
        void drawTexture(SDL_Renderer*, SDL_Texture* texture, SDL_Rect* dst, SDL_RendererFlip flip);
        void drawText(SDL_Renderer* renderer, const std::string& text, const int& x, const int& y, TTF_Font* font, SDL_Color colour);
        SDL_Texture* createTextureFromString(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color colour);

        // Particle Effects
        //void drawParticles(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color colour);
    
};

#endif
