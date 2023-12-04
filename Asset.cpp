#include "Asset.h"

Asset::Asset(){
    price = 0.00;
    symbol = " ";
}

Asset::Asset(double p, string s, int q){
    price = p;
    symbol = s;
}
