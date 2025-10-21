#ifndef CURRENCYCONVERTER_H
#define CURRENCYCONVERTER_H
#include <bits/stdc++.h>
#include "cur.h"
using namespace std;
class CurrencyConverter {
    map<string, Currency> currencies;
    map<string, double> exchangeRates;
public:
    CurrencyConverter();
    void addCurrency(const Currency& currency);
    void setExchangeRate(const string& fromCurrency, const string& toCurrency, double rate);
    double convert(double amount, const string& fromCurrency, const string& toCurrency) const;
    bool hasCurrency(const string& code) const;
    double getExchangeRate(const string& fromCurrency, const string& toCurrency) const;
    vector<Currency> listCurrencies() const;
};
#endif
