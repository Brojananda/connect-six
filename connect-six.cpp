#include <vector>
#include <string>
#include <iostream>
#include <cstdlib> // Used for random number generation. Better than <random> 
                   // because of flexibility of number ranges
#include <set>
#include <deque>
#include <limits>

// #include <getopt.h>
using namespace std;

enum class Status {
    Unoccupied = 0,
    Player,
    NPC
};

struct City {
    public:
        City(const string& namein, const int& randomIncome, const int& randomSoldiers, const int& randomTanks) :
            name(namein), incomePerTurn(randomIncome), neutralSoldiers(randomSoldiers), neutralTanks(randomTanks)
        { }

        string name;
        int incomePerTurn;
        int neutralSoldiers;
        int neutralTanks;
        int playerSoldiers;
        int playerTanks;
        int npcSoldiers;
        int npcTanks;
        Status status = Status::Unoccupied;
};

struct Player {
    public:
        Player(const size_t& hqNum) :
            hqNum(hqNum)
        { }

        int getIncome() {
            return totalIncome;
        }
        void addIncome(const int& newInc) {
            totalIncome += newInc;
        }

        bool getHQ() {
            return hq;
        }
        void setHQ(const bool& hasHQ) {
            hq = hasHQ;
        }

        size_t getHQNum() {
            return hqNum;
        }

    private:
        int totalIncome = 0;
        bool hq = true;
        size_t hqNum = 6; // (1-5)
};

int main() {
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed for RNG

    // Choose 6 random city names from list of 16
    // Potential future functionality: push back names inputted by user (easy)
    vector<string> names;
    names.reserve(16); // Capacity: 16
    names.push_back("Morrowind");
    names.push_back("Skyrim");
    names.push_back("Summerset Isles");
    names.push_back("Hammerfell");
    names.push_back("Narnia");
    names.push_back("Elysium");
    names.push_back("Hyrule");
    names.push_back("Olympus");
    names.push_back("Equestria");
    names.push_back("Termina");
    names.push_back("Konoha");
    names.push_back("Paradis");
    names.push_back("Oz");
    names.push_back("Shijima");
    names.push_back("Musubi");
    names.push_back("Yosuga");
    set<size_t> indices; // Unique indices
    deque<size_t> indicesOrder;
    
    while (indices.size() < 6) {
        size_t oldSize = indices.size();
        size_t randNum = static_cast<size_t>(rand() % 16); // Random number between 0 (inclusive) and 16 (exclusive)
        indices.insert(randNum); // Inserts index only if not a repeat (search)
        size_t newSize = indices.size();

        if (oldSize != newSize) { // New index added means it was unique
            indicesOrder.push_back(randNum);
        }
    }

    // Generate 6 cities
    vector<City> cities;
    for (size_t i = 0; i < 6; ++i) {
        size_t currentIndex = indicesOrder.front();        
        string cityName = names[currentIndex];  // Unique name for each city
        City city(cityName, rand() % 100, rand() % 20, rand() % 10); // 3 random numbers in one line
        cities.push_back(city);

        indicesOrder.pop_front(); // Make way for next loop
    }

    // Select Player & NPC bases
    cout << "Welcome to Connect Six! Select a city as your HQ. \n";
    for (size_t i = 0; i < cities.size(); ++i) {
        cout << "City " << i << ": " << cities[i].name << "\n";
    }
    cout << "Please enter a number (0-5): ";

    size_t choice = 6;
    cin >> choice;
    while (choice > 5) { // Positive or negative input (no need to check for negatives for size_t)
        cout << "Invalid choice. Please enter a number (0-5): ";
        cin >> choice;
    }

    Player player(choice);
    cities[choice].status = Status::Player;
    cities[choice].playerSoldiers = cities[choice].neutralSoldiers;
    cities[choice].playerTanks = cities[choice].neutralTanks;
    cities[choice].neutralSoldiers = 0;
    cities[choice].neutralTanks = 0;
    cout << "\nExcellent. Your HQ is " << cities[choice].name << ".\n";

    size_t npcChoice = static_cast<size_t>(rand() % 6);
    while (npcChoice == choice) {
        npcChoice = static_cast<size_t>(rand() % 6); // Must be different from player's
    }

    Player npc(npcChoice);
    cities[npcChoice].status = Status::NPC;
    cities[choice].npcSoldiers = cities[choice].neutralSoldiers;
    cities[choice].npcTanks = cities[choice].neutralTanks;
    cities[choice].neutralSoldiers = 0;
    cities[choice].neutralTanks = 0;
    cout << "\nPlayer 2 chose " << cities[npcChoice].name << " as their HQ.\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

    vector<City> pCities; // except for HQ
    vector<City> nCities; // except for HQ

    // Game begins
    int turn = 0;
    while (player.getHQ() && npc.getHQ()) { // While neither HQ is captured
        ++turn;
        cout << "Round " << turn << "\n";
        // Add income from each city
        player.addIncome(cities[player.getHQNum()].incomePerTurn);
        npc.addIncome(cities[npc.getHQNum()].incomePerTurn);
        for (size_t i = 0; i < pCities.size(); ++i) {
            player.addIncome(pCities[i].incomePerTurn);
        }
        for (size_t i = 0; i < nCities.size(); ++i) {
            npc.addIncome(nCities[i].incomePerTurn);
        }

        cout << "Player Income: " << player.getIncome() << "\n";
        cout << "NPC Income: " << npc.getIncome() << "\n";
        cout << "City Stats:\n";
        for (size_t i = 0; i < cities.size(); ++i) {
            cout << cities[i].name << ": ";

            switch(cities[i].status) {
                case Status::Player:
                    cout << "Controlled by Player // Income: " << cities[i].incomePerTurn;
                    cout << " // Soldiers: " << cities[i].playerSoldiers;
                    cout << " // Tanks: " << cities[i].playerTanks << "\n";
                    break;
                case Status::NPC:
                    cout << "Controlled by NPC // Income: " << cities[i].incomePerTurn;
                    cout << " // Soldiers: " << cities[i].npcSoldiers;
                    cout << " // Tanks: " << cities[i].npcTanks << "\n";
                    break;
                case Status::Unoccupied:
                    cout << "Neutral // Income: " << cities[i].incomePerTurn;
                    cout << " // Soldiers: " << cities[i].neutralSoldiers;
                    cout << " // Tanks: " << cities[i].neutralTanks << "\n"; 
                    break;
            }
        }
        cout << "\nPlayer's HQ: " << cities[player.getHQNum()].name << "\n";
        cout << "NPC's HQ: " << cities[npc.getHQNum()].name << "\n\n";

        // Decision Phase
        bool repeat = true;
        while (repeat) {
            repeat = false;
            cout << "What would you like to do? Please type carefully.\n";
            cout << "Options: Buy units, Move units, Nothing";

            string action = "";
            cin >> action;
            char actionLetter = action[0];

            switch(actionLetter) {
                case 'B': {
                    cout << "How many soldiers would you like to hire? Cost: 20 each. ";
                    int num = 0;
                    cin >> num;
                    if (num > 0) {
                        cout << "\nIn which of your cities should they be stationed? Please enter a name: ";
                        string name = "";
                        cin >> name;
                        
                        for (size_t i = 0; i < pCities.size(); ++i) {
                            if (pCities[i].name == name) {
                                pCities[i].playerSoldiers += num;
                                player.addIncome(-20 * num); // Income subtracted only if city found
                                cout << pCities[i].name << " now has " << pCities[i].playerSoldiers << " soldiers.\n";
                            }
                        }
                    }

                    cout << "Your remaining income is " << player.getIncome() << ".\n";

                    cout << "How many tanks would you like to build? Cost: 50 each. ";
                    int numTwo = 0;
                    cin >> numTwo;
                    if (numTwo > 0) {
                        cout << "\nIn which of your cities should they be stationed? Please enter a name: ";
                        string name = "";
                        cin >> name;
                        
                        for (size_t i = 0; i < pCities.size(); ++i) {
                            if (pCities[i].name == name) {
                                pCities[i].playerTanks += num;
                                player.addIncome(-50 * num); // Income subtracted only if city found
                                cout << pCities[i].name << " now has " << pCities[i].playerTanks<< " tanks.\n";
                            }
                        }
                    }

                    cout << "Your remaining income is " << player.getIncome() << ".\n";

                    repeat = true;
                    break;
                }

                case 'M': {
                    cout << "Select one of your cities to move units from. Please enter a name: ";
                    string name = "";
                    cin >> name;
                    for (size_t i = 0; i < pCities.size(); ++i) {
                        if (pCities[i].name == name) {
                            cout << "\nSelect one of your adjacent cities to move units to. Please enter a name: ";
                            cin >> name;
                            for (size_t j = 0; j < pCities.size(); ++j) {
                                if (pCities[j].name == name) {
                                    cout << "How many soldiers would you like to move? ";
                                    int num = 0;
                                    cin >> num;
                                    if (num > 0 && num <= pCities[i].playerSoldiers) {
                                        pCities[i].playerSoldiers -= num;
                                        pCities[j].playerSoldiers += num;
                                        cout << num << " soldiers are on their way.\n\n";
                                    }
                                    
                                    cout << "How many tanks would you like to move? ";
                                    int numTwo = 0;
                                    cin >> numTwo;
                                    if (numTwo > 0 && numTwo <= pCities[i].playerTanks) {
                                        pCities[i].playerTanks -= numTwo;
                                        pCities[j].playerTanks += numTwo;
                                        cout << numTwo << " tanks are on their way.\n\n";
                                    }
                                }
                            }
                        }
                    }

                    repeat = true;
                    break;
                }

                default: // Includes "Nothing". repeat = false
                    break;
            }
        }

        // NPC's turn. Algorithmic strategy
        if (npc.getIncome() >= 20) {
            int leastCount = numeric_limits<int>::max();
            size_t leastIndex = 0;
            for (size_t i = 0; i < nCities.size(); ++i) {
                if (nCities[i].npcSoldiers + nCities[i].npcTanks < leastCount) {
                    leastCount = nCities[i].npcSoldiers + nCities[i].npcTanks;
                    leastIndex = i;
                }
            }

            // Buy tanks first
            int buyTanks = npc.getIncome() / 50; // Decimal is truncated
            if (buyTanks > 0) {
                nCities[leastIndex].npcTanks += buyTanks;
                npc.addIncome(-50 * buyTanks);
                cout << "NPC constructed " << buyTanks << " tanks at " << nCities[leastIndex].name << ".\n";
            }

            // Then buy soldiers
            int buySoldiers = npc.getIncome() / 20; // Decimal is truncated
            if (buySoldiers > 0) {
                nCities[leastIndex].npcSoldiers += buySoldiers;
                npc.addIncome(-50 * buySoldiers);
                cout << "NPC hired " << buySoldiers << " soldiers at " << nCities[leastIndex].name << ".\n";
            }

            // Move units
            string leastName = nCities[leastIndex].name;
            size_t leastIndexTrue = 6;
            for (size_t i = 0; i < cities.size(); ++i) {
                if (cities[i].name == leastName) {
                    leastIndexTrue = i;
                }
            }

            if ((cities[leastIndexTrue-1].status == Status::Unoccupied || cities[leastIndexTrue-1].status == Status::Player)
                && (cities[leastIndexTrue].npcSoldiers + cities[leastIndexTrue].npcTanks >= 
                    3 * (cities[leastIndexTrue-1].neutralSoldiers + cities[leastIndexTrue-1].neutralTanks)
                       + cities[leastIndexTrue-1].playerSoldiers + cities[leastIndexTrue-1].playerTanks)) {
                // Move units to adjacent city
                cities[leastIndexTrue-1].npcSoldiers += cities[leastIndexTrue].npcSoldiers;
                cities[leastIndexTrue-1].npcTanks += cities[leastIndexTrue].npcTanks;
                cities[leastIndexTrue].npcSoldiers = 0;
                cities[leastIndexTrue].npcTanks = 0;
                cout << "NPC moved units from " << cities[leastIndexTrue].name << " to " << cities[leastIndexTrue-1].name << ".\n";
            }

            if ((cities[leastIndexTrue+1].status == Status::Unoccupied || cities[leastIndexTrue+1].status == Status::Player)
                && (cities[leastIndexTrue].npcSoldiers + cities[leastIndexTrue].npcTanks >= 
                    3 * (cities[leastIndexTrue+1].neutralSoldiers + cities[leastIndexTrue+1].neutralTanks)
                       + cities[leastIndexTrue+1].playerSoldiers + cities[leastIndexTrue+1].playerTanks)) {
                // Move units to adjacent city
                cities[leastIndexTrue+1].npcSoldiers += cities[leastIndexTrue].npcSoldiers;
                cities[leastIndexTrue+1].npcTanks += cities[leastIndexTrue].npcTanks;
                cities[leastIndexTrue].npcSoldiers = 0;
                cities[leastIndexTrue].npcTanks = 0;
                cout << "NPC moved units from " << cities[leastIndexTrue].name << " to " << cities[leastIndexTrue+1].name << ".\n";
            }
        }

        else {
            cout << "NPC did not make a move.\n\n";
        }

        // Battles play out
        cout << "Results of Round " << turn << "\n";
        for (size_t i = 0; i < cities.size(); ++i) {
            if (cities[i].playerSoldiers + cities[i].playerTanks > 0 && cities[i].npcSoldiers + cities[i].npcTanks > 0) {
                cout << "Battle in " << cities[i].name << " between Player and NPC.\n";
                // Tanks first
                int nTanks = cities[i].npcTanks;
                cities[i].npcTanks -= cities[i].playerTanks;
                if (cities[i].npcTanks < 0) {
                    cities[i].npcTanks = 0;
                }
                cities[i].playerTanks -= nTanks;
                if (cities[i].playerTanks < 0) {
                    cities[i].playerTanks = 0;
                }
            }
        }



        player.setHQ(false);
    }


}
// g++ -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic connect-six.cpp -o connect-six