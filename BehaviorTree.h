#pragma once
#include <vector>

enum class BT_state { Success, Failure, Running};

class BTNode
{
public:
	virtual BT_state Run() const = 0;
};

class Composite : public BTNode
{
public:
	std::vector<BTNode*> mChildren;
};

class Decorator : public BTNode
{
public:
	BTNode* mChild;
};

class Selector : public Composite
{
public:
	BT_state Run() const;
};

class Sequence : public Composite
{
public:
	BT_state Run() const;
};
