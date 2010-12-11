

// 这里注意拼接地图。
CMap::MoveCamera(x, z)
{
	if(!IsScreenInMap())
	{
		LoadNewMap();
	}
	CRender::MoveCamera(x, z);
}







