
/*

	这个类用来管理所有怪物。
	

*/



// 注意：这个函数的示例中只描述了一只怪物的行为规范，事实上，这个函数需要遍历
// 所有登记在册的怪物。
CMonster::WanderAbout()
{
	// 如果怪物进入战斗并且没在屏幕的一定范围内，就不要为它计算。
	if(!IsInHeroArea() && !IsInBattle())
	{
		return;
	}

	// 这里应该判断怪物与玩家的距离，当玩家走进怪物的警戒范围内时，怪物不应
	// 该继续走来走去。同样，NPC类也是，当一个NPC与玩家互动时，他不应该还在
	// 到处游荡。
	if(DistanceTest())
	{
		// 战斗时，不可能玩家一走出怪物的初始警戒范围后怪物就不再追击。
		// 所以每个怪物都应该有两个警戒范围。
		EnlargeWarningDistance();
		
		// 怪物的各种AI。。。
		CScript::MonsterScript(this->GetObjID(), ...);
	}else{
		// NPC与怪物的共有表现都是走来走去，只是乱逛的路点或脚本不同。
		// 因此可以把这个事件抽象出来。
		ReduceWarningDistance();
		CCreature::WanderAbout();
		
	}

}

