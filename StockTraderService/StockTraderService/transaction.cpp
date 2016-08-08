#include "stdafx.h"
#include "transaction.h"


transaction::transaction()
{
}

transaction::transaction(int StockCode, int Quantity, string date, string strStatus, string strType)
{
	iStockCode = StockCode;
	iQuantity = Quantity;
	strDate = date;
	strStatus = strStatus;
	strType = strType;
}


transaction::~transaction()
{
}

json::value transaction::ToJSON() const
{
	json::value result = json::value::object();
	result[L"stockcode"] = json::value::number(iStockCode);
	result[L"quantity"] = json::value::number(iQuantity);
	result[L"datetime"] = json::value::string(utility::conversions::to_string_t(strDate));
	result[L"status"] = json::value::string(utility::conversions::to_string_t(strStatus));
	result[L"type"] = json::value::string(utility::conversions::to_string_t(strType));
	return result;
}
