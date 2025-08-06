#include "MyGame.h"
#include <memory>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<Framework> myGame(new MyGame);
	myGame->Run();
	return 0;
}