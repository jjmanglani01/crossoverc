// StockTraderService.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_DEPRECATE
#undef UNICODE
#define UNICODE
#undef _WINSOCKAPI_
#define _WINSOCKAPI_

#include <windows.h>
#include <winsock2.h>
#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h" // for the driver

#include <cpprest/http_listener.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cpprest\json.h>
#include "stdafx.h"
#include "stock.h"
#include "transaction.h"
#include "Helper.h"

using namespace std;
using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;


// cpprest provides macros for all streams but std::clog in basic_types.h
#ifdef _UTF16_STRINGS
// On Windows, all strings are wide
#define uclog std::wclog
#else
// On POSIX platforms, all strings are narrow
#define uclog std::clog
#endif // endif _UTF16_STRINGS

mongo::DBClientConnection c;

bool checkForCredential(utf8string strUsername, utf8string strPassword)
{
	auto_ptr<mongo::DBClientCursor> cursor = c.query("stock-trader.trader", MONGO_QUERY("username" << strUsername << "password" << strPassword));

	if (cursor->more())
	{
		return true;
	}
	return false;
}


bool checkForUserNameExist(utf8string strUsername)
{
	auto_ptr<mongo::DBClientCursor> cursor = c.query("stock-trader.trader", MONGO_QUERY("username" << strUsername));

	if (cursor->more())
	{
		return true;
	}
	return false;
}

void registerUser(utf8string strUsername, utf8string strPassword)
{
	mongo::BSONObjBuilder objBuilder;
	objBuilder << "username" << strUsername << "password" << strPassword << "balancecash" << 100000;
	c.insert("stock-trader.trader", objBuilder.obj());
}

json::value quote(int stockCode)
{
	auto_ptr<mongo::DBClientCursor> cursor = c.query("stock-trader.stock", MONGO_QUERY("stockcode" << stockCode));
	if (cursor->more())
	{
		mongo::BSONObj bsonObject = cursor->next();
		stock st;
		st.iStockCode = bsonObject["stockcode"].Double();
		st.dLastsalePrice = bsonObject["lastsaleprice"].Double();
		return st.ToJSON();
	}
	json::value jsonData;
	jsonData[L"error"] = json::value::string(U("Stockcode Not Found!!"));
	return jsonData;
}


json::value getTransaction(utf8string strUserName)
{
	auto_ptr<mongo::DBClientCursor> cursor = c.query("stock-trader.transaction", MONGO_QUERY("username" << strUserName));
	json::value arr = json::value::array();
	int i = 0;
	while (cursor->more())
	{
		mongo::BSONObj bsonObject = cursor->next();
		transaction tra;
		tra.iStockCode = bsonObject["stockcode"].Double();
		tra.iQuantity = bsonObject["quantity"].Double();
		tra.strDate = Helper::convertDate(bsonObject["datetime"]);
		tra.strStatus = bsonObject["status"].str();
		tra.strType = bsonObject["type"].str();
		arr[i] = tra.ToJSON();
		i++;
	}
	if (i == 0)
	{
		json::value jsonData;
		jsonData[L"error"] = json::value::string(U("No transaction found"));
		return jsonData;
	}
	return arr;
}

void connectToMongoDb()
{
	//Connect MongoDb
	try
	{
		c.connect("localhost:27017");
		mongo::client::initialize();
	}
	catch (exception& e)
	{
		throw e;
	}
}

int main()
{
	uclog << U("Starting listener.") << endl;
	http_listener listener(U("http://*/json"));
	listener.open().wait();

	// Handle incoming requests.
	uclog << U("Setting up JSON listener.") << endl;
	listener.support(methods::GET, [](http_request req) {
		uri reqUri = req.request_uri();
		/*utility::string_t foundPath = reqUri.path();
		ucout << foundPath << endl;*/
		vector<utility::string_t> vecPath = uri::split_path(reqUri.path());

		auto httpGetVars = uri::split_query(reqUri.query());

		auto foundName = httpGetVars.find(U("userName"));
		auto foundPassword = httpGetVars.find(U("password"));


		utility::string_t requestName = foundName->second;
		utility::string_t requestPassword = foundPassword->second;

		utf8string strFunc = utility::conversions::to_utf8string(vecPath[1]);
		utf8string strUserName = utility::conversions::to_utf8string(requestName);
		utf8string strPassword = utility::conversions::to_utf8string(requestPassword);

		const char* strLogin = "login";
		const char *strRegister = "RegisterTrader";
		const char *strBuy = "buy";
		const char *strSell = "sell";
		const char *strQuote = "Quote";
		const char *strPortfolio = "portfolio";
		const char *strTransactions = "Transactions";

		//Connect MongoDb
		c.connect("localhost");
		mongo::client::initialize();

		if (strFunc == strLogin)
		{
			bool bCorrect = checkForCredential(strUserName, strPassword);
			cout << "login request recieved: "<<bCorrect << endl;
			json::value jsonData = json::value::object();
			jsonData[L"login"] = bCorrect;
			req.reply(http::status_codes::OK, jsonData);
		}
		else if (strFunc == strRegister)
		{
			json::value jsonData = json::value::object();
			if (checkForUserNameExist(strUserName))
			{
				jsonData[L"register"] = false;
			}
			else
			{
				registerUser(strUserName, strPassword);
				jsonData[L"register"] = true;
			}
			req.reply(http::status_codes::OK, jsonData);
		}
		else if (strFunc == strQuote)
		{
			if (checkForCredential(strUserName, strPassword))
			{
				auto foundStockCode = httpGetVars.find(U("stockcode"));
				utility::string_t requestStockCode = foundStockCode->second;
				utf8string strStockCode = utility::conversions::to_utf8string(requestStockCode);

				json::value jsonQuote = quote(stoi(strStockCode));
				req.reply(http::status_codes::OK, jsonQuote);
			}
			req.reply(http::status_codes::OK, json::value(U("Credential are wrong")));
		}
		else if (strFunc == strTransactions)
		{
			if (checkForCredential(strUserName, strPassword))
			{
				json::value jsonData = getTransaction(strUserName);
				req.reply(http::status_codes::OK, jsonData);
			}
			req.reply(http::status_codes::OK, json::value(U("Credential are wrong")));
		}


		//string strData = "";


		//auto_ptr<mongo::DBClientCursor> cursor = c.query("National-Stolen-Vehicles.Vehicle", MONGO_QUERY("Vehicle_Registration" << stoi(str)));
		//stringstream stream;
		//json::value jsonData;
		//while (cursor->more())
		//{
		//	//strData += mongo::tojson(cursor->next());
		//	mongo::BSONObj bsonObject = cursor->next();
		//	jsonData[U("Vehicle_Registration")] = json::value(bsonObject["Vehicle_Registration"].Double());
		//	jsonData[U("Vehicle_Model")] = json::value(utility::conversions::to_string_t(bsonObject["Vehicle_Model"].str()));
		//	jsonData[U("Vehicle_Make")] = json::value(utility::conversions::to_string_t(bsonObject["Vehicle_Make"].str()));
		//	jsonData[U("Vehicle_Owner")] = json::value(utility::conversions::to_string_t(bsonObject["Vehicle_Owner"].str()));
		//}

		//cout << stream.str();

		//utility::string_t strt = utility::conversions::to_string_t(strData);
		//uclog << U("Received request: ") << requestName << endl;
	});

	// Wait while the listener does the heavy lifting.
	// TODO: Provide a way to safely terminate this loop.
	//Better way to do it listener.listen([]() { fgetc(stdin); }).wait(); reamining to try 
	uclog << U("Waiting for incoming connection...") << endl;
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(2000));
	}

	// Nothing left to do but commit suicide.
	uclog << U("Terminating JSON listener.") << endl;
	listener.close();
	return 0;
}

