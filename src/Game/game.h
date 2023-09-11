#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
// Include guards 
 class Game {

    public:
        Game();
        ~Game();
        void init(const char* title, int width, int height);
        void setup();
        void frameStart();
        void frameEnd();
        void handleEvents();
        void update();
        void render();
        void clean();
        bool running();

    private:
        int window_width;
        int window_height;
        int counter;
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;
        int FPS;
        Uint64 frameStartTimestamp;
        Uint64 frameEndTimeStamp;
        float frameDuration;
        float dT;

 };


 #endif