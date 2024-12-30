/*
MODULES USED
1.DIJKSTRA'S algorithm (to find the nearest parking spot)
2.WARSHALL'S algorithm (to check whether path exist between two points)
3.BFS for displaying the city
4.INSERTION SORT, to display the places based on the remaining parking spots
5.KMP search
6.FLOYD'S, to display the shortest path between all the points
 */
#include <bits/stdc++.h>
#include <iostream>
using namespace std;

//This the definition of the structure
struct node
{
    string name;
    bool hasParking;
    int carSpots;
    int bikeSpots;
    int blockedCarSpots; 
    int blockedBikeSpots; 
};
typedef node NODE;

class CityGraph
{
    unordered_map<string, NODE> nodes;
    unordered_map<string, unordered_map<string, int>> adjList; //This is adjacency list
    vector<vector<int>> adjMatrix; //This is adjacency matrix
    vector<string> places;
    unordered_map<string ,string> userBlockedSpot;//maps users to place they blocked

    void resizeMatrix()
    {
        int n=places.size();
        adjMatrix.resize(n, vector<int>(n,0));
    }

    int getIndex(const string& place)
    {
        return find(places.begin(),places.end(), place)-places.begin();
    }

    public:
        //function to add places
        void addPlace(string name, bool hasParking, int carSpots, int bikeSpots)
        {
            if(nodes.find(name)==nodes.end())
            {
            nodes[name]={name, hasParking, carSpots, bikeSpots};
            places.push_back(name);
            resizeMatrix();
            }
            else
            {
                cout<<"Place already exists.\n";
            }
        }

        //function to add connection between the places
        //This is not yet perfect
        void addConnection(string place1, string place2, int distance)
        {
            if(nodes.find(place1)==nodes.end() || nodes.find(place2)==nodes.end())
            {
                cout<<"One or both places do not exist.\n";
                return;
            }

            //Update adjacency list
            adjList[place1][place2]=distance;
            adjList[place2][place1]=distance;

            //Update adjacency matrix
            int idx1=getIndex(place1);
            int idx2=getIndex(place2);
            adjMatrix[idx1][idx2]=distance;
            adjMatrix[idx2][idx1]=distance;
        }

        //function to display the details of the city 
        void displayCity()
        {
            for(auto& [place, data]:nodes)
            {
                cout<<"Place: "<<place;
                if(data.hasParking)
                {
                    cout << " (Parking: Yes, Remaining Car Spots: " << data.carSpots-data.blockedCarSpots<< ", Remaining Bike Spots: " << data.bikeSpots-data.blockedBikeSpots << ")";
                }
                else
                {
                    cout<<" (Parking: No)";
                }
                cout<<"\nConnected to: ";
                if(adjList.find(place)!=adjList.end())
                {
                    for(const auto& neighbor : adjList[place])
                    {
                        cout<<neighbor.first<<" (Distance: "<<neighbor.second<<")";
                    }
                }
                else
                {
                    cout<<"No connections.";
                }
                cout<<endl<<endl;
            }
        }

        //Display city using BFS
        void displayCityBFS(const string &startCity)
        {
            if(nodes.find(startCity)==nodes.end())
            {
                cout<<"The starting city does not exist in the graph.\n";
                return;
            }

            unordered_map<string, bool> visited;
            queue<string> q;

            //start BFS from the given city
            q.push(startCity);
            visited[startCity]=true;

            cout<<"Cities in BFS order starting from "<<startCity<<":.\n";
            while(!q.empty())
            {
                string currentCity=q.front();
                q.pop();
                
                NODE& cityData = nodes[currentCity];
                cout<<"City: "<<currentCity;
                if(cityData.hasParking)
                {
                    cout<<"(Parking: Yes, Remaining Car Spots: )"<<cityData.carSpots-cityData.blockedCarSpots<< ", Remaining Bike Spots: " << cityData.bikeSpots - cityData.blockedBikeSpots << ")";
                }
                else
                {
                    cout << " (Parking: No)";
                }
                cout<<"\n";

                for(const auto& neighbor : adjList[currentCity])
                {
                    if(!visited[neighbor.first]){
                        visited[neighbor.first]=true;
                        q.push(neighbor.first);
                    }
                }
            }
            cout<<endl;
        }

        //this is the function where used to find the nearest parking spot
        //Here we are using DIJKSTRA'S algorithm as we are using priority queue
        //That is using min-heap 
        //time complexity of algorithm is O(V+ElogE) 
        //where V is number of nodes and E number of edges
        void findNearestParking(const string& currentLocation,bool searchCarSpot)
        {
            // here we check whether the given currentLocation is in the city or not
            if(nodes.find(currentLocation)==nodes.end())
            {
                cout<<"The current location does not exist in the city\n";
                return;
            }

            //Priority queue for dijkstra's algorithm :{distance, place}
            priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
            unordered_map<string, int> distances;
            unordered_map<string, bool> visited;

            //Initialize distance
            for(const auto& node:nodes)
            {
                distances[node.first]=INT_MAX;
            }
            distances[currentLocation]=0;
            pq.push({0,currentLocation});

            while(!pq.empty())
            {
                auto [currentDist,currentPlace]=pq.top();
                pq.pop();

                if(visited[currentPlace]) continue;
                visited[currentPlace]=true;

                //checking if currentPlace has parking spots available
                if(nodes[currentPlace].hasParking)
                {
                    if(searchCarSpot && nodes[currentPlace].carSpots>0)
                    {
                        cout <<"Nearest car parking found at: " << currentPlace <<" (Distance: " << currentDist << ")\n";
                        return;
                    }

                    if(!searchCarSpot && nodes[currentPlace].bikeSpots>0)
                    {
                        cout<<"Nearest bike parking found at: "<<currentPlace<<" (Distance: "<<currentDist<<")\n";
                        return;
                    }
                }

                for(const auto&[neighbor, distance]:adjList[currentPlace])
                {
                    if(!visited[neighbor]&&distances[neighbor]>currentDist+distance)
                    {
                        distances[neighbor]=currentDist+distance;
                        pq.push({distances[neighbor],neighbor});
                    }
                }
            }
            cout<<"No parking slot is available from the current location.\n";
        }

        //function to check whether the path exist between two points
        void checkPathExistence(const string& place1,const string& place2)
        {
            if(nodes.find(place1)==nodes.end() || nodes.find(place2)==nodes.end())
            {
                cout<<"One or both places do not exist.\n";
                return;
            }

            //warshall algorithm
            vector<vector<int>> closure=adjMatrix;
            int n=places.size();
            for(int k=0;k<n;k++)
            {
                for(int i=0;i<n;i++)
                {
                    for(int j=0;j<n;j++)
                    {
                        closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);
                    }
                }
            }

            int idx1=getIndex(place1);
            int idx2=getIndex(place2);
            if(closure[idx1][idx2]||closure[idx2][idx1])
            {
                cout<<"A path exist between "<<place1<<" and "<<place2<<".\n";
            }
            else
            {
                cout<<"No path exist between "<<place1<<" and "<<place2<<".\n";
            }
        }

        //function to block and unblock the parking spot
        void blockUnblockParkingSpot(const string &user,const string &place, bool isCarSpot, bool block)
        {
            if(nodes.find(place)==nodes.end())
            {
                cout<<"Place does not exist.\n";
                return;
            }

            NODE &node=nodes[place];
            if(!node.hasParking)
            {
                cout<<"This place does not have parking.\n";
                return;
            }

            if(block)
            {
                //logic to block the spot
                if(userBlockedSpot.find(user)!=userBlockedSpot.end())
                {
                    cout<<"User: "<<user<<" has already blocked a spot at "<<userBlockedSpot[user]<<".\n";
                    return;
                }

                if(isCarSpot)
                {
                    if(node.carSpots>node.blockedCarSpots)
                    {
                        node.blockedCarSpots++;
                        userBlockedSpot[user]=place;
                        cout<<"Car parking spot blocked at "<<place<<" by user "<<user<<".\n";
                    }
                    else
                    {
                        cout<<"No available car parking spots to block at "<<place<<".\n"; 
                    }
                }
                else
                {
                    if(node.bikeSpots>node.blockedBikeSpots)
                    {
                        node.blockedBikeSpots++;
                        userBlockedSpot[user]=place;
                        cout<<"Bike parking spot blocked at "<<place<<" by user "<<user<<".\n";
                    }
                    else
                    {
                        cout<<"No available car parking spots to block at "<<place<<".\n";
                    }
                }
            }
            else
            {
                //unblocking logic
                if(userBlockedSpot.find(user)==userBlockedSpot.end() || userBlockedSpot[user]!=place)
                {
                    cout<<"User "<<user<<" has not blocked any spot at "<<place<<".\n";
                    return;
                }

                if(isCarSpot)
                {
                    if(node.blockedCarSpots>0)
                    {
                        node.blockedCarSpots--;
                        userBlockedSpot.erase(user);
                        cout<<"Car parking spot unblocked at "<<place<<" by user "<<user<<".\n";
                    }
                    else
                    {
                        cout<<"No blocked car parking spots to unblock at "<<place<<".\n";
                    }
                }
                else
                {
                    if(node.blockedBikeSpots>0)
                    {
                        node.blockedBikeSpots--;
                        userBlockedSpot.erase(user);
                        cout<<"Bike parking spot unblocked at "<<place<<" by user "<<user<<".\n";
                    }
                    else
                    {
                        cout<<"No blocked car parking spots to unblock at "<<place<<".\n";
                    }
                }
            }
        }

        //function to sort the cities based on the remaining parking spots using insertion sort
        void sortCitiesByParking()
        {
            vector<pair<string, int>> parkingData;

            for(const auto& [place,node]:nodes)
            {
                if(node.hasParking)
                {
                    int remainingSpots=(node.carSpots-node.blockedCarSpots)+(node.bikeSpots-node.blockedBikeSpots);
                    parkingData.emplace_back(place, remainingSpots);
                }
                else
                {
                    parkingData.emplace_back(place,0);
                }
            }

            for(int i=1;i<parkingData.size();i++)
            {
                auto key = parkingData[i];
                int j=i-1;

                while(j>=0&&parkingData[j].second<key.second)
                {
                    parkingData[j+1]=parkingData[j];
                    --j;
                }
                parkingData[j+1]=key;
            }

            cout<<"Cities sorted by remaining parking spots.\n";
            for(const auto&[city,spots]: parkingData)
            {
                cout<<"City: "<<city<<" | Remaining parking spots: "<<spots<<endl;
            }
        }

        //KMP 
        void KMPsearch(const string& pattern, const string& text)
        {
            int m=pattern.size();
            int n=text.size();

            vector<int> lps(m,0);

            //LPS table creation
            int len,i;
            len=0;
            i=1;
            while(i<m)
            {
                if(pattern[i]==pattern[len])
                {
                    len++;
                    lps[i]=len;
                    i++;
                }
                else
                {
                    if(len!=0)
                    {
                        len=lps[len-1];
                    }
                    else
                    {
                        lps[i]=0;
                        i++;
                    }
                }
            }

            i=0;
            int j=0;
            bool found=false;
            while(i<n)
            {
                if(pattern[j]==text[i])
                {
                    i++;
                    j++;
                }

                if(j==m)
                {
                    cout<<"Found \""<<pattern<<"\" at index "<<i-j<<" in: \""<<text<<"\"\n";
                    j=lps[j-1];
                    found=true; 
                }
                else if(i<n && pattern[j]!=text[i])
                {
                    if(j!=0)
                    {
                        j=lps[j-1];
                    }
         
                    else
                    {
                        i++;
                    }
                }
            }

            if(!found)
            {
                cout<<"\""<<pattern<<"\" not found in: \""<<text<<"\"\n";
            }
        }

        void searchCityName(const string& cityName)
        {
            cout<<"Searching for \""<<cityName<<"\" in description\n";  

            for(const auto&[place,node]:nodes)
            {
                KMPsearch(cityName,place);
            }
        }

        //function to display the shortest path between all the points using Floyd's algorithm
        void floydWarshall()
        {
            int n=places.size();

            vector<vector<int>> dist(n,vector<int>(n,INT_MAX));

            for(int i=0;i<n;i++)
            {
                dist[i][i]=0;
            }

            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    if(adjMatrix[i][j]!=0)
                    {
                        dist[i][j]=adjMatrix[i][j];
                    }
                }
            }

            for(int k=0;k<n;k++)
            {
                for(int i=0;i<n;i++)
                {
                    for(int j=0;j<n;j++)
                    {
                        if(dist[i][k]!=INT_MAX && dist[k][j]!=INT_MAX)
                        {
                            dist[i][j]=min(dist[i][j],dist[i][k]+dist[k][j]);
                        }
                    }
                }
            }

            for (int i = 0; i < n; i++) 
            {
                for (int j = 0; j < n; j++) 
                {
                    if (dist[i][j] == INT_MAX) 
                    {
                        cout << "No path from " << places[i] << " to " << places[j] << endl;
                    } 
                    else 
                    {
                        cout << "Shortest distance from " << places[i] << " to " << places[j] << " is " << dist[i][j] << " km" << endl;
                    }
                }
            }
        }

};

//Function to check whether the user has enter correct valid bool value
bool getBooleanInput(string input1, string input2) 
{
    int input;
    cin >> input;
    while (input != 0 && input != 1) 
    {
        cout << "Invalid input! Please enter 1 for "<<input1<<" or 0 for "<<input2<<": ";
        cin >> input;
    }
    return input == 1;
}

int main(){
    CityGraph city;
    int choice;
    while(1)
    {
        cout<<"\n1-Add Place\n";
        cout<<"2-Connect Places\n";
        cout<<"3-Display city\n";
        cout<<"4-BFS traversal from any starting place.\n";
        cout<<"5-Find the nearest parking spot\n";
        cout<<"6-Check if path exist between two places.\n";
        cout<<"7-Block or unblock a parking spot.\n";
        cout<<"8-Sort cities by remaining parking spot.\n";
        cout<<"9-Search for a city name in descriptions or routes.\n";
        cout<<"10-Display shortest path between all the points.\n";
        cout<<"11-Exit\n";
        cout<<"Enter your choice:";
        cin>>choice;

        switch(choice)
        {
            case 1:
            {
                string name;
                bool hasParking;
                int carSpots, bikeSpots;
                cout<<"Enter the place name:";
                cin.ignore();//this is used to buffer because we are using getline to take
                            //input of string to handle the spaces
                getline(cin, name);
                cout<<"Does this place have parking?(1 for Yes and 0 for No):";
                hasParking=getBooleanInput("Yes","No");
                if(hasParking)
                {
                    cout<<"Enter the number of car spots:";
                    cin>>carSpots;
                    cout<<"Enter the number of bike spots:";
                    cin>>bikeSpots;
                }
                else
                {
                    carSpots=bikeSpots=0;
                }
                city.addPlace(name,hasParking,carSpots,bikeSpots);
                break;
            }
            case 2:
            {
                string place1,place2;
                int distance;
                cin.ignore();//same reason
                cout<<"Enter the first place:";
                getline(cin, place1);
                cout<<"Enter the second place:";
                getline(cin, place2);
                cout<<"Enter the distance(km/s):";
                cin>>distance;
                city.addConnection(place1,place2,distance);
                break;
            }
            case 3:
            {
                city.displayCity();
                break;
            }
            case 4:
            {
                string startCity;
                cin.ignore();
                cout<<"Enter the start city:";
                getline(cin,startCity);
                city.displayCityBFS(startCity);
                break;
            }
            case 5:
            {
                string currentLocation;
                bool searchCarSpot;
                cin.ignore();
                cout<<"Enter the currentLocation:";
                getline(cin,currentLocation);
                cout<<"Are you looking for the car spot or bike spot?(1 for car and 0 for bike):";
                searchCarSpot=getBooleanInput("car","bike");
                city.findNearestParking(currentLocation,searchCarSpot);
                break;
            }
            case 6:
            {
                string place1,place2;
                cin.ignore();
                cout<<"Enter the first place:";
                getline(cin,place1);
                cout<<"Enter the second place:";
                getline(cin,place2);
                city.checkPathExistence(place1,place2);
                break;
            }
            case 7:
            {
                string user,place;
                bool isCarSpot, block;
                cin.ignore();
                cout<<"Enter your name: ";
                getline(cin,user);
                cout<<"Enter the place name:";
                getline(cin,place);
                cout<<"Car spot or Bike spot?(1 for car, 0 for bike):";
                isCarSpot=getBooleanInput("car","bike");
                cout<<"Block or unblock?(1 from block, 0 for unblock):";
                block=getBooleanInput("block","unblock");
                city.blockUnblockParkingSpot(user,place,isCarSpot,block);
                break;
            }
            case 8:
            {
                city.sortCitiesByParking();
                break;
            }
            case 9:
            {
                string cityName;
                cin.ignore();
                cout<<"Enter the city name to search:";
                getline(cin,cityName);
                city.searchCityName(cityName);
                break;
            }
            case 10:
            {
                city.floydWarshall();
                break;
            }
            case 11:
            {
                cout<<"Exiting the program...\n";
                return 0;
            }
            default:
                cout<<"Invalid choice. Please try again\n";
        }
    }
}
/*
    TO RUN THE CODE
first execute the below command in termianl
    g++ -std=c++17 parking.cpp -o parking
then
    ./parking
*/