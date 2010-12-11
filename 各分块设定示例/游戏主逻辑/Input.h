







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
	
	// 以暗黑鼠标模式为例。
	
	//
	// 左键按下时的行为：
	// 	点到空地，移动
	//	点到怪物，对应列表中的法术（包括普通攻击）。
	//	其中普通攻击的行为：先移动到怪物身边再攻击，可以
	//	用法术脚本来完成。
	// 右键按下时的行为：
	// 	施法
	//
	
	int x, z;
	int button = CMouse::ButtonDown();
	
	// 只写简单的鼠标按下示例。。鼠标拖动、抬起、键盘事件同理。
	if(button)
	{
		GetCursorPos(&point);
		
		int object = CRender::HitTest(point.x, point.y);
		void * spirit = CSpirit::GetSpirit(object);
		
		// 转化为世界坐标
		ParseToWorldCoordinates(point, &x, &z);

		if(spirit == NULL)
		{
			// 这里判断是点击到了空地或是操作物品栏。
			if(CMod::IsMouseInPanel(point.x, poin.y))
			{
				// 使用物品的示例。当然，使用物品不一定非要发送
				// 给CHero类处理。这里只是遵循一个类只完成单一
				// 功能的思想。
				CItem * item = CMod::GetPanelItem(point.x, point.y);
				if(item != NULL)
				{	
					// 右键点下使用物品。
					// 左键点下拖动物品。
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
			
			// 不在物品栏内。那么就是点到空地了。
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
				// 取得相应侧法术ID
				// 当然，可以不指向怪物施法。
				arg1 = CMod::GetSpellID(CMouse::ButtonDown());	
				arg2 = x;
				arg3 = y;
				arg4 = spirit->GetObjID();	//怪物ID
			}else{
				// 同法术一样，判断与NPC距离的事情交跟调用者来
				// 搞定。究竟是直接对话还是走几步再对话，与这个
				// 类无关。
				input = INPUT_USENPC;
				arg1 = spirit->GetObjID();
			}
		}
		
	}
	
	
}












