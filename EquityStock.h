#include <vector>
#include <cmath>
#include <numeric>
#include "Asset.h"
#pragma once

using namespace std;

class EquityStock : public Asset{
    private:
    double expectedReturn;
    double variance;
    double stdDev;
    double avgReturn;
    string sector;
    vector<double> monthlyReturns;
    vector<double> excessReturns;
    double avgExcessReturns;
    double beta;

    public:
    EquityStock();
    EquityStock(double p, string s);

    EquityStock(const EquityStock& s);

    double getExpectedReturn(){return expectedReturn;}
    void setExpectedReturn(double er){expectedReturn = er;}

    double getVariance(){return variance;}
    void setVariance(double v){variance = v;}

    double getStandardDeviation(){return stdDev;}
    void setStandardDeviation(double sd){stdDev = sd;}

    string getSector(){return sector;}
    void setSector(string s){sector = s;}

    double getAvgReturn(){return avgReturn;}
    void setAvgReturn(double ag){avgReturn = ag;}

    void setMonthlyReturns(vector<double>& m);
    vector<double> getMonthlyReturns(){return monthlyReturns;}

    void setExcessReturns(vector<double>& e);
    vector<double> getExcessReturns(){return excessReturns;}

    void calculateAverageReturn();

    void calculateExcessR(vector<double>& tb);

    void calculateStdev();

    void calculateExpectedReturn(EquityStock &market, vector<double> &tbills);

    void calculateBeta(EquityStock &market);

    double getBeta(){return beta;}
    void setBeta(double b){beta = b;}

    void operator= (EquityStock &rhs);

};