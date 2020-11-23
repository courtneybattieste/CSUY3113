#include "MainMenu.h"
#define MAINMENU_WIDTH 11
#define MAINMENU_HEIGHT 8

#define MAINMENU_ENEMY_COUNT 1

unsigned int mainMenu_data[] =
{
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
     3, 1, 1, 0, 0, 0, 0, 0, 1, 1, 3, 
     3, 2, 2, 1, 1, 1, 1, 1, 2, 2, 3, 
     3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 
};

void MainMenu::Initialize() {
    state.nextScene = 0;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(MAINMENU_WIDTH, MAINMENU_HEIGHT, mainMenu_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.

    state.player = new Entity();  

}
void MainMenu::Update(float deltaTime) {
}

void MainMenu::Render(ShaderProgram* program) {
    state.map->Render(program);
    Util::DrawText(program, Util::LoadTexture("font2.png"), "i freaking HATE scorpions", .65, -0.30f, glm::vec3(.75, -3, 0));
    Util::DrawText(program, Util::LoadTexture("font2.png"), "Press ENTER to start", .5, -0.20f, glm::vec3(2.25, -6.5, 0));
}