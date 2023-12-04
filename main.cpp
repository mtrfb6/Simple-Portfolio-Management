#include "Functions.h"

using namespace std;
using namespace ansi_escape_code; //https://vorbrodt.blog/2019/03/21/ansi-escape-codes/ - I wanted a way for things to be more pleasant to look at and found this

int main(){
    //Initialize variables
    EquityStock market = EquityStock(); //creating an object for accessing S&P 500 data
    vector<EquityStock> Stocks;
    vector<double> tbillreturns;
    Linked portfolio = Linked();
    char userSelect = ' ';
    //read in files
    readStockData(Stocks);

    getRiskFreeRate(tbillreturns);
    //process data and assign our market object as SPY
    for(int i = 0; i < Stocks.size(); i++){
        Stocks[i].calculateAverageReturn();
        Stocks[i].calculateStdev();
        Stocks[i].calculateExcessR(tbillreturns);
        if(Stocks[i].getSymbol() == "SPY"){
            market = Stocks[i];
        }
    }
    vector<double> excessMarketReturns = market.getExcessReturns();
    //with the other calculations done we can calculate beta - this feels kinda clunky but its serviceable
    for(int i = 0; i <Stocks.size(); i++){
        Stocks[i].calculateBeta(market);
        Stocks[i].calculateExpectedReturn(market, tbillreturns);
    }
    cout << bold << "Welcome to the Simple Portfolio Management Tool\n" << reset << endl;

    //Main menu instance:
    while (toupper(userSelect) != 'Q'){
        cout << bold << setfill('=') << setw(32) << '\n';
        cout << "\nMain Menu: " << endl <<"V) View Available Stocks" << endl <<
        "B) Start Building a Portfolio" << endl <<
        "P) View Your Current Portfolio" << endl <<
        "I) Information on Data" << endl <<
        "Q) Quit" << reset << endl;
        cin >> userSelect;
        if (cin.fail()){
            cin.clear();
            cin.ignore(100, '\n');
        }
        if (toupper(userSelect) == 'V')
            viewStocks(Stocks);
        else if (toupper(userSelect) == 'B')
            buildPortfolio(Stocks, portfolio);
        else if (toupper(userSelect) == 'P')
            portfolio.printList();
        else if (toupper(userSelect) == 'I')
            getInfo();
        else if (toupper(userSelect) != 'Q'){
            cout << "Enter 'V' to view available stocks, 'B' to start building your portfolio, 'P' to view your portfolio, 'Q' to quit." << endl;
        }
    }
    portfolio.~Linked(); //call destructor on program close
    return 0;
}