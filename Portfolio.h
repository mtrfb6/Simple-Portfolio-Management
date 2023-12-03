#pragma once
#include <iostream>
#include <string>
#include "EquityStock.h"
using namespace std;

struct ownedStock {
  EquityStock stock;
  int quantity;
  ownedStock *nextPtr;
};

class Linked {
private:
  ownedStock *headPtr;
  ownedStock *tailPtr;
  double beta;
  double totalMrktValue;

public:
  Linked();
  ~Linked(); // destructor
  void addNode(EquityStock s, int shares);
  void delNode(string symbol);
  void printList();
  void calculatePortfolioBeta(EquityStock& market);
};