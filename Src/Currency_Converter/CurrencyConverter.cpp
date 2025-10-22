#include "CurrencyConverter.h"
#include <iostream>
#include <stdexcept> 

CurrencyConverter::CurrencyConverter() {
    // We will use USD as the "base currency" for all conversions.
    // These rates represent how many USD 1 unit of the currency is worth.
    // Example: 1 EUR = 1.08 USD
    rates["USD"] = 1.00;
    rates["EUR"] = 1.08;
    rates["INR"] = 0.012;
    rates["GBP"] = 1.25;
    rates["JPY"] = 0.0067; 
}

bool CurrencyConverter::isCurrencySupported(const std::string& currencyCode) {
    return rates.find(currencyCode) != rates.end();
}

// Helper function to print all supported currency codes
void CurrencyConverter::printAvailableCurrencies() {
    std::cout << "Supported currencies: ";
    for (const auto& pair : rates) {
        std::cout << pair.first << " "; // pair.first is the key (e.g., "USD")
    }
    std::cout << std::endl;
}

// The main conversion logic
double CurrencyConverter::convert(double amount, const std::string& fromCurrency, const std::string& toCurrency) {
    
    // 1. Error Handling: Check if both currencies are supported
    if (!isCurrencySupported(fromCurrency)) {
        throw std::invalid_argument("Error: 'From' currency code not supported: " + fromCurrency);
    }
    if (!isCurrencySupported(toCurrency)) {
        throw std::invalid_argument("Error: 'To' currency code not supported: " + toCurrency);
    }

    // 2. Handle simple case: No conversion needed
    if (fromCurrency == toCurrency) {
        return amount;
    }

    // 3. Conversion Logic (using USD as a base)
    // First, convert the 'from' amount into our base currency (USD)
    double amountInUSD = amount * rates[fromCurrency];

    // Second, convert from USD into the 'to' currency
    // (We divide because the rate is X_USD = 1_TARGET_CURRENCY)
    // To get 1 USD = Y_TARGET_CURRENCY, we do 1 / rate.
    double finalAmount = amountInUSD / rates[toCurrency];

    return finalAmount;
}