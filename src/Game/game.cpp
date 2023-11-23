#include <iostream>
#include "game.h"

#include "../entt/entt.hpp"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
int speed = 2;
const int MANUAL_PADDLE_SPEED = 50;  // Velocidad para el paddle controlado manualmente
const int AI_PADDLE_SPEED = 500;     // Velocidad para el paddle controlado por la IA


entt::registry mRegistry;
entt::entity paddle1, paddle2;

struct PositionComponent {
    int x, y;
};

struct VelocityComponent {
    int x, y;
};

struct CubeComponent {
    int w, h;
};

struct PlayerComponent{
    int playerID;
};

struct BallComponent{
};

PositionComponent playerSpawnPosition = {20, 20};
VelocityComponent playerSpawnVelocity = {200, 200};
CubeComponent playerRect = {200, 20};

void createPlayerEntity(PositionComponent pos, VelocityComponent vel, CubeComponent rect, PlayerComponent player)
{
    const entt::entity e = mRegistry.create();
    mRegistry.emplace<PositionComponent>(e, pos);
    mRegistry.emplace<VelocityComponent>(e, vel);
    mRegistry.emplace<CubeComponent>(e, rect);  

    mRegistry.emplace<PlayerComponent>(e, player);  

    if (player.playerID == 1){
        paddle1 = e;
    }
    else{
        paddle2 = e;
    }



}
void createBallEntity(PositionComponent pos, VelocityComponent vel, CubeComponent rect)
{
    const entt::entity e = mRegistry.create();
    mRegistry.emplace<PositionComponent>(e, pos);
    mRegistry.emplace<VelocityComponent>(e, vel);
    mRegistry.emplace<CubeComponent>(e, rect);  

    mRegistry.emplace<BallComponent>(e); 

}


float dx = 1.0f;
float dy = 1.0f;
bool playing = true;
bool player1Won = false;

void ballMovementSystem(float dT) {
    PositionComponent& pad1Position = mRegistry.get<PositionComponent>(paddle1);
    CubeComponent& pad1Rect = mRegistry.get<CubeComponent>(paddle1);
    PositionComponent& pad2Position = mRegistry.get<PositionComponent>(paddle2);
    CubeComponent& pad2Rect = mRegistry.get<CubeComponent>(paddle2);
    
    auto view2 = mRegistry.view<BallComponent, PositionComponent, CubeComponent>();
    for (const entt::entity e : view2) {
        PositionComponent& pos = view2.get<PositionComponent>(e);
        CubeComponent& rect = view2.get<CubeComponent>(e);

        // Actualizar la posición de la pelota usando DELTA TIME
        pos.x += speed * dx;
        pos.y += speed * dy;


        // Si la pelota toca la parte superior o inferior de la ventana
        if (pos.y + rect.h >= SCREEN_HEIGHT || pos.y <= 0){
            dy *= -1.15f;  // Invierte la dirección y aumenta la velocidad
        }

        // Si la pelota toca los lados de la ventana
        if (pos.x + rect.w >= SCREEN_WIDTH){
            playing = false;
            player1Won = true;
        }
        if (pos.x <= 0){
            playing = false;
            player1Won = false;
        }

        // Colisión con el paddle 1
        if (pos.y + rect.h >= pad1Position.y && pos.y <= pad1Position.y + pad1Rect.h && pos.x <= pad1Position.x + pad1Rect.w && pos.x + rect.w >= pad1Position.x){
            dx *= -1.05f;  // Invierte la dirección y aumenta la velocidad
        }

        // Colisión con el paddle 2
        if (pos.y <= pad2Position.y + pad2Rect.h && pos.y + rect.h >= pad2Position.y && pos.x <= pad2Position.x + pad2Rect.w && pos.x + rect.w >= pad2Position.x){
            dx *= -1.05f;  // Invierte la dirección y aumenta la velocidad
        }

        // Actualizar la posición de la pelota de nuevo, esto puede ser opcional según cómo estés manejando tu bucle principal
        pos.x += speed * dx;
        pos.y += speed * dy;
    }
}

void cubeRenderSystem(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

    const auto view = mRegistry.view<PositionComponent, CubeComponent>();
    for (const entt::entity e : view) {
        const PositionComponent position = view.get<PositionComponent>(e);
        const CubeComponent cube = view.get<CubeComponent>(e);

        // Verificar si la entidad es uno de los paddles y, de ser así, imprimir su posición
        if (e == paddle1 || e == paddle2) {
            std::cout << "Rendering paddle at position Y: " << position.y << std::endl;
        }

        SDL_Rect rect = { position.x, position.y, cube.w, cube.h };
        SDL_RenderFillRect(renderer, &rect);
    }
}


Game::Game ()
{
    std::cout << "WELCOME TO PONG!" << std::endl;
    std::cout << " " << std::endl;

    FPS = 60;
    frameDuration = (1.0f / FPS) * 1000.0f;
    counter = 0;
    dT = 0.0f;

}

Game::~Game ()
{
    std::cout << "~Game" << std::endl;
}
void Game::setup(){

    PositionComponent paddle1Position = {0, SCREEN_HEIGHT / 2 - 100};
    VelocityComponent paddle1Velocity = {0, 0};
    CubeComponent paddle1Rect = {20, 200};
    PlayerComponent paddle1Player = {1};

    createPlayerEntity(paddle1Position, paddle1Velocity, paddle1Rect, paddle1Player);

    PositionComponent paddle2Position = {SCREEN_WIDTH - 20, SCREEN_HEIGHT / 2 - 100};
    VelocityComponent paddle2Velocity = {0, 0};
    CubeComponent paddle2Rect = {20, 200};
    PlayerComponent paddle2Player = {2};

    createPlayerEntity(paddle2Position, paddle2Velocity, paddle2Rect, paddle2Player);

    PositionComponent ballPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    VelocityComponent ballVelocity = {0, 0};
    CubeComponent ballRect = {20, 20};

    createBallEntity(ballPosition, ballVelocity, ballRect);
}

void Game::frameStart(){
    Uint32 currentTime = SDL_GetTicks();
    dT = (currentTime - frameStartTimeStamp) / 1000.0f;
    frameStartTimeStamp = currentTime; // Actualizar para el próximo frame
}

void Game::frameEnd(){
    frameEndTimeStamp = SDL_GetTicks();

    float actualFrameDuration = frameEndTimeStamp - frameStartTimeStamp;

    if (actualFrameDuration < frameDuration){
        SDL_Delay(frameDuration - actualFrameDuration);
    }
    counter++;


}

void Game::init(const char* title){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    isRunning = true;

}

void Game::handleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        if(event.type == SDL_KEYDOWN){
            PositionComponent& pad1Position = mRegistry.get<PositionComponent>(paddle1);
            CubeComponent& pad1Rect = mRegistry.get<CubeComponent>(paddle1);

            switch(event.key.keysym.sym){
                case SDLK_w:
                    if (pad1Position.y - MANUAL_PADDLE_SPEED >= 0){
                        pad1Position.y -= MANUAL_PADDLE_SPEED;
                    }
                    break;
                case SDLK_s:
                    if (pad1Position.y + pad1Rect.h + MANUAL_PADDLE_SPEED <= SCREEN_HEIGHT){
                        pad1Position.y += MANUAL_PADDLE_SPEED;
                    }
                    break;
            }
        }
    }
}

void autoPaddleMovementSystem(float dT) {
    auto ballView = mRegistry.view<BallComponent, PositionComponent, CubeComponent>();
    for (auto ball : ballView) {
        PositionComponent& ballPosition = ballView.get<PositionComponent>(ball);
        CubeComponent& ballRect = ballView.get<CubeComponent>(ball);
        PositionComponent& pad2Position = mRegistry.get<PositionComponent>(paddle2);
        VelocityComponent& pad2Velocity = mRegistry.get<VelocityComponent>(paddle2);

        // Puedes eliminar esta condición para probar si el paddle se mueve en cualquier situación
        if (ballPosition.x > SCREEN_WIDTH / 2) {
        
            float targetY = ballPosition.y + (ballRect.h / 2); // Calcula el centro de la pelota
            float paddleCenterY = pad2Position.y + (20 / 2); // Asumiendo que la altura del paddle es 20

            if (paddleCenterY < targetY) {
                    pad2Velocity.y = AI_PADDLE_SPEED; // Mover hacia abajo
                } else if (paddleCenterY > targetY) {
                    pad2Velocity.y = -AI_PADDLE_SPEED; // Mover hacia arriba
                } else {
                    pad2Velocity.y = 0; // Detener si está en la posición correcta
                }

                pad2Position.y += pad2Velocity.y * dT;
            
            // Agregar una impresión de depuración para verificar la nueva posición
            std::cout << "New Paddle 2 position: " << pad2Position.y << std::endl;
        }
    }
}


void Game::update(){
    ballMovementSystem(dT);
    isRunning = playing;
    winnerTop = player1Won;
    // Dentro del bucle principal del juego
    autoPaddleMovementSystem(dT);


}

void Game::render(){

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    cubeRenderSystem(renderer); 


    SDL_RenderPresent(renderer);

}

void Game::clean(){

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "GAME OVER" << std::endl;
    if (winnerTop){
        std::cout << "PLAYER 1 WINS" << std::endl;
    }
    else{
        std::cout << "PLAYER 2 WINS" << std::endl;
    }

}

bool Game::running(){
    return isRunning;
}