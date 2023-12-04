#pragma once
#include <iostream>
#include <iomanip>
#include "ansi_escape_code.hpp"

using namespace std;

class Asset{
    protected:
    double price;
    string symbol;

    public:
    Asset();
    Asset(double p, string s, int q);

    void setPrice(double p){price = p;}
    double getPrice(){return price;}

    void setSymbol(string s){symbol = s;}
    string getSymbol(){return symbol;}

};