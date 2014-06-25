#include "Window.h"
#include "MainGame.h"

#include "AudioPlayer.h"

#include <chrono>
#include <thread>


int main(int argc, char* argv[])
{
	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");
	qfcgame::MainGame* game = new qfcgame::MainGame();

	qfcbase::AudioPlayer::SetMusicPath("Content/bgm/");
	qfcbase::AudioPlayer::SetMusicPath("Content/sound/");


	qfcbase::AudioPlayer::PlayBGM("Call to Adventure");

	window.Run(game);

	return 0;
}