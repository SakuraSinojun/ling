




// 只提供了主逻辑循环。至于模组的预加载，游戏菜单的实现等等都与这个循环无关。

void mainloop()
{
	int	input;
	int	arg1, arg2, arg3;
	
	// 如果怪物和NPC不是独立线程的话，这里应该使每个while所需的时间片
	// 尽可能地小。否则会出现玩家、怪物、NPC一人动一格的不连惯情况。
	// 获取到输入后，先调用CMod类（或者应该是其它类）以检查是否是操作物品面
	// 板或者菜单。如果是的话，把input清空，以便让CHero类直接返回。
	// 或者，用以下逻辑来实现：
	//	if(!CMod::ParseInput())
	//	{
	//		CHero::ParseInput(); 
	//	}
	// 上述方案的前提是while()的间隔足够短，以致玩家做出两个不同的操作时肯定
	// 会被分布在两次循环中，这种情况下不会遗漏玩家的任何操作。
	// 否则的话，就只能为input建立一个队列，以确保玩家操作不丢失。
	// 而整个游戏流程中时间占用最多的过程是渲染，所以这也是将渲染过程独立成
	// 一条线程的原因所在。（设想传统的单线程流水过程，在渲染时玩家的输入肯
	// 定会有不同程度的丢失，尤其是渲染一幅画面所用的时间超出了玩家反应时间
	// 时。更不用说用GetTickCount锁定帧速的机制。）
	while(true)
	{
		CInput::ParseInput(input, arg1, arg2, arg3, arg4);
		CMod::ParseInput(input, arg1, arg2, arg3, arg4);
		CHero::ParseInput(input, arg1, arg2, arg3, arg4);
		CSpell::CalcSpellFrame();	// 法术漂来漂去。。。
		CNpc::WanderAbout();		// NPC走来走去。。。
		CMonster::WanderAbout();	// 怪物也走来走去。。。
		CPet::WanderAbout();		// 如果有宝宝。它也走来走去。。
	}
	
}


























