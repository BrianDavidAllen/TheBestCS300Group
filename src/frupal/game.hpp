/* Daniel Dupriest, 2017
 * Sample game class
 */
#pragma once
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "cgiReader.hpp"
#include "grovnick.hpp"
#include "logger.hpp"
#include "player.hpp"
#include "map.hpp"
#include "hero.h"

using namespace std;
using json = nlohmann::json;

class Game
{
    public:
        Game();
        ~Game();
        void checkHeroEnergy();
        void endGame();
        bool gameStateExists();
        bool loadGameState(fstream &file);
        void parseCommand(json input);
        bool playerIsDead();
        bool saveGameState(fstream &file);
        void sendData();
        void selectMap();
        void setNextGrovnick(string command);
        bool terrainCanBeTraversed();
        void tryToBuy();
        void tryToMove(string command);

    private:
        json current;
        json toSend;
        Hero hero;
        Logger log;
		Map map;
        Grovnick * nextGrovnick;
};
