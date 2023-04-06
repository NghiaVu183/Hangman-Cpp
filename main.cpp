#include "Game.h"
#include "MySDL.h"

using namespace std;

const int PLAY_TIME = 100;               // Time of a hangman game
const int SCREEN_WIDTH = 950;           // SDL window width
const int SCREEN_HEIGHT = 700;          // SDL window height
const string WINDOW_TITLE = "Hangman";  // SDL window title

int main(int argc, char* argv[]) {
    MySDL* SDL = new MySDL(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL -> openFont("VeraMoBd.ttf", 30);
    srand(time(0));
    Game* hangman = new Game(SDL, PLAY_TIME);
    while (hangman -> playing) {
        hangman -> startGame();
        do {
            hangman -> renderGameSDL();
            hangman -> guessEvent();
            hangman -> handleGuess();
            hangman -> updateTime();
        } while (hangman -> guessing());
        hangman -> gameOver();
    }
    return 0;
}
