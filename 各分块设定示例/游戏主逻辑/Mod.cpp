


/*
	
	ģ���ࡣ�������������һ��ģ�飬����һЩģ���е�ͨ�ù�����ͨ�ýű���
	������Ʒ���������¼���������ȡ�
	
	��������沼����Ҳ���Կ����ýű���ɣ���wow������~��

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






