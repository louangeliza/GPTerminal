#include "Utility.h"
#include <iostream>
#include <limits>

using namespace std;

int Utility::getIntegerInput(const string &prompt, int min)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number";
            if (min > 0)
                cout << " >= " << min;
            cout << ".\n";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

string Utility::getStringInput(const string &prompt, bool toLowercase)
{
    string input;
    cout << prompt;
    getline(cin, input);
    if (toLowercase)
    {
        transform(input.begin(), input.end(), input.begin(),
                  [](unsigned char c)
                  { return tolower(c); });
    }
    return input;
}

void Utility::generateGraphvizDOT(const string &filename)
{
    // Implementation for Graphviz DOT generation would go here
    // This would visualize the parking graph
}