
enum INPUT
{
	INPUT_NONE,
	INPUT_MOVE,
	INPUT_ATTACK,
	INPUT_CAST,
	INPUT_USEITEM,
	INPUT_USENPC,
	INPUT_DRAGITEM,
};





class CHero : public CCreature;



CHero::ParseInput(input, arg1, arg2, arg3, arg4)
{
	
	// 这个函数存在的目的见CHero::Move的注释。
	CAction::ConflictTest(input, arg1, arg2, arg3, arg4);
	
	switch(input)
	{
	case INPUT_MOVE:
		CHero::Move(arg1, arg2, arg3, arg4);
		break;
	case INPUT_ATTACK:
		CHero::Attack();
		break;
	......
	}

}

// arg1与arg2应该是世界坐标。
// CHero类根据生物速度来决定移动的距离与动画。
// teleport用于把角色直接传送到另一个地点。
// 这里应该做的是根据速度计算每个循环的移动量，并且移动角色以及地图。
// 但是这里有个问题，如果只用鼠标点一下目的地，正常行为应该是：每个循环以不同的
// 动作走到目的地才停止。所以如果只是简单地把行为转化为移动，会出现“不点鼠标就
// 不会行动，然而每次点击只移动很少一点点距离”的问题。
// 为解决这个问题，定义“行为缓存”机制，每个主循环中，把当前的行为（也可能是没
// 有行为）与缓存的行为作比较，如果有冲突就执行当前行为，没有冲突（更多的时候是
// 没有新行为）的情况下执行缓存中的行为，直到当前行动结束。（CAction类）
void CHero::Move(arg1, arg2, arg3)
{
	x = calc_dest_x(arg1, arg2);
	z = calc_dest_z(arg1, arg2);
	if(CCreature::Move(arg1, arg2, arg3))
	{
		CMap::MoveCamera(x, z);
	}else{
		CAction::Clear();	// 不能移动或移动结束时，清空行为缓存。
	}
}


// 角色向正面攻击。暂时没想到三个参数有什么用
// 从抽象层面上讲，普通攻击也可以视为魔法的一种。
void CHero::Attack(arg1, arg2, arg3, arg4)
{
	Cast(arg1, arg2, arg3, arg4);
}


// arg1--魔法索引
// arg2, arg3 施法点
// arg4--施展目标。可能是无目标。
void CHero::Cast(arg1, arg2, arg3, arg4)
{
	CSpell::CastSpellHero(arg1, arg2, arg3, arg4);
}
























