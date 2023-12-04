#include "Functions.h"cxf 
#pragma once

using namespace std;
using namespace ansi_escape_code;

void readStockData(vector<EquityStock> &stocks){
    //Need a string array for the filepath
    const string tickerSymbols[] = {"AAPL", "BEP", "COP", "CVX", "DIS", "FDX", "GOOGL", "JPM", "LEN", "LGIH", "LMT", "MAA", "MMM", "MSFT", "NFLX", "NKE", "NVDA", "PEP", "PFE", "PG", "PM", "PYPL", "SBUX", "SPY", "SQ", "TGT", "TMUS", "TSLA", "UL", "UNH", "V", "WM", "WMT", "XOM"};
    string inRec, tempStr;
    vector<string> line;
    ifstream StockReturns;
    int filenum = 0; // used for indexing the vector we passed in
    for (auto &tickerSymbol : tickerSymbols){ //Open up each corresponding data file: https://stackoverflow.com/questions/20234898/more-modern-way-of-looping-through-c-arrays
        string path = "stockdata/" + tickerSymbol + ".csv";
        StockReturns.open(path);
        if (!StockReturns){
            cout << "Unable to open CSV file" << endl;
        }
        vector<double> tempMonthlyClose; // creating a temp vector to set monthly returns for each stock
        string tempsector; //Get the sector info from the csv
        while (getline(StockReturns, inRec)){
            stringstream inSS(inRec);
            line.clear();
            //Seperate each value in the csv and add to the vector
            while (getline(inSS, tempStr, ',')){
                line.push_back(tempStr);
            }
            try{
                if (line[1] == "Date"){
                     //couldve fixed this in my data scraping but I was already handling it here
                    tempsector = line[2]; 
                }
                else if (line[0] == "0"){
                    EquityStock tempStock = EquityStock(stod(line[6]), tickerSymbol); 
                    tempMonthlyClose.push_back(stod(line[6])); //Since the most recent price is also our first period close
                    tempStock.setSector(tempsector);
                    stocks.push_back(tempStock);
                }
                else{
                    tempMonthlyClose.push_back(stod(line[6])); // add the remaining closing prices from the file to the beginning of the vector
                }
            }
            catch(...){
                cout << "error occured" << endl;
            }
        }
        stocks[filenum].setMonthlyReturns(tempMonthlyClose);
        StockReturns.close();
        filenum++;
    }
}

void getRiskFreeRate(vector<double> &r){
    string inRec, tempStr;
    vector<string> line;
    ifstream tbilldata;
    //Open up data for returns on 1-mo Treasury Bills
    //This is where risk-free rate comes from. 
    //Tbills are generally regarded as a risk free investment because they're dependent on the US being able to pay off debt.
    tbilldata.open("1mo_T_Bill.csv");
    if(!tbilldata)
        cout << "Unable to open T-Bills";
    vector<double> tempClose;
    while(getline(tbilldata, inRec)){
        stringstream inSS(inRec);
        line.clear();
        while(getline(inSS, tempStr, ',')){
            line.push_back(tempStr);
        } 
        try{
            tempClose.push_back(stod(line[4])); //Add the closing price for that month
        }
        catch(...){
            cout << "error occured" << endl;
        }
    }
    for(int i = 1; i < tempClose.size(); i++){
        r.insert(r.begin(), (tempClose[i-1] / tempClose[i] -1)); //Get the percentage return. Using insert because data is in the opposite order from the stock data
    }
}

void viewStocks(vector<EquityStock> &stocks){
    string userSelect;
    //Array is used to match with what the objects data is and group those together with the following loop
    const string SectorNames[] = {"Market Index", "Technology", "Energy", "Consumer Discretionary", "Health Care", "Industrials", "Consumer Staples", "Finance", "Utilities", "Real Estate", "Telecommunications"};
    cout << bold << "Stocks By Sector: \n" << reset << endl;
        for (auto sector: SectorNames){ //for each sector only output the stocks that match.
            cout << bold << underline << sector << ":\n" << reset << endl;
            for (auto stock: stocks){
                if (stock.getSector() == sector){
                    cout << setfill(' ') << setw(5) << left << stock.getSymbol() << " - Last Close: $" << fixed << setprecision(2) << stock.getPrice() << endl;
                }
                
            }
            cout << '\n';
        }
    //Allowing users to select stocks to view more info:
    cout << "To see more information for a particular stock, Enter the symbol below.\nOtherwise enter 'exit' to return" << endl;
    cin >> userSelect;
    if (cin.fail()){
        cin.clear();
        cin.ignore(100, '\n');
    }
    while (userSelect != "exit"){
        for (int i = 0; i < stocks.size(); i++){
            if (userSelect != stocks[i].getSymbol() && i == stocks.size() - 1){
                cout << "Data not availabe for entered stock." << endl;
            }
            else if(userSelect == stocks[i].getSymbol()){
                cout << stocks[i].getSymbol() << " Information:" << endl <<
                "Sector: " << stocks[i].getSector() << endl <<
                "Last close: $" << stocks[i].getPrice() << "%" << endl <<
                "Average return: " << stocks[i].getAvgReturn() << "%" << endl <<
                "Variance: " << stocks[i].getVariance() << endl <<
                "Beta: " << stocks[i].getBeta() << endl <<
                "Expected Return: " << stocks[i].getExpectedReturn() << "%" <<endl <<
                "To view another stock, enter the symbol below. Otherwise enter 'exit'" << endl;
                break;
            }
            else{
                continue;
            }
        }
        cin >> userSelect;
        if (cin.fail()){
            cin.clear();
            cin.ignore(100, '\n');
        }
    }
}

void buildPortfolio(vector<EquityStock> &stocks, Linked& portfolio){
    string userSelect;
    double quantity;
    cout << "To begin building a portfolio, enter the symbol of the stock to be added/changed." << endl;
    cin >> userSelect;
    while (userSelect != "exit"){
        if (cin.fail()){
            cin.clear();
            cin.ignore(100, '\n');
            cout << "An error occured" << endl;
            return;
        }
        if (portfolio.isOwned(userSelect)){ //Check and see if the stock has already been added, prompt for update if so
            cout << "Enter the new number of shares for " << userSelect << ". Enter to 0 to remove from your portfolio." << endl;
            cin >> quantity;
            if (cin.fail()){
                cin.clear();
                cin.ignore(100, '\n');
                cout << "An error occured" << endl;
                return;
            }
            while (quantity < 0){
                cout << "Quantity value entered must be greater than 0." << endl << 
                "Enter the new number of shares for " << userSelect << ". Enter to 0 to remove from your portfolio." << endl;
                cin >> quantity;
            }
            
            if (quantity == 0)
                portfolio.delNode(userSelect); //remove if the new quantity is 0
            else
                portfolio.updateNode(userSelect, quantity);
            cout << userSelect << " has been updated.\nTo add another stock, enter the symbol. Otherwise enter 'exit'." << endl;

        }
        else{
            cout << "Enter the number of shares of " << userSelect << " to purchase: " << endl;
            cin >> quantity;
            if (cin.fail()){
                cin.clear();
                cin.ignore(100, '\n');
                cout << "An error occured" << endl;
                return;
            }
            while (quantity < 0){
                cout << "Quantity value entered must be greater than 0." << endl << 
                "Enter the new number of shares for " << userSelect << ". Enter to 0 to remove from your portfolio." << endl;
                cin >> quantity;
            }
            for(int i = 0; i < stocks.size(); i++){
                if(stocks[i].getSymbol() == userSelect){ //find the stock referenced by the user and add it to the list with their quantity
                    portfolio.addNode(stocks[i], quantity);
                    break;
                }
                else if (userSelect != stocks[i].getSymbol() && i == stocks.size() - 1){ //if it doesnt exist
                    cout << "Data unavailable for the entered stock. View list to see available stocks to add." << endl;
                    return;
                }
            }
            cout << userSelect << " has been added to your portfolio.\nTo add another stock, enter the symbol. Otherwise enter 'exit'." << endl;
        }
        cin >> userSelect;
    }
}

void getInfo(){
    char userSelect;
    cout << "To get more information on a particular measure, enter one of the following" << endl << endl;
    cout << "B) - Beta Values" << endl <<
    "E) - Expected Return" << endl <<
    "V) - Variance" << endl <<
    "R) - Risk Free Rate" << endl <<
    "X) - Excess Returns" << endl <<
    "Q) - Return to main menu" << endl;
    cin >> userSelect;
    if (cin.fail()){
        cin.clear();
        cin.ignore(100, '\n');
        cout << "An error occured" << endl;
        return;
    }
    while (toupper(userSelect) != 'Q'){
        if(toupper(userSelect) == 'B'){
            cout << bold << "\nBeta: " << reset << endl <<
            "Beta is a measure of a stocks volatility relative to the broader market. This is why the S&P 500 has a beta value of 1\n" <<
            "Stocks vary on a scale from less-volatile to most starting at 0. Values below 1 indicate less volatility than the market.\n" <<
            "Whereas values greater than 1 are more volatile.\nHigh and low beta scores are not inherently good or bad. Stocks with low\n"<<
            "beta values can still be in a large downtrend" << underline << "\nIdeal values are generally close to 1.\n" << reset << endl;
        }
        else if(toupper(userSelect) == 'E'){
            cout << bold << "\nExpected Returns: " << reset << endl <<
            "Expected Returns is a probabilistic measure that is commonly used to determine the average net outcome of an investment.\n"<<
            "It uses historic returns to measure the probability of different outcomes. It's important to keep in mind that Expected\n" <<
            "Return values are in no way promised outcomes. They should always be considered in tandem with risk." << endl;
        }
        else if(toupper(userSelect) == 'V'){
            cout << bold << "\nVariance: " << reset << endl <<
            "Variance in finance can be used as a proxy score for measuring risk. Generally, if the variance of data for a particular\n"<<
            "stock is high, this indicates a more volatile stock. By taking the square root of the variance, we can examine\n" <<
            "the standard deviation and distribution of the returns of a particular stock, or even a portfolio." << endl;
        }
        else if(toupper(userSelect) == 'R'){
            cout << bold << "\nRisk-Free Rate: " << reset << endl <<
            "Risk-Free Rate is a measure of the return on an asset with 0 risk. Typically, this rate comes from the return on\n" <<
            "1-month US Treasuries. The reason being that risk for a Treasury Bill depends on the ability of the US to pay\n" <<
            "Off its debt. We assume this is the case because the alternative would likely be apocalyptic :)" << endl;
        }
        else if(toupper(userSelect) == 'X'){
            cout << bold << "\nExcess Returns: " << reset << endl <<
            "Excess returns measure the returns of a stock in-excess of the risk free rate. E.g., Stock XYZ returns 10% in a month\n" <<
            "Where the return on a 1-month Treasury returns 5%. By taking the return of XYZ minus the return of the T-Bill, we\n" <<
            "get an excess return value of 5%. Excess returns are very important in finance, as generating equity above the return\n"<<
            "of the market is the goal of practically all financial institutions." << endl;
        }
        else{
            cout << "Invalid selection. Enter one of the following: " << endl;
        }
        cout << "\nB) - Beta Values" << endl <<
        "E) - Expected Return" << endl <<
        "V) - Variance" << endl <<
        "R) - Risk Free Rate" << endl <<
        "X) - Excess Returns" << endl <<
        "Q) - Return to main menu" << endl;
        cin >> userSelect;
    }
}