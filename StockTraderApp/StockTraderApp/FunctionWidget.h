#pragma once

#include <Wt/WContainerWidget>
#include <Wt/WComboBox>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WLabel>
#include <Wt/Http/Client>

using namespace Wt;

class FunctionWidget :
	public WContainerWidget
{
public:
	FunctionWidget(const std::string & name, Wt::WContainerWidget * parent);
	~FunctionWidget();
	void setUserName(std::string strUserName) {  _strUserName = strUserName; }
	void setPassword(std::string strPassword) {  _strPassword = strPassword; }
private:
	WComboBox *_comboFunction;
	WLineEdit *_stockcode;
	WPushButton *_call;
	WLabel *_lblWarning;
	WContainerWidget *_containerQuote;
	WContainerWidget *_containerTransaction;
	void processComboChanged(WString strFunction);
	void processFunctionality();
	void handleHttpResponse(boost::system::error_code err, const Http::Message& response);
	std::string _strUserName;
	std::string _strPassword;

	std::string Quote;
	std::string Transaction;

	bool isInteger(const std::string & s);
};

