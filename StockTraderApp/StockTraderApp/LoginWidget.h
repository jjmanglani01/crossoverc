#pragma once

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/Http/Client>

using namespace Wt;

class LoginWidget :
	public WContainerWidget
{
public:
	LoginWidget(const std::string &name, Wt::WContainerWidget *parent = 0);
	~LoginWidget();
	Wt::Signal<std::string,std::string>& logedin() { return _logedin; }
private:
	WText *_lblUsername;
	WText *_lblPassword;
	WText *_lblWarning;
	WLineEdit *_username;
	WLineEdit *_password;
	WPushButton *_login;
	WPushButton *_register;
	void registerUser();
	void authenticate();
	bool _bAuthenticated;
	bool authenticate(std::string strUserName, std::string strPassword);
	void handleHttpResponse(boost::system::error_code err, const Http::Message& response);
	Wt::Signal<std::string,std::string> _logedin;
	void processLogin();
};

