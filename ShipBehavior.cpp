#include "ShipBehavior.h"

#include "hlt/game.hpp"
#include "hlt/log.hpp"
#include "hlt/player.hpp"

#include "ShipBlackboard.h"
#include "ShipFunctions.h"

using namespace std;
using namespace hlt;




BT_state CheckEnoughHaliteToMove::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    Position pos = blackboard->ship->position;
    int halite_here = ShipBlackboard::spGame->game_map->at(pos)->halite;
    if (halite_here <= blackboard->ship->halite * 10)
        return BT_state::Success;
    else
        return BT_state::Failure;
}

BT_state Navigate::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    Position pos = blackboard->ship->position;
    
    if (pos == blackboard->destination)
    {
        blackboard->next_direction = Direction::STILL;
        return BT_state::Success;
    }
    else
    {
        blackboard->next_direction = ShipBlackboard::spGame->game_map->naive_navigate(blackboard->ship, blackboard->destination);
        if (blackboard->next_direction == Direction::STILL)
        {
            // navigation failed
            log::log("ID " + to_string(blackboard->ship->id) + " destination is " + to_string(blackboard->destination.x) + " " + to_string(blackboard->destination.y));
            log::log("ID " + to_string(blackboard->ship->id) + " navigation failed");
            return BT_state::Failure;
        }
        else
            return BT_state::Success;
    }
}


BT_state CheckCanMoveThere::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    if (blackboard->next_direction == Direction::STILL)
        return BT_state::Success;
    Position pos = blackboard->ship->position;
    Position nextpos = pos.directional_offset(blackboard->next_direction);
    MapCell* cell = ShipBlackboard::spGame->game_map->at(nextpos);
    if (cell->is_occupied() && cell->ship != blackboard->ship)
    {
        string tolog = "can't move ";
        tolog.push_back((char)blackboard->next_direction);
        tolog.append(" at " + to_string(nextpos.x) + " " + to_string(nextpos.y));
        log::log(tolog);
        return BT_state::Failure;
    }
    else
        return BT_state::Success;
}

BT_state MoveNextDirection::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    if (blackboard->SetCommand(blackboard->ship->move(blackboard->next_direction)))
    {
        MapCell *nextcell = ShipBlackboard::spGame->game_map->at(blackboard->ship->position.directional_offset(blackboard->next_direction));
        nextcell->mark_unsafe(blackboard->ship);
        return BT_state::Success;
    }
    else
        return BT_state::Failure;
}

BT_state RandomFreeDirection::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    Position pos = blackboard->ship->position;
    
    
    vector<Direction> safedirs;
    for (int i = 0; i < 4; i++)
    {
        Position checkpos = pos.directional_offset(ALL_CARDINALS[i]);
        if (!ShipBlackboard::spGame->game_map->at(checkpos)->is_occupied())
        {
            safedirs.push_back(ALL_CARDINALS[i]);
        }
    }
    int n = safedirs.size();
    if (n > 0)
    {
        int random = ShipBlackboard::rng() % n + 1;
        if (random == n)
            blackboard->next_direction = Direction::STILL;
        else
            blackboard->next_direction = safedirs[random];
        return BT_state::Success;
    }
    else
    {
        blackboard->next_direction = Direction::STILL;
        return BT_state::Failure;
    }
}

BT_state StayStill::Run() const
{
    ShipBlackboard::spCurrent->next_direction = Direction::STILL;
    return BT_state::Success;
}

BT_state MakeDropoff::Run() const
{
    ShipBlackboard* blackboard = ShipBlackboard::spCurrent;
    if (!blackboard->make_dropoff)
        return BT_state::Failure;
    blackboard->SetCommand(blackboard->ship->make_dropoff());
    ShipBlackboard::spDropoffOrdered = false;
}
