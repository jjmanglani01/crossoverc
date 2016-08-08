#pragma once

#include <cpprest\json.h>
#include <cpprest\http_listener.h>
#include <iostream>

using namespace web;
using namespace web::http::experimental::listener;
using namespace web::http;

class stock
{
public:
	int iStockCode;
	double dLastsalePrice;
	stock();
	stock(int stockId, double lastsalePrice);
	~stock();
	json::value ToJSON() const;
	static stock FromJSON(json::value object)
	{
		const json::value &stockCode = object[L"stockcode"];
		const json::value &lastSalePrice = object[L"lastsaleprice"];

		stock result(stockCode.as_integer(), lastSalePrice.as_double());
		return result;
	}

};

