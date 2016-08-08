#include "FunctionWidget.h"
#include <Wt/WGridLayout>
#include <Wt/WHBoxLayout>
#include <Wt/Json/Parser>
#include <Wt/Json/Object>
#include <Wt/WBreak>

bool FunctionWidget::isInteger(const std::string & s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

FunctionWidget::FunctionWidget(const std::string & name, Wt::WContainerWidget * parent)
	:WContainerWidget(parent)
{
	Quote = "Quote";
	Transaction = "Transactions";
	_comboFunction = new WComboBox(this);
	_comboFunction->addItem(Quote);
	_comboFunction->addItem(Transaction);
	_comboFunction->sactivated().connect(this, &FunctionWidget::processComboChanged);
	this->addWidget(new WBreak());

	_stockcode = new WLineEdit(this);
	this->addWidget(new WBreak());

	_lblWarning = new WLabel(this);
	_lblWarning->hide();

	this->addWidget(new WBreak());

	_call = new WPushButton("Do The Task", this);
	_call->clicked().connect(this, &FunctionWidget::processFunctionality);

	this->addWidget(new WBreak());

	_containerQuote = new WContainerWidget(this);
	_containerQuote->hide();
	this->addWidget(new WBreak());

	_containerTransaction = new WContainerWidget(this);
	_containerTransaction->hide();
}

FunctionWidget::~FunctionWidget()
{
}

void FunctionWidget::processFunctionality()
{
	_lblWarning->hide();
	Http::Client *client = new Http::Client(this);
	client->setTimeout(20);
	client->done().connect(boost::bind(&FunctionWidget::handleHttpResponse, this, _1, _2));

	std::string strSelectedCombo = _comboFunction->currentText().toUTF8();

	std::string strUrl = "http://localhost/json/" + strSelectedCombo + "?userName=" + _strUserName + "&password=" + _strPassword;
	if (strSelectedCombo == Quote)
	{
		std::string stockCode = _stockcode->text().toUTF8();
		if (isInteger(stockCode))
		{
			strUrl += "&stockcode=" + stockCode;
			std::cout << strUrl << std::endl;
		}
		else
		{
			_lblWarning->setText("Stock code should be numeric");
			_lblWarning->show();
			return;
		}
	}

	if (client->get(strUrl)) {
	}
	else {
		_lblWarning->setText("There is something wrong with system");
		_lblWarning->show();
	}
}

void FunctionWidget::processComboChanged(WString strFunction)
{
	_lblWarning->hide();
	if (strFunction == Transaction)
	{
		_stockcode->hide();
		_containerQuote->hide();
	}
	else if (strFunction == Quote)
	{
		_containerTransaction->hide();
	}
}

void FunctionWidget::handleHttpResponse(boost::system::error_code err, const Http::Message & response)
{
	Json::Object result;
	if (response.status() == 200)
	{
		Json::parse(response.body(), result);
	}
	if (result.get("error").isNull())
	{
		if (_comboFunction->currentText().toUTF8() == Quote)
		{
			_containerQuote->show();
			_containerQuote->setHeight(100);
			_containerQuote->clear();
			Wt::WGridLayout *grid = new Wt::WGridLayout();
			_containerQuote->setLayout(grid);

			Wt::WText *ts = new Wt::WText("Stock Code");
			grid->addWidget(ts, 0, 0);

			Wt::WText *tp = new Wt::WText("Last Sale Price");
			grid->addWidget(tp, 0, 1);

			Wt::WText *tsValue = new Wt::WText(result.get("stockcode").toString());
			grid->addWidget(tsValue, 1, 0);

			Wt::WText *tpValue = new Wt::WText(result.get("lastsaleprice").toString());
			grid->addWidget(tpValue, 1, 1);

		}
		else if (_comboFunction->currentText().toUTF8() == Transaction)
		{
			/*"stockcode"
				"quantity"]
				"datetime"]
				"status"] =
				"type"] = j
*/
			_containerTransaction->show();
			_containerTransaction->setHeight(100);
			_containerTransaction->clear();
			Wt::WGridLayout *grid = new Wt::WGridLayout();
			_containerTransaction->setLayout(grid);

			Wt::WText *ts = new Wt::WText("Stock Code");
			grid->addWidget(ts, 0, 0);

			Wt::WText *tq = new Wt::WText("Quantity");
			grid->addWidget(tq, 0, 1);

			Wt::WText *tdate = new Wt::WText("Date Time");
			grid->addWidget(tdate, 0, 2);

			Wt::WText *tstatus = new Wt::WText("Status");
			grid->addWidget(tstatus, 0, 3);

			Wt::WText *tTra = new Wt::WText("Transaction Type");
			grid->addWidget(tTra, 0, 4);

			//Wt::WText *tValue = new Wt::WText(result.get("stockcode").toString());
			//grid->addWidget(tValue, 1, 1);

			std::cout << "Transactions array size: " << result.size()<<std::endl;

		}
	}
}
