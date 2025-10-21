#include "conv.h"

CurrencyConverter::CurrencyConverter() {}

void CurrencyConverter::addCurrency(const Currency& currency) {
    currencies[currency.getCode()] = currency;
}

void CurrencyConverter::setExchangeRate(const string& fromCurrency, const string& toCurrency, double rate) {
    if (rate <= 0)
        throw invalid_argument("Invalid rate");
    exchangeRates[fromCurrency + toCurrency] = rate;
}

double CurrencyConverter::getExchangeRate(const string& fromCurrency, const string& toCurrency) const {
    auto it = exchangeRates.find(fromCurrency + toCurrency);
    if (it == exchangeRates.end())
        throw invalid_argument("Exchange rate not found");
    return it->second;
}

bool CurrencyConverter::hasCurrency(const string& code) const {
    return currencies.find(code) != currencies.end();
}

vector<Currency> CurrencyConverter::listCurrencies() const {
    vector<Currency> result;
    for (auto& x : currencies)
        result.push_back(x.second);
    return result;
}

double CurrencyConverter::convert(double amount, const string& fromCurrency, const string& toCurrency) const {
    if (!hasCurrency(fromCurrency) || !hasCurrency(toCurrency))
        throw invalid_argument("Invalid currency code");
    return amount * getExchangeRate(fromCurrency, toCurrency);
}
