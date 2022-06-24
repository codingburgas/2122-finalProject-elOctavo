#include "../Header files/Precompile.h"
#include "../Header files/El Octavo-Functions.h"
#include "../Header files/GameClass.h"
#include "../Header files/Additional Functions.h"

/*class Npc {
private:
    int hp;
    string name;
    Vector2f pos;
    string nestashev = "NESTASHEV"

public:

    Npc(int hp, string name, Vector2f pos) : this->hp(hp), this->name(name), this->pos(pos);
    ~Npc();

    void draw(RenderWindow& window);


};*/

collisionBlock::collisionBlock(Vector2f pos, Vector2f size) {
    hitbox.setSize(size);
    hitbox.setPosition(pos);
}

collisionBlock::~collisionBlock() {
}

bool collisionBlock::checkForCollision(RectangleShape& body) {
    if (body.getGlobalBounds().intersects(hitbox.getGlobalBounds())) {
        return true;
    }
    else {
        return false;
    }
}

void collisionBlock::drawHitbox(RenderWindow& window) {
    window.draw(hitbox);
}

namespace variables {

    //movemenent
    float deltaTime = 0.0f;
    bool movementToggle = true;

    //event
    Event ev;

    //background
    Texture bgTexture;
    Sprite bgImage;

    //background for stage 2
    Texture adventureBgTexture;
    Sprite adventureBgImage;

    //background for stage 2
    Texture messageTexture;
    Sprite messageImage;

    //ground
    Texture plrT;
    RectangleShape ground;

    //ramp
    Texture rampT;
    Sprite ramp;

    //sound
    Sound soundJump, soundWalk;
    SoundBuffer jumpBuffer, walkBuffer;

    //time
    Clock c = Clock::Clock();
    Time previousTime = c.getElapsedTime();
    Time currentTime;

    //font
    Font font;

    //fps
    float fps;
    Text fpsCounter;
    int roundedFps;

    string cutsceneStr = "Press Q to talk with the mafia.";
    Text cutsceneText;

    bool drawBubble;

    collisionBlock blocks[2] = {{Vector2f(2428, 416), Vector2f(107, 45)}, {Vector2f(1428, 338), Vector2f(211, 43)}};
}

using namespace variables;

void moveStaticImages(RectangleShape& body, RenderWindow& window);
void cutscene(RectangleShape& body, string& cutsceneStr, Sprite& adventureBgImage, Sprite& messageImage, RenderWindow& window);

void setVars()
{
    bgTexture.loadFromFile("../Images and fonts/Bg/test bg.png");
    bgImage.setTexture(bgTexture);
    drawBubble = false;

    adventureBgTexture.loadFromFile("../Images and fonts/Bg/spawn start.png");
    adventureBgImage.setTexture(adventureBgTexture);

    messageTexture.loadFromFile("../Images and fonts/Bg/message.png");
    messageImage.setTexture(messageTexture);

    messageImage.setPosition(500, 300);

    //setting position of the background
    bgImage.setOrigin(1920, 1080);

    ground.setSize(Vector2f(1280, 225));
    ground.setPosition(Vector2f(0, 535));
    ground.setFillColor(Color::Black);

    plrT.loadFromFile("../Images and fonts/Main character/unknown.png");
    plrT.setRepeated(true);

    rampT.loadFromFile("../Images and fonts/Ramp Test.png");
    ramp.setTexture(rampT);
    ramp.setOrigin(0, 0);
    ramp.setPosition(200, 430);

    jumpBuffer.loadFromFile("../Audios/Jump.wav");
    walkBuffer.loadFromFile("../Audios/Walk.wav");

    soundJump.setBuffer(jumpBuffer);
    soundWalk.setBuffer(walkBuffer);

    soundJump.setVolume(15.0f);
    soundWalk.setVolume(35.0f);

    font.loadFromFile("../Images and fonts/Fonts/Header font.ttf");
    fpsCounter.setFont(font);
    fpsCounter.setCharacterSize(56);

    cutsceneText.setPosition(20, 20);
    cutsceneText.setCharacterSize(30);
    cutsceneText.setFont(font);
}

void setup(RenderWindow& window)
{
    setVars();

    int blocksSize = sizeof(blocks) / sizeof(*blocks);

    // start of sussy variables

    Clock clock;
    Player plr(&plrT, Vector2u(3, 2), 0.3f, 225.0f);
    bgImage.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    // end of sussy variables
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        window.setKeyRepeatEnabled(true);

        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
            }

            if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Escape)
            {
                window.close();
            }
        }

        plr.updateMovement(deltaTime, window, adventureBgImage, soundWalk, soundJump, movementToggle, blocks, blocksSize);

        window.clear(Color::Green);

        if (movementToggle) {
            window.draw(ground);
        }

        cutscene(plr.body, cutsceneStr, adventureBgImage, messageImage, window);

        window.draw(adventureBgImage);
        window.draw(ramp);

        for (int i = 0; i < 2; i++) {
            window.draw(blocks[i].hitbox);
        }

        plr.draw(window);

        // calculate fps
        currentTime = c.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
        previousTime = currentTime;

        // show fps

        roundedFps = (int)fps;
        fpsCounter.setString(to_string(roundedFps));

        fpsCounter.setPosition(1200, 20);
        window.draw(fpsCounter);
        window.draw(cutsceneText);

        if (drawBubble) {
            window.draw(messageImage);
        }


        moveStaticImages(plr.body, window);

        window.display();
    }
}

bool checkCollideWithGround(RectangleShape& body) {
    return ground.getGlobalBounds().intersects(body.getGlobalBounds());
}

bool checkCollideWithRamp(RectangleShape& body) {
    if (ramp.getGlobalBounds().intersects(body.getGlobalBounds())) {
        Image rampImage = rampT.copyToImage();

        int pixelX = ramp.getPosition().x - body.getPosition().x;
        int pixelY = ramp.getPosition().y - (body.getPosition().y + 64);

        if (pixelX <= 110 && pixelX >= 0 && pixelY <= 110 && pixelY >= 0) {
            if (rampImage.getPixel(pixelX, pixelY).a != 0) {
                return true;
            }
            else {
                return false;
            }
        }
    }
}

void moveStaticImages(RectangleShape& body, RenderWindow& window)
{
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        if (body.getPosition().x >= window.getSize().x / 2)
        {
            //make images static
            messageImage.move(-(225.0f * deltaTime), 0.f);
            ramp.move(-(225.0f * deltaTime), 0.f);

            for (int i = 0; i < 2; i++) {
                blocks[i].hitbox.move(-(225.0f * deltaTime), 0.f);
            }
        }
    }
}

void cutscene(RectangleShape& body, string& cutsceneStr, Sprite& adventureBgImage, Sprite& messageImage, RenderWindow& window)
{
    if (body.getPosition().x >= messageImage.getPosition().x - 200.0f && body.getPosition().x <= messageImage.getPosition().x + 200.0f)
    {
        cutsceneText.setString(cutsceneStr);
        drawBubble = true;
    }
    else {
        cutsceneText.setString("");
        drawBubble = false;
    }
}