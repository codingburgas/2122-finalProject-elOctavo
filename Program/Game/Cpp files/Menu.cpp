#include "../Header files/Precompile.h"
#include "../Header files/Menu.h"
#include "../Header files/El Octavo-Functions.h"

namespace variables {
    Event evMenu;
    Texture textureMenu;
    Sprite bgImageMenu;
}

using namespace variables;

void setupMenu(RenderWindow& window)
{
    while (window.isOpen())
    {
        textureMenu.loadFromFile("../Images and fonts/Bg/Menu.png");
        bgImageMenu.setTexture(textureMenu);;
        bgImageMenu.setOrigin(1280 / 2, 720 / 2);
        bgImageMenu.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        while (window.pollEvent(evMenu))
        {
            switch (evMenu.type)
            {
            case Event::MouseButtonPressed:
            {
                switch (evMenu.key.code)
                {
                case Mouse::Left:

                    if (evMenu.mouseButton.x >= 530 && evMenu.mouseButton.x <= 750)
                    {
                        if (evMenu.mouseButton.y >= 335 && evMenu.mouseButton.y <= 392)
                        {
                            setup(window);
                        }

                        if (evMenu.mouseButton.y >= 459 && evMenu.mouseButton.y <= 516)
                        {
                            cout << "Options";
                        }

                        if (evMenu.mouseButton.y >= 581 && evMenu.mouseButton.y <= 638)
                        {
                            window.close();
                        }
                    }
                }
            }
            }
        }

        window.draw(bgImageMenu);

        window.display();
    }
}