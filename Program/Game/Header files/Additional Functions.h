#pragma once
#include "Precompile.h"

void changeFPS(int FPS, RenderWindow& window);

void addIcon(RenderWindow& window);

void backstory(RenderWindow& window, Texture backstoryTexture, Sprite backstoryImage);

extern Texture backstoryTexture;
extern Sprite backstoryImage;