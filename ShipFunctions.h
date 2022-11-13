#pragma once

#include "hlt/game.hpp"
#include "ShipBlackboard.h"

const int DROPOFF_RADIUS = 7;
const int DROPOFF_THRESHOLD = 200000;
const int MAX_DROPOFFS = 4;

Position FindNearestDepot(Position pos);

int CalculateGain(Position pos);

int TotalHaliteInRadius(Position pos, int radius);

Direction TurnLeft(Direction dir);

int DropoffAttractivity(Position pos);