#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Road
{
      int city1;
      int city2;
      double budget;
};

class TransportSystem
{
private:
      map<int, string> cities;
      vector<Road> roads;
      int nextCityId;

public:
      TransportSystem() : nextCityId(1) {}

      void addCity(const string &name)
      {
            cities[nextCityId++] = name;
      }

      void addRoad(int city1, int city2, double budget = 0)
      {
            Road road;
            road.city1 = min(city1, city2);
            road.city2 = max(city1, city2);
            road.budget = budget;
            roads.push_back(road);
      }

      void updateRoadBudget(int city1, int city2, double budget)
      {
            int c1 = min(city1, city2);
            int c2 = max(city1, city2);

            for (auto &road : roads)
            {
                  if (road.city1 == c1 && road.city2 == c2)
                  {
                        road.budget = budget;
                        return;
                  }
            }
            // If road doesn't exist, we create it
            addRoad(c1, c2, budget);
      }

      void editCity(int index, const string &newName)
      {
            if (cities.find(index) != cities.end())
            {
                  cities[index] = newName;
            }
      }

      string searchCity(int index)
      {
            if (cities.find(index) != cities.end())
            {
                  return cities[index];
            }
            return "City not found";
      }

      void displayCities()
      {
            cout << "Cities:\n";
            for (const auto &city : cities)
            {
                  cout << city.first << ". " << city.second << endl;
            }
      }

      void displayRoads()
      {
            cout << "Roads:\n";
            for (const auto &road : roads)
            {
                  cout << cities[road.city1] << " - " << cities[road.city2]
                       << " (Budget: " << road.budget << " Billion Frw)" << endl;
            }
      }

      void displayAdjacencyMatrices()
      {
            if (cities.empty())
            {
                  cout << "No cities recorded yet!\n";
                  return;
            }

            int size = cities.size();
            vector<vector<int>> roadMatrix(size, vector<int>(size, 0));
            vector<vector<double>> budgetMatrix(size, vector<double>(size, 0.0));

            for (const auto &road : roads)
            {
                  roadMatrix[road.city1 - 1][road.city2 - 1] = 1;
                  roadMatrix[road.city2 - 1][road.city1 - 1] = 1;
                  budgetMatrix[road.city1 - 1][road.city2 - 1] = road.budget;
                  budgetMatrix[road.city2 - 1][road.city1 - 1] = road.budget;
            }

            // Road Adjacency Matrix
            cout << "\nRoad Adjacency Matrix:\n   ";
            for (int i = 1; i <= size; i++)
                  cout << setw(4) << i;
            cout << endl;

            for (int i = 0; i < size; i++)
            {
                  cout << setw(3) << i + 1;
                  for (int j = 0; j < size; j++)
                  {
                        cout << setw(4) << roadMatrix[i][j];
                  }
                  cout << endl;
            }

            // Budget Adjacency Matrix
            cout << "\nBudget Adjacency Matrix (Billion Frw):\n   ";
            for (int i = 1; i <= size; i++)
                  cout << setw(8) << i;
            cout << endl;

            for (int i = 0; i < size; i++)
            {
                  cout << setw(3) << i + 1;
                  for (int j = 0; j < size; j++)
                  {
                        cout << setw(8) << fixed << setprecision(2) << budgetMatrix[i][j];
                  }
                  cout << endl;
            }
      }

      void saveToFiles()
      {
            ofstream cityFile("cities.txt");
            cityFile << "Index\tCity_name\n";
            for (const auto &city : cities)
            {
                  cityFile << city.first << "\t" << city.second << "\n";
            }
            cityFile.close();

            ofstream roadFile("roads.txt");
            roadFile << "Nbr\tRoad\tBudget\n";
            int nbr = 1;
            for (const auto &road : roads)
            {
                  roadFile << nbr++ << "\t"
                           << cities[road.city1] << "-" << cities[road.city2]
                           << "\t" << road.budget << "\n";
            }
            roadFile.close();
      }

      int getCityIndex(const string &name)
      {
            for (const auto &city : cities)
            {
                  if (city.second == name)
                  {
                        return city.first;
                  }
            }
            return -1; // City not found
      }

      bool roadExists(int city1, int city2)
      {
            int c1 = min(city1, city2);
            int c2 = max(city1, city2);

            for (const auto &road : roads)
            {
                  if (road.city1 == c1 && road.city2 == c2)
                  {
                        return true;
                  }
            }
            return false;
      }

      int getNextCityId() const
      {
            return nextCityId;
      }

      bool hasCities() const
      {
            return !cities.empty();
      }

      void loadCitiesFromFile()
      {
            ifstream cityFile("cities.txt");
            if (!cityFile.is_open())
                  return; // File doesn't exist

            string line;
            getline(cityFile, line);
            while (getline(cityFile, line))
            {
                  size_t tabPos = line.find('\t');
                  if (tabPos != string::npos)
                  {
                        int index = stoi(line.substr(0, tabPos));
                        string name = line.substr(tabPos + 1);
                        cities[index] = name;
                        if (index >= nextCityId)
                        {
                              nextCityId = index + 1;
                        }
                  }
            }
            cityFile.close();
      }

      void loadRoadsFromFile()
      {
            ifstream roadFile("roads.txt");
            if (!roadFile.is_open())
                  return;

            string line;
            getline(roadFile, line); // Skip header
            while (getline(roadFile, line))
            {
                  size_t tab1 = line.find('\t');
                  size_t tab2 = line.find('\t', tab1 + 1);
                  if (tab1 != string::npos && tab2 != string::npos)
                  {
                        size_t dashPos = line.find('-', tab1 + 1);
                        if (dashPos != string::npos)
                        {
                              string city1 = line.substr(tab1 + 1, dashPos - tab1 - 1);
                              string city2 = line.substr(dashPos + 1, tab2 - dashPos - 1);
                              double budget = stod(line.substr(tab2 + 1));

                              int c1 = getCityIndex(city1);
                              int c2 = getCityIndex(city2);
                              if (c1 != -1 && c2 != -1)
                              {
                                    addRoad(c1, c2, budget);
                              }
                        }
                  }
            }
            roadFile.close();
      }
};

int main()
{
      TransportSystem system;

      system.loadCitiesFromFile();
      system.loadRoadsFromFile();

      int choice;

      do
      {
            cout << "\nMenu:\n";
            cout << "1. Add new city(ies)\n";
            cout << "2. Add roads between cities\n";
            cout << "3. Add the budget for roads\n";
            cout << "4. Edit city\n";
            cout << "5. Search for a city using its index\n";
            cout << "6. Display cities\n";
            cout << "7. Display roads\n";
            cout << "8. Display recorded data on console\n";
            cout << "9. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                  int numCities;
                  cout << "Enter the number of cities to add: ";
                  cin >> numCities;
                  cin.ignore();

                  for (int i = 0; i < numCities; i++)
                  {
                        string name;
                        cout << "Enter the name for city " << system.getNextCityId() << ": ";
                        getline(cin, name);
                        system.addCity(name);
                  }
                  system.saveToFiles();
                  break;
            }

            case 2:
            {
                  if (!system.hasCities())
                  {
                        cout << "No cities exist yet! Please add cities first.\n";
                        break;
                  }

                  string city1, city2;
                  cout << "Enter the name of the first city: ";
                  cin.ignore();
                  getline(cin, city1);
                  cout << "Enter the name of second city: ";
                  getline(cin, city2);

                  int c1 = system.getCityIndex(city1);
                  int c2 = system.getCityIndex(city2);

                  if (c1 == -1 || c2 == -1)
                  {
                        cout << "One or both cities not found!\n";
                  }
                  else if (c1 == c2)
                  {
                        cout << "Cannot add road between the same city!\n";
                  }
                  else
                  {
                        system.addRoad(c1, c2);
                        cout << "Road added between " << city1 << " and " << city2 << endl;
                        system.saveToFiles();
                  }
                  break;
            }

            case 3:
            {
                  if (!system.hasCities())
                  {
                        cout << "No cities exist yet! Please add cities first.\n";
                        break;
                  }

                  string city1, city2;
                  double budget;
                  cout << "Enter the name of the first city: ";
                  cin.ignore();
                  getline(cin, city1);
                  cout << "Enter the name of second city: ";
                  getline(cin, city2);
                  cout << "Enter the budget for the road (in Billion Frw): ";
                  cin >> budget;

                  int c1 = system.getCityIndex(city1);
                  int c2 = system.getCityIndex(city2);

                  if (c1 == -1 || c2 == -1)
                  {
                        cout << "One or both cities not found!\n";
                  }
                  else if (!system.roadExists(c1, c2))
                  {
                        cout << "No road exists between these cities! Add the road first.\n";
                  }
                  else
                  {
                        system.updateRoadBudget(c1, c2, budget);
                        cout << "Budget added for the road between " << city1 << " and " << city2 << endl;
                        system.saveToFiles();
                  }
                  break;
            }

            case 4:
            {
                  if (!system.hasCities())
                  {
                        cout << "No cities exist yet! Please add cities first.\n";
                        break;
                  }

                  int index;
                  string newName;
                  cout << "Enter the index of the city to edit: ";
                  cin >> index;
                  cout << "Enter the new name of the city: ";
                  cin.ignore();
                  getline(cin, newName);

                  string oldName = system.searchCity(index);
                  if (oldName == "City not found")
                  {
                        cout << oldName << endl;
                  }
                  else
                  {
                        system.editCity(index, newName);
                        cout << "City updated from '" << oldName << "' to '" << newName << "' successfully\n";
                        system.saveToFiles();
                  }
                  break;
            }

            case 5:
            {
                  if (!system.hasCities())
                  {
                        cout << "No cities exist yet! Please add cities first.\n";
                        break;
                  }

                  int index;
                  cout << "Enter the city's index: ";
                  cin >> index;
                  string city = system.searchCity(index);
                  cout << "The city with index " << index << " is " << city << endl;
                  break;
            }

            case 6:
            {
                  if (!system.hasCities())
                  {
                        cout << "No cities recorded yet!\n";
                  }
                  else
                  {
                        system.displayCities();
                  }
                  break;
            }

            case 7:
            {
                  if (!system.hasCities())
                  {
                        cout << "No cities recorded yet!\n";
                  }
                  else
                  {
                        system.displayRoads();
                  }
                  break;
            }

            case 8:
            {
                  cout << "Recorded data:\n";
                  system.displayCities();
                  system.displayRoads();
                  system.displayAdjacencyMatrices();
                  break;
            }

            case 9:
            {
                  cout << "Program exited successfully\n";
                  break;
            }

            default:
            {
                  cout << "Invalid choice. Please try again.\n";
                  break;
            }
            }
      } while (choice != 9);

      return 0;
}