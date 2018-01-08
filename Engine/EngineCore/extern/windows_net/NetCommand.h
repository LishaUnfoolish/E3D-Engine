#pragma once
// command ���л��������ֽ�����ǰ���룬�ڶ���������ID����������ʼ��int������ȣ�����������

#include <iostream>
#include <string>
#include <windows.h>

struct NetCommand
{
	BYTE	FrontCode;
	BYTE	ID;
	int		Length;
	std::string	Content;
	int GetLength()
	{
		return 1 + 1 + 4 + Content.length();
	}
};

class CommandHandler
{
public:
	virtual void Handler(NetCommand * cmd) = 0;
};

enum NetCommandEnum
{
	CreateScenes		= 0,
	ShowParticle	= 1,
	ShowMagicWord	= 2,
	ChangeScenes	= 3,
	ClearScene		= 4,
	Login			= 5,
	PauseParticle	= 6,
	PauseWord		= 7,
	StopParticle	= 8,
	StopeWord		= 9,
	SelectParticle  = 10,
	FrameSpeed		= 11,
	SelectFilter	= 12,
	EnginePause		= 13,
	GotoNextFrame	= 14,
	LogOutputCMD	= 15,
};