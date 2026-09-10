#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Item {
    string name;
    double weight;
    double value;
    bool divisible;
    double ratio;
};

bool compareRatio(const Item& a, const Item& b) {
    return a.ratio > b.ratio;
}

int main() {
    int n;
    double capacity;

    cout << "Enter number of relief items: ";
    cin >> n;

    cout << "Enter boat capacity (kg): ";
    cin >> capacity;

    vector<Item> items(n);

    cout << "\nEnter item details:\n";
    cout << "Name Weight Value Divisible(1/0)\n";

    for (int i = 0; i < n; i++) {
        cin >> items[i].name
            >> items[i].weight
            >> items[i].value
            >> items[i].divisible;

        items[i].ratio = items[i].value / items[i].weight;
    }

    sort(items.begin(), items.end(), compareRatio);

    double totalWeight = 0;
    double totalValue = 0;

    cout << "\nSelected Relief Supplies:\n";
    cout << fixed << setprecision(2);

    cout << "\nItem\tTaken Weight\tUtility\tFraction\n";

    for (const auto& item : items) {

        if (totalWeight >= capacity)
            break;

        double remaining = capacity - totalWeight;

        // Complete item
        if (!item.divisible) {
            if (item.weight <= remaining) {
                totalWeight += item.weight;
                totalValue += item.value;

                cout << item.name << "\t"
                     << item.weight << "\t\t"
                     << item.value << "\t"
                     << "1.00\n";
            }
        }

        // Divisible item
        else {
            double fraction = min(1.0, remaining / item.weight);
            double takenWeight = item.weight * fraction;
            double gainedValue = item.value * fraction;

            totalWeight += takenWeight;
            totalValue += gainedValue;

            cout << item.name << "\t"
                 << takenWeight << "\t\t"
                 << gainedValue << "\t"
                 << fraction << "\n";
        }
    }

    cout << "\n----------------------------------------\n";
    cout << "Total Weight: " << totalWeight << " kg\n";
    cout << "Total Utility: " << totalValue << "\n";
    cout << "Remaining Capacity: "
         << capacity - totalWeight << " kg\n";

    return 0;
}
