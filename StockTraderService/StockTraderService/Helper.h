#pragma once

#include <cpprest/http_listener.h>
#include <iostream>

#include <array>
#include <time.h>
#include "mongo\bson\bsonelement.h"
#include "mongo/util/time_support.h"

using namespace std;
static class Helper
{
public:
	static utf8string convertToUtf8String(map<utility::string_t, utility::string_t> httpGetVars,utility::string_t strQuery)
	{
		auto foundQuery = httpGetVars.find(strQuery);
		utility::string_t requestValue = foundQuery->second;
		utf8string strValue = utility::conversions::to_utf8string(requestValue);
		return strValue;
	}


	static string convertDate(mongo::BSONElement dateData)
	{
		std::time_t time = dateData.Date().toTimeT();
		char buff[20];
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&time));
		return buff;
	}
};
