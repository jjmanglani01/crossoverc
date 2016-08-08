#include "WebApplication.h"
#include "stdafx.h"

WebApplication::WebApplication(const Wt::WEnvironment & env)
	: WApplication(env)
{
	setTitle("Stock Trader");
	_loginWidget = new LoginWidget("login", root());
	_loginWidget->logedin().connect(this, &WebApplication::userLogedin);
	_lblWelcome = new WLabel(root());
	_functionWidget = new FunctionWidget("function", root());

	_lblWelcome->hide();
	_functionWidget->hide();
}

WebApplication::~WebApplication()
{
}

void WebApplication::userLogedin(std::string strUserName, std::string strPassword)
{
	_lblWelcome->setText("Welcome " + strUserName + ",");
	_lblWelcome->show();
	_functionWidget->setUserName(strUserName);
	_functionWidget->setPassword(strPassword);
	_functionWidget->show();
}
