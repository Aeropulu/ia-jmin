#include "ShipBlackboard.h"

#include <random>

#include "hlt/ship.hpp"
#include "hlt/game.hpp"

#include "BehaviorTree.h"
#include "ShipFunctions.h"


hlt::Game * ShipBlackboard::spGame;
ShipBlackboard* ShipBlackboard::spCurrent;
mt19937 ShipBlackboard::rng;
bool ShipBlackboard::spDropoffOrdered = false;

ShipBlackboard::ShipBlackboard(shared_ptr<Ship> ship, BTNode * BT)
{
    this->mBehaviorTree = BT;
	this->ship = ship;
    this->destination = ship->position;
    this->positions_in_range = new unordered_map<Position, int>();
}



void ShipBlackboard::PushCommand(vector<Command>& command_queue)
{
    
    ShipBlackboard::spCurrent = this;
    log::log("ship ID is " + to_string(ship->id) + " position is " + to_string(ship->position.x) + ", " + to_string(ship->position.y));

    mCommandSet = false;
    mNextCommand = this->ship->stay_still();

    CalculateDropoffAttraction();
    mBehaviorTree->Run();
    command_queue.push_back(mNextCommand);
    
    
    
}

bool ShipBlackboard::SetCommand(Command com)
{
    

    if (this->mCommandSet)
        return false;
    this->mNextCommand = com;
    mCommandSet = true;
    return true;
}



void ShipBlackboard::PopulateRange()
{
    positions_in_range->clear();
    AddToRange(ship->position, 0);
}

void ShipBlackboard::CalculateDropoffAttraction()
{
    dropoff_attraction = DropoffAttractivity(ship->position);
    log::log(to_string(dropoff_attraction));
}

void ShipBlackboard::AddToRange(Position pos, int cost)
{
    if (spGame->game_map->calculate_distance(ship->position, pos) > max_range)
        return;
    if (positions_in_range->count(pos) == 0)
        positions_in_range->insert(make_pair(pos, cost));
    else if (cost < positions_in_range->at(pos))
        positions_in_range->at(pos) = cost;
    else
        return;
    int total_cost = cost + (spGame->game_map->at(pos)->halite / 10);
    if (total_cost > ship->halite)
        return;

    for (Position p : pos.get_surrounding_cardinals())
        AddToRange(p, total_cost);

}
