#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

const string CLIENTS_FILE_NAME = "clients-list.txt";

enum enMainMenuOption
{
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposit = 3,
    eCheckBalance = 4,
    eLogout = 5
};

struct stClient
{
    string AccountNumber, PINCode, Name, Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

stClient CurrentClient;

void ShowMainMenuScreen();
void Login();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void BackToMainMenu();

vector<string>
SplitString(string Str, string Delim)
{
    int pos = 0;
    string sWord = "";
    vector<string> vString;

    while ((pos = Str.find(Delim)) != string::npos)
    {
        sWord = Str.substr(0, pos);

        if (sWord != "")
            vString.push_back(sWord);

        Str.erase(0, pos + Delim.length());
    }

    if (Str != "")
        vString.push_back(Str);

    return vString;
}

stClient ConvertDataLineToRecord(string DataLine, string Separator = "#//#")
{
    vector<string> vString = SplitString(DataLine, Separator);
    stClient Client;

    Client.AccountNumber = vString[0];
    Client.PINCode = vString[1];
    Client.Name = vString[2];
    Client.Phone = vString[3];
    Client.AccountBalance = stod(vString[4]);

    return Client;
}

string ConvertRecordToDataLine(stClient Client, string Separator = "#//#")
{
    string DataLine = "";

    DataLine += Client.AccountNumber + Separator;
    DataLine += Client.PINCode + Separator;
    DataLine += Client.Name + Separator;
    DataLine += Client.Phone + Separator;
    DataLine += to_string(Client.AccountBalance);

    return DataLine;
}

vector<stClient> LoadClientsFromFile(string FileName)
{
    fstream MyFile;
    vector<stClient> vClients;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line = "";
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertDataLineToRecord(Line));
        }

        MyFile.close();
    }

    return vClients;
}

void SaveClientsToFile(vector<stClient> vClients, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (stClient &C : vClients)
        {
            if (!C.MarkForDelete)
                MyFile << ConvertRecordToDataLine(C) << endl;
        }
    }
}

short ReadMainMenuOption()
{
    short Option;
    do
    {
        cout << "Choose what do you want to do? [1 to 5]? ";
        cin >> Option;
    } while (Option < 1 || Option > 5);

    return Option;
}

void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "====================================\n";
    cout << "\tCheck Balance Screen\n";
    cout << "====================================\n";
    cout << "Your Balanace is " << CurrentClient.AccountBalance << endl;
}

short ReadQuickWithdrawOption()
{
    short Option;
    do
    {
        cout << "Choose what to do form [1] to [8]? ";
        cin >> Option;
    } while (Option < 1 || Option > 9);

    return Option;
}

short GetQuickWithdrawAmount(short QuickWithdrawOption)
{
    switch (QuickWithdrawOption)
    {
    case 1:
        return 20;
        break;
    case 2:
        return 50;
        break;
    case 3:
        return 100;
        break;
    case 4:
        return 200;
        break;
    case 5:
        return 400;
        break;
    case 6:
        return 600;
        break;
    case 7:
        return 800;
        break;
    case 8:
        return 1000;
        break;
    default:
        return 0;
        break;
    }
}

void ApplyTransaction(vector<stClient> &vClients, string AccountNumber, double Amount)
{
    char Answer = 'n';

    cout << "\nAre you sure you want to perform this transactions: ";
    cin >> Answer;

    if (tolower(Answer) == 'y')
    {
        for (stClient &Client : vClients)
        {
            if (Client.AccountNumber == AccountNumber)
            {
                Client.AccountBalance += Amount;
                SaveClientsToFile(vClients, CLIENTS_FILE_NAME);
                cout << "\nDone Successfully. New Balance now is: " << Client.AccountBalance << endl;
                break;
            }
        }
    }
}

void PerformQuickWithdrawOption(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)
        return;

    short WithdrawAmount = GetQuickWithdrawAmount(QuickWithdrawOption);

    if (WithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your Balanace, make another choice.\n";
        cout << "\nPress any key to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    vector<stClient> vClients = LoadClientsFromFile(CLIENTS_FILE_NAME);
    ApplyTransaction(vClients, CurrentClient.AccountNumber, WithdrawAmount * -1);
    CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "=============================================\n";
    cout << "\t  Quick Withdraw Screen\n";
    cout << "=============================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "=============================================\n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;

    PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

int ReadWithdrawAmount()
{
    int Amount = 0;
    do
    {
        cout << "\nEnter an amount multiple of 5's: ";
        cin >> Amount;
    } while (Amount % 5 != 0);

    return Amount;
}

void PerformNormalWithdrawOption()
{
    int WithdrawAmount = ReadWithdrawAmount();

    if (WithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your Balanace, make another choice.\n";
        cout << "\nPress any key to continue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;
    }

    vector<stClient> vClients = LoadClientsFromFile(CLIENTS_FILE_NAME);
    ApplyTransaction(vClients, CurrentClient.AccountNumber, WithdrawAmount * -1);
    CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowNormalWithdrawScreen()
{

    system("cls");
    cout << "=============================================\n";
    cout << "\t  Normal Withdraw Screen\n";
    cout << "=============================================\n";
    PerformNormalWithdrawOption();
}

double ReadDepositeAmount()
{
    double Amount = 0;
    do
    {
        cout << "Enter a positive Deposite Amount: ";
        cin >> Amount;
    } while (Amount <= 0);

    return Amount;
}

void PerformDepositOption()
{
    double Amount = ReadDepositeAmount();

    vector<stClient> vClients = LoadClientsFromFile(CLIENTS_FILE_NAME);

    ApplyTransaction(vClients, CurrentClient.AccountNumber, Amount);
    CurrentClient.AccountBalance += Amount;
}

void ShowDepositScreen()
{

    system("cls");
    cout << "=============================================\n";
    cout << "\t  Deposit Screen\n";
    cout << "=============================================\n";
    PerformDepositOption();
}

void BackToMainMenu()
{
    cout << "\n\nPlease press any key to go to main menu...";
    system("pause>0");
    ShowMainMenuScreen();
}

void PerformMainMenuOptions(enMainMenuOption MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOption::eQuickWithdraw:
        ShowQuickWithdrawScreen();
        BackToMainMenu();
        break;
    case enMainMenuOption::eNormalWithdraw:
        ShowNormalWithdrawScreen();
        BackToMainMenu();
        break;
    case enMainMenuOption::eDeposit:
        ShowDepositScreen();
        BackToMainMenu();
        break;
    case enMainMenuOption::eCheckBalance:
        ShowCheckBalanceScreen();
        BackToMainMenu();
        break;
    case enMainMenuOption::eLogout:
        Login();
        break;
    }
}

void ShowMainMenuScreen()
{
    system("cls");
    cout << "=============================================\n";
    cout << "\t  ATM Main Menu Screen\n";
    cout << "=============================================\n";
    cout << "\t[1]Quick Withdraw.\n";
    cout << "\t[2]Normal Withdraw.\n";
    cout << "\t[3]Deposit.\n";
    cout << "\t[4]Check Balance.\n";
    cout << "\t[5]Logout.\n";
    cout << "=============================================\n";
    PerformMainMenuOptions(enMainMenuOption(ReadMainMenuOption()));
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient &Client)
{
    vector<stClient> vClients = LoadClientsFromFile(CLIENTS_FILE_NAME);

    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PINCode == PinCode)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;

    else
        false;
}

void Login()
{
    bool LoginFailed = false;
    string AccountNumber, PinCode;
    do
    {
        system("cls");
        cout << "====================================\n";
        cout << "\tLogin Screen\n";
        cout << "====================================\n";

        if (LoginFailed)
            cout << "Invalid AccountNumber/PinCode!\n";

        cout << "Please enter AccountNumber: ";
        cin >> AccountNumber;

        cout << "Please enter PinCode: ";
        cin >> PinCode;

        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFailed);

    ShowMainMenuScreen();
}

int main()
{
    Login();
    system("pause>0");
    return 0;
}