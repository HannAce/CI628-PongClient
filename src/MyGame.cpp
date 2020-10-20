#include "MyGame.h"

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
        }
    } else {
        std::cout << "Received: " << cmd << std::endl;
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

// Controls to move the bat up and down
void MyGame::input(SDL_Event& event) {
    if (isPlayer1 && canPickPlayer) {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        }
    }
    else if (isPlayer2 && canPickPlayer) {
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
}

void MyGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &player1);
    SDL_RenderDrawRect(renderer, &player2);
    SDL_RenderDrawRect(renderer, &ball);
}
