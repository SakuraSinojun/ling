

// ����ע��ƴ�ӵ�ͼ��
CMap::MoveCamera(x, z)
{
	if(!IsScreenInMap())
	{
		LoadNewMap();
	}
	CRender::MoveCamera(x, z);
}







