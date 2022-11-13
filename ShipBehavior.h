#pragma once

#include "BehaviorTree.h"


class CheckEnoughHaliteToMove : public BTNode
{
public:
	BT_state Run() const;
};

class Navigate : public BTNode
{
public:
	BT_state Run() const;
};

class CheckCanMoveThere : public BTNode
{
public:
	BT_state Run() const;
};

class MoveNextDirection : public BTNode
{
public:
	BT_state Run() const;
};

class RandomFreeDirection : public BTNode
{
public:
	BT_state Run() const;
};

class StayStill : public BTNode
{
public:
	BT_state Run() const;
};

class MakeDropoff : public BTNode
{
public:
	BT_state Run() const;
};

