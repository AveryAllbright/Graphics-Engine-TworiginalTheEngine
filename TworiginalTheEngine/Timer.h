#pragma once
class Timer
{
public:
	float fTime;
	float fDelt;
	float fPrev;
	float FPS;
	Timer();
	void Start();
	void Update();
	virtual ~Timer();
};

