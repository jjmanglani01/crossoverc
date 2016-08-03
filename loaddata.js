function addData(){
	var arrUserName = ["Jitesh","Alex","Mark","John","Chuck"];
	var strPassword = "123";
	var arrBalanceCash = [80000,90000,60000,70000,100000];

	var transaction = [{
						username: arrUserName[0],
						"stockcode": 1,
						"quantity": 100,
						"datetime": new Date(2016,7,2,2,10),
						"status": "executed",
						"type": "buy"
					  },
					  {
						"username": arrUserName[0],
						"stockcode": 1,
						"quantity": 50,
						"datetime": new Date(2016,7,2,3,10),
						"status": "executed",
						"type": "sell"
					  },
					  {
						"username": arrUserName[0],
						"stockcode": 2,
						"quantity": 100,
						"datetime": new Date(2016,7,2,3,15),
						"status": "executed",
						"type": "buy"
					  },
					  {
						"username": arrUserName[1],
						"stockcode": 3,
						"quantity": 100,
						"datetime": new Date(2016,7,2,3,20),
						"status": "executed",
						"type": "buy"
					  },
					  {
						"username": arrUserName[2],
						"stockcode": 2,
						"quantity": 150,
						"datetime": new Date(2016,7,2,3,25),
						"status": "executed",
						"type": "buy"
					  },
					  {
						"username": arrUserName[2],
						"stockcode": 4,
						"quantity": 70,
						"datetime": new Date(2016,7,2,3,30),
						"status": "executed",
						"type": "buy"
					  },
					  {
						"username": arrUserName[3],
						"stockcode": 5,
						"quantity": 100,
						"datetime": new Date(2016,7,2,3,45),
						"status": "executed",
						"type": "buy"
					  }
					];

	var portfolio = [
					{
						"username": arrUserName[0],
						"stockcode": 1,
						"quantity": 50,
						"totalcost": 10000
					},
					{
						"username": arrUserName[0],
						"stockcode": 2,
						"quantity": 100,
						"totalcost": 10000
					},
					{
						"username": arrUserName[1],
						"stockcode": 3,
						"quantity": 100,
						"totalcost": 10000
					},
					{
						"username": arrUserName[2],
						"stockcode": 2,
						"quantity": 150,
						"totalcost": 25000
					},
					{
						"username": arrUserName[2],
						"stockcode": 4,
						"quantity": 70,
						"totalcost": 15000
					},
					{
						"username": arrUserName[3],
						"stockcode": 5,
						"quantity": 100,
						"totalcost": 30000
					}
	];

	var stock = [
		{
			"stockcode" : 1,
			"lastsaleprice": 100
		},
		{
			"stockcode" : 2,
			"lastsaleprice": 150
		},
		{
			"stockcode" : 3,
			"lastsaleprice": 70
		},
		{
			"stockcode" : 4,
			"lastsaleprice": 200
		},
		{
			"stockcode" : 5,
			"lastsaleprice": 90
		}
	];

	for(var i=0;i<5;i++)
	{
		db.trader.insert({
			"username":arrUserName[i],
			"password":strPassword,
			"balancecash":arrBalanceCash[i]
		});
	}
	db.transaction.insert(transaction);
	db.portfolio.insert(portfolio);
	db.stock.insert(stock);
};
addData();
