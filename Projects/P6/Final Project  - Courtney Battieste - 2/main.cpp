#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "SDL_mixer.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"
#include "MainMenu.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[4];

int lives = 3;
int coins = 7;

bool start = false;
bool gameOver = false;
bool successful = false;

Mix_Music* music;
Mix_Chunk* died;
Mix_Chunk* collected;

float x;
float y;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Final Project!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1280, 720);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("myst_on_the_moor.mp3");
    Mix_PlayMusic(music, -1);
    //Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

    died = Mix_LoadWAV("hurt.wav");
    collected = Mix_LoadWAV("coin_sound.wav");
    Mix_VolumeChunk(collected, MIX_MAX_VOLUME / 2);
    Mix_VolumeChunk(died, MIX_MAX_VOLUME / 2);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sceneList[0] = new MainMenu();
    sceneList[1] = new Level1();
    SwitchToScene(sceneList[0]);

}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                    if (currentScene->state.nextScene == 0) {
                        SwitchToScene(sceneList[1]);
                        start = true;
                    }
                    break;

                case SDLK_LEFT:
                    // Move the player left
                    break;

                case SDLK_RIGHT:
                    // Move the player right
                    break;

                case SDLK_SPACE:
             
                    break;

            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    else if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->movement.y = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animUp;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->movement.y = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animDown;
    }


    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {

    if (gameOver) {
        currentScene->state.player->isActive = false;
        return;
    }

    float ticks = (float)SDL_GetTicks() / 1000.0f; //first we get how much time has passed
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator; 
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f); 
  

    x = -currentScene->state.player->position.x;
    y = -currentScene->state.player->position.y;

    if (currentScene->state.player->position.x >= 9) {
        x = -9;
    }
    if (currentScene->state.player->position.x <= 5) {
        x = -5;
    }
    if (currentScene->state.player->position.y >= -3.75) {
        y = 3.75;
    }
    if (currentScene->state.player->position.y <= -8.25) {
        y = 8.25;
    }
    viewMatrix = glm::translate(viewMatrix, glm::vec3(x, y, 0));
    

    if (currentScene->state.player->loseLife) {
        lives -= 1;
        Mix_PlayChannel(-1, died, 0);
        coins = 7;
        if (lives > 0) {
            SwitchToScene(currentScene);
            
        }
        else {
            gameOver = true;
            successful = false;
            lives = 0;
        }
    }
    
    if (currentScene->state.player->collectCoin) {
        coins -= 1;
        Mix_PlayChannel(-1, collected, 0);
    }

    if (currentScene->state.exited) {
        gameOver = true;
        //if (coins <= 0) {
            successful = true;
            coins = 0;
        //}
    }
    
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);

    GLuint font2 = Util::LoadTexture("font2.png");

    float xoffset = 3.25;
    float yoffset = -3.5;

    //if (x == -5 || x == -9) xoffset = 0;
    //if (y == 8.25) yoffset = 0;

    if (start) {
        Util::DrawText(&program, font2, "Lives: " + std::to_string(lives), .35, -0.15f, glm::vec3(-x + xoffset + .1, -y + yoffset + .4, 0));
        //Util::DrawText(&program, font2, "Coins Left: " + std::to_string(coins), .3, -0.12f, glm::vec3(-x + xoffset - .65, -y + yoffset, 0));
    }

    if (gameOver) {
        if (successful) {
            Util::DrawText(&program, font2, "You Win!", 1.35, -0.60f, glm::vec3(-x-2.5, -y, 0));
        }
        /*else if (lives > 0 && coins > 0) {
            Util::DrawText(&program, font2, "Mission Failed :(", .75, -0.25f, glm::vec3(-x - 3.75, -y+.5, 0));
            Util::DrawText(&program, font2, "Reason: Didn't Collect All Coins", .35, -0.12f, glm::vec3(-x - 3.5, -y-.5, 0));
        }*/
        else {
            Util::DrawText(&program, font2, "Mission Failed :(", .75, -0.25f, glm::vec3(-x-3.75, -y, 0));
        }
    }

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {

    Mix_FreeChunk(died);
    Mix_FreeChunk(collected);
    Mix_FreeMusic(music);

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        //if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);

        Render();
    }

    Shutdown();
    return 0;
}

