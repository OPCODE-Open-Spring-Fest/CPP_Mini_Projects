#include <iostream>
#include <limits>
#include <iomanip>
#include <cmath>
using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

double getNumber(const string &prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            clearInput();
            return x;
        }
        cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

int getIntInRange(const string &prompt, int lo, int hi) {
    int c;
    while (true) {
        cout << prompt;
        if (cin >> c) {
            clearInput();
            if (c >= lo && c <= hi) return c;
            cout << "Choice out of range (" << lo << " - " << hi << "). Try again.\n";
        } else {
            cout << "Invalid input. Enter a number.\n";
            clearInput();
        }
    }
}

namespace conv {
    double metersToKilometers(double m) { return m / 1000.0; }
    double kilometersToMeters(double km) { return km * 1000.0; }
    double metersToMiles(double m) { return m / 1609.344; }
    double milesToMeters(double mi) { return mi * 1609.344; }
    double feetToMeters(double ft) { return ft * 0.3048; }
    double metersToFeet(double m) { return m / 0.3048; }

    double cToF(double c) { return (c * 9.0/5.0) + 32.0; }
    double fToC(double f) { return (f - 32.0) * 5.0/9.0; }
    double cToK(double c) { return c + 273.15; }
    double kToC(double k) { return k - 273.15; }

    double gramsToKilograms(double g) { return g / 1000.0; }
    double kilogramsToGrams(double kg) { return kg * 1000.0; }
    double kilogramsToPounds(double kg) { return kg * 2.2046226218; }
    double poundsToKilograms(double lb) { return lb / 2.2046226218; }

    double litersToMilliliters(double L) { return L * 1000.0; }
    double millilitersToLiters(double mL) { return mL / 1000.0; }
    double litersToGallons(double L) { return L * 0.2641720524; }
    double gallonsToLiters(double gal) { return gal / 0.2641720524; }
}

void pressEnterToContinue() {
    cout << "\nPress ENTER to continue...";
    cin.get();
}

void lengthMenu() {
    while (true) {
        cout << "\n--- Length Converter ---\n";
        cout << "1) Meters → Kilometers\n";
        cout << "2) Kilometers → Meters\n";
        cout << "3) Meters → Miles\n";
        cout << "4) Miles → Meters\n";
        cout << "5) Feet → Meters\n";
        cout << "6) Meters → Feet\n";
        cout << "7) Back to Main Menu\n";
        int ch = getIntInRange("Choose (1-7): ", 1, 7);
        if (ch == 7) return;

        double v = getNumber("Enter value: ");
        cout << fixed << setprecision(6);
        switch (ch) {
            case 1: cout << v << " m = " << conv::metersToKilometers(v) << " km\n"; break;
            case 2: cout << v << " km = " << conv::kilometersToMeters(v) << " m\n"; break;
            case 3: cout << v << " m = " << conv::metersToMiles(v) << " miles\n"; break;
            case 4: cout << v << " miles = " << conv::milesToMeters(v) << " m\n"; break;
            case 5: cout << v << " ft = " << conv::feetToMeters(v) << " m\n"; break;
            case 6: cout << v << " m = " << conv::metersToFeet(v) << " ft\n"; break;
        }
        pressEnterToContinue();
    }
}

void temperatureMenu() {
    while (true) {
        cout << "\n--- Temperature Converter ---\n";
        cout << "1) Celsius → Fahrenheit\n";
        cout << "2) Fahrenheit → Celsius\n";
        cout << "3) Celsius → Kelvin\n";
        cout << "4) Kelvin → Celsius\n";
        cout << "5) Back to Main Menu\n";
        int ch = getIntInRange("Choose (1-5): ", 1, 5);
        if (ch == 5) return;

        double v = getNumber("Enter value: ");
        cout << fixed << setprecision(4);
        switch (ch) {
            case 1: cout << v << " °C = " << conv::cToF(v) << " °F\n"; break;
            case 2: cout << v << " °F = " << conv::fToC(v) << " °C\n"; break;
            case 3: cout << v << " °C = " << conv::cToK(v) << " K\n"; break;
            case 4: cout << v << " K = " << conv::kToC(v) << " °C\n"; break;
        }
        pressEnterToContinue();
    }
}

void weightMenu() {
    while (true) {
        cout << "\n--- Weight Converter ---\n";
        cout << "1) Grams → Kilograms\n";
        cout << "2) Kilograms → Grams\n";
        cout << "3) Kilograms → Pounds\n";
        cout << "4) Pounds → Kilograms\n";
        cout << "5) Back to Main Menu\n";
        int ch = getIntInRange("Choose (1-5): ", 1, 5);
        if (ch == 5) return;

        double v = getNumber("Enter value: ");
        cout << fixed << setprecision(6);
        switch (ch) {
            case 1: cout << v << " g = " << conv::gramsToKilograms(v) << " kg\n"; break;
            case 2: cout << v << " kg = " << conv::kilogramsToGrams(v) << " g\n"; break;
            case 3: cout << v << " kg = " << conv::kilogramsToPounds(v) << " lb\n"; break;
            case 4: cout << v << " lb = " << conv::poundsToKilograms(v) << " kg\n"; break;
        }
        pressEnterToContinue();
    }
}

void volumeMenu() {
    while (true) {
        cout << "\n--- Volume Converter ---\n";
        cout << "1) Liters → Milliliters\n";
        cout << "2) Milliliters → Liters\n";
        cout << "3) Liters → Gallons (US)\n";
        cout << "4) Gallons (US) → Liters\n";
        cout << "5) Back to Main Menu\n";
        int ch = getIntInRange("Choose (1-5): ", 1, 5);
        if (ch == 5) return;

        double v = getNumber("Enter value: ");
        cout << fixed << setprecision(6);
        switch (ch) {
            case 1: cout << v << " L = " << conv::litersToMilliliters(v) << " mL\n"; break;
            case 2: cout << v << " mL = " << conv::millilitersToLiters(v) << " L\n"; break;
            case 3: cout << v << " L = " << conv::litersToGallons(v) << " gal\n"; break;
            case 4: cout << v << " gal = " << conv::gallonsToLiters(v) << " L\n"; break;
        }
        pressEnterToContinue();
    }
}

// --- Main ---
void printMainMenu() {
    cout << "\n=============================\n";
    cout << "      UNIT CONVERTER\n";
    cout << "=============================\n";
    cout << "1) Length\n";
    cout << "2) Temperature\n";
    cout << "3) Weight\n";
    cout << "4) Volume\n";
    cout << "5) Exit\n";
}

int main() {
    cout << "Simple Unit Converter (console)\n";
    while (true) {
        printMainMenu();
        int ch = getIntInRange("Choose (1-5): ", 1, 5);
        switch (ch) {
            case 1: lengthMenu(); break;
            case 2: temperatureMenu(); break;
            case 3: weightMenu(); break;
            case 4: volumeMenu(); break;
            case 5:
                cout << "Goodbye!\n";
                return 0;
        }
    }
    return 0;
}
