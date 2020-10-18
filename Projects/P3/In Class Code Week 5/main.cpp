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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define ENEMY_COUNT 3


struct GameState {
    Entity* player;
    Entity* enemies;
};

GameState state;
    
SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix, modelMatrix;


//when using images, you should use something that is 64x64, or 128x128, 1024x1024
//avoid odd resolutions and things that aren't multiplies of 16
GLuint LoadTexture(const char* filePath) { //returns ID number of a texture
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha); //loading the png file off the hard drive and into ram

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID); //create the texture ID
    glBindTexture(GL_TEXTURE_2D, textureID); //now bind that texture ID

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); //here is the raw pixel data, put it onto the video card and heres the id number

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //use the nearest filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image); //freeing from our main memory since it's already on the video card memory
    return textureID; //now it returns the texture id
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl"); // Setting up the shader program
    //loads a shader program that can handle textured polygons

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f); //applies this identity matrix to the triangle
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);


    //this is the drawing of the object
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f); //set the color to red

    glUseProgram(program.programID); //use this shader program

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //whenever you go to draw something, use this background color
    glEnable(GL_BLEND);

    //good for setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    state.player = new Entity();
    state.player->position = glm::vec3(0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("george_0.png");

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.enemies = new Entity[ENEMY_COUNT];

    GLuint enemyTextureID = LoadTexture("ctg.png");
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(-2.0f, -2.0f, 0.0f);

    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(0.0f, -2.0f, 0.0f);
    state.enemies[1].speed = .0001f;
    state.enemies[1].movement.y = -1;

    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(2.0f, -2.0f, 0.0f);
}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE: //both quit or close, stop game 
                gameIsRunning = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) { //.. now we are going to be looking for which key was it
                    case SDLK_LEFT: 
                        //Move the player left
                        break;

                    case SDLK_RIGHT: 
                        //Move the player right
                        //player_movement.x = 1.0f; makes you press it a million times to move
                        break;

                    case SDLK_SPACE: 
                        //Usually jump or some sort of action
                        break;
                }
                break; //SDL_KEYDOWN
        }
    }

    //not inside the while loop
    //this is to help with a key being held down
    const Uint8* keys = SDL_GetKeyboardState(NULL); //pointer to array

    if (keys[SDL_SCANCODE_LEFT]) { //if the left key is down rn (being held down_
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }

    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }

    if (keys[SDL_SCANCODE_UP]) { //if the left key is down rn (being held down_
        state.player->movement.y = 1.0f;
        state.player->animIndices = state.player->animUp;
    }

    else if (keys[SDL_SCANCODE_DOWN]) {
        state.player->movement.y = -1.0f;
        state.player->animIndices = state.player->animDown;
    }

    //makes sure that if both keys are held down, you aren't moving faster in some way
    //not move faster along a diagonal, simulates motion on a joystick
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }
}

float lastTicks = 0.0f; //time since last frame

//update only runs as fast as your computer can run it
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime);
    }

    state.player->Update(deltaTime);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT); //clear the color and use the background color

    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }

    state.player->Render(&program);


    SDL_GL_SwapWindow(displayWindow); //swap window basically pushes it to the screen
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) { //this is happening as fast as the computer can handle (framerate)
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}