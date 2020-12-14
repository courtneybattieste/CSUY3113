#include "Level1.h"
#define LEVEL1_WIDTH 15
#define LEVEL1_HEIGHT 13

#define LEVEL1_ENEMY_COUNT 4
#define COIN_COUNT 7


unsigned int level1_data[] =
{
   // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14
	 162, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 163, 162, 194, 163,//  0   
	 167,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10, 166, 167,  10, 166,// -1 
     167,  10, 115, 193, 221, 110, 220, 193, 193, 116,  10, 142, 143,  10, 166,// -2
     167,  10, 142, 194, 143,  10, 245, 194, 194, 246,  10,  10,  10,  10, 166,// -3
     167,  10,  10,  10,  10,  10, 137,  10,  10, 135, 110, 113,  10, 115, 190,// -4
     167,  10, 108, 113,  10, 141, 112, 109,  10,  10,  10,  10,  10, 166, 168,// -5
     167,  10, 137,  10,  10,  10,  10, 137,  10, 140,  10, 115, 221, 247, 163,// -6
     191, 110, 112, 110, 109,  10, 108, 136,  10, 135, 110, 192, 167,  10, 166,// -7
     167,  10,  10,  10, 137,  10, 137,  10,  10,  10,  10, 142, 143,  10, 166,// -8
     167,  10,  10,  10, 137,  10, 137,  10, 141, 109,  10,  10,  10,  10, 166,// -9 
     167,  10, 141, 110, 136,  10, 137,  10,  10, 135, 110, 109,  10, 141, 247,// -10     
     167,  10,  10,  10,  10,  10, 137,  10,  10,  10,  10, 137,  10,  10, 10,// -11
     189, 193, 193, 193, 193, 193, 165, 193, 193, 193, 193, 165, 193, 193, 193 // -12
};


void Level1::Initialize() {

    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tilesheet_complete.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 27, 20);

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -9, 0);
    state.player->movement = glm::vec3(0, 0, 0);
    //state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f; /*1.0f*/
    state.player->textureID = Util::LoadTexture("astronaut.png");

    state.player->animRight = new int[4]{ 8, 9, 10, 11 };
    state.player->animLeft = new int[4]{ 4, 5, 6, 7 };
    state.player->animUp = new int[4]{ 12, 13, 14, 15 };
    state.player->animDown = new int[4]{ 0, 1, 2, 3 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.8f; 
    state.player->width = 0.6f;

    //state.player->jumpPower = 6.0f;

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("alien.png");

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].speed = .8f;
        state.enemies[i].width = 0.7f;
        state.enemies[i].height = 0.7f;

        state.enemies[i].animLeft = new int[3]{ 6, 7, 8 };
        state.enemies[i].animRight = new int[3]{ 3, 4, 5 };
        state.enemies[i].animUp = new int[3]{ 9, 10, 11 };
        state.enemies[i].animDown = new int[3]{ 0, 1, 2 };

        state.enemies[i].animFrames = 3;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 3;
        state.enemies[i].animRows = 4;
        
        
        //state.enemies[i].isActive = false;
    }
    
    state.enemies[0].position = glm::vec3(4, -6, 0);
    state.enemies[0].movement = glm::vec3(-.75, 0, 0);
    state.enemies[0].animIndices = state.enemies->animLeft;
    state.enemies[0].aiType = XWALKER;

    state.enemies[1].position = glm::vec3(1, -6, 0);
    state.enemies[1].movement = glm::vec3(0, -.75, 0);
    state.enemies[1].animIndices = state.enemies->animDown;
    state.enemies[1].aiType = YWALKER;
    
    state.enemies[2].position = glm::vec3(9, -5, 0);
    state.enemies[2].movement = glm::vec3(-.75, 0, 0);
    state.enemies[2].animIndices = state.enemies->animLeft;
    state.enemies[2].aiType = XWALKER;

    state.enemies[3].position = glm::vec3(7, -10, 0);
    state.enemies[3].movement = glm::vec3(0, .75, 0);
    state.enemies[3].animIndices = state.enemies->animUp;
    state.enemies[3].aiType = YWALKER;



    //COINS
    state.coins = new Entity[COIN_COUNT];
    GLuint coinTextureID = Util::LoadTexture("coin_2.png");

    for (int i = 0; i < COIN_COUNT; i++) {
        state.coins[i].entityType = COIN;
        state.coins[i].textureID = coinTextureID;
        state.coins[i].width = 1.0f;
        state.coins[i].height = 1.0f;

        state.coins[i].animRight = new int[6]{ 0, 1, 2, 3, 4, 5 };
        
        state.coins[i].animFrames = 6;
        state.coins[i].animIndex = 0;
        state.coins[i].animCols = 6;
        state.coins[i].animRows = 1;

        state.coins[i].animIndices = state.coins->animRight;
    }

    state.coins[0].position = glm::vec3(4, -11, 0);
    state.coins[1].position = glm::vec3(1, -6, 0);
    state.coins[2].position = glm::vec3(5, -3, 0);
    state.coins[3].position = glm::vec3(13, -1, 0);
    state.coins[4].position = glm::vec3(13, -7, 0);
    state.coins[5].position = glm::vec3(9, -11, 0);
    state.coins[6].position = glm::vec3(7, -4, 0);



    state.exitSign = new Entity();
    state.exitSign->textureID = Util::LoadTexture("exit_sign.png");
    state.exitSign->position = glm::vec3(13, -11, 0);

}
void Level1::Update(float deltaTime) {
    if (state.player->position.x >= 14) {
        state.exited = true;
    }

    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.coins, COIN_COUNT, state.map);

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, NULL, 0, state.map);
    }

    for (int i = 0; i < COIN_COUNT; i++) {
        state.coins[i].Update(deltaTime, NULL, 0, NULL, NULL, 0, state.map);
    }
    

    state.exitSign->Update(0, NULL, 0, NULL, NULL, 0, state.map);
}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    for (int i = 0; i < COIN_COUNT; i++) {
        state.coins[i].Render(program);
    }

    state.exitSign->Render(program);
    state.player->Render(program);
}