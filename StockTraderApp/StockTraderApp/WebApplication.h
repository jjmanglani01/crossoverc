#pragma once

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WLabel>
#include "LoginWidget.h"
#include "FunctionWidget.h"

class WebApplication :
	public Wt::WApplication
{
public:
	WebApplication(const Wt::WEnvironment& env);
	~WebApplication();
private:
	LoginWidget *_loginWidget;
	void userLogedin(std::string strUserName, std::string strPassword);
	WLabel *_lblWelcome;
	FunctionWidget *_functionWidget;
};
