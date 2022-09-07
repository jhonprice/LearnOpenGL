#include "game.h"
#include "resource_manager.h"
#include "sprite_render.h"
#include "game_object.h"

// Game-related State data
SpriteRenderer* Renderer;
GameObject* Player;



Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    ResourceManager::LoadShader("../shader/final_shader.vert", "../shader/final_shader.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    auto shader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(shader);
    // load textures
    ResourceManager::LoadTexture("../image/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../image/background.jpg", false, "background");
    ResourceManager::LoadTexture("../image/block.png", false, "block");
    ResourceManager::LoadTexture("../image/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../image/paddle.png", true, "paddle");

    // load levels
    GameLevel one; one.Load("../levels/one.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // draw background
        auto bg = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(bg, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        // draw player
        Player->Draw(*Renderer);
    }
}