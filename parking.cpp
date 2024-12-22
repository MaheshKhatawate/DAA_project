#include <bits/stdc++.h>
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
};

int main(){
    CityGraph city;
    int choice;
    while(1){
        cout<<"1-Add Place\n";
        cout<<"2-Connect Places\n";
        cout<<"3-Display city\n";
        cout<<"4-Exit\n";
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