/* Daniel Dupriest, 2017
 * Testing communication between server and client with json library
 */

#include "game.hpp"

Game::Game()
{
    Logger log;
    Hero hero;
    json toSend;
    toSend["log"] = "";
    Grovnick * nextGrovnick = NULL;
}

Game::~Game()
{
}

void Game::checkHeroEnergy()
{
    //Punam, check hero energy in here please
	int hero_energy = hero.getEnergy();
    if(hero_energy < 1)
        endGame();
	
}

void Game::endGame()
{
    //Punam, alert box here maybe?
	toSend["alert"] = "You died!";

	hero.resetState(); //Paul Hubbard

	string defaultStateName = "default.txt";
	ifstream defaultState(defaultStateName);
	if(!loadGameState(defaultState, true))
		log.write("Could not load from '" + defaultStateName + "'.");
	defaultState.close();
}

void Game::endGameHappy(){
	toSend["alert"] = "You've found the Royal Diamonds! You win!";

	hero.resetState();	

	string defaultStateName = "default.txt";
	ifstream defaultState(defaultStateName);
	if(!loadGameState(defaultState, true))
		log.write("Could not load from '" + defaultStateName + "'.");
	defaultState.close();
}

bool Game::gameStateExists(const string filename)
{
    //function stub
	ifstream stateFile(filename);

	if(stateFile)
    {
        stateFile.close();
        return true;
    }

    stateFile.close();
    return false;
}

bool Game::loadGameState(ifstream &file, bool reloading)
{
    log.write("Inside loadGameState()");
    /* Get map, objects, and hero info from file and load into objects. */
    string identifier, dimensionsString, blankSpace;
	getline(file, identifier);
	if(identifier.empty())
		return false;
    log.write("Identifier is: " + identifier);

	getline(file, dimensionsString);
	if(dimensionsString.empty())
		return false;

    log.write("Dimensions is: " + dimensionsString);

	int dimensions = stoi(dimensionsString);

	getline(file, blankSpace);
	if(blankSpace.empty() || blankSpace[0] != '#')
		return false;

	hero.loadState(); //Paul Hubbard

	getline(file, blankSpace);
	if(blankSpace.empty() || blankSpace[0] != '#')
		return false;

	// Either the game is reloading the default map file after the hero's death or not
	if(reloading)
		return map.reloadDefaultFile(identifier, dimensions, file);
	else
		return map.loadFile(identifier, dimensions, file);
}

void Game::parseCommand(json input)
{
    string command = input["command"];
    if(command == "up" ||
        command == "down" ||
        command == "left" ||
        command == "right")
        tryToMove(command);
    else if(command == "space")
        tryToBuy();
    else if(command == "map")
        selectMap();
    else
        log.write("Command not recognized.");

    map.setHeroVisited(hero.getX(), hero.getY());
    map.setHeroVision(hero.getX(), hero.getY()); 
}

bool Game::saveGameState(ofstream &file)
{
    //Write out hero and map data here.
	map.saveIdentifier(file);
    hero.saveState(); //Paul Hubbard
    log.write("Hero X after hero.saveState(): " + to_string(hero.getX()));
    map.saveMap(file);
    return true;
}

void Game::selectMap()
{
    //Stub
}

void Game::sendData()
{
    toSend["hero"] = hero.toJson();
    toSend["map"] = map.toJson();

    //Send the header and json 
    cout << "Content-Type:application/json; charset=utf-8" << endl << endl;
    cout << toSend.dump();
}

void Game::setNextGrovnick(string command)
{
    int tempX = hero.getX();
    int tempY = hero.getY();
    log.write("hero.getX/Y inside setNextGrovnick function: " + to_string(tempX) + ", " + to_string(tempY));
    
    if(command == "up")
        tempY -= 1;
    else if(command == "down")
        tempY += 1;
    else if(command == "left")
        tempX -= 1;
    else if(command == "right")
        tempX += 1;
    
    log.write("X/Y to send to map.getGrovnick(x,y): " + to_string(tempX) + ", " + to_string(tempY));
    nextGrovnick = map.getGrovnick(tempX, tempY);
    if(!nextGrovnick)
        log.write("nextGrovnick is null!");
}

bool Game::terrainCanBeTraversed()
{
    //always true for now. Eventually we'll check for walls, boat, etc.
    return true;
}

void Game::tryToBuy()
{
    //Stub
}

void Game::tryToMove(string command)
{
    setNextGrovnick(command);
    if(terrainCanBeTraversed())
    {
        //Do lots more checking in here for items/objects
        //...

        //nextGrovnick->setVisited();
        int nextX = nextGrovnick->getX();
        int nextY = nextGrovnick->getY();
	string contentString = nextGrovnick->getContent();
        bool onDiamonds = false;
	if(onRoyalDiamond(contentString)){
		endGameHappy();
	}
	log.write("X/Y from nextGrovnick->getX/Y(): " + to_string(nextX) + ", " + to_string(nextY));
        hero.setCoords(nextX, nextY);

        //Deduct terrain movement cost from hero energy
        hero.changeEnergy(-1);
    }
    else
    {
        hero.changeEnergy(-1);
    }
    checkHeroEnergy();
}

bool Game::onRoyalDiamond(string content){
	if(content == "Royal Diamonds"){
		return true;
	}
	else
		return false;
		
}

int main()
{
    Game game;
    Logger log;
    log.wipe();
    CgiReader cgi;
	string gameStateName = "state.txt";
	string defaultStateName = "default.txt";

    if(game.gameStateExists(gameStateName))
    {
        log.write("Game state '" + gameStateName + "' exists. Attempting to load.");
        ifstream existingState(gameStateName);
        if(!game.loadGameState(existingState))
            log.write("Could not load from '" + gameStateName + "'.");
        existingState.close();
        game.parseCommand(cgi.getCommand());
    }
    else
    {
        log.write("Game state '" + gameStateName +"' does not exist. Attempting to load '" + defaultStateName + "'");
        ifstream defaultState(defaultStateName);
        if(!game.loadGameState(defaultState))
            log.write("Could not load from '" + defaultStateName + "'.");
        defaultState.close();
    }

    log.write("Attempting to save new state to '" + gameStateName + "'.");
    ofstream newState(gameStateName);
    if(!game.saveGameState(newState))
        log.write("Could not save new state to '" + gameStateName + "'.");
    newState.close();

    game.sendData();
    return 0;
}
