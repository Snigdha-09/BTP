#include<iostream>
#include<map>
#include<iterator>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#define INF 1000
#define N -1
#define noOfCoordinates 378
using namespace std;

void getHamiltonianPath(vector<vector<double> > distanceSubArray);
int findNearestNode(vector<double> distanceArray, int nodeIndex);
vector<int> getCompleteRoute(vector<vector<int> > pathArray);
int calculateTotalDistance(vector<vector<double> > distanceSubArray);
map <int,int> nodeOrderMap,realNodeMapping;
int noOfSpecialNodes = 0;

int main()
{
    vector<vector<double> > distanceArray, distanceSubArray;
    vector<vector<int> > pathArray;
    vector<int> traversalPathOrder;

    ifstream adjMatrixFile, adjSubMatrixFile,specialNodePointsFile,pathMatrixFile;
    adjMatrixFile.open("distanceData.txt");
    if(adjMatrixFile.is_open()){
        for(int i=0;i<noOfCoordinates;i++){
            vector<double> tempVector;
            for(int j=0;j<noOfCoordinates;j++){
                string temp;
                adjMatrixFile>>temp;
                if(temp == "INF")
                    tempVector.push_back(INF);
                else tempVector.push_back(std::stod(temp));
            }
            distanceArray.push_back(tempVector);
        }
        /*for(int i=0;i<noOfCoordinates;i++){
            for(int j=0;j<noOfCoordinates;j++){
                cout<<distanceArray[i][j]<<"\t";
            }
            cout<<endl;
        }
        cout<<endl; */
    }else cout<<"Couldn't open adjacency matrix file of the main graph"<<endl;

    pathMatrixFile.open("pathData.txt");
    if(pathMatrixFile.is_open()){
        for(int i=0;i<noOfCoordinates;i++){
            vector<int> tempVector;
            for(int j=0;j<noOfCoordinates;j++){
                string temp;
                pathMatrixFile>>temp;
                if(temp == "N")
                    tempVector.push_back(N);
                else tempVector.push_back(std::stoi(temp));
            }
            pathArray.push_back(tempVector);
        }
    } else cout<<"Couldn't open path matrix file of the main graph"<<endl;
    /*for(int i=0;i<noOfCoordinates;i++){
        for(int j=0;j<noOfCoordinates;j++){
            cout<<pathArray[i][j]<<"\t";
        }
        cout<<endl;
    } */

    specialNodePointsFile.open("delhiEVNodes.txt");
    if(specialNodePointsFile.is_open()){
        int i=0;
        while(true){
            if(specialNodePointsFile.eof())
                break;
            specialNodePointsFile>>realNodeMapping[i];
            i++;
        }
        noOfSpecialNodes = i-1;
        cout<<endl<<"Total number of junctions in the city where EVs are stationed = "<<noOfSpecialNodes<<endl<<endl;
    }else cout<<"Couldn't open the special node points file"<<endl;
    for(int i = 0; i < noOfSpecialNodes; i++)
        nodeOrderMap.insert(pair <int,int> (i,i));

    for(int i=0;i<noOfSpecialNodes;i++){
        vector<double> tempVector;
        for(int j=0;j<noOfSpecialNodes;j++)
            tempVector.push_back(distanceArray[realNodeMapping[i]-1][realNodeMapping[j]-1]);
        distanceSubArray.push_back(tempVector);
    }
    /*for(int i=0;i<noOfSpecialNodes;i++){
        for(int j=0;j<noOfSpecialNodes;j++){
            cout<<distanceSubArray[i][j]<<"\t";
        }
        cout<<endl;
    }   */

    getHamiltonianPath(distanceSubArray);
    cout<<"Optimal route(only through EVs) to be followed by an MCV is : "<<endl;
    for(int i=0;i<noOfSpecialNodes;i++)
        cout<<realNodeMapping[nodeOrderMap[i]]<<" -> ";
    cout<<"CS"<<endl<<endl;
    traversalPathOrder = getCompleteRoute(pathArray);
    cout<<"The complete route to be taken by the MCS is:"<<endl;
    for(vector<int>::iterator itr = traversalPathOrder.begin();itr!=traversalPathOrder.end();++itr)
        cout<<*itr<<" -> ";
    cout<<"CS"<<endl<<endl<<"Total distance to be travelled by an MCV = "<<calculateTotalDistance(distanceSubArray)<<endl;
    return 0;
}

int calculateTotalDistance(vector<vector<double> > distanceSubArray){
    int start = nodeOrderMap[0],totalDistance = 0;
    for(int i=1;i<noOfSpecialNodes;i++){
        totalDistance += (distanceSubArray[start][nodeOrderMap[i]] != INF) ? (distanceSubArray[start][nodeOrderMap[i]]):0;
        start = nodeOrderMap[i];
    }
    return totalDistance;
}

vector<int> getCompleteRoute(vector<vector<int> > pathArray){
    vector<int> route;
    int previous, middle, last = realNodeMapping[nodeOrderMap[noOfSpecialNodes-1]];
    route.push_back(last);
    for(int i=noOfSpecialNodes-2;i>=0;i--){
        previous = realNodeMapping[nodeOrderMap[i]];
        do{
            middle=pathArray[previous-1][last-1];
            if(middle == -1){
                route.push_back(previous);
                last = previous;
                break;
            }
            route.push_back(middle);
            last = middle;
        }while(previous!=middle);
    }
    std::reverse(route.begin(),route.end());
    return route;
}

int findNearestNode(vector<double> distanceArray, int nodeIndex){
    int nearestNodeIndex = nodeIndex+2;
    for(int j=nodeIndex+3;j<noOfSpecialNodes;j++)
        if(distanceArray[nodeOrderMap[j]]<distanceArray[nodeOrderMap[nearestNodeIndex]])
            nearestNodeIndex = j;
    return nearestNodeIndex;
}

void getHamiltonianPath(vector<vector<double> > distanceSubArray){
    int minIndex = 0;
    for(int i=0;i<noOfSpecialNodes-2;i++){
        minIndex = findNearestNode(distanceSubArray[nodeOrderMap[i]], i);
        if(distanceSubArray[nodeOrderMap[i]][nodeOrderMap[i+1]] > distanceSubArray[nodeOrderMap[i]][nodeOrderMap[minIndex]]){
            swap(nodeOrderMap[i+1],nodeOrderMap[minIndex]);
        }
    }
}
