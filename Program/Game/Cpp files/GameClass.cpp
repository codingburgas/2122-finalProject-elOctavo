#include "../Header files/GameClass.h";
#include "../Header files/El Octavo-Functions.h"
#include "../Header files/Menu.h"
#include "../Header files/Precompile.h"

namespace vars {
	Clock clock;
	Time currTime;
	int keyTime = 0;
	int frameJumped = 0;
	RectangleShape npcBody;
	RectangleShape npcBody2;
	Sprite image;
}

using namespace vars;

//consturctors - setup variables
Player::Player(Texture* texture, Vector2u imageCount, float switchTime, float speed)
{
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	this->speed = speed;
	this->totalTime = 0.0f;
	this->currentImage.x = 0;
	this->ramped = false;

	this->uvRect.width = texture->getSize().x / float(imageCount.x);
	this->uvRect.height = texture->getSize().y / float(imageCount.y);

	//movement mechanic
	this->faceLeft = true;
	this->row = 0;

	this->body.setSize(Vector2f(81.0f, 129.0f));
	this->body.setOrigin(81.0f / 2, 129.0f / 2);
	this->body.setPosition(300, 720 / 2);
	this->body.setTexture(texture);
}

//destructor
Player::~Player()
{
}

//update for displaying character
void Player::update(int row, float deltaTime, bool faceLeft)
{
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
}

void Player::moveCameraFirstStage(Sprite& adventureBgImage, float& deltaTime)
{
	adventureBgImage.move(-(225.0f * deltaTime), 0.f);
}

//moving camera with the character
void Player::moveCharacter(int& keyTime, RenderWindow& window, Sprite& adventureBgImage, float& deltaTime)
{
	if (keyTime < 1)
	{
		keyTime++;
	}

	//WASD moving
	if (keyTime >= 1)
	{
		if (Keyboard::isKeyPressed(Keyboard::D) && adventureBgImage.getPosition().x > -5582)
		{
			moveCameraFirstStage(adventureBgImage, deltaTime);
			keyTime = 0;
		}
	}
}


//Update for moving character
void Player::updateMovement(float deltaTime, RenderWindow& window, Sprite& adventureBgImage, Sound& soundWalk, Sound& soundJump, bool& toggle, CollisionBlock blocks[], int blocksSize)
{
	velocity.x = 0;
	jumped = false;

	if (body.getPosition().x >= 35.0f)
	{
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			if (soundWalk.getStatus() == 0 && isAudioRunning(audioToggle) == true) {
				soundWalk.play();
			}

			velocity.x -= abs(speed - 50.0f) * deltaTime;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		if (soundWalk.getStatus() == 0 && isAudioRunning(audioToggle) == true) {
			soundWalk.play();
		}

		velocity.x += abs(speed - 50.0f) * deltaTime;
	}

	if (!Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S)) {
		if (soundWalk.getStatus() == 2 && isAudioRunning(audioToggle) == true) {
			soundWalk.stop();
		}
	}


	if (toggle) {
		if (Keyboard::isKeyPressed(Keyboard::Space)) {

			currTime = vars::clock.getElapsedTime();
			if (currTime.asSeconds() >= 1.0f && !jumped) {
				if (isAudioRunning(audioToggle) == true)
				{
					soundJump.play();
				}

				vars::clock.restart();
				jumpY = body.getPosition().y;
				jump(deltaTime, 600);
				jumped = true;
			}
		}

		if (body.getPosition().x >= window.getSize().x / 2)
		{
			Player::moveCharacter(keyTime, window, adventureBgImage, deltaTime);
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				velocity.x = 0.000001;
			}
		}
	}
	else {
		velocity.y = 0;
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			if (soundWalk.getStatus() == 0 && isAudioRunning(audioToggle) == true) {
				soundWalk.play();
			}

			velocity.y -= abs(speed - 35.0f) * deltaTime;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			if (soundWalk.getStatus() == 0 && isAudioRunning(audioToggle) == true) {
				soundWalk.play();
			}

			velocity.y += abs(speed - 35.0f) * deltaTime;
		}
	}


	if (velocity.x == 0.0f)
	{
		row = 0;
	}
	else
	{
		row = 1;
		if (velocity.x > 0.0f)
		{
			faceLeft = true;
		}

		else
		{
			faceLeft = false;
		}
	}

	update(row, deltaTime, faceLeft);
	body.setTextureRect(uvRect);

	if (toggle) {
		velocity.y += 160.0f * deltaTime;

		if (checkCollideWithGround(body) && !ramped) {
			body.setPosition(body.getPosition().x, 490.0f);
		}

		if (checkCollideWithGround(body) && !jumped && !ramped) {
			velocity.y = 0;
		}

		float bodyY = body.getPosition().y;
		float bodyX = body.getPosition().x;

		for (int i = 0; i < 8; i++) {
			if (blocks[i].checkForCollision(body, npcBody,npcBody2,image)) {
				float hitboxX = blocks[i].hitbox.getPosition().x;
				float hitboxY = blocks[i].hitbox.getPosition().y;
				float hitboxSizeX = blocks[i].hitbox.getSize().x;
				float hitboxSizeY = blocks[i].hitbox.getSize().y;

				if (bodyX > hitboxX + hitboxSizeX && bodyY > hitboxY) {
					velocity.x = 0.f;
					body.setPosition(hitboxX + hitboxSizeX + 40.5f, bodyY);
				}
				else if (bodyX < hitboxX && bodyY > hitboxY) {
					velocity.x = 0.f;
					body.setPosition(hitboxX - 40.5f, bodyY);
				}

				if ((bodyY < hitboxY + hitboxSizeY && not bodyX < hitboxX) or (bodyY < hitboxY + hitboxSizeY && not bodyX > hitboxX + hitboxSizeX)) {
					if (!jumped) {
						velocity.y = 0.f;
					}
					jumped = false;

					body.setPosition(bodyX, hitboxY - 64.0f);
				}
				else if (((bodyY > hitboxY && not bodyX < hitboxX) or (bodyY > hitboxY && not bodyX < bodyX >= hitboxX + hitboxSizeX)) && i != 0) {
					if (jumped) {
						velocity.y = 0.f;
					}
					jumped = false;

					body.setPosition(bodyX, hitboxY + hitboxSizeY + 64.5f);
				}
			}
		}

		if (checkCollideWithRamp(body) && !jumped) {
			//if (bodyY == 490) {
			//	body.setPosition(bodyX, 480);
			//	bodyY = body.getPosition().y;
			//	//cout << bodyY << endl;
			//}

			if (Keyboard::isKeyPressed(Keyboard::D)) {
				if (-velocity.x < 999) {
					velocity.x = abs(speed - 500.0f) * deltaTime;
				}

			}
			else if (Keyboard::isKeyPressed(Keyboard::A)) {
				if (velocity.x > 999) {
					velocity.x = -abs(speed - 50.0f) * deltaTime;
				}
			}

			if (!jumped) {
				velocity.y = -velocity.x;
			}
			ramped = true;
		}
		else if (((bodyX > getRamp2Pos().x + 110 && (bodyX - (getRamp2Pos().x + 110) < 30.0f)) && bodyY > 450 && !ramped)) {
			cout << bodyY << endl;
			velocity.x = 0.f;
			body.setPosition(bodyX + 0.75f, bodyY);
		}
		else {
			if (checkCollideWithRamp(body) && frameJumped > 0) {
				cout << frameJumped << endl;
				jumped = false;
			}
			if (ramped) {
				ramped = false;
			}
		}
	}

	if (jumped) {
		frameJumped++;
		if (frameJumped > 1) {
			frameJumped = 0;
		}
	}

	body.move(velocity.x, velocity.y);

}

void Player::draw(RenderWindow& window)
{
	window.draw(body);
}

void Player::jump(float deltaTime, float jumpHeight) {
	velocity.y = -sqrtf((160.0f * jumpHeight) * 0.016);
}