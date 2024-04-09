// CANTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "DBCConfig.h"
#include "CANManager.h"
#include "Util.h"

enum CommandEnum{
	Command_OpenDevice,
	Command_CloseDevice,
	Command_Send,
	Command_End,
	Command_Other,
};

const int STRING_BUFF_LEN = 1000;

char cmdStr[STRING_BUFF_LEN];

CommandEnum GetNextCommand(){
	scanf("%s", cmdStr);

	if (strcmp(cmdStr, "1") == 0)
		return Command_OpenDevice;
	if (strcmp(cmdStr, "2") == 0)
		return Command_CloseDevice;
	if (strcmp(cmdStr, "3") == 0)
		return Command_Send;
	if (strcmp(cmdStr, "4") == 0)
		return Command_End;

	return Command_Other;
}

std::string TCHAR2STRING(TCHAR *STR)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	delete chRtn;
	return str;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Init dbc
	if (argc < 2) {
		return 0;
	}
	std::string filepath = TCHAR2STRING(argv[1]);
	DBCConfig::GetInstance()->Init(filepath);

	// print help
	PrintMessage("--------------------------------");
	PrintMessage("OpenDevice:\t1");
	PrintMessage("CloseDevice:\t2");
	PrintMessage("Send:\t\t3");
	PrintMessage("End:\t\t4");
	PrintMessage("--------------------------------");

	bool bStop = false;
	CANManager& canManager = *CANManager::GetInstance();

	while (!bStop){
		CommandEnum cmd = GetNextCommand();

		switch (cmd){
		case Command_OpenDevice:
			canManager.OpenDevice();
			canManager.StartReceive();
			break;
		case Command_CloseDevice:
			canManager.CloseDevice();
			break;
		case Command_Send:
			canManager.Send();
			break;
		case Command_End:
			canManager.CloseDevice();
			bStop = true;
			break;
		case Command_Other:
			PrintMessage("Unknown command!");
			break;
		default:
			break;
		}
	}

	return 0;
}
