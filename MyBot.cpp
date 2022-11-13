#include "hlt/game.hpp"
#include "hlt/constants.hpp"
#include "hlt/log.hpp"

#include <random>
#include <ctime>
#include <map>

#include "ShipBlackboard.h"
#include "BehaviorTree.h"
#include "ShipBehavior.h"
#include "ShipDestination.h"


using namespace std;
using namespace hlt;

int main(int argc, char* argv[]) {
    unsigned int rng_seed;
    if (argc > 1) {
        rng_seed = static_cast<unsigned int>(stoul(argv[1]));
    } else {
        rng_seed = static_cast<unsigned int>(time(nullptr));
    }
    //mt19937 rng(rng_seed);
    ShipBlackboard::rng = mt19937(rng_seed);
    ShipBlackboard::spDropoffOrdered = false;

    map<int, shared_ptr<ShipBlackboard>> ship_controllers;

    // create ship behavior tree
    Sequence* ship_behavior = new Sequence();
    // destination selection branch
    Selector* choose_destination = new Selector();
        choose_destination->mChildren.push_back(new CheckMakeDropoff());
        choose_destination->mChildren.push_back(new CheckHasDestination());
        Sequence* if_full_go_back = new Sequence();
            if_full_go_back->mChildren.push_back(new CheckIsFull());
            if_full_go_back->mChildren.push_back(new GoToDepot());
        choose_destination->mChildren.push_back(if_full_go_back);
        choose_destination->mChildren.push_back(new FindMoreHalite());
        choose_destination->mChildren.push_back(new DontMove()); // always succeeds and sets some sort of destination
    ship_behavior->mChildren.push_back(choose_destination);
    // direction selection branch
    Selector* set_direction = new Selector();
        set_direction->mChildren.push_back(new MakeDropoff());
        Sequence* has_halite = new Sequence();
            has_halite->mChildren.push_back(new CheckEnoughHaliteToMove());
            Selector* nav_or_random = new Selector();
                Sequence* navigation = new Sequence();
                    navigation->mChildren.push_back(new Navigate());
                    navigation->mChildren.push_back(new CheckCanMoveThere());
                nav_or_random->mChildren.push_back(navigation);
                nav_or_random->mChildren.push_back(new RandomFreeDirection());
            has_halite->mChildren.push_back(nav_or_random);
        set_direction->mChildren.push_back(has_halite);      
        set_direction->mChildren.push_back(new StayStill()); // always succeds and sets some sort of direction
    ship_behavior->mChildren.push_back(set_direction);
    // last branch pushes the move on the queue
    ship_behavior->mChildren.push_back(new MoveNextDirection());


    
    Game game;
    // At this point "game" variable is populated with initial map data.
    // This is a good place to do computationally expensive start-up pre-processing.
    // As soon as you call "ready" function below, the 2 second per turn timer will start.

    ShipBlackboard::spGame = &game;
   

    game.ready("Guillaume David");
    
    

    log::log("Successfully created bot! My Player ID is " + to_string(game.my_id) + ". Bot rng seed is " + to_string(rng_seed) + ".");

    for (;;) {
        game.update_frame();
        shared_ptr<Player> me = game.me;
        unique_ptr<GameMap>& game_map = game.game_map;
        
        vector<Command> command_queue;

        // TODO clean ship_controllers of dead ships


        for (const auto& ship_iterator : me->ships) {
            shared_ptr<Ship> ship = ship_iterator.second;

            
            shared_ptr<ShipBlackboard> controller;
            auto it = ship_controllers.find(ship->id);
            if (it != ship_controllers.end())
            {
                // ship was in the map
                controller = it->second;
                controller->ship = ship;
            }
            else
            {
                // ship is new, create its controller and add it to the map
                controller = make_shared<ShipBlackboard>(ship, ship_behavior);
                ship_controllers.insert(make_pair(ship->id, controller));
            }
            
            controller->PushCommand(command_queue);
        }


        
        if (
            game.turn_number <= 250 &&
            me->halite >= constants::SHIP_COST &&
            !game_map->at(me->shipyard)->is_occupied() &&
            me->ships.size() < max(10, (int)me->dropoffs.size() * 4)) 
            
        {
            command_queue.push_back(me->shipyard->spawn());
        }
        

        if (!game.end_turn(command_queue)) {
            break;
        }
    }

    return 0;
}
