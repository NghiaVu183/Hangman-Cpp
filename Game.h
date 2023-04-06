#ifndef GAME_H
#define GAME_H
#include <ctime>
#include <string>
#include "MySDL.h"
using namespace std;

class Game {
    const int MAX_BAD_GUESS = 7;  // số lần đoán sai tối đa
    string category;              // tên file chủ đề
    string catName;               // tên chủ đề
    int difficult = -1;           // độ khó
    MySDL* SDL;
    int playTime;
    int win;                      // số lần win
    int loss;                     // số lần thua
    int fl = 0;                      // cờ đổi ảnh
    bool quit;
    string word;
    char guessChar;
    string guessedWord;
    string guessedStr;
    int badGuessCount;
    int suggested;
    int maxSuggest;               // số lần gợi ý tối đa
    int timeLeft;
    string badGuess;              // kí tự sai
    time_t startTime;
    int animatedTime;

    //Mix_Chunk *gCo;

   public:
    bool playing;                       // Game is playing or not
    Game(MySDL*, int);
    void startGame();
    void chooseCategory();              // Chọn chủ đề
    void renderCategory();
    void chooseCategoryEvent();
    void chooseDifficulty();            // Chọn độ khó
    void renderDifficulty();
    void chooseDifficultyEvent();
    void initWord();
    void getSuggest();                  // Lấy gợi ý
    void updateGuessedWord();           // update guessed word
    void updateSuggest();               // update số gợi ý tối đa
    bool guessing();
    void guessEvent();
    void handleGuess();
    void updateTime();                  // update playing time
    void badGuessed();
    void gameOver();
    void checkContinue(SDL_Event);      // check if player want to continue playing game
    void renderGameSDL();
    void renderGameOverSDL(int);
    void createGameOverSDL();
    void renderHuman(char, int, int, int);
    void humanEvent(SDL_Event, bool&);
    //bool loadMedia();
    //void close();
};

#endif  // GAME_H
