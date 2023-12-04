#include "EquityStock.h"

EquityStock::EquityStock() : Asset(){
    price = 0;
    symbol = " ";
    expectedReturn = 0;
    variance = 0;
    stdDev = 0;
    avgReturn = 0;
    sector = " ";
    beta = 0.0;
}
/**/
EquityStock::EquityStock(double p, string s) : Asset(){
    price = p;
    symbol = s;
    expectedReturn = 0;
    variance = 0;
    stdDev = 0;
    avgReturn = 0;
    sector = " ";
    beta = 0.0;
}

//I don't actually know that this is used but for safety's sake i'm adding it
EquityStock::EquityStock(const EquityStock &s){
    price = s.price;
    symbol = s.symbol;
    expectedReturn = s.expectedReturn;
    variance = s.variance;
    avgReturn = s.avgReturn;
    monthlyReturns = s.monthlyReturns;
    beta = s.beta;
    excessReturns = s.excessReturns;
    sector = s.sector;
}

void EquityStock::setMonthlyReturns(vector<double>& m){
    double tempReturnPerc = 0.0;
    for(int i = 1; i < m.size(); i++){
        tempReturnPerc = ((m[i] - m[i-1]) / m[i-1]) * 100; //percentage return for the month
        monthlyReturns.push_back(tempReturnPerc);
    }
}

void EquityStock::calculateAverageReturn(){
    double sum = 0.0;
    for (int i = 0; i < monthlyReturns.size(); i++){
        sum += monthlyReturns.at(i);
    }
    setAvgReturn(sum/static_cast<double>(monthlyReturns.size()));
}

void EquityStock::calculateStdev(){
    double tempValue = 0.0;
    for (int i = 0; i < monthlyReturns.size(); i++){
        tempValue += pow(monthlyReturns.at(i) - avgReturn, 2);
    }
    variance = tempValue / monthlyReturns.size();
    stdDev = sqrt(variance);
}

void EquityStock::calculateExcessR(vector<double>& tb){
    for(int i = 0; i < tb.size(); i++){
        excessReturns.push_back(monthlyReturns[i] - tb[i]); //excess returns we just take away risk free rate for that period
    }
}

void EquityStock::calculateBeta(EquityStock &market){   
    //https://stackoverflow.com/questions/18939869/how-to-get-the-slope-of-a-linear-regression-line-using-c
    //I started with the link above, it turned out to be wildly incorrect so I just wrote the beta formula how i know it
    //Surprise surprise that was right.
    double n = excessReturns.size();
    double covarSum = 0.0;
    for(int i = 0; i < excessReturns.size(); i++){
        //Take the sum of returns minus the average, multiplied by the market's returns minus average, divide by n-1
        covarSum += ((excessReturns[i] - avgReturn) * (market.getExcessReturns()[i] - market.getAvgReturn()) / (n-1) );
        //this is covariance for stock
    }
    //beta is the covariance divided by the variance:
    if (symbol == "SPY")
        setBeta(1);
    else
        setBeta(covarSum / market.getVariance());
}

void EquityStock::calculateExpectedReturn(EquityStock &market, vector<double> &tbills){
    double er = 0.0;
    for (int i = 0; i < tbills.size(); i++){
        er += tbills[i] + beta * (market.excessReturns[i]); //sum of risk free rate for the period plus the product of beta and excess returns of the market (risk-free premium) for that period
    }
    er = (pow(1 + er/100, 1/2.67) -1) * 100; //We want per year, so use compound interest formula
    setExpectedReturn(er);
}

void EquityStock::operator= (EquityStock &rhs){ 
    symbol = rhs.symbol;
    price = rhs.price;
    monthlyReturns = rhs.monthlyReturns;
    variance = rhs.variance;
    excessReturns = rhs.excessReturns;
    beta = rhs.beta;
    avgReturn = rhs.avgReturn;
    avgExcessReturns = rhs.avgExcessReturns;
    expectedReturn = rhs.expectedReturn;
    sector = rhs.sector;
}
