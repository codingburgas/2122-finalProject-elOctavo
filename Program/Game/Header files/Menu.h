#pragma once

void setupMenu(RenderWindow& window);

bool isAudioRunning(bool& audioToggle);

bool isShowingFPS(bool& showFPS);

void optionMenu(Event& ev, Texture texture, RenderWindow& window);

void setupVars(RenderWindow& window);

extern bool audioToggle;
extern bool showFPS;