#include "MainMenu.h"
#define MAINMENU_WIDTH 16
#define MAINMENU_HEIGHT 9


unsigned int mainMenu_data[] =
{
      1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
     16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
     32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
     48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
     64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
     80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 
     96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
     112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
     128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
};

void MainMenu::Initialize() {
    state.nextScene = 0;

    GLuint mapTextureID = Util::LoadTexture("main_menu.jpg");
    state.map = new Map(MAINMENU_WIDTH, MAINMENU_HEIGHT, mainMenu_data, mapTextureID, 1.0f, 16, 9);
    // Move over all of the player and enemy code from initialization.

    state.player = new Entity();  

}
void MainMenu::Update(float deltaTime) {
}

void MainMenu::Render(ShaderProgram* program) {
    state.map->Render(program);

    Util::DrawText(program, Util::LoadTexture("font2.png"), "Apollo 420", .85, -0.33f, glm::vec3(2.85, -3, 0));
    Util::DrawText(program, Util::LoadTexture("font2.png"), "Press ENTER to start", .5, -0.20f, glm::vec3(2.25, -4.5, 0));
}