#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Asset.h"
#include "EquityStock.h"
#include "Portfolio.h"
#pragma once

using namespace std;

void readStockData(vector<EquityStock> &stocks);

void getRiskFreeRate(vector<double> &r);

void viewStocks(vector<EquityStock> &stocks);

void buildPortfolio(vector<EquityStock> &stocks, Linked &portfolio);

void getInfo();