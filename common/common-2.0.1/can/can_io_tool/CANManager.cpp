
#include "stdafx.h"
#include <process.h>
#include "DBCConfig.h"
#include "CANManager.h"
#include "can_io/ControlCAN.h"
#include "Util.h"


bool CANManager::m_bReceiveFlag = false;
CANManager* CANManager::m_pInstance = NULL;

CANManager* CANManager::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CANManager();

	return m_pInstance;
}

CANManager::CANManager()
{
	m_nSendFrameFormat = 0;
	m_nSendFrameType = 0;
	m_strSendData = "7C 09 00 00 00 00 40 09";
	m_strSendID = "00 00 01 1E";
	m_radioIDFormat = 1;
	m_nCanIndex = 0;
	m_DevIndex = 0;
	m_DevType = VCI_USBCAN2;

	m_handle = NULL;
}

CANManager::~CANManager()
{
}

//打开设备
void CANManager::OpenDevice()
{
	DWORD Reserved = 0;
	//打开设备
	if (VCI_OpenDevice(m_DevType, m_DevIndex, Reserved) != 1)
	{
		PrintMessage("open failed");
		return;
	}
	VCI_INIT_CONFIG InitInfo[1];
	InitInfo->Timing0 = 0x00;
	InitInfo->Timing1 = 0x14;
	InitInfo->Filter = 0;
	InitInfo->AccCode = 0x80000008;
	InitInfo->AccMask = 0xFFFFFFFF;
	InitInfo->Mode = 2;
	//初始化通道0
	if (VCI_InitCAN(m_DevType, m_DevIndex, 0, InitInfo) != 1)	//can-0
	{
		PrintMessage("Init-CAN failed!");
		return;
	}
	Sleep(100);
	//初始化通道0
	if (VCI_StartCAN(m_DevType, m_DevIndex, 0) != 1)	//can-0
	{
		PrintMessage("Start-CAN failed!");
		return;
	}
	//初始化通道1
	if (VCI_InitCAN(m_DevType, m_DevIndex, 1, InitInfo) != 1)	//can-1
	{
		PrintMessage("Init-CAN failed!");
		return;
	}
	Sleep(100);
	//初始化通道1
	if (VCI_StartCAN(m_DevType, m_DevIndex, 1) != 1)	//can-0
	{
		PrintMessage("Start-CAN failed!");
		return;
	}

	PrintMessage("Open successfule!\n Start CAN OK!");
}

//关闭设备
void CANManager::CloseDevice()
{
	StopReceive();
	if (VCI_CloseDevice(m_DevType, m_DevIndex) != 1)
	{
		PrintMessage("Close failed!");
		return;
	}

	PrintMessage("Close successful!");
}

//发送信息
void CANManager::Send()
{
	//从界面获取发送信息
	VCI_CAN_OBJ sendbuf[1];

	std::string str5[32];
	BYTE buf[50];
	BYTE SendID[10];
	std::string strtemp, strtemp1;
	std::string str1;
	int len, datanum = 0, IDnum = 0, newflag = 1, i;

	len = m_strSendID.length();	//from dialog
	for (i = 0; i < len; i++)
	{
		strtemp = m_strSendID[i];
		if (strtemp == " ")
			newflag = 1;
		else if (newflag == 1)
		{
			newflag = 0;
			strtemp = m_strSendID[i];
			if (i == (len - 1))
			{
				str5[IDnum] = "0" + strtemp;
			}
			else
			{
				strtemp1 = m_strSendID[i + 1];

				if (strtemp1 == " ")
					str5[IDnum] = "0" + strtemp;
				else
					str5[IDnum] = strtemp + strtemp1;
			}
			SendID[IDnum] = Str2Hex(str5[IDnum]);
			IDnum++;
			if (IDnum >= 4)
				break;
		}
	}

	newflag = 1;
	len = m_strSendData.length();
	for (i = 0; i < len; i++)
	{
		strtemp = m_strSendData[i];
		if (strtemp == " ")
			newflag = 1;
		else if (newflag == 1)
		{
			newflag = 0;
			strtemp = m_strSendData[i];
			if (i == (len - 1))
			{
				str5[datanum] = "0" + strtemp;
			}
			else
			{
				strtemp1 = m_strSendData[i + 1];

				if (strtemp1 == " ")
				{
					str5[datanum] = "0" + strtemp;

				}
				else
					str5[datanum] = strtemp + strtemp1;

			}
			buf[datanum] = Str2Hex(str5[datanum]);
			datanum++;
			if (datanum >= 8)
				break;
		}
	}
	sendbuf->ExternFlag = m_nSendFrameType;
	sendbuf->DataLen = datanum;
	sendbuf->RemoteFlag = m_nSendFrameFormat;
	if (m_nSendFrameFormat == 1)//if remote frame, data area is invalid
		for (i = 0; i < datanum; i++)
			buf[i] = 0;

	if ((sendbuf->ExternFlag) == 1)//
	{
		sendbuf->ID = 0x11E;
	}
	else//basic frame ID
	{

		if (m_radioIDFormat)//
		{
			sendbuf->ID = 0x11E;
		}
		else
		{
			sendbuf->ID = 0x11E;
		}
	}

	for (i = 0; i < datanum; i++)
		sendbuf->Data[i] = buf[i];
	/****************************************************************************/
	/******************************从界面获取发送信息完毕***********************/
	/****************************************************************************/
	int flag;

	if ((m_nCanIndex == 1) && (m_DevType != VCI_USBCAN2))
	{
		PrintMessage("the device only support CAN index 0");
		m_nCanIndex = 0;
	}
	//调用动态链接库发送函数
	flag = VCI_Transmit(m_DevType, m_DevIndex, m_nCanIndex, sendbuf, 1);//CAN message send
	if (flag < 1)
	{
		if (flag == -1)
			PrintMessage("failed- device not open\n");
		else if (flag == 0)
			PrintMessage("send error\n");
		return;
	}
}

//UINT CANManager::ReceiveThread(LPVOID v)
unsigned int __stdcall CANManager::ReceiveThread(PVOID pM)
{
	const int STRING_BUFF_LEN = 1000;

	char strBuff[STRING_BUFF_LEN];

	int k = 0;
	while (1)
	{
		int value;
		VCI_CAN_OBJ pCanObj[200];

		for (int kCanIndex = 0; kCanIndex < 2; kCanIndex++)
		{
			//调用动态链接看接收函数
			//value = VCI_Receive(m_DevType, m_DevIndex, kCanIndex, pCanObj, 200, 0);
			value = VCI_Receive(VCI_USBCAN2, 0, kCanIndex, pCanObj, 200, 0);
			//接收信息列表显示
			k++;

			if (value > 0){
				printf("Receive %d messages\n", value);
			}

			SYSTEMTIME systime;
			GetLocalTime(&systime);

			for (int num = 0; num < value; num++)
			{
				sprintf(strBuff, "receive message id %u\n", pCanObj[num].ID);
				PrintMessage(strBuff);
				if (DBCConfig::GetInstance()->HaveMessageID(pCanObj[num].ID))
				{
					const Message msg = DBCConfig::GetInstance()->GetMessageByID(pCanObj[num].ID);
					if (pCanObj[num].DataLen != msg.getDlc())
						continue;

					for (auto sig : msg)
					{
						double data = GetBitValue(pCanObj[num].Data, pCanObj[num].DataLen, sig.getStartbit(), sig.getLength());
						sprintf(strBuff, "%.6lf %.6lf %.6lf", data, sig.getFactor(), sig.getOffset());
						PrintMessage(strBuff);

						data *= sig.getFactor();
						data += sig.getOffset();

						sprintf(strBuff, "(%02d:%02d:%02d:%03d) %s : %.6lf %s", systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds, sig.getName().c_str(), data, sig.getUnit().c_str());
						PrintMessage(strBuff);
					}
				}
			}
		}
		Sleep(10);

		if (!m_bReceiveFlag)
			return 0;
	}

	return 1;
}

void CANManager::StartReceive()
{
	if (!m_bReceiveFlag)
	{
		m_bReceiveFlag = true;
		//开启接收线程
		//AfxBeginThread(ReceiveThread, 0);
		m_handle = (HANDLE)_beginthreadex(NULL, 0, ReceiveThread, NULL, 0, NULL);
	}
}

void CANManager::StopReceive()
{
	if (m_bReceiveFlag){
		m_bReceiveFlag = false;
		// stop
		CloseHandle(m_handle);
	}
}
