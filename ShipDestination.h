#pragma once

#include "BehaviorTree.h"

class FindMoreHalite : public BTNode
{
public:
	BT_state Run() const;
};

class CheckHasDestination : public BTNode
{
public:
	BT_state Run() const;
};

class CheckIsFull : public BTNode
{
public:
	BT_state Run() const;
};

class GoToDepot : public BTNode
{
public:
	BT_state Run() const;
};

class DontMove : public BTNode
{
public:
	BT_state Run() const;
};

class CheckMakeDropoff : public BTNode
{
public:
	BT_state Run() const;
};