#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include "EquityStock.h"
using namespace std;

struct ownedStock {
  EquityStock stock;
  double quantity;
  ownedStock *nextPtr;
};

class Linked {
private:
  ownedStock *headPtr;
  ownedStock *tailPtr;
  double beta;
  double totalMrktValue;
  double expectedReturn;

public:
  Linked();
  ~Linked(); // destructor
  void addNode(EquityStock &s, double shares);
  void delNode(string symbol);
  void printList();
  void calculatePortfolioBeta();
  void calculateExpectedReturn();
  bool isEmpty();
  bool isOwned(string symbol);
  void updateNode(string symbol, double shares);

};
