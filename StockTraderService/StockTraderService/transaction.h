#pragma once
#include<string>
#include <time.h>
#include <cpprest\json.h>
#include <cpprest\http_listener.h>

using namespace std;
using namespace web;
using namespace web::http::experimental::listener;
using namespace web::http;

class transaction
{
public:
	int iStockCode;
	int iQuantity;
	string strDate;
	string strStatus;
	string strType;
	transaction();
	transaction(int StockCode, int Quantity, string date, string strStatus, string strType);
	~transaction();

	json::value ToJSON() const;
	static transaction FromJSON(json::value object)
	{
		const json::value &stockCode = object[L"stockcode"];
		const json::value &quantity = object[L"quantity"];
		const json::value &datetime = object[L"datetime"];
		const json::value &status = object[L"status"];
		const json::value &type = object[L"type"];

		transaction result(stockCode.as_integer(), quantity.as_integer(), utility::conversions::to_utf8string(datetime.as_string()),
			utility::conversions::to_utf8string(status.as_string()), utility::conversions::to_utf8string(type.as_string()));

		return result;
	}
};

