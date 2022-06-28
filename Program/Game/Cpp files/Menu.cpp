#include "../Header files/Precompile.h"
#include "../Header files/Menu.h"
#include "../Header files/El Octavo-Functions.h"
#include "../Header files/Additional Functions.h"

bool audioToggle = true;
bool showFPS = true;

namespace variables {
    Event evMenu;

    Texture textureMenu;
    Sprite bgImageMenu;

    Texture confirmationT;
    Sprite confirmation;

    RectangleShape blackEffect(Vector2f(1280, 720));

    Music music;

    Texture texture;

    Texture checkButtonTexture;

    Sprite checkButtonImage[2];

    bool setPositionByDefault = true;
    bool displayConfirmation = false;
}

using namespace variables;

void setupMenu(RenderWindow& window);

void setupVars(RenderWindow& window)
{
    if (!music.openFromFile("../Audios/Main.ogg"))
    {
        cout << "Error" << endl;
    }

    if (audioToggle)
    {
        music.play();
    }
    else {
        cout << "";
    }

    checkButtonTexture.loadFromFile("../Images and fonts/Bg/OptionChecked.png");

    for (int i = 0; i < 2; i++)
    {
        checkButtonImage[i].setTexture(checkButtonTexture);
    }

    if (setPositionByDefault == true)
    {
        checkButtonImage[0].setPosition(369, 395);
        checkButtonImage[1].setPosition(782, 395);
        setPositionByDefault = false;
    }
    else {
        cout << "";
    }

    confirmationT.loadFromFile("../Images and fonts/Bg/Confirmation.png");
    confirmation.setTexture(confirmationT);
    confirmation.setOrigin(0, 0);
    confirmation.setPosition(0, 0);
    blackEffect.setFillColor(Color(0, 0, 0, 120));

    addIcon(window);
    music.setLoop(true);
}

//initialising option menu
void optionMenu(Event& ev, Texture texture, RenderWindow& window)
{
    texture.loadFromFile("../Images and fonts/Bg/OptionsMenu.png");

    while (true)
    {
        Sprite sprite(texture);

        while (window.pollEvent(ev))
        {
            switch (ev.type)
            {
            case Event::MouseButtonPressed:
            {
                switch (ev.key.code)
                {
                case Mouse::Left:
                {
                    if ((ev.mouseButton.x >= 357 && ev.mouseButton.x <= 428) && (ev.mouseButton.y >= 384 && ev.mouseButton.y <= 455))
                    {
                        audioToggle = true;

                        if (audioToggle)
                        {
                            cout << "";
                        }
                        else {
                            music.stop();
                        }

                        checkButtonImage[0].setPosition(369, 395);
                    }

                    else if ((ev.mouseButton.x >= 465 && ev.mouseButton.x <= 536) && (ev.mouseButton.y >= 384 && ev.mouseButton.y <= 455))
                    {
                        audioToggle = false;

                        if (audioToggle)
                        {
                            cout << "";
                        }
                        else {
                            music.stop();
                        }

                        checkButtonImage[0].setPosition(477, 395);
                    }

                    else if ((ev.mouseButton.x >= 770 && ev.mouseButton.x <= 841) && (ev.mouseButton.y >= 384 && ev.mouseButton.y <= 455))
                    {
                        showFPS = true;

                        checkButtonImage[1].setPosition(782, 395);
                    }

                    else if ((ev.mouseButton.x >= 878 && ev.mouseButton.x <= 949) && (ev.mouseButton.y >= 384 && ev.mouseButton.y <= 455))
                    {
                        showFPS = false;

                        checkButtonImage[1].setPosition(890, 395);
                    }

                    else if ((ev.mouseButton.x >= 1054 && ev.mouseButton.x <= 1197) && (ev.mouseButton.y >= 593 && ev.mouseButton.y <= 637))
                    {
                        setupMenu(window);
                    }
                    break;
                }
                break;
                }
            }
            }
        }

        window.draw(sprite);

        for (int i = 0; i < 2; i++)
        {
            window.draw(checkButtonImage[i]);
        }

        window.display();
    }
}

//main menu/setup function
void setupMenu(RenderWindow& window)
{
    setupVars(window);

    textureMenu.loadFromFile("../Images and fonts/Bg/Menu.png");
    bgImageMenu.setTexture(textureMenu);
    bgImageMenu.setOrigin(1280 / 2, 720 / 2);
    bgImageMenu.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    while (window.isOpen())
    {
        while (window.pollEvent(evMenu))
        {

            if (evMenu.type == Event::Closed)
            {
                if (!displayConfirmation) {
                    displayConfirmation = true;
                }
                else {
                    window.close();
                }
            }

            if (evMenu.type == Event::KeyPressed && evMenu.key.code == Keyboard::Escape)
            {
                if (!displayConfirmation) {
                    displayConfirmation = true;
                }
                else {
                    window.close();
                }
            }

            switch (evMenu.type)
            {
            case Event::MouseButtonPressed:
            {
                switch (evMenu.key.code)
                {
                case Mouse::Left:

                    if (evMenu.mouseButton.x >= 530 && evMenu.mouseButton.x <= 750)
                    {

                        if (!displayConfirmation) {
                            if (evMenu.mouseButton.y >= 335 && evMenu.mouseButton.y <= 392)
                            {
                                music.stop();

                                backstory(window, backstoryTexture, backstoryImage);
                                setup(window);
                            }

                            if (evMenu.mouseButton.y >= 459 && evMenu.mouseButton.y <= 516)
                            {
                                optionMenu(evMenu, texture, window);
                            }

                            if (evMenu.mouseButton.y >= 581 && evMenu.mouseButton.y <= 638)
                            {
                                displayConfirmation = true;

                            }
                        }

                        // 541, 397
                        // 594, 421

                        // 638, 397
                        // 722, 421
                        if (evMenu.mouseButton.y <= 524 && evMenu.mouseButton.y >= 381) {
                            if (evMenu.mouseButton.x >= 541 && evMenu.mouseButton.x <= 594) {
                                window.close();
                            }
                            if (evMenu.mouseButton.x >= 659 && evMenu.mouseButton.x <= 738) {
                                displayConfirmation = false;
                            }
                        }
                    }
                    break;
                }
            }
            }
        }

        window.draw(bgImageMenu);

        if (displayConfirmation) {
            window.draw(blackEffect);
        }

        if (displayConfirmation) {
            window.draw(confirmation);
        }

        window.display();
    }
}

bool isAudioRunning(bool& audioToggle)
{
    return audioToggle;
}

bool isShowingFPS(bool& showFPS)
{
    return showFPS;
}