#include <bits/stdc++.h>
#include <iostream>
using namespace std;

//This the definition of the structure
struct node{
    string name;
    bool hasParking;
    int carSpots;
    int bikeSpots;
};
typedef node NODE;

class CityGraph{
    unordered_map<string, NODE> nodes;
    unordered_map<string, unordered_map<string, int>> adjList;

    public:
        //function to add places
        void addPlace(string name, bool hasParking, int carSpots, int bikeSpots){
            nodes[name]={name, hasParking, carSpots, bikeSpots};
        }

        //function to add connection between the places
        //This is not yet perfect
        void addConnection(string place1, string place2, int distance){
            adjList[place1][place2]=distance;
            adjList[place2][place1]=distance;
        }

        //function to display the details of the city 
        void displayCity(){
            for(auto& [place, data]:nodes){
                cout<<"Place: "<<place;
                if(data.hasParking){
                    cout << " (Parking: Yes, Car Spots: " << data.carSpots<< ", Bike Spots: " << data.bikeSpots << ")";
                }
                else{
                    cout<<" (Parking: No)";
                }
                cout<<"\nConnected to: ";
                if(adjList.find(place)!=adjList.end()){
                    for(const auto& neighbor : adjList[place]){
                        cout<<neighbor.first<<" (Distance: "<<neighbor.second<<")";
                    }
                }
                else{
                    cout<<"No connections.";
                }
                cout<<endl<<endl;
            }
        }

        //this is the function where used to find the nearest parking spot
        //Here we are using DIJKSTRA'S algorithm as we are using priority queue
        //That is using min-heap 
        //time complexity of algorithm is O(V+ElogE) 
        //where V is number of nodes and E number of edges
        void findNearestParking(const string& currentLocation,bool searchCarSpot){
            // here we check whether the given currentLocation is in the city or not
            if(nodes.find(currentLocation)==nodes.end()){
                cout<<"The current location does not exist in the city\n";
                return;
            }

            //Priority queue for dijkstra's algorithm :{distance, place}
            priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
            unordered_map<string, int> distances;
            unordered_map<string, bool> visited;

            //Initialize distance
            for(const auto& node:nodes){
                distances[node.first]=INT_MAX;
            }
            distances[currentLocation]=0;
            pq.push({0,currentLocation});

            while(!pq.empty()){
                auto [currentDist,currentPlace]=pq.top();
                pq.pop();

                if(visited[currentPlace]) continue;
                visited[currentPlace]=true;

                //checking if currentPlace has parking spots available
                if(nodes[currentPlace].hasParking){
                    if(searchCarSpot && nodes[currentPlace].carSpots>0){
                        cout <<"Nearest car parking found at: " << currentPlace <<" (Distance: " << currentDist << ")\n";
                        return;
                    }

                    if(!searchCarSpot && nodes[currentPlace].bikeSpots>0){
                        cout<<"Nearest bike parking found at: "<<currentPlace<<" (Distance: "<<currentDist<<")\n";
                        return;
                    }
                }

                for(const auto&[neighbor, distance]:adjList[currentPlace]){
                    if(!visited[neighbor]&&distances[neighbor]>currentDist+distance){
                        distances[neighbor]=currentDist+distance;
                        pq.push({distances[neighbor],neighbor});
                    }
                }
            }
            cout<<"No parking slot is available from the current location.\n";
        }
};

int main(){
    CityGraph city;
    int choice;
    while(1){
        cout<<"1-Add Place\n";
        cout<<"2-Connect Places\n";
        cout<<"3-Display city\n";
        cout<<"4-Find the nearest parking spot\n";
        cout<<"5-Exit\n";
        cout<<"Enter your choice:";
        cin>>choice;

        switch(choice){
            case 1:{
                string name;
                bool hasParking;
                int carSpots, bikeSpots;
                cout<<"Enter the place name:";
                cin.ignore();//this is used to buffer because we are using getline to take
                            //input of string to handle the spaces
                getline(cin, name);
                cout<<"Does this place have parking?(1 for Yes and 0 for No):";
                cin>>hasParking;
                if(hasParking){
                    cout<<"Enter the number of car spots:";
                    cin>>carSpots;
                    cout<<"Enter the number of bike spots:";
                    cin>>bikeSpots;
                }
                else{
                    carSpots=bikeSpots=0;
                }
                city.addPlace(name,hasParking,carSpots,bikeSpots);
                break;
            }
            case 2:{
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
            case 3:{
                city.displayCity();
                break;
            }
            case 4:{
                string currentLocation;
                bool searchCarSpot;
                cin.ignore();
                cout<<"Enter the currentLocation:";
                getline(cin,currentLocation);
                cout<<"Are you looking for the car spot or bike spot?(1 for car and 0 for bike):";
                cin>>searchCarSpot;
                city.findNearestParking(currentLocation,searchCarSpot);
                break;
            }
            case 5:{
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
