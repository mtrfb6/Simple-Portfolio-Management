#include "Portfolio.h"

using namespace ansi_escape_code;

Linked::Linked() {
    headPtr = nullptr;
    tailPtr = nullptr;
    beta = 0.0;
    totalMrktValue = 0.0;
}

Linked::~Linked() {
    //Destructor will loop through the list and delete each node
    ownedStock* tempPtr = headPtr;
    ownedStock* delPtr = tempPtr;
    beta = 0.0;
    totalMrktValue = 0.0;
    while (tempPtr != nullptr){
        tempPtr = tempPtr->nextPtr;
        delete delPtr;
        delPtr = tempPtr;
  }
  headPtr = nullptr;
  tailPtr = nullptr;
}

void Linked::addNode(EquityStock &S, double shares) {
  //Create a new node (set data & pointer values)
  if (shares <= 0)
    return;
  ownedStock* newStock = new ownedStock;
  newStock->stock = S;
  newStock->quantity = shares;
  totalMrktValue += newStock->quantity * newStock->stock.getPrice();
  
  //if the list is empty, set head & tail pointers to new node
  if (headPtr == nullptr){
    headPtr = newStock;
    tailPtr = newStock;
  }
  //else add node to the end
  else{
    tailPtr->nextPtr = newStock;
    tailPtr = newStock;
  }
}

void Linked::delNode(string symbol) {
  //start at the headPtr and loop until you find key or get to the end of the list
  ownedStock* tempPtr = headPtr;
  ownedStock* delPtr;
  
  //condition 1: delete data at headptr
  if (tempPtr != nullptr && tempPtr->stock.getSymbol() == symbol){
    delPtr = tempPtr;
    headPtr = tempPtr->nextPtr;
    //Remove the beta and market value from the portfolio
    beta -= ((delPtr->quantity * delPtr->stock.getPrice()) / totalMrktValue) * delPtr->stock.getBeta();
    totalMrktValue -= delPtr->quantity * delPtr->stock.getPrice();
    delete delPtr;
    delPtr = nullptr;
    return;
  }
  //condition 2: delete a value in the middle of the list
  else{
    while (tempPtr != nullptr && tempPtr->nextPtr != nullptr){
      if (tempPtr->nextPtr->stock.getSymbol() == symbol){
        delPtr = tempPtr->nextPtr;
        tempPtr->nextPtr = tempPtr->nextPtr->nextPtr;
        //condition 3: delete value at the end of the list
        if (tempPtr->nextPtr == tailPtr){
          tailPtr = tempPtr;
        }
        beta -= ((delPtr->quantity * delPtr->stock.getPrice()) / totalMrktValue) * delPtr->stock.getBeta();
        totalMrktValue -= delPtr->quantity * delPtr->stock.getPrice();
        delete delPtr;
        delPtr = nullptr;
        return;
      }
      tempPtr = tempPtr->nextPtr;
    }
  }  
}

bool Linked::isEmpty(){return (headPtr == nullptr);}

bool Linked::isOwned(string symbol){
  ownedStock* tempPtr = headPtr;
  while (tempPtr != nullptr){
    if (tempPtr->stock.getSymbol() == symbol)
      return true;
    tempPtr = tempPtr->nextPtr;
  }
  return false;
}

void Linked::updateNode(string symbol, double quantity){
  ownedStock* tempPtr = headPtr;
  //cond1: node is at the beginning of the list
  if (tempPtr != nullptr && tempPtr->stock.getSymbol() == symbol){
    totalMrktValue -= headPtr->quantity * headPtr->stock.getPrice(); //remove the old value
    headPtr->quantity = quantity; //update quantity
    totalMrktValue += headPtr->quantity * headPtr->stock.getPrice(); //add back the new value
    calculatePortfolioBeta();
    calculateExpectedReturn();
    return;
  }
  //cond2: node is in the middle
  else{
    while (tempPtr != nullptr && tempPtr->nextPtr != nullptr){
      if(tempPtr->nextPtr->stock.getSymbol() == symbol){
        totalMrktValue -= tempPtr->nextPtr->quantity * tempPtr->nextPtr->stock.getPrice();
        tempPtr->nextPtr->quantity = quantity;
        totalMrktValue += tempPtr->nextPtr->quantity * tempPtr->nextPtr->stock.getPrice();
        return;
      }
      //cond3: node is at the end
      if(tempPtr->nextPtr == tailPtr){
        totalMrktValue -= tailPtr->quantity * tailPtr->stock.getPrice();
        tailPtr->quantity = quantity;
        totalMrktValue += tailPtr->quantity * tailPtr->stock.getPrice();
        return;
      }
      tempPtr = tempPtr->nextPtr;
    }
  }
}

void Linked::printList() {
    cout << "\nYour Portfolio Breakdown: " << endl;
  //start at the headPtr and loop - printing data from each node
  if (headPtr == nullptr){
    cout << "No assets owned.\nCreate a portfolio in the build menu.\n";
  }
  else{
    cout << setfill('=') << setw(32) << '\n';
    calculatePortfolioBeta(); //get most recent measures
    calculateExpectedReturn();
    ownedStock* tempPtr = headPtr;
    while (tempPtr != nullptr){
      cout << fixed << setprecision(2) << '\n' << bold << tempPtr->stock.getSymbol() << reset <<  ": $" << tempPtr->stock.getPrice() << 
      endl << "# of Shares Owned: "<< tempPtr->quantity <<
      endl << "Average Monthly Return: " << tempPtr->stock.getAvgReturn() << "%" <<
      endl << "Beta: " << cyan << tempPtr->stock.getBeta() << reset <<
      endl << "Expected Return: " << magenta << tempPtr->stock.getExpectedReturn() << "%" << reset << endl << endl;
      tempPtr = tempPtr->nextPtr;
    }
    cout << fixed << setprecision(2) << "\nYour total market value is currently: " << green << "$" << totalMrktValue << reset <<
    endl << "Your Portfolio's Beta measures as: " << cyan << beta << reset << endl <<
    "Your portfolio's estimated expected return measures as: " << magenta << expectedReturn << "%" << reset << endl;
  }
}

void Linked::calculatePortfolioBeta(){
  ownedStock* tempPtr = headPtr;
  double weight = 0.0;
  double tempBeta = 0.0;
  if(tempPtr->nextPtr == nullptr){
    cout << "Portfolio is empty. Beta cannot be calculated." << endl << endl;
  }
  while(tempPtr != nullptr){
    //portfolio beta is the sum of the product of the stocks weight within the portfolio and its beta
    weight = (tempPtr->quantity * tempPtr->stock.getPrice()) / totalMrktValue;
    tempBeta += weight * tempPtr->stock.getBeta();
    tempPtr = tempPtr->nextPtr;
  }
  if (tempBeta != beta)
    beta = tempBeta;
}

void Linked::calculateExpectedReturn(){
  ownedStock* tempPtr = headPtr;
  double weight = 0.0;
  double tempER = 0.0;
  while(tempPtr != nullptr){
    //similar to beta, we take the product of the weight and expected return
    weight = (tempPtr->quantity * tempPtr->stock.getPrice()) / totalMrktValue;
    tempER += weight * tempPtr->stock.getExpectedReturn();
    tempPtr = tempPtr->nextPtr;
  }
  expectedReturn = tempER;
}