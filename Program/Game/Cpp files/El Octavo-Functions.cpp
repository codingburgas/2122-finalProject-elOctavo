#include "../Header files/Precompile.h"
#include "../Header files/El Octavo-Functions.h"
#include "../Header files/GameClass.h"
#include "../Header files/Additional Functions.h"
#include "../Header files/Menu.h"

// Npc class
Npc::Npc(Texture * texture, Vector2u imageCount, float switchTime, float speed, string name) {
    this->imageCount = imageCount;
    this->switchTime = switchTime;
    this->speed = speed;
    this->totalTime = 0.0f;
    this->currentImage.x = 0;
    this->name = name;
    this->posIndex = 0;
    this->reset = false;
    this->delay = false;

    this->uvRect.width = texture->getSize().x / float(imageCount.x);
    this->uvRect.height = texture->getSize().y / float(imageCount.y);

    //movement mechanic
    this->faceLeft = true;
    this->row = 0;


    this->body.setSize(Vector2f(81.0f, 109.0f));
    this->body.setOrigin(81.0f / 2, 109.0f / 2);
    this->body.setPosition(1849.0f, 538.0f - 109.0f / 2);
    this->body.setTexture(texture);
}

Npc::~Npc() {
}

void Npc::draw(RenderWindow& window) {
    window.draw(this->body);
}

void Npc::moveX(float amount) {
    this->body.move(amount, 0.0f);
}

void Npc::update(int row, float deltaTime, bool delay) {
    currentImage.y = row;
    totalTime += deltaTime;

    if (totalTime >= switchTime)
    {
        totalTime -= switchTime;
        currentImage.x++;

        if (currentImage.x >= imageCount.x)
        {
            currentImage.x = 0;
        }
    }

    uvRect.top = currentImage.y * uvRect.height;

    if (faceLeft)
    {
        uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
        uvRect.width = -abs(uvRect.width);
    }
    else
    {
        uvRect.left = currentImage.x * uvRect.width;
        uvRect.width = abs(uvRect.width);
    }

    body.setTextureRect(uvRect);
}

void Npc::moveTo(float pos[], float deltaTime, bool& done, bool& faceLeft) {
    //cout << npcCurrentTime.asSeconds() << endl;
    
    if (!delay) {
        if (pos[posIndex] > body.getPosition().x) {
            distance = pos[posIndex] - body.getPosition().x;
            moveX(speed * deltaTime);
        }
        else {
            distance = body.getPosition().x - pos[posIndex];
            moveX(-speed * deltaTime);
        }
    }

    if (distance <= 20) {
        if (!reset) {
            npcClock.restart();
            reset = true;
            cout << "Reset" << endl;
            delay = true;

            if (faceLeft) 
            {
                cout << "if" << endl;
                faceLeft = false;
            }
            else {
                cout << "else" << endl;
                faceLeft = true;
            }
            
        }
        
        npcCurrentTime = npcClock.getElapsedTime();
        //cout << npcClock.getElapsedTime().asSeconds() << endl;

        if (npcCurrentTime.asSeconds() >= 0.3) {
            posIndex++;
            if (posIndex > 1) {
                posIndex = 0;
            }
            reset = false;
            delay = false;
        }
    }
}

// CollisionBlock class
CollisionBlock::CollisionBlock(Vector2f pos, Vector2f size) {
    hitbox.setSize(size);
    hitbox.setPosition(pos);
}

CollisionBlock::~CollisionBlock() {
}

bool CollisionBlock::checkForCollision(RectangleShape& body) {
    if (body.getGlobalBounds().intersects(hitbox.getGlobalBounds())) {
        return true;
    }
    else {
        return false;
    }
}

void CollisionBlock::drawHitbox(RenderWindow& window) {
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

    // player / npc
    Texture plrT;
    Texture npcT;
   
    //ground
    RectangleShape ground;
    //ramp
    Texture rampT;
    Sprite ramp;
    RectangleShape points[12];

    //sound
    Sound soundJump, soundWalk;
    SoundBuffer jumpBuffer, walkBuffer;

    //time
    Clock fpsClock = Clock::Clock();
    Time fpsPreviousTime = fpsClock.getElapsedTime();
    Time fpsCurrentTime;
    Clock npcClock = Clock::Clock();
    Time npcPreviousTime = npcClock.getElapsedTime();
    Time npcCurrentTime;


    //font
    Font font;

    //fps
    float fps;
    Text fpsCounter;
    int roundedFps;

    string cutsceneStr = "Press Q to talk with the mafia.";
    Text cutsceneText;

    bool drawBubble;

    CollisionBlock blocks[2] = {{Vector2f(2428, 416), Vector2f(107, 45)}, {Vector2f(1428, 338), Vector2f(211, 43)}};

    bool done;
    float moveToPos[2] = { 100, 800 };
}

using namespace variables;

void moveStaticImages(RectangleShape& body, RenderWindow& window, Npc& test);
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

    npcT.loadFromFile("../Images and fonts/NPCs/enemy npc sprite sheet.png");
    npcT.setRepeated(true);

    rampT.loadFromFile("../Images and fonts/Ramp Test.png");
    ramp.setTexture(rampT);
    ramp.setOrigin(0, 0);
    ramp.setPosition(800, 430);

    jumpBuffer.loadFromFile("../Audios/Jump.wav");
    walkBuffer.loadFromFile("../Audios/Walk.wav");

    soundJump.setBuffer(jumpBuffer);
    soundWalk.setBuffer(walkBuffer);

    soundJump.setVolume(5.0f);
    soundWalk.setVolume(25.0f);

    font.loadFromFile("../Images and fonts/Fonts/Header font.ttf");
    fpsCounter.setFont(font);
    fpsCounter.setCharacterSize(56);

    cutsceneText.setPosition(20, 20);
    cutsceneText.setCharacterSize(30);
    cutsceneText.setFont(font);

    for (int i = 0; i < 12; i++) {
        points[i] = RectangleShape(Vector2f(10, 10));
        points[i].setOrigin(5, 5);
    }

    for (int i = 0; i < 12; i++) {
        if (i == 0) {
            points[i].setPosition(ramp.getPosition().x, ramp.getPosition().y + 110);
        }
        else {
            points[i].setPosition(points[i - 1].getPosition().x + 10, points[i - 1].getPosition().y - 10);
        }
    }
}

void setup(RenderWindow& window)
{
    setVars();

    int blocksSize = sizeof(blocks) / sizeof(*blocks);

    // start of sussy variables

    Clock clock;
    Player plr(&plrT, Vector2u(3, 2), 0.3f, 225.0f);
    Npc test(&npcT, Vector2u(3,1), 0.3f, 190.0f, "Test");
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
        test.update(0, deltaTime,test.delay);

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

        window.draw(test.body);

        plr.draw(window);

        test.moveTo(moveToPos, deltaTime, done, test.faceLeft);

        // calculate fps
        fpsCurrentTime = fpsClock.getElapsedTime();
        fps = 1.0f / (fpsCurrentTime.asSeconds() - fpsPreviousTime.asSeconds());
        fpsPreviousTime = fpsCurrentTime;

        // show fps
        roundedFps = (int)fps;
        fpsCounter.setString(to_string(roundedFps));
        fpsCounter.setPosition(1200, 20);

        if (isShowingFPS(showFPS) == true)
        {
            window.draw(fpsCounter);
        }
        
        window.draw(cutsceneText);

        for (int i = 0; i < 11; i++) {
            window.draw(points[i]);
        }

        // draw mafia bubble
        if (drawBubble) {
            window.draw(messageImage);
        }

        moveStaticImages(plr.body, window, test);

        window.display();
    }
}

bool checkCollideWithGround(RectangleShape& body) {
    return ground.getGlobalBounds().intersects(body.getGlobalBounds());
}

bool checkCollideWithRamp(RectangleShape& body) {
    /*if (ramp.getGlobalBounds().intersects(body.getGlobalBounds())) {
        Image rampImage = rampT.copyToImage();

        int pixelX = body.getPosition().x - ramp.getPosition().x;
        int pixelY = (body.getPosition().y + 44) - ramp.getPosition().y;

        if ((pixelX < 110 && pixelX > 0) && (pixelY < 110 && pixelY >= 0)) {
            if (rampImage.getPixel(pixelX, pixelY).a != 0) {
                return true;
            }
            else {
                return false;
            }
        }
    }*/

    for (int i = 0; i < 12; i++) {
        if (points[i].getGlobalBounds().intersects(body.getGlobalBounds()) && body.getPosition().x <= ramp.getPosition().x + 110) {
            cout << "collide" << endl;
            return true;
        }
    }
    //cout << "falsed" << endl;
    return false;
}

void moveStaticImages(RectangleShape& body, RenderWindow& window, Npc& test)
{
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        if (body.getPosition().x >= window.getSize().x / 2)
        {
            //make images static
            messageImage.move(-(225.0f * deltaTime), 0.f);
            ramp.move(-(225.0f * deltaTime), 0.f);
            test.moveX(-(225.0f * deltaTime));

            for (int i = 0; i < 2; i++) {
                blocks[i].hitbox.move(-(225.0f * deltaTime), 0.f);
            }

            for (int i = 0; i < 11; i++) {
                points[i].move(-(225.0f * deltaTime), 0.f);
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