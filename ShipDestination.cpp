#include "ShipDestination.h"

#include "hlt/game.hpp"
#include "hlt/log.hpp"
#include "hlt/player.hpp"

#include "ShipBlackboard.h"
#include "ShipFunctions.h"

BT_state FindMoreHalite::Run() const
{

    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    
    Position pos = blackboard->ship->position;

    int halite_here = ShipBlackboard::spGame->game_map->at(blackboard->ship)->halite;
    
    int max_gain = 0;
    blackboard->PopulateRange();
    for (auto p : *blackboard->positions_in_range)
    {
        int gain = CalculateGain(p.first);
        if (gain > max_gain)
        {
            max_gain = gain;
            pos = p.first;
        }
    }
    
    blackboard->destination = pos;

    if (blackboard->destination == blackboard->ship->position)
    {
        return BT_state::Failure;
    }
    else
    {

        return BT_state::Success;
    }
    
}

BT_state CheckHasDestination::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    if (blackboard->destination == blackboard->ship->position)
    {
        return BT_state::Failure;
    }
    else
        return BT_state::Success;
}

BT_state CheckIsFull::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    if (blackboard->ship->is_full())
        return BT_state::Success;
    else
        return BT_state::Failure;
}

BT_state GoToDepot::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    int playerID = blackboard->ship->owner;
    Position depot = FindNearestDepot(blackboard->ship->position);
    blackboard->destination = depot;

    return BT_state::Success;
}

BT_state DontMove::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    blackboard->destination = blackboard->ship->position;
    return BT_state::Success;
}

BT_state CheckMakeDropoff::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    int halite = ShipBlackboard::spGame->me->halite;
    int min_halite = constants::DROPOFF_COST * 2;
    int num_dropoffs = ShipBlackboard::spGame->me->dropoffs.size();
    if (ShipBlackboard::spDropoffOrdered || blackboard->dropoff_attraction < DROPOFF_THRESHOLD || halite < min_halite || num_dropoffs > MAX_DROPOFFS)
        return BT_state::Failure;
    blackboard->make_dropoff = true;
    ShipBlackboard::spDropoffOrdered = true;
}
