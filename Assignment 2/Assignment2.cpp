#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;

struct Movie {
    string name;
    double rating;
    int year;
    long long votes;
};

// Reads one complete CSV line while handling commas inside quotes
vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool insideQuotes = false;

    for (char c : line) {
        if (c == '"') {
            insideQuotes = !insideQuotes;
        }
        else if (c == ',' && !insideQuotes) {
            fields.push_back(field);
            field.clear();
        }
        else {
            field += c;
        }
    }

    fields.push_back(field);
    return fields;
}

bool comesBefore(const Movie& a, const Movie& b, int choice) {
    if (choice == 1)
        return a.rating < b.rating;

    if (choice == 2)
        return a.year < b.year;

    return a.votes < b.votes;
}

void quickSort(vector<Movie>& movies, int low, int high, int choice) {
    while (low < high) {
        int i = low;
        int j = high;

        Movie pivot = movies[low + (high - low) / 2];

        while (i <= j) {
            while (comesBefore(movies[i], pivot, choice))
                i++;

            while (comesBefore(pivot, movies[j], choice))
                j--;

            if (i <= j) {
                swap(movies[i], movies[j]);
                i++;
                j--;
            }
        }

        // Process smaller partition first
        if (j - low < high - i) {
            if (low < j)
                quickSort(movies, low, j, choice);

            low = i;
        }
        else {
            if (i < high)
                quickSort(movies, i, high, choice);

            high = j;
        }
    }
}

int main() {

    ifstream file("movie.csv");

    if (!file.is_open()) {
        cerr << "Error: Could not open movie.csv\n";
        return 1;
    }

    string line;

    // Skip header
    getline(file, line);

    vector<Movie> movies;

    while (getline(file, line)) {

        vector<string> fields = parseCSVLine(line);

        if (fields.size() < 6)
            continue;

        try {
            Movie movie;

            movie.name = fields[1];
            movie.year = stoi(fields[2]);
            movie.rating = stod(fields[4]);
            movie.votes = stoll(fields[5]);

            movies.push_back(movie);
        }
        catch (...) {
            continue;
        }
    }

    file.close();

    cout << "Movies loaded: " << movies.size() << "\n\n";

    int choice;

    cout << "Sort movies by:\n";
    cout << "1. IMDb Rating\n";
    cout << "2. Year of Release\n";
    cout << "3. Popularity (Votes)\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice < 1 || choice > 3) {
        cout << "Invalid choice.\n";
        return 1;
    }

    auto start = high_resolution_clock::now();

    quickSort(movies, 0, movies.size() - 1, choice);

    auto stop = high_resolution_clock::now();

    auto duration =
        duration_cast<microseconds>(stop - start);

    cout << "\nQuick Sort completed successfully.\n";
    cout << "Sorting time: "
         << duration.count()
         << " microseconds\n";

    cout << "\nFirst 20 sorted movies:\n";
    cout << "------------------------------------------------------------\n";
    cout << "Movie\t\tRating\tYear\tVotes\n";
    cout << "------------------------------------------------------------\n";

    int limit = min(20, (int)movies.size());

    for (int i = 0; i < limit; i++) {
        cout << movies[i].name << "\t"
             << movies[i].rating << "\t"
             << movies[i].year << "\t"
             << movies[i].votes << "\n";
    }

    return 0;
}
