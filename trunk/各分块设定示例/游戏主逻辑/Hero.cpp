
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
	
	// ����������ڵ�Ŀ�ļ�CHero::Move��ע�͡�
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

// arg1��arg2Ӧ�����������ꡣ
// CHero����������ٶ��������ƶ��ľ����붯����
// teleport���ڰѽ�ɫֱ�Ӵ��͵���һ���ص㡣
// ����Ӧ�������Ǹ����ٶȼ���ÿ��ѭ�����ƶ����������ƶ���ɫ�Լ���ͼ��
// ���������и����⣬���ֻ������һ��Ŀ�ĵأ�������ΪӦ���ǣ�ÿ��ѭ���Բ�ͬ��
// �����ߵ�Ŀ�ĵز�ֹͣ���������ֻ�Ǽ򵥵ذ���Ϊת��Ϊ�ƶ�������֡���������
// �����ж���Ȼ��ÿ�ε��ֻ�ƶ�����һ�����롱�����⡣
// Ϊ���������⣬���塰��Ϊ���桱���ƣ�ÿ����ѭ���У��ѵ�ǰ����Ϊ��Ҳ������û
// ����Ϊ���뻺�����Ϊ���Ƚϣ�����г�ͻ��ִ�е�ǰ��Ϊ��û�г�ͻ�������ʱ����
// û������Ϊ���������ִ�л����е���Ϊ��ֱ����ǰ�ж���������CAction�ࣩ
void CHero::Move(arg1, arg2, arg3)
{
	x = calc_dest_x(arg1, arg2);
	z = calc_dest_z(arg1, arg2);
	if(CCreature::Move(arg1, arg2, arg3))
	{
		CMap::MoveCamera(x, z);
	}else{
		CAction::Clear();	// �����ƶ����ƶ�����ʱ�������Ϊ���档
	}
}


// ��ɫ�����湥������ʱû�뵽����������ʲô��
// �ӳ�������Ͻ�����ͨ����Ҳ������Ϊħ����һ�֡�
void CHero::Attack(arg1, arg2, arg3, arg4)
{
	Cast(arg1, arg2, arg3, arg4);
}


// arg1--ħ������
// arg2, arg3 ʩ����
// arg4--ʩչĿ�ꡣ��������Ŀ�ꡣ
void CHero::Cast(arg1, arg2, arg3, arg4)
{
	CSpell::CastSpellHero(arg1, arg2, arg3, arg4);
}
























