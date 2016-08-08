// StockTraderApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WebApplication.h"

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	return new WebApplication(env);
}

int main(int argc, char **argv)
{
	return Wt::WRun(argc, argv, &createApplication);
}

