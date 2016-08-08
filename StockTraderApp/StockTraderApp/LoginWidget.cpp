#include "LoginWidget.h"
#include <Wt/WBreak>
#include <Wt/Json/Parser>
#include <Wt/Json/Object>

LoginWidget::LoginWidget(const std::string & name, Wt::WContainerWidget * parent)
	:WContainerWidget(parent)
{
	_lblUsername = new WText("UserName:  ", this);
	_username = new WLineEdit(this);
	this->addWidget(new WBreak());

	_lblPassword = new WText("Password:  ", this);
	_password = new WLineEdit(this);
	_password->setEchoMode(WLineEdit::EchoMode::Password);
	this->addWidget(new WBreak());

	_login = new WPushButton("Login", this);
	_login->clicked().connect(this, &LoginWidget::authenticate);

	_register = new WPushButton("Register", this);
	_register->clicked().connect(this, &LoginWidget::registerUser);

	this->addWidget(new WBreak());
	this->addWidget(new WBreak());
	this->addWidget(new WBreak());



	_lblWarning = new WText("Incorrect Username or Password", this);
	_lblWarning->hide();
}

LoginWidget::~LoginWidget()
{
}

void LoginWidget::authenticate()
{
	_lblWarning->hide();
	Http::Client *client = new Http::Client(this);
	client->setTimeout(15);
	client->setMaximumResponseSize(10 * 1024);
	client->done().connect(boost::bind(&LoginWidget::handleHttpResponse, this, _1, _2));
	if (client->get("http://localhost/json/login?userName=" + _username->text().toUTF8() + "&password=" + _password->text().toUTF8())) {
	}
	else {
		_lblWarning->setText("There is something wrong with system");
		_lblWarning->show();
	}
}

void LoginWidget::registerUser()
{
	_lblWarning->hide();
	Http::Client *client = new Http::Client(this);
	client->setTimeout(30);
	//client->setMaximumResponseSize(10 * 1024);
	client->done().connect(boost::bind(&LoginWidget::handleHttpResponse, this, _1, _2));
	if (client->get("http://localhost/json/RegisterTrader?userName=" + _username->text().toUTF8() + "&password=" + _password->text().toUTF8())) {
	}
	else {
		_lblWarning->setText("There is something wrong with system");
		_lblWarning->show();
	}
}

void LoginWidget::handleHttpResponse(boost::system::error_code err, const Http::Message& response)
{
	Json::Object result;
	if (response.status() == 200)
	{
		Json::parse(response.body(), result);
	}
	if (!result.get("login").isNull())
	{
		int iLogin = result.get("login");
		if (iLogin)
		{
			processLogin();
		}
		else
		{
			_lblWarning->setText("Incorrect Username or Password");
			_lblWarning->show();
		}
	}

	if (!result.get("register").isNull())
	{
		int iRegister = result.get("register");
		if (iRegister)
		{
			processLogin();
		}
		else
		{
			_lblWarning->setText("User Name already exist!!");
			_lblWarning->show();
		}
	}
}

void LoginWidget::processLogin()
{
	//_lblWarning->hide();
	hide();
	std::string strUserName = _username->text().toUTF8();
	std::string strPassword = _password->text().toUTF8();
	_logedin.emit(strUserName, strPassword);
}
