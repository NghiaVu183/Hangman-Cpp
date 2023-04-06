#include "Game.h"
#include <ctime>
#include <tr1/unordered_map>
#include "MySDL.h"
#include "utility.h"

using namespace std::tr1;

Game::Game(MySDL* MySDL, int time) : SDL(MySDL), playTime(time)
{
    playing = true;
    win = 0;
    loss = 0;
}

void Game::startGame()
{
    quit = false;
    system("cls");
    chooseCategory();
    chooseDifficulty();
    initWord();
    guessedWord = string(word.length(), '-');
    guessChar = ' ';
    badGuessCount = 0;
    maxSuggest = 10;
    badGuess = "";
    suggested = 0;
    guessedStr = "";
    animatedTime = 0;
    time(&startTime);
    for (unsigned int i = 0; i < word.length(); i++)
        if (word[i] == ' ')
            guessedWord[i] = ' ';
    updateSuggest();
}

void Game::chooseCategory()
{
    category = "";
    while (category == "" && playing && !quit)
    {
        renderCategory();
        chooseCategoryEvent();
    }
}

void Game::chooseCategoryEvent()
{
    SDL_Event event;
    if (SDL_WaitEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            playing = false;
            quit = true;
        }
        else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
        {
            playing = false;
        }
        else if (event.type == SDL_KEYUP)
        {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName.length() == 1 && keyName[0] >= '1' && keyName[0] <= '9')
                switch (keyName[0])
                {
                case '1':
                    category = "sports.txt";
                    catName = "Sports";
                    break;
                case '2':
                    category = "countries.txt";
                    catName = "Countries";
                    break;
                case '3':
                    category = "jobs.txt";
                    catName = "Jobs";
                    break;
                case '4':
                    category = "fruits.txt";
                    catName = "Fruits";
                    break;
                case '5':
                    category = "plants.txt";
                    catName = "Plants";
                    break;
                case '6':
                    category = "animals.txt";
                    catName = "Animals";
                    break;
                case '7':
                	category = "feeling.txt";
                	catName = "Feelings";
                	break;
                case '8':
                    category = "all.txt";
                    catName = "All Categories 1->7";
                    break;
                case '9':
                    category = "dictionary.txt";
                    catName = "Super dictionary";
                    break;
                }
        }
    }
}

void Game::renderCategory()
{
    SDL->createImageBackground("main.png");
    SDL->createTextTexture("Choose word category:", 100, 25);
    SDL->createTextTexture("1. Sports", 150, 75);
    SDL->createTextTexture("2. Countries", 150, 125);
    SDL->createTextTexture("3. Jobs", 150, 175);
    SDL->createTextTexture("4. Fruits", 150, 225);
    SDL->createTextTexture("5. Plants", 150, 275);
    SDL->createTextTexture("6. Animals",150, 325);
    SDL->createTextTexture("7. Feelings", 150, 375);
    SDL->createTextTexture("8. All Categories 1->7", 150, 425);
    SDL->createTextTexture("9. Super dictionary", 150, 475);
    SDL->updateScreen();
}

void Game::chooseDifficulty()
{
    difficult = -1;
    while (difficult == -1 && playing && !quit)
    {
        renderDifficulty();
        chooseDifficultyEvent();
    }
}

void Game::chooseDifficultyEvent()
{
    SDL_Event event;
    if (SDL_WaitEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            playing = false;
            quit = true;
        }
        else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
        {
            playing = false;
        }
        else if (event.type == SDL_KEYUP)
        {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName.length() == 1 && keyName[0] >= '1' && keyName[0] <= '5')
                switch (keyName[0])
                {
                case '1':
                    difficult = 0;
                    break;
                case '2':
                    difficult = 1;
                    break;
                case '3':
                    difficult = 2;
                    break;
                }
        }
    }
}

void Game::renderDifficulty()
{
    SDL->createImageBackground("main.png");
    SDL->createTextTexture("Category: " + catName, 100, 25);
    SDL->createTextTexture("Choose word difficulty:", 100, 75);
    SDL->createTextTexture("1. Easy", 150, 125);
    SDL->createTextTexture("2. Medium", 150, 175);
    SDL->createTextTexture("3. Hard" , 150, 225);
    SDL->updateScreen();
}

void Game::renderHuman(char guessedChar, int num, int badGuessCount, int fl)
{
    time_t startT, endT;
    time(&startT);
    int i = -300;
    bool skip = false;
    while (i < 1000 && !skip)
    {
        SDL_Event event;
        humanEvent(event, skip);
        SDL->createImageBackground("hang0.png");
        if(num == 0){
            SDL->createImage("daden" + to_string(badGuessCount-1) + ".jpg", i , 0);
        }else{
            SDL->createImageBackground("vocuc" + to_string(fl) + ".png");
        }
        SDL->createTextTexture(string("There ") + (num == 1 ? "is " : "are ") + to_string(num) + " of " + guessedChar,  350, 580);
        SDL->createTextTexture("Press 'Space' to skip", 300, 650);
        SDL->updateScreen();
        i += 6;
    }
    time(&endT);
    animatedTime += difftime(endT, startT);
}

void Game::humanEvent(SDL_Event e, bool& skip)
{
    if (SDL_PollEvent(&e))
    {
        if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE)
            skip = true;
        if (e.type == SDL_QUIT)
        {
            playing = false;
            quit = true;
            skip = true;
        }
    }
}

void Game::initWord()
{
    word = chooseWord(category, difficult);
    if (word.empty())
    {
        cout << "No word available in " << category << endl;
        playing = false;
        quit = true;
    }
}

void Game::guessEvent()
{
    SDL_Event event;
    guessChar = ' ';
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            playing = false;
            quit = true;
        }
        else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
        {
            playing = false;
        }
        else if (event.type == SDL_KEYUP)
        {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName == "Escape")
                playing = false;
            else if (keyName == "Space")
                guessChar = '$';
            else if (keyName.length() == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z')
                guessChar = keyName[0];
        }
    }
}

void Game::handleGuess()
{
    if (guessChar == ' ') return;
    if (guessChar == '$'){
        fl = 1;
        getSuggest();
        }
    else if (contains(guessedStr, guessChar))
        return;
    else if (contains(word, guessChar))
    {
        fl = 0;
        updateGuessedWord();
        updateSuggest();
    }
    else if (!contains(badGuess, guessChar))
    {
        badGuessed();
        renderHuman(guessChar, 0, badGuessCount, fl);
    }
}

void Game::badGuessed()
{
    badGuessCount++;
    badGuess += guessChar;
    guessedStr += guessChar;
}

bool Game::guessing()
{
    return badGuessCount < MAX_BAD_GUESS && guessedWord != word && timeLeft > 0 && playing;
}

void Game::updateTime()
{
    time_t now;
    time(&now);
    timeLeft = playTime - difftime(now, startTime) + animatedTime;
}

void Game::gameOver()
{
    if (guessedWord != word)
        loss++;
    else
        win++;
    createGameOverSDL();
}

void Game::updateGuessedWord()
{
    int n = guessedWord.length(), numOfChar = 0;
    for (int i = 0; i < n; i++)
    {
        if (word[i] == guessChar)
        {
            guessedWord[i] = guessChar;
            numOfChar++;
        }
    }
    guessedStr += guessChar;
    renderHuman(guessChar, numOfChar, badGuessCount, fl);
}

void Game::updateSuggest()
{
    if (suggested < maxSuggest)
    {
        int suggest = 0, n = guessedWord.length();
        unordered_map<char, int> m;
        for (int i = 0; i < n; i++)
            if (guessedWord[i] == '-')
                m[word[i]]++;
        suggest = m.size();
        maxSuggest = suggest / 2;
    }
    if (suggested > maxSuggest) maxSuggest = suggested;
}

void Game::getSuggest()
{
    if (suggested < maxSuggest)
    {
        suggested++;
        while (true)
        {
            int i = rand() % guessedWord.length();
            if (guessedWord[i] == '-')
            {
                guessChar = word[i];
                updateGuessedWord();
                break;
            }
        }
    }
}

void Game::renderGameSDL()
{
    SDL->createImageBackground("hang" + to_string(badGuessCount) + ".png");
    SDL->createTextTexture("Time: " + to_string(timeLeft), 750, 20);
    SDL->createTextTexture("Win : " + to_string(win), 750, 65);
    SDL->createTextTexture("Loss: " + to_string(loss), 750, 105);
    SDL->createTextTexture("Current Guess    :     " + guessedWord, 100, 550);
    SDL->createTextTexture("Bad Guesses      :     " + badGuess, 100, 600);
    SDL->createTextTexture("Used suggestions :     " + to_string(suggested) + "/" + to_string(maxSuggest) + "   (Press 'Space')", 100, 650);
    SDL->updateScreen();
    SDL_Delay(10);
}

void Game::renderGameOverSDL(int imageIndex)
{
    string status = (guessedWord == word ? "free" : "hanged");
    SDL->createImageBackground(status + to_string(imageIndex) + ".png");
    if (timeLeft <= 0)
        SDL->createTextTexture("Time Up!!!", 600, 5);
    SDL->createTextTexture("Win : " + to_string(win), 750, 45);
    SDL->createTextTexture("Loss: " + to_string(loss), 750, 85);
    if (guessedWord == word)
        SDL->createTextTexture("Congratulations!!! You win!!! You are free.", 100, 550);
    else
        SDL->createTextTexture("Game Over!!! You are hanged!", 100, 550);
        SDL->createTextTexture("Correct word: " + word, 100, 600);
        SDL->createTextTexture("Press 'Enter' to keep playing, 'ESC' to exit.", 100, 650);
        SDL->updateScreen();

}

void Game::createGameOverSDL()
{
    int imageIndex = 0;
    while (!quit)
    {
        SDL_Event e;
        checkContinue(e);
        renderGameOverSDL(imageIndex);
        SDL_Delay(200);
        ++imageIndex %= 4;
    }
}

void Game::checkContinue(SDL_Event e)
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE))
        {
            playing = false;
            quit = true;
        }
        else if (e.type == SDL_KEYUP &&
                 (e.key.keysym.sym == SDLK_RETURN ||
                  e.key.keysym.sym == SDLK_RETURN2 ||
                  e.key.keysym.sym == SDLK_KP_ENTER))
        {
            playing = true;
            quit = true;
        }
    }
}
/*
bool Game::loadMedia()
{
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }
    gCo = Mix_LoadWAV("beat.wav");
}

void Game::close()
{
	Mix_FreeChunk(gCo);
	gCo = NULL;
	Mix_Quit();
}
*/
