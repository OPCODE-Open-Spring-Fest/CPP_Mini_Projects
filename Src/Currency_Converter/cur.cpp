#include "cur.h"

Currency::Currency(const string& code, const string& name, const string& symbol)
    : code(code), name(name), symbol(symbol) {}

string Currency::getCode() const {
    return code;
}

string Currency::getName() const {
    return name;
}

string Currency::getSymbol() const {
    return symbol;
}
