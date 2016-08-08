#include "stdafx.h"
#include "stock.h"

stock::stock()
{
}

stock::stock(int stockId, double lastsalePrice)
{
	iStockCode = stockId;
	dLastsalePrice = lastsalePrice;
}


stock::~stock()
{
}

json::value stock::ToJSON() const
{
	json::value result = json::value::object();
	result[L"stockcode"] = json::value::number(iStockCode);
	result[L"lastsaleprice"] = json::value::number(dLastsalePrice);
	return result;
}
