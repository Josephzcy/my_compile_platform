#pragma once

#include <string>
#include <set>
#include "can_dbc/message.hpp"
#include "can_dbc/dbciterator.hpp"
#include "Util.h"

class DBCConfig
{

public:
	~DBCConfig();
	static DBCConfig* GetInstance();
	void Init(const std::string& filePath);
	bool HaveMessageID(unsigned int id);
	Message GetMessageByID(unsigned int id);

private:
	DBCConfig();
	void ClearDBC();

private:
	static DBCConfig* m_pInstance;

	DBCIterator *m_dbc;
	std::set<unsigned int> m_ids;
	Message m_emptyMessage;
	char strBuff[1000];
};
