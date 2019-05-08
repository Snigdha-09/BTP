#include<iostream>
#include<iterator>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#define INF 1000
#define N -1
using namespace std;

vector<vector<int> > findShortestPath(vector<vector<int> >, vector<vector<int> >, int);
vector<vector<int> > clusterPaths(vector<vector<int> >);

int main()
{
  vector<vector<double> > distanceArray;
  vector<vector<int> > pathArray, EVSourceDestination, EVTraversalPathOrder;
  int noOfCoordinates, noOfEVs;

  cout<<"Number of coordinates = ";
  cin>>noOfCoordinates;
  ifstream adjMatrixFile,pathMatrixFile;
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
      /*
      for(int i=0;i<noOfCoordinates;i++){
          for(int j=0;j<noOfCoordinates;j++){
              cout<<distanceArray[i][j]<<"\t";
          }
          cout<<endl;
      }
      cout<<endl;
      */
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
      /*
      for(int i=0;i<noOfCoordinates;i++){
          for(int j=0;j<noOfCoordinates;j++){
              cout<<pathArray[i][j]<<"\t";
          }
          cout<<endl;
      }
      */
  } else cout<<"Couldn't open path matrix file of the main graph"<<endl;

  cout<<"Number of EVs = ";
  cin>>noOfEVs;
  cout<<"Enter the source and destination for each EV"<<endl;
  for(int i=0;i<noOfEVs;i++){
    vector<int> tempVector;
    for(int j=1;j<=2;j++){
      int nodeVariable = 0;
      cin>>nodeVariable;
      tempVector.push_back(nodeVariable);
    }
    EVSourceDestination.push_back(tempVector);
  }
  /*
  for(int i=0;i<noOfEVs;i++){
    cout<<EVSourceDestination[i][0]<<" "<<EVSourceDestination[i][1]<<endl;
  }
  */
  vector<vector<int> > EVTraversalPath = findShortestPath(EVSourceDestination, pathArray, noOfEVs);
  for(vector<vector<int> >::iterator it = EVTraversalPath.begin();it!=EVTraversalPath.end();it++){
    for (vector<int>::iterator i = (*it).begin();i!=(*it).end();i++){
      cout<<*i<<" <- ";
    }
    cout<<"start"<<endl;
  }
  vector<vector<int> > pathClusters = clusterPaths(EVTraversalPath);
  for(vector<vector<int> >::iterator it = pathClusters.begin();it!=pathClusters.end();it++){
    for (vector<int>::iterator i = (*it).begin();i!=(*it).end();i++){
      cout<<*i<<"  ";
    }
    cout<<endl;
  }
  return 0;
}

vector<vector<int> > findShortestPath(vector<vector<int> > SourceDestinationArray, vector<vector<int> > pathArray, int noOfEVs){
  vector<vector<int> > traversalPath;
  for(int i=0;i<noOfEVs;i++){
    vector<int> tempVector;
    int first = SourceDestinationArray[i][0];
    int last = SourceDestinationArray[i][1], middle = last;
    tempVector.push_back(last);
    while(first!=middle){
        middle = pathArray[first][last];
        last = middle;
        tempVector.push_back(middle);
    }
    traversalPath.push_back(tempVector);
  }
  return traversalPath;
}

vector<vector<int> > clusterPaths(vector<vector<int> > traversalPaths){
  vector<vector<int> > pathClusters;
  vector<int> totalPairedPaths;
  for(int i=0;i<traversalPaths.size();i++){
    if(std::find(totalPairedPaths.begin(),totalPairedPaths.end(),i) != totalPairedPaths.end())
      continue;
    vector<int> singleCluster, pairedPaths;
    totalPairedPaths.push_back(i);
    int maxi=1,clusterCount=0;
    for(int j=i+1;j<traversalPaths.size();j++){
      if(std::find(totalPairedPaths.begin(),totalPairedPaths.end(),j) != totalPairedPaths.end())
        continue;
      int nodeCount=0;
      for (vector<int>::iterator it = traversalPaths[i].begin();it!=traversalPaths[i].end();it++){
        vector<int>::iterator index = std::find (traversalPaths[j].begin(), traversalPaths[j].end(), *it);
        if (index != traversalPaths[j].end())
          nodeCount++;
      }
      if(nodeCount>maxi){
        if(clusterCount==0){
          clusterCount = 1;
          singleCluster.push_back(j);
          maxi = nodeCount;
          pairedPaths.push_back(j);
        }else{
          singleCluster.clear();
          singleCluster.push_back(j);
          maxi = nodeCount;
          pairedPaths.clear();
          pairedPaths.push_back(j);
        }
      }else if(nodeCount == maxi && clusterCount==0){
          singleCluster.push_back(j);
          pairedPaths.push_back(j);
      }
    }
    for(int k=0;k<pairedPaths.size();k++)
      totalPairedPaths.push_back(pairedPaths[k]);
    singleCluster.push_back(i);
    pathClusters.push_back(singleCluster);
  }
  return pathClusters;
}
