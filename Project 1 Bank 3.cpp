
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

enum enWhatToShow {enShowDeposit = 1, enShowWithdraw = 2};

enum enTransaction { enDeposit = 1, enWithdraw = 2, TotalBalances = 3, ShowTransactions = 4, MainMenu = 5 };

enum enWhatToDo { enShowClientList = 1, enAddNewClient = 2, enDeleteClient = 3, enUpdateClientInfo = 4, enFindClient = 5, enTransactions = 6, enExit = 7 };

const string ClientsFile = "Clients.txt";
const string TransactionsFile = "Transactions.txt";

void DoTask(enWhatToDo WhatToDo);
void ShowTransactionsMenu();

struct stTransaction
{
	int TransactionNumber;
	string TransactionType;
	double TransactionAmount;
};

struct stClient
{
	string AccountNumber;
	string PinCode;
	string ClientName;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
	stTransaction Transaction;
};

void ResetScreen()
{
	system("color 0F");
	system("cls");
}

short GetTaskNumber()
{
	short TaskNumber;
	cout << "Choose What do you wand to do? [1 to 6]? ";
	cin >> TaskNumber;

	return TaskNumber;
}

void ShowMainMenu()
{
	short TaskNumber;

	ResetScreen();

	cout << "===============================================" << endl;
	cout << "\t\tMain Menu Screen" << endl;
	cout << "===============================================" << endl;

	cout << "\t\t[1] Show Client List." << endl;
	cout << "\t\t[2] Add New Client." << endl;
	cout << "\t\t[3] Delete Client." << endl;
	cout << "\t\t[4] Update Client Info." << endl;
	cout << "\t\t[5] Find Client." << endl;
	cout << "\t\t[6] Transactions." << endl;
	cout << "\t\t[7] Exit." << endl;

	cout << "===============================================" << endl;

	cout << "Choose what do you want to do? [1 to 7]? ";
	cin >> TaskNumber;

	DoTask((enWhatToDo)(TaskNumber));
}

void GobackToMainMenu()
{
	cout << "\n\nPress any key to go back to main menu......";
	system("Pause>0");
	ShowMainMenu();
}

void PrintClientRecord(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(12) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.ClientName;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}

void PrintTransactionRecord(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(30) << Client.ClientName;
	cout << "| " << left << setw(20) << Client.Transaction.TransactionNumber;
	cout << "| " << left << setw(20) << Client.Transaction.TransactionType;
	cout << "| " << left << setw(20) << Client.Transaction.TransactionAmount;
	cout << "| " << left << setw(15) << Client.AccountBalance;
}

string ReadAccountNumber()
{
	string AccountNumber;
	cout << "\nEnter Account Number? ";
	cin >> AccountNumber;

	return AccountNumber;
}

vector<string> SplitString(string S1, string Delimiter)
{
	vector<string> vString;
	short Pos = 0;
	string sWord;

	while ((Pos = S1.find(Delimiter)) != std::string::npos)
	{
		sWord = S1.substr(0, Pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, Pos + Delimiter.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

stClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
	vector<string> vString = SplitString(Line, Separator);
	stClient Client;

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.ClientName = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
	vector<stClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	string Line;
	stClient Client;

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

bool CheckIfClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	string Line;
	stClient Client;

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
		}

		MyFile.close();
	}

	return false;
}

stClient ReadNewClient()
{
	stClient Client;

	cout << "\nEnter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (CheckIfClientExistsByAccountNumber(Client.AccountNumber, ClientsFile))
	{
		cout << "\nClient With the Account Number [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "\nEnter Client Name? ";
	getline(cin, Client.ClientName);

	cout << "\nEnter Phone? ";
	getline(cin, Client.Phone);

	cout << "\nEnter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

void AddDataLineToFile(string FileName, string Line)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << Line << endl;
		MyFile.close();
	}
}

string ConvertRecordToLine(stClient ClientRecord, string Separator = "#//#")
{
	string DataLine = "";

	DataLine += ClientRecord.AccountNumber + Separator;
	DataLine += ClientRecord.PinCode + Separator;
	DataLine += ClientRecord.ClientName + Separator;
	DataLine += ClientRecord.Phone + Separator;
	DataLine += to_string(ClientRecord.AccountBalance);

	return DataLine;
}

void SaveClientsDataToFile(string FileName, vector<stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	string DataLine;

	if (MyFile.is_open())
	{
		for (stClient& C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
}

void ShowClientsList()
{

	ResetScreen();

	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);

	cout << "\t\t\t\tClient List (" << vClients.size() << ") Client(s)." << endl;
	cout << "________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(12) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n________________________________________________________________________________________________\n";

	for (stClient& Client : vClients)
	{
		PrintClientRecord(Client);
		cout << endl;
	}

	cout << "________________________________________________________________________________________________\n";
}

void AddNewClient()
{
	stClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFile, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMoreClients = 'Y';
	do {
		cout << "Adding New Client : " << endl;

		AddNewClient();

		cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
		cin >> AddMoreClients;

	} while (toupper(AddMoreClients) == 'Y');
}

void AddClients()
{
	ResetScreen();

	cout << "------------------------------------------\n";
	cout << "\tAdd New Clients Screen\n";
	cout << "------------------------------------------\n";

	AddNewClients();
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient> vClients, stClient& Client)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}

	return false;
}

bool MarkForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}

	return false;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe Following are the client details : " << endl;
	cout << "------------------------------------------\n";
	cout << "\nAccount Number  : " << Client.AccountNumber;
	cout << "\nPin Code        : " << Client.PinCode;
	cout << "\nClient Name     : " << Client.ClientName;
	cout << "\nPhone           : " << Client.Phone;
	cout << "\nAccount Balance : " << Client.AccountBalance;
	cout << "\n------------------------------------------\n";
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;
	char DeleteClient = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want to delete this client? Y/N? ";
		cin >> DeleteClient;

		if (DeleteClient == 'Y' || DeleteClient == 'y')
		{
			MarkForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFile, vClients);

			cout << "\nClient Deleted Successfully.\n\n";
			return true;
		}
	}
	else
	{
		cout << "\nThe Client with the Account Number (" << AccountNumber << ") is Not Found!\n\n";
		return false;
	}
}

void DeleteClient()
{
	ResetScreen();

	cout << "------------------------------------------\n";
	cout << "\tDelete Client Screen\n";
	cout << "------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\nEnter Pin Code? ";
	getline(cin >> ws, Client.PinCode);

	cout << "\nEnter Client Name? ";
	getline(cin, Client.ClientName);

	cout << "\nEnter Phone? ";
	getline(cin, Client.Phone);

	cout << "\nEnter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;
	char UpdateClient = 'Y';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want to update this client? Y/N? ";
		cin >> UpdateClient;

		if (UpdateClient == 'Y' || UpdateClient == 'y')
		{
			for (stClient& Client : vClients)
			{
				if (Client.AccountNumber == AccountNumber)
				{
					Client = ChangeClientRecord(AccountNumber);
				}
			}

			SaveClientsDataToFile(ClientsFile, vClients);

			cout << "\nClient Updated Successfully.\n\n";
			return true;
		}
	}
	else
	{
		cout << "\nThe Client with the Account Number (" << AccountNumber << ") is Not Found!\n\n";
		return false;
	}
}

void UpdateClientInfo()
{
	ResetScreen();

	cout << "------------------------------------------\n";
	cout << "\tUpdate Client Info Screen\n";
	cout << "------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void FindClient()
{
	ResetScreen();

	cout << "------------------------------------------\n";
	cout << "\tFind Client Screen\n";
	cout << "------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << endl;
	}
	else
	{
		cout << "\nThe Client with the Account Number (" << AccountNumber << ") is Not Found!\n\n";
	}
}

void ExitProgram()
{
	ResetScreen();

	cout << "------------------------------------------\n";
	cout << "\tProgram Ends :-)\n";
	cout << "------------------------------------------\n";
}

string ConvertTransactionRecordToLine(stClient& Client, string Separator = "#//#")
{
	string DataLine = "";

	DataLine += Client.AccountNumber + Separator;
	DataLine += Client.ClientName + Separator;
	DataLine += to_string(Client.Transaction.TransactionNumber) + Separator;
	DataLine += Client.Transaction.TransactionType + Separator;
	DataLine += to_string(Client.Transaction.TransactionAmount) + Separator;
	DataLine += to_string(Client.AccountBalance);

	return DataLine;
}

void SaveTransactionToFile(string FileName, stClient &Client)
{
	fstream MyFile;
	MyFile.open(FileName, ios::app);

	if (MyFile.is_open())
	{
		string DataLine = ConvertTransactionRecordToLine(Client);
		MyFile << DataLine << endl;
		MyFile.close();
	}
}

int GenerateTransactionNumber(int From, int To)
{
	int TransactionNumber = rand() % (To - From + 1) + From;
	return TransactionNumber;
}

string GetTransactionType(enWhatToShow WhatToShow)
{
	return ((WhatToShow == enWhatToShow::enShowDeposit) ? "Deposit" : "Withdraw");
}

enWhatToShow enTransactionType(double DepositAmount)
{
	return((DepositAmount >= 0) ? enWhatToShow::enShowDeposit : enWhatToShow::enShowWithdraw);
}

void SaveTransactionData(stClient &Client, double DepositAmount, enWhatToShow WhatToShow)
{
	Client.Transaction.TransactionType = GetTransactionType(WhatToShow);
	Client.Transaction.TransactionAmount = DepositAmount;
	Client.Transaction.TransactionNumber = GenerateTransactionNumber(1000, 10000);
}

void DoTask(enWhatToDo WhatToDo)
{
	switch (WhatToDo)
	{
	case enWhatToDo::enShowClientList:
		ShowClientsList();
		GobackToMainMenu();
		break;

	case enWhatToDo::enAddNewClient:
		AddClients();
		GobackToMainMenu();
		break;

	case enWhatToDo::enDeleteClient:
		DeleteClient();
		GobackToMainMenu();
		break;

	case enWhatToDo::enUpdateClientInfo:
		UpdateClientInfo();
		GobackToMainMenu();
		break;

	case enWhatToDo::enFindClient:
		FindClient();
		GobackToMainMenu();
		break;

	case enWhatToDo::enTransactions:
		ShowTransactionsMenu();
		GobackToMainMenu();

	case enWhatToDo::enExit:
		ExitProgram();
		break;
	}
}

bool AddDepositAmount(string AccountNumber, double DepositAmount, vector<stClient>& vClients)
{
	char Answer = 'n';
	cout << "\nAre you sure you want perform this transaction? Y/N? ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		for (stClient& Client : vClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				Client.AccountBalance += DepositAmount;
				SaveClientsDataToFile(ClientsFile, vClients);
				cout << "\n\nDone Successfully, New balance is : " << Client.AccountBalance;
				SaveTransactionData(Client, DepositAmount, enTransactionType(DepositAmount));
				SaveTransactionToFile(TransactionsFile, Client);
				return true;
			}
		}

	}

	return false;
}

double GetDepositAmount()
{
	double DepositAmount;

	do {
		cout << "\nPlease enter deposit amount? ";
		cin >> DepositAmount;

	} while (DepositAmount <= 0);

	return DepositAmount;
}

double GetWithdrawAmount(stClient Client)
{
	double WithdrawAmount;

	cout << "\nPlease enter withdraw amount? ";
	cin >> WithdrawAmount;

	while (WithdrawAmount >= Client.AccountBalance)
	{
		cout << "\n\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "\nPlease enter another amount? ";
		cin >> WithdrawAmount;
	}

	return WithdrawAmount * -1;
}

void DepositByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with Account Number [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(Client);

	double DepositAmount;
	DepositAmount = GetDepositAmount();

	AddDepositAmount(AccountNumber, DepositAmount, vClients);
}

void ShowDepositScreen()
{
	ResetScreen();

	cout << "------------------------------------------\n";
	cout << "\tDeposit Screen\n";
	cout << "------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	DepositByAccountNumber(AccountNumber, vClients);
}

void ShowTotalBalancesScreen()
{
	ResetScreen();

	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	double TotalBalances = 0;

	cout << "\t\t\t\tBalances List (" << vClients.size() << ") Client(s)." << endl;
	cout << "________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(12) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n________________________________________________________________________________________________\n";

	if (vClients.size() == 0)
	{
		cout << "\n\t\t\t\t\tNo Clients Available In the System!" << endl;
	}
	else
	{
		for (stClient& Client : vClients)
		{
			PrintClientRecord(Client);
			TotalBalances += Client.AccountBalance;
			cout << endl;
		}
	}
	cout << "________________________________________________________________________________________________\n";

	cout << "\n\t\t\t\t\tTotal Balances = " << TotalBalances << "$" << endl;
}

void WithdrawByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with Account Number [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(Client);

	double WithdrawAmount;
	char Answer = 'n';
	WithdrawAmount = GetWithdrawAmount(Client);

	AddDepositAmount(AccountNumber, WithdrawAmount, vClients);
}

void ShowWithdrawScreen()
{

	ResetScreen();

	cout << "------------------------------------------\n";
	cout << "\tWithdraw Screen\n";
	cout << "------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	WithdrawByAccountNumber(AccountNumber, vClients);
}

stClient ConvertFromLineToTransactionRecord(string Line, string Separator = "#//#")
{
	vector<string> vString = SplitString(Line, Separator);
	stClient Client;

	Client.AccountNumber = vString[0];
	Client.ClientName = vString[1];
	Client.Transaction.TransactionNumber = stod(vString[2]);
	Client.Transaction.TransactionType = vString[3];
	Client.Transaction.TransactionAmount = stod(vString[4]);
	Client.AccountBalance = stod(vString[5]);

	return Client;
}

vector<stClient> LoadTransactionsDataFromFile(string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	vector<stClient> vTransactions;

	stClient Client;
	string Line;

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			Client = ConvertFromLineToTransactionRecord(Line);
			vTransactions.push_back(Client);
		}

		MyFile.close();
	}

	return vTransactions;
}

void ShowTransactionsScreen()
{
	ResetScreen();

	vector<stClient> vTransactions = LoadTransactionsDataFromFile(TransactionsFile);

	cout << "\t\t\t\t\t\tTransactions List (" << vTransactions.size() << ") Transaction(s)." << endl;
	cout << "_____________________________________________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(20) << "Transaction Number";
	cout << "| " << left << setw(20) << "Transaction Type";
	cout << "| " << left << setw(20) << "Transaction Amount";
	cout << "| " << left << setw(15) << "Account Balance";
	cout << "\n_____________________________________________________________________________________________________________________________________\n";

	for (stClient& Transaction : vTransactions)
	{
		PrintTransactionRecord(Transaction);
		cout << endl;
	}

	cout << "_____________________________________________________________________________________________________________________________________\n";
}

void GoBackToTransactionsMenu()
{
	cout << "\n\nPress any key to go back to transactions menu.....";
	system("pause>0");
	ShowTransactionsMenu();
}

void PerformTransaction(enTransaction Transaction)
{
	switch (Transaction)
	{
	case enTransaction::enDeposit:
		ShowDepositScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransaction::enWithdraw:
		ShowWithdrawScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransaction::TotalBalances:
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransaction::ShowTransactions:
		ShowTransactionsScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransaction::MainMenu:
		ShowMainMenu();
		break;
	}
}

void ShowTransactionsMenu()
{
	short TaskNumber;

	ResetScreen();

	cout << "===============================================" << endl;
	cout << "\t\tTransactions Menu Screen" << endl;
	cout << "===============================================" << endl;

	cout << "\t\t[1] Deposit." << endl;
	cout << "\t\t[2] Withdraw." << endl;
	cout << "\t\t[3] Total Balances." << endl;
	cout << "\t\t[4] Show Transactions." << endl;
	cout << "\t\t[5] Main Menu." << endl;

	cout << "===============================================" << endl;

	cout << "Choose what do you want to do? [1 to 4]? ";
	cin >> TaskNumber;

	PerformTransaction((enTransaction)TaskNumber);
}

int main()
{
	srand((unsigned)time(NULL));

	ShowMainMenu();

	return 0;
}