
#include "stdafx.h"
#include "DBCConfig.h"
#include <iostream>
#include <stdexcept>
#include "Util.h"


DBCConfig* DBCConfig::m_pInstance = NULL;

DBCConfig::DBCConfig() : m_dbc(NULL)
{
}

DBCConfig::~DBCConfig()
{
	ClearDBC();
}

DBCConfig* DBCConfig::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new DBCConfig();

	return m_pInstance;
}

void DBCConfig::ClearDBC()
{
	if (m_dbc != NULL)
	{
		delete m_dbc;
		m_dbc = NULL;
	}

	m_ids.clear();
}

void DBCConfig::Init(const std::string& filePath)
{
	ClearDBC();

	try {
		m_dbc = new DBCIterator(filePath);
		for (auto msg : *m_dbc)
		{
			m_ids.insert(msg.getId());

			sprintf(strBuff, "%u %s", msg.getId(), msg.getName().c_str());
			PrintMessage(strBuff);
		}
	}
	catch (std::invalid_argument& ex) {
		std::cout << ex.what() << std::endl;
	}
}

bool DBCConfig::HaveMessageID(unsigned int id)
{
	if (m_dbc == NULL)
		return false;

	return (m_ids.find(id) != m_ids.end());
}

Message DBCConfig::GetMessageByID(unsigned int id)
{
	if (HaveMessageID(id))
	{
		for (auto msg : *m_dbc)
		{
			if (msg.getId() == id)
			{
				return msg;
			}
		}
	}

	return m_emptyMessage;
}
