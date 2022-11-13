#pragma once
#include <random>
#include <unordered_map>

#include "hlt/command.hpp"
#include "hlt/ship.hpp"
#include "hlt/game.hpp"

#include "BehaviorTree.h"

using namespace std;
using namespace hlt;
class ShipBlackboard
{
	BTNode * mBehaviorTree;

	bool mCommandSet;
	Command mNextCommand;

public:
	static Game * spGame;
	static ShipBlackboard* spCurrent;
	static mt19937 rng;
	static bool spDropoffOrdered;

	shared_ptr<Ship> ship;
	Direction next_direction;
	Position destination;
	unordered_map<Position, int>* positions_in_range;
	bool make_dropoff = false;
	int dropoff_attraction = 0;

	ShipBlackboard(shared_ptr<Ship> ship, BTNode * BT);

	void PushCommand(vector<Command>& command_queue);
	bool SetCommand(Command com);
	void PopulateRange();
	void CalculateDropoffAttraction();
private:
	int max_range = 8;
	void AddToRange(Position pos, int cost);
};