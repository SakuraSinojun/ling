

// 生物移动。。。
bool CCreature::Move(arg1, arg2, arg3)
{

	teleport = arg3;	
	x = calc_dest_x(arg1, arg2);
	z = calc_dest_z(arg1, arg2);
	
	if(!CMap::IsPositionAvailable(x, z))
	{
		return false;
	}
	if(!teleport)
	{
		if(!CSpirit::CollisionDetection(x, z))
		{
			CRender::MoveObject(this->GetObjID(), x, z);
			return true;
		}
		return false;
	}else{
		CRender::MoveObject(this->GetObjID(), x, z);
		return true;
	}

}



void CCreature::WanderAbout()
{
	// 走来走去。。。
}