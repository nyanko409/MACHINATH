#pragma once

#include "mydirect3d.h"
#include "transform.h"

class BillBoard
{
public:
	Transform transform;
	float lifeCycle;
	float speed;
	bool isActive;
	
	float xVel, yVel, zVel;

	BillBoard(Transform trans = Transform(), float lifeCycle = 20, float speed = 1.0) : 
		transform(trans), lifeCycle(lifeCycle), speed(speed), isActive(false) 
	{
		xVel = yVel = zVel = 0;
	}

	~BillBoard() {}
};


void InitBillboard();
void UpdateBillboard();
void UninitBillboard();
void DrawBillboard();
