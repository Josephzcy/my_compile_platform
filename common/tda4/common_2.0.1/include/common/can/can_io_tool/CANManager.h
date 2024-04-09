#pragma once

#include <string>
#include "WTypesbase.h"

class CANManager
{

public:
	static CANManager* GetInstance();
	~CANManager();

	//static UINT ReceiveThread(void *param);
	static unsigned int __stdcall ReceiveThread(PVOID pM);

	void OpenDevice();
	void Send();
	void StartReceive();
	void StopReceive();
	void CloseDevice();

private:
	CANManager();

public:
	int		m_nSendFrameFormat;
	int		m_nSendFrameType;
	std::string	m_strSendData;
	std::string	m_strSendID;
	int		m_radioIDFormat;
	int		m_nCanIndex;
	int DeviceIndex;
	int m_DevType;
	int m_DevIndex;

	HANDLE m_handle;

	class CGarbo
	{
	public:
		~CGarbo()
		{
			if (CANManager::m_pInstance)
				delete CANManager::m_pInstance;
		}
	};
	static CGarbo Garbo;

private:
	static CANManager* m_pInstance;
	static bool m_bReceiveFlag;
};

