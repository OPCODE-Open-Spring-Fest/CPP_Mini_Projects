#ifndef CURRENCY_H
#define CURRENCY_H
#include <bits/stdc++.h>
using namespace std;
class Currency {
    string code, name, symbol;
public:
    Currency(const string& code, const string& name, const string& symbol);
    string getCode() const;
    string getName() const;
    string getSymbol() const;
};
#endif
