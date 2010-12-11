


/*
	
	模组类。这个类用来加载一个模组，还有一些模组中的通用规则与通用脚本。
	比如物品栏、任务记录、操作面板等。
	
	（或许界面布置上也可以考虑用脚本完成？像wow那样。~）

*/



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



CMod::ParseInput(input, arg1, arg2, arg3, arg4)
{
	
	switch(input)
	{
	case INPUT_USEITEM:
	case INPUT_DRAGITEM:
		input = 0;
	}
}






