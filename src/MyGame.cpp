#include "MyGame.h"

MyGame::MyGame(SDL_Renderer* renderer) {

    TTF_Font* font1 = TTF_OpenFont("res/Goldman-Bold.ttf", 30);

    batTexture = loadTexture(renderer, "res/bat.png");
    ballTexture = loadTexture(renderer, "res/ball.png");
    score0Texture = loadTexture(renderer, "res/Score0.png");
    score1Texture = loadTexture(renderer, "res/Score1.png");
    score2Texture = loadTexture(renderer, "res/Score2.png");
    score3Texture = loadTexture(renderer, "res/Score3.png");
    
    // Check assets have loaded
    if (font1 != nullptr) {
        std::cout << "loaded font" << std::endl;
    }
    if (batTexture != nullptr) {
        std::cout << "loaded texture" << std::endl;
    }
}

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {

    if (cmd == "GAME_DATA") {
        // should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
        }
    } else {
        std::cout << "Received: " << cmd << std::endl;
        //for (int i = 0; i < args.size(); i++) {
           // std::cout << "Received: " << args.at(i) << std::endl;
       // }
    }
    if (cmd == "SCORES" && args.size() == 2) {
        game_data.score1 = (args.at(0));
        game_data.score2 = args.at(1);
    }
}

void MyGame::send(std::string message) {

    messages.push_back(message);
}

// Controls to move the bat up and down depending on which player
void MyGame::input(SDL_Event& event) {
    
    if (isPlayer1 && canPickPlayer == false) {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        }
    }
    else if (isPlayer2 && canPickPlayer == false) {
        switch (event.key.keysym.sym) {
        case SDLK_i:
            send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
            break;
        case SDLK_k:
            send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
            break;
        }
    }
    else { // Safety net
        send("Not a valid player");
    }

    assignPlayer(event);
}

// assigns player 1 or 2 depending on input at start of game
bool MyGame::assignPlayer(SDL_Event& event) {

    if (canPickPlayer) {
        switch (event.key.keysym.sym) {
        case SDLK_1:
            isPlayer1 = true;
            isPlayer2 = false;
            canPickPlayer = false;

            return isPlayer1;
            break;

        case SDLK_2:
            isPlayer2 = true;
            isPlayer1 = false;
            canPickPlayer = false;

            return isPlayer2;
            break;
        }
    }
}

void MyGame::update() {
    
    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    ball.x = game_data.ballX;
    ball.y = game_data.ballY;

   // std::cout << game_data.score1 << std::endl;
   // std::cout << game_data.score2 << std::endl;
}

void MyGame::render(SDL_Renderer* renderer) {

    drawTexture(renderer, batTexture, &player1, SDL_FLIP_NONE);
    drawTexture(renderer, batTexture, &player2, SDL_FLIP_NONE);
    drawTexture(renderer, ballTexture, &ball, SDL_FLIP_NONE);
    drawTexture(renderer, score0Texture, &scoreP1, SDL_FLIP_NONE);
    drawTexture(renderer, score0Texture, &scoreP2, SDL_FLIP_NONE);
    

    //drawText(renderer, game_data.score1, 20, 20, font1);
    //drawText(renderer, game_data.score2, 80, 80, font1);
}

// Loads textures to add to the game
SDL_Texture* MyGame::loadTexture(SDL_Renderer* renderer, std::string fileName) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(fileName.c_str());
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

// Draws textures to the screen
void MyGame::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* dst, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(renderer, texture, 0, dst, 0.0, 0, flip);
}

// Draws text to the screen
void MyGame::drawText(SDL_Renderer* renderer, const std::string& text, const int& x, const int& y, TTF_Font* font, SDL_Color colour) {
    SDL_Texture* textTexture = createTextureFromString(renderer, text, font, colour);
    int w, h;
    SDL_QueryTexture(textTexture, 0, 0, &w, &h);
    SDL_Rect dst = { x, y, w, h };
    drawTexture(renderer, textTexture, &scoreP1, SDL_FLIP_NONE);
    SDL_DestroyTexture(textTexture);
}

// Turns strings into textures to be drawn to screen
SDL_Texture* MyGame::createTextureFromString(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color colour) {
    SDL_Texture* textTexture = nullptr;
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), colour);
    if (textSurface != nullptr) {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
    }
    else {
        std::cout << "Failed to create texture from string: " << text << std::endl;
        std::cout << TTF_GetError() << std::endl;
    }

    return textTexture;
    
}
