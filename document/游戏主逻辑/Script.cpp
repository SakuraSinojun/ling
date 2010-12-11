


/*


	这个类的存在是为了管理各种脚本。
	
	考虑到脚本设计不完善的情况，比如出现一个死循环或者调用了Sleep()函数，每
	个脚本函数都应该有一个可接受的执行时间。时超过这个时间而脚本还没有结束
	时，应该考虑强制结束这个脚本的执行以确保游戏流程。
	
	在这个思想下，也许每个脚本函数的执行都应该做成独立的线程，当一个线程超时
	时，强制中止这个线程。


*/

CScript::CScript()
{

	// 初始化游戏脚本
	InitModScript();

	// 初始化魔法插件。（每个魔法都是单一DLL）
	InitSpell();

	
}



CScript::CastStart()
{
	CCast::SomeFunc......
}


CScript::CastProcessing()
{
	CCast::SomeFunc......
}




