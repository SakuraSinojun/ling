







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



CInput::ParseInput(int& input, int& arg1, int& arg2, int& arg3, int& arg4)
{
	
	// �԰������ģʽΪ����
	
	//
	// �������ʱ����Ϊ��
	// 	�㵽�յأ��ƶ�
	//	�㵽�����Ӧ�б��еķ�����������ͨ��������
	//	������ͨ��������Ϊ�����ƶ�����������ٹ���������
	//	�÷����ű�����ɡ�
	// �Ҽ�����ʱ����Ϊ��
	// 	ʩ��
	//
	
	int x, z;
	int button = CMouse::ButtonDown();
	
	// ֻд�򵥵���갴��ʾ����������϶���̧�𡢼����¼�ͬ��
	if(button)
	{
		GetCursorPos(&point);
		
		int object = CRender::HitTest(point.x, point.y);
		void * spirit = CSpirit::GetSpirit(object);
		
		// ת��Ϊ��������
		ParseToWorldCoordinates(point, &x, &z);

		if(spirit == NULL)
		{
			// �����ж��ǵ�����˿յػ��ǲ�����Ʒ����
			if(CMod::IsMouseInPanel(point.x, poin.y))
			{
				// ʹ����Ʒ��ʾ������Ȼ��ʹ����Ʒ��һ����Ҫ����
				// ��CHero�ദ������ֻ����ѭһ����ֻ��ɵ�һ
				// ���ܵ�˼�롣
				CItem * item = CMod::GetPanelItem(point.x, point.y);
				if(item != NULL)
				{	
					// �Ҽ�����ʹ����Ʒ��
					// ��������϶���Ʒ��
					if(button == BUTTON_RIGHT)
					{
						input = INPUT_USEITEM;
						arg2 = item->GetObjID();
					}else{
						input = INPUT_DRAGITEM;
						arg2 = item->GetObjID();
					}
				}
				return;
			}
			
			// ������Ʒ���ڡ���ô���ǵ㵽�յ��ˡ�
			if(button == BUTTON_LEFT)
			{
				input = INPUT_MOVE;
				arg2 = x;
				arg3 = y;
			}
		}else{
			
			if(spirit->GetCamp() == CAMP_OFFEND)
			{
				input = INPUT_CAST;
				// ȡ����Ӧ�෨��ID
				// ��Ȼ�����Բ�ָ�����ʩ����
				arg1 = CMod::GetSpellID(CMouse::ButtonDown());	
				arg2 = x;
				arg3 = y;
				arg4 = spirit->GetObjID();	//����ID
			}else{
				// ͬ����һ�����ж���NPC��������齻����������
				// �㶨��������ֱ�ӶԻ������߼����ٶԻ��������
				// ���޹ء�
				input = INPUT_USENPC;
				arg1 = spirit->GetObjID();
			}
		}
		
	}
	
	
}












