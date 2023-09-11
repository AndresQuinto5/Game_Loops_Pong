#include <iostream>
#include "game.h"
#include <SDL2/SDL.h>


SDL_Rect ball;
SDL_Rect paddlePlayer1;
SDL_Rect paddlePlayer2;
int ball_speed = 3;
int player_speed = 30;


Game::Game (){
    counter = 0;
    FPS = 60;
    frameDuration = (1.0f / FPS) * 1000.0f; //miliseconds
    counter = 0;
}

Game::~Game (){

}
void Game::init(const char* title, int width, int height){
    std::cout << "Game Setup..." << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 0,0, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    isRunning = true;
    window_width = width;
    window_height = height;
};

void Game::setup(){
    std::cout << "Game Setup..." << std::endl;
    ball.x = 300;
    ball.y = 20;
    ball.w = 15;
    ball.h = 15;

    paddlePlayer1.w = 20;
    paddlePlayer1.h = 100; 
    paddlePlayer1.x = 0;
    paddlePlayer1.y = (window_height / 2) - (paddlePlayer1.h / 2);

    paddlePlayer2.w = 20;
    paddlePlayer2.h = 100;
    paddlePlayer2.x = window_width - paddlePlayer2.w;
    paddlePlayer2.y = (window_height / 2) - (paddlePlayer2.h / 2);
};

void Game::frameStart(){

    std::cout << "--- Frame: " << counter << " ---" << std::endl;

    frameStartTimestamp = SDL_GetTicks();

    dT = frameEndTimeStamp - frameStartTimestamp;

};

void Game::frameEnd(){
    std::cout << "--- ---" << std::endl;
    frameEndTimeStamp = SDL_GetTicks();
    float actualFrameDuration = frameEndTimeStamp - frameStartTimestamp;
    if(actualFrameDuration < frameDuration){
        SDL_Delay(frameDuration - actualFrameDuration);
    }
    counter++;
    std::cout << " " << std::endl;
};


// Managing key pressing
void Game::handleEvents(){
    std::cout << "Game Handling events..." << std::endl;
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        // Paddle movement
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                // PLAYER 1 (left): uses w and s keys
                case SDLK_w:
                    if(paddlePlayer1.y - player_speed >= 0){
                        paddlePlayer1.y -= player_speed;
                    }
                    break;
                case SDLK_s:
                    if(paddlePlayer1.y + paddlePlayer1.h + player_speed <= window_height){
                        paddlePlayer1.y += player_speed;
                    }
                    break;

                // PLAYER 2 (right): uses up and down arrows
                case SDLK_UP:
                    if(paddlePlayer2.y - player_speed >= 0){
                        paddlePlayer2.y -= player_speed;
                    }
                    break;
                case SDLK_DOWN:
                    if(paddlePlayer2.y + paddlePlayer2.h + player_speed <= window_height){
                        paddlePlayer2.y += player_speed;
                    }
                    break;
            }
        }
    }
};
int dx = -1;
int dy = 1;

// GAME LOGIC
void Game::update(){
    std::cout << "Game Updating..." << std::endl;

    // if it crashes right or left side of the window
    if((ball.x + ball.w) >= window_width || ball.x <= 0){
        isRunning = false;
        if (ball.x + ball.w >= window_width) {
            std::cout << "Player 1 wins ..." << std::endl;
        } else if (ball.x <= 0) {
            std::cout << "Player 2 wins ..." << std::endl;
        }
    }
    
    // if it crashes top or bottom of the window
    if((ball.y + ball.h) >= window_height || ball.y <= 0){
        dy *= -1;
        ball_speed *= 1.15;  // Aumenta la velocidad
        std::cout << "ADD VEL" << std::endl;
    }

    // if it hits player's 1 paddle
    if(ball.y + ball.h >= paddlePlayer1.y && ball.y <= paddlePlayer1.y + paddlePlayer1.h && ball.x <= paddlePlayer1.x + paddlePlayer1.w){
        dx *= -1.2;
        ball_speed *= 1.15;  // Aumenta la velocidad
        std::cout << "ADD VEL" << std::endl;

    }

    // if it hits player's 2 paddle
    if(ball.y  <= paddlePlayer2.y + paddlePlayer2.h && ball.y + ball.h >= paddlePlayer2.y && ball.x + ball.w >= paddlePlayer2.x){
        dx *= -1.2;
        ball_speed *= 1.15;  // Aumenta la velocidad
        std::cout << "ADD VEL" << std::endl;

    }

    // ball movement
    ball.x += ball_speed * dx;
    ball.y += ball_speed * dy;
};



void Game::render(){
    std::cout << "Game Rendering..." << std::endl;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderFillRect(renderer,&paddlePlayer1);
    SDL_RenderFillRect(renderer,&paddlePlayer2);
    SDL_RenderPresent(renderer);
};

void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Over..." << std::endl;
};

bool Game::running(){
    return isRunning; 
};