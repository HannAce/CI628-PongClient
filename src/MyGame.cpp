#include "MyGame.h"

Particle::Particle(double x, double y, double velocityX, double velocityY, double accelerateX, double accelerateY,
    int size, SDL_Color colour) {
    this->x = x;
    this->y = y;
    this->velocityX = velocityX;
    this->velocityY = velocityY;
    this->accelerateX = accelerateX;
    this->accelerateY = accelerateY;
    this->size = size;
    this->life = 1.0;
    this->colour = colour;
}

MyGame::MyGame(SDL_Renderer* renderer) {

    // Load and check font
    font1 = TTF_OpenFont("res/Goldman-Bold.ttf", 30);

    if (font1 != nullptr) {
        std::cout << "Loaded font" << std::endl;
    }
    else {
        std::cout << "Unable to load font" << std::endl;
        printf("TTF_OpenFont: %s\n", TTF_GetError()); // handle error
    }

    // Open audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        printf("TTF_OpenAudio: %s\n", TTF_GetError()); // handle error
        exit(2);
    }
    else {
        std::cout << "Opened audio" << std::endl;
    }

    // Load and check sounds
    pingSFX = Mix_LoadWAV("res/audio/ping.wav");
    pongSFX = Mix_LoadWAV("res/audio/pong.wav");
    victorySFX = Mix_LoadWAV("res/audio/victory.wav");

    for (int i = 0; i < 3; i++) {
        if (soundFX[i] != nullptr) {
            std::cout << "Loaded sound " << i << std::endl;
        }
        else {
            std::cout << "Unable to load sound" << std::endl;
            printf("Mix_LoadWAV: %s\n", Mix_GetError()); // handle error
        }
    }

    // Load and check textures
    backgroundP1Texture = loadTexture(renderer, "res/backgroundP1.png");
    backgroundP2Texture = loadTexture(renderer, "res/backgroundP2.png");
    backgroundNeutralTexture = loadTexture(renderer, "res/backgroundNeutral.png");
    batP1Texture = loadTexture(renderer, "res/bat1.png");
    batP2Texture = loadTexture(renderer, "res/bat2.png");
    ballP1Texture = loadTexture(renderer, "res/ballP1.png");
    ballP2Texture = loadTexture(renderer, "res/ballP2.png");
    ballNeutralTexture = loadTexture(renderer, "res/ballNeutral.png");

    for (int i = 0; i < 7; i++) {
        if (textures[i] != nullptr) {
            std::cout << "Loaded texture " << i << std::endl;
        }
        else {
            std::cout << "Unable to load texture " << i << std::endl;
            printf("loadTexture: %s\n", SDL_GetError()); // handle error
        }
    }
}

MyGame::~MyGame() {
    TTF_CloseFont(font1);
    Mix_CloseAudio();
    for (int i = 0; i < 7; i++) {
        SDL_DestroyTexture(textures[i]);
    }
}

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {

    if (cmd == "GAME_DATA") {

        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
        }
    }
    else {
        std::cout << "Received: " << cmd << std::endl;
    }

    // Save scores as variables
    if (cmd == "SCORES" && args.size() == 2) {
        game_data.score1 = (args.at(0));
        game_data.score2 = args.at(1);
    }

    if (cmd == "BALL_HIT_BAT1" || cmd == "BALL_HIT_BAT2") {
        // Play sound when ball hits bat
        if (Mix_PlayChannel(-1, pingSFX, 0) == -1) {
            printf("Mix_PlayChannel: %s\n", Mix_GetError()); // handle error
        }
    }

    if (cmd == "HIT_WALL_LEFTGAME_DATA" || cmd == "HIT_WALL_RIGHTGAME_DATA") {
        // Play sound when ball hits left or right wall
        if (Mix_PlayChannel(-1, pongSFX, 0) == -1) {
            printf("Mix_PlayChannel: %s\n", Mix_GetError()); // handle error
        }
    }

    if (cmd == "GAME_WONP1HIT_WALL_RIGHTGAME_DATA" || cmd == "GAME_WONP2HIT_WALL_LEFTGAME_DATA") {
        // Play victory sound when game is won
        if (Mix_PlayChannel(-1, victorySFX, 0) == -1) {
            printf("Mix_PlayChannel: %s\n", Mix_GetError()); // handle error
        }
    }
}

void MyGame::send(std::string message) {

    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {

    if (isPlayer1 && canPickPlayer == false) {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_i:
            send("Wrong input, pleae use Player 1 controls: W and S");
            break;
        case SDLK_k:
            send("Wrong input, pleae use Player 1 controls: W and S");
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
        case SDLK_w:
            send("Wrong input, pleae use Player 2 controls: I and K");
            break;
        case SDLK_s:
            send("Wrong input, pleae use Player 2 controls: I and K");
            break;
        }
    }

    assignPlayer(event);
}

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

bool MyGame::playerWin() {
    if (game_data.score1 == "10") {
        player1Win = true;
        return player1Win;
    }
    else if (game_data.score2 == "10") {
        player2Win = true;
        return player2Win;
    }
    else {
        return false;
    }
}

void MyGame::update() {

    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    ball.x = game_data.ballX;
    ball.y = game_data.ballY;

    playerWin();
    particleLife();

    double velocityX = getRandomValue() * 0.5;
    double velocityY = -1 * getRandomValue();

    double accelerateX = (getRandomValue() - 0.5) * 0.05;
    double accelerateY = -0.1 * getRandomValue();

    for (int i = 0; i < 100; i++) {
        if (player1Win) {
            particles.push_back(new Particle(250 + getRandomValue(), 300 + getRandomValue(), velocityX, velocityY, accelerateX,
                accelerateY, 1, blue));
            particles.push_back(new Particle(550 + getRandomValue(), 300 + getRandomValue(), velocityX, velocityY, accelerateX,
                accelerateY, 1, blue));
        }
        else if (player2Win) {
            particles.push_back(new Particle(250 + getRandomValue(), 300 + getRandomValue(), velocityX, velocityY, accelerateX,
                accelerateY, 1, red));
            particles.push_back(new Particle(550 + getRandomValue(), 300 + getRandomValue(), velocityX, velocityY, accelerateX,
                accelerateY, 1, red));
        }
    }
}

void MyGame::render(SDL_Renderer* renderer) {

    int score1 = std::stoi(game_data.score1);
    int score2 = std::stoi(game_data.score2);

    // different coloured ball depending on which player is winning
    if (score1 > score2 || player1Win) {
        drawTexture(renderer, backgroundP1Texture, &background, SDL_FLIP_NONE);
        drawTexture(renderer, ballP1Texture, &ball, SDL_FLIP_NONE);
    }
    else if (score2 > score1 || player2Win) {
        drawTexture(renderer, backgroundP2Texture, &background, SDL_FLIP_NONE);
        drawTexture(renderer, ballP2Texture, &ball, SDL_FLIP_NONE);
    }
    else {
        drawTexture(renderer, backgroundNeutralTexture, &background, SDL_FLIP_NONE);
        drawTexture(renderer, ballNeutralTexture, &ball, SDL_FLIP_NONE);
    }

    if (canPickPlayer && !player1Win && !player2Win) {
        drawText(renderer, "Please press 1 or 2 to pick a player.", 0, 0, font1, white);
    }

    drawText(renderer, "Player 1: " + game_data.score1, 20, 70, font1, blue);
    drawText(renderer, "Player 2: " + game_data.score2, 580, 70, font1, red);

    drawTexture(renderer, batP1Texture, &player1, SDL_FLIP_NONE);
    drawTexture(renderer, batP2Texture, &player2, SDL_FLIP_NONE);

    if (player1Win) {
        drawText(renderer, "Player 1 wins!", 285, 280, font1, white);
        drawParticles(renderer);
    }
    else if (player2Win) {
        drawText(renderer, "Player 2 wins!", 285, 280, font1, white);
        drawParticles(renderer);
    }
}

double MyGame::getRandomValue() {
    return rand() * 1.0 / RAND_MAX;
}

// Texture and Text functions written with reference to Almas Baimagambetov, https://github.com/AlmasB/xcube2d,
// under the GNU General Public License v2.0
SDL_Texture* MyGame::loadTexture(SDL_Renderer* renderer, std::string fileName) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* surface = IMG_Load(fileName.c_str());
    newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Memory management
    return newTexture;
}

void MyGame::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* dst, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(renderer, texture, 0, dst, 0.0, 0, flip);
}

void MyGame::drawText(SDL_Renderer* renderer, const std::string& text, const int& x, const int& y, TTF_Font* font, SDL_Color colour) {
    SDL_Texture* textTexture = createTextureFromString(renderer, text, font, colour);
    int w, h;
    SDL_QueryTexture(textTexture, 0, 0, &w, &h);
    SDL_Rect dst = { x, y, w, h };
    drawTexture(renderer, textTexture, &dst, SDL_FLIP_NONE);
    SDL_DestroyTexture(textTexture); // Memory management
}

SDL_Texture* MyGame::createTextureFromString(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color colour) {
    SDL_Texture* textTexture = nullptr;
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), colour);
    if (textSurface != nullptr) {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface); // Memory management
    }
    else {
        std::cout << "Could not create texture from string: " << text << std::endl;
        std::cout << TTF_GetError() << std::endl;
    }

    return textTexture;
}

void MyGame::drawParticles(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    for (auto particle : particles) {
        SDL_Rect particleRect = { (int)particle->x, (int)particle->y, particle->size * 2, particle->size * 2 };

        SDL_SetRenderDrawColor(renderer, particle->colour.r, particle->colour.g, particle->colour.b, particle->colour.a);
        SDL_RenderFillRect(renderer, &particleRect);
    }
}

void MyGame::particleLife() {
    for (auto particle : particles) {
        particle->x += particle->velocityX;
        particle->y += particle->velocityY;

        particle->velocityX += particle->accelerateX;
        particle->velocityY += particle->accelerateY;

        particle->life -= 0.016;

        if (particle->life <= 0.0) {
            particle->colour.a = 0;
        }
        else {
            particle->colour.a = (Uint8)((particle->life / 1.0) * 255);
        }
    }
}

