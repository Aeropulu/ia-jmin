#include "ShipFunctions.h"

#include "hlt/game.hpp"
#include "ShipBlackboard.h"

using namespace hlt;

Position FindNearestDepot(Position pos)
{
	int player_id = ShipBlackboard::spCurrent->ship->owner;
	Player& player = *ShipBlackboard::spCurrent->spGame->players[player_id];
	GameMap& map = *ShipBlackboard::spCurrent->spGame->game_map;
	Position nearest = player.shipyard->position;
	int min_distance = map.calculate_distance(pos, nearest);
	if (!player.dropoffs.empty())
	{
		for (auto pair : player.dropoffs)
		{
			Dropoff& d = *pair.second;
			int distance = map.calculate_distance(pos, d.position);
			if (distance < min_distance)
			{
				nearest = d.position;
				min_distance = distance;
			}
		}
	}
	return nearest;

}

int CalculateGain(Position pos)
{
	GameMap& map = *ShipBlackboard::spCurrent->spGame->game_map;
	int halite_gain_there = map.at(pos)->halite / 4;
	int total_cost = ShipBlackboard::spCurrent->positions_in_range->at(pos);
	int distance = map.calculate_distance(ShipBlackboard::spCurrent->ship->position, pos);
	// add distance to nearest depot
	distance += map.calculate_distance(pos, FindNearestDepot(ShipBlackboard::spCurrent->ship->position));

	return (halite_gain_there - total_cost) / (distance + 1);
}

int TotalHaliteInRadius(Position pos, int radius)
{
	GameMap& map = *ShipBlackboard::spCurrent->spGame->game_map;
	int total_halite = 0;
	for (int j = pos.y - radius; j <= pos.y + radius; j++)
	{
		int offset = abs(radius - (pos.y - j));
		for (int i = pos.x - offset; i <= pos.x + offset; i++)
		{
			total_halite += map.at(Position(i, j))->halite;
		}
	}
	return total_halite;
}

Direction TurnLeft(Direction dir)
{
	switch (dir)
	{
	case Direction::EAST:
		return Direction::NORTH;
	case Direction::NORTH:
		return Direction::WEST;
	case Direction::WEST:
		return Direction::SOUTH;
	case Direction::SOUTH:
		return Direction::EAST;
	}
	return Direction::STILL;
}

int DropoffAttractivity(Position pos)
{
	GameMap& map = *ShipBlackboard::spGame->game_map;
	return TotalHaliteInRadius(pos, DROPOFF_RADIUS) * map.calculate_distance(pos, FindNearestDepot(pos));
}
