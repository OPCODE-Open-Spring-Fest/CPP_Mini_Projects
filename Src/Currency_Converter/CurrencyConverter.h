#pragma once 
#include <string>
#include <map>

class CurrencyConverter {
private:
    // A map to store exchange rates, with USD as the base currency.
    // The key is the currency code (e.g., "EUR"), and the value is
    // how many USD 1 unit of that currency is worth.
    std::map<std::string, double> rates;

public:
    // This is the constructor. It's called when you create a new
    // CurrencyConverter object. We use it to load our predefined rates.
    CurrencyConverter();

    // The main conversion function.
    // Throws an exception if a currency code is invalid.
    double convert(double amount, const std::string& fromCurrency, const std::string& toCurrency);

    // A helper function to check if we support a currency.
    bool isCurrencySupported(const std::string& currencyCode);

    // A helper function to show the user what currencies are available.
    void printAvailableCurrencies();
};