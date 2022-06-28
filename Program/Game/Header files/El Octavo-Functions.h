#pragma once
#include "Precompile.h"

bool checkCollideWithGround(RectangleShape& body);

void setVars();

void setup(RenderWindow& window);

bool checkCollideWithRamp(RectangleShape& body);

void respawnPlayer(RectangleShape& body, RectangleShape& npcBody, RectangleShape& npcBody2, Sprite& adventureBgImage);
void moveStaticImages(RectangleShape& body, RenderWindow& window, RectangleShape& npcBody, RectangleShape& npcBody2, Sprite& adventureBgImage);

void resetStaticImages(float& offset, RectangleShape& npcBody, RectangleShape& npcBody2, Sprite& adventureBgImage);

Vector2f getRamp2Pos();

class CollisionBlock {
public:
    RectangleShape hitbox;

    CollisionBlock(Vector2f pos, Vector2f size);

    ~CollisionBlock();

    void drawHitbox(RenderWindow& window);

    bool checkForCollision(RectangleShape& body, RectangleShape& npcBody, RectangleShape& npcBody2, Sprite adventureBgImage);
};

class Npc {
private:
    
    // build variables
    float totalTime;
    float switchTime;
    
    string name;

    Vector2u imageCount;
    Vector2u currentImage;

    // movement variables
    unsigned int row;
    float speed;

    float distance;
    int posIndex;

    Clock npcClock; 
    Time npcPreviousTime; 
    Time npcCurrentTime;

    bool reset;

public:
    bool faceLeft;
    IntRect uvRect;
    RectangleShape body;
    Vector2f velocity;
    bool delay;
    bool plrFound;
    int npcNum;

    Npc(Texture* texture, Vector2u imageCount, float switchTime, float speed, string name, float posX, float posY);
    ~Npc();

    void draw(RenderWindow& window);
    void moveX(float amount);
    void update(int row, float deltaTime, bool delay);
    void moveTo(float pos[], float deltaTime, bool& faceLeft, RectangleShape& plrBody, bool jumped, Sprite& adventureBgImage, RectangleShape& npcBody2);
};