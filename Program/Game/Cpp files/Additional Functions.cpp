#include "../Header files/Precompile.h";
#include "../Header files/Additional Functions.h";
#include "../Header files/Menu.h";

Texture backstoryTexture;
Sprite backstoryImage;

void changeFPS(int FPS, RenderWindow& window)
{
	window.setFramerateLimit(FPS);
}

//add windows icon
void addIcon(RenderWindow& window)
{
	Image icon;
	
	if (!icon.loadFromFile("../Images and fonts/Icon.png"))
	{
		cout << "Error";
	}

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void backstory(RenderWindow& window, Texture backstoryTexture, Sprite backstoryImage)
{
	Sound soundWrite;
	SoundBuffer writeBuffer;

	writeBuffer.loadFromFile("../Audios/Write.wav");

	soundWrite.setBuffer(writeBuffer);

	backstoryTexture.loadFromFile("../Images and fonts/Bg/BackstoryFrame.png");
	backstoryImage.setTexture(backstoryTexture);

	//Typewriting effect
	Font fontBackstory;
	fontBackstory.loadFromFile("../Images and fonts/Fonts/Header font.ttf");

	string Backstory = "Hello friend!\nI'm a Kurabirov and I'm here to tell you my story.\nSome time ago I was a professional boxer and I was very successful in that.\nWhen cryptocurrencies came out, I really liked them and started studying them.\nAnyway, because I knew how the market works,\nit was time to test my knowledge and bought stoshevcoin and staichocoin.\nBut things went wrong and these two cryptocurrencies collapsed.\nA lot of people have become attached to the propaganda that this will be\nthe currency of the future and we have lost all our money. Anyway, I became homeless.\nOne day, three costumed men abducted me in a van and put on a mask so I couldn't see. After a while,\nthey stopped the van, removed my mask, and I was in front of a mansion.\nIt turned out that the boss of the turkish mafia lives here(he also has a telsa).\nHe wanted me to place an order for him. Without telling me much, I agreed and here I am now.";
	Text textBackstory("", fontBackstory, 34);
	textBackstory.setFillColor(Color(0, 0, 0));
	textBackstory.setPosition(81, 205);

	//frame timing
	Clock timer;
	unsigned int character = 0;

	bool isTouched = false, isTouchedForBreak = false;

	if(isAudioRunning(audioToggle) == true)
	{
		soundWrite.play();
	}
	
	soundWrite.setVolume(200);
	Event ev;

	while (window.isOpen())
	{
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

			if (ev.type == Event::KeyReleased && ev.key.code == Keyboard::Enter) {
				isTouchedForBreak = true;
				textBackstory.setString(String(Backstory.substr(0, Backstory.length())));
            }
		}


		if (isTouched == false)
		{
			if (timer.getElapsedTime().asSeconds() > 0.05 && character < Backstory.length())
			{
				timer.restart();

				character++;

				textBackstory.setString(String(Backstory.substr(0, character)));
			}
		}
		else if (isTouched == true) {
			textBackstory.setString(String(Backstory.substr(0, Backstory.length())));
		}

		window.clear();
		window.draw(backstoryImage);
		window.draw(textBackstory);
		window.display();

		if (isTouchedForBreak == true)
		{
			break;

		}
	}

	soundWrite.stop();
}
