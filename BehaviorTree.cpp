#include "BehaviorTree.h"

#include "hlt/log.hpp"

BT_state Selector::Run() const
{
	BT_state result = BT_state::Failure;
	for (const auto& child : mChildren)
	{
		result = child->Run();
		if (result != BT_state::Failure)
		{
			return result;
		}
		
	}
	return BT_state::Failure;
}

BT_state Sequence::Run() const
{
	BT_state result = BT_state::Failure;
	//for (const auto& child : mChildren)
	for (int i = 0; i < mChildren.size(); i++)
	{
		BTNode *child = mChildren[i];
		result = child->Run();
		if (result != BT_state::Success)
		{
			return result;
		}
		
	}
	return BT_state::Success;
}
