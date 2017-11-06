//////////////////////////
//                      // 
//   Hero.cpp           //
//                      //
//   CS300-Group D      //
//   Fall 2017          //
//   Ben Portis         //
//                      //
////////////////////////

#include "hero.h"

using namespace std;

//Default constructor: sets energy level to 100, x,y to zero, whiffles to zero, and name/invetory to NULL
Hero::Hero() : energy(100), whiffles(0), x(0), y(0), name(""), inventory("")
{
    
}


//Parameterized constructor: sets energy/whiffles/name/inventory to the passed in arguments
Hero::Hero(int energy, int whiffles, int x, int y, string name, string inventory) : energy(energy), whiffles(whiffles), x(0), y(0), name(name), inventory(inventory)
{
    setCoords(x,y);
}

//Hero destructor
Hero::~Hero()
{

}

//Display the Hero's name,stats, and items
void Hero::displayHero()
{
    cout << "Name: " << name << endl;
    cout << "Energy level: " << energy << endl;
    cout << "Whiffle count: " << whiffles << endl;
    cout << "Inventory contents : " << inventory << endl;
    cout << "X,Y coordinates : " << x << "," << y << endl << endl;
}

//Sets the Hero's name to a string passed in
void Hero::setName(string newName)
{
    name = newName;
}

//Returns the hero's name
string Hero::getName()
{
    return name;
}


//Sets the Hero's whiffles to a value passed in, adds it to the hero's whiffles.
void Hero::setWhiffles(int newWhiffles)
{
    whiffles += newWhiffles;
}

//Rerturns the hero's whiffle count
int Hero::getWhiffles()
{
    return whiffles;
}

//Add an item to the inventory
void Hero::addItem(string item)
{
    //If inventory is not empty we'll add a comma and then add the item. 
    if(inventory.empty())
        inventory = item;
    else
        inventory = inventory + "," + " " + item;
}

//Returns the hero's inventory
string Hero::getInventory()
{
    return inventory;
}

//Sets the hero's energy, pass in a negative value to subtract
void Hero::setEnergy(int newEnergy)
{
    energy += newEnergy; 
}

//Returns the hero's energy
int Hero::getEnergy()
{
    return energy;
}

//Sets the hero's coordinates
void Hero::setCoords(int newX, int newY)
{
    x = newX;
    y = newY;
}

//Returns the hero's x coordinate
int Hero::getX()
{
    return x;
}

//Returns the hero's y coordinate
int Hero::getY()
{
    return y;
}