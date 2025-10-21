#include <bits/stdc++.h>
#include "cur.h"
#include "conv.h"
using namespace std;
using nlim = numeric_limits<streamsize>;

void show(const CurrencyConverter& converter) {
    for (auto& currency : converter.listCurrencies())
        cout << currency.getCode() << "-" << currency.getName() << "(" << currency.getSymbol() << ")\n";
    cout << endl;
}

int main() {
    CurrencyConverter converter;
    converter.addCurrency(Currency("USD", "US Dollar", "$"));
    converter.addCurrency(Currency("EUR", "Euro", "€"));
    converter.addCurrency(Currency("INR", "Indian Rupee", "₹"));
    converter.addCurrency(Currency("GBP", "British Pound", "£"));
    converter.setExchangeRate("USD", "INR", 83.0);
    converter.setExchangeRate("INR", "USD", 0.012);
    converter.setExchangeRate("USD", "EUR", 0.93);
    converter.setExchangeRate("EUR", "USD", 1.08);
    converter.setExchangeRate("USD", "GBP", 0.79);
    converter.setExchangeRate("GBP", "USD", 1.27);
    converter.setExchangeRate("EUR", "INR", 89.0);
    converter.setExchangeRate("INR", "EUR", 0.011);
    int choice;
    do {
        cout << "\nMenu:\n1.List\n2.Convert\n3.Update\n0.Exit\n> ";
        cin >> choice;
        cin.ignore(nlim::max(),'\n');
        if (choice == 1) {
            show(converter);
        } else if (choice == 2) {
            string fromCurrency, toCurrency;
            double amount;
            cout << "From: ";
            cin >> fromCurrency;
            cout << "To: ";
            cin >> toCurrency;
            cout << "Amount: ";
            cin >> amount;
            if (cin.fail() || amount <= 0) {
                cout << "Invalid input\n";
                cin.clear();
                cin.ignore(nlim::max(),'\n');
                continue;
            }
            try {
                double result = converter.convert(amount, fromCurrency, toCurrency);
                cout << fixed << setprecision(2) << amount << fromCurrency << "=" << result << toCurrency << "\n";
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        } else if (choice == 3) {
            string fromCurrency, toCurrency;
            double rate;
            cout << "From: ";
            cin >> fromCurrency;
            cout << "To: ";
            cin >> toCurrency;
            cout << "Rate(1" << fromCurrency << "=" << toCurrency << "): ";
            cin >> rate;
            if (cin.fail() || rate <= 0) {
                cout << "Invalid rate\n";
                cin.clear();
                cin.ignore(nlim::max(),'\n');
                continue;
            }
            try {
                converter.setExchangeRate(fromCurrency, toCurrency, rate);
                cout << "Updated\n";
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        } else if (choice == 0) {
            cout << "bye\n";
        } else {
            cout << "bad\n";
        }
    } while (choice != 0);
    return 0;
}
