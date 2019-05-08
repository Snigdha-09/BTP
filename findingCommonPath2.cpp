#include<iostream>
#include<iterator>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<list>
#define INF 1000
#define N -1
using namespace std;

vector<vector<int> > findShortestPath(vector<vector<int> >, vector<vector<int> >, int);
void clusterPaths(vector<vector<int> >, vector<vector<int> >);
vector<int> findCommonElements(vector<int>, vector<int>);
vector< list<vector<int> > > totalCommonPaths;

void printVector(vector<int> v){
  for(int i=0;i<v.size();i++){
    cout<<v[i]<<" ";
  }
  cout<<endl;
}
int main()
{
  vector<vector<double> > distanceArray;
  vector<vector<int> > pathArray, EVSourceDestination, EVTraversalPathOrder, mainPaths;
  int noOfCoordinates, noOfEVs, mainPathsCount;

  //Finding number of coordinates/nodes in the city
  cout<<"Number of coordinates = ";
  cin>>noOfCoordinates;
  ifstream adjMatrixFile,pathMatrixFile;

  //Storing the data from distanceData file into distanceArray
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

  //Storing the data from pathData file into pathArray
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

  //Getting input data such as EV source and destination
/*
  cout<<"Number of main paths in the city = ";
  cin>>mainPathsCount;
  cout<<"Enter all the main paths in the order: count of nodes followed by all the nodes"<<endl;
  for(int i=0;i<mainPathsCount;i++){
    vector<int> tempVector;
    int n;
    cin>>n;
    for(int j=0;j<n;j++){
      int node = 0;
      cin>>node;
      tempVector.push_back(node);
    }
    mainPaths.push_back(tempVector);
    list <vector<int> > tempList;
    totalCommonPaths.push_back(tempList);
  } */

  mainPathsCount = 7;
  vector<int> delhi1{ 250, 249, 248, 247, 231, 230, 229, 227, 226, 225, 224, 222, 269, 268, 271, 295, 296, 297, 345, 275, 276 };
  vector<int> delhi2{340,338,337,336,343,319,315,314,313,312,311,309,308,307,304,303,301};
  vector<int> delhi3{368,367,366,365,363,369,290,289,288,287,271,272,273,221,220,82,81,80,69,70};
  vector<int> delhi4{218,219,220,221,222,223,209};
  vector<int> delhi5{60,61,62,63,66,67,83,84,85,214,213,212,211,217,216,136,137,204,179,178,139,104,105,106,107,108,109,110,111,9,8,7,6,4,3,2,1};
  vector<int> delhi6{46,45,44,43,30,31,33,32,17,16,15,14,96,97};
  vector<int> delhi7{183,184,185,186,187,188,172,170,171,144,146,164,163,162,161,160,159,158,156,246,245,244,243,242,241,240,239};
  for(int i=0;i<7;i++){
    list <vector<int> > tempList;
    totalCommonPaths.push_back(tempList);
  }

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

  //finding shortest path taken by each EV given the source and destination
  vector<vector<int> > EVTraversalPath = findShortestPath(EVSourceDestination, pathArray, noOfEVs);
  for(vector<vector<int> >::iterator it = EVTraversalPath.begin();it!=EVTraversalPath.end();it++){
    for (vector<int>::iterator i = (*it).begin();i!=(*it).end();i++){
      cout<<*i<<" <- ";
    }
    cout<<"start"<<endl;
  }

  //clustering the EV paths for scheduling the mobile charging stations
  clusterPaths(mainPaths, EVTraversalPath);
  cout<<"final clusters: "<<endl;
  for(vector<list<vector<int> > >::iterator itr = totalCommonPaths.begin();itr!=totalCommonPaths.end();itr++){
    for(list<vector<int> >::iterator itr1 = (*itr).begin();itr1!=(*itr).end();++itr1){
      for(vector<int>::iterator itr2 = (*itr1).begin();itr2!=(*itr1).end();itr2++){
        cout<<*itr2<<" ";
      }
      cout<<"     ";
    }
    cout<<endl;
  }
  /*for(vector<vector<int> >::iterator it = pathClusters.begin();it!=pathClusters.end();it++){
    for (vector<int>::iterator i = (*it).begin();i!=(*it).end();i++){
      cout<<*i<<"  ";
    }
    cout<<endl;
  }*/
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

void clusterPaths(vector<vector<int> > mainPaths, vector<vector<int> > traversalPaths){
  for(int i = 0;i<traversalPaths.size();i++){
    vector<int> commonNodesVector;
    int clusterIndex, maxCommonNodes=1;
    bool belongsToMainPath = false;
    for(int j=0;j<mainPaths.size();j++){
      vector<int> v1 = findCommonElements(traversalPaths[i],mainPaths[j]);
      if(v1.size()>maxCommonNodes){
        belongsToMainPath = true;
        maxCommonNodes = v1.size();
        commonNodesVector = v1;
        clusterIndex = j;
      }
    }
    if(belongsToMainPath == false){
      mainPaths.push_back(traversalPaths[i]);
      clusterIndex = mainPaths.size()-1;
      commonNodesVector = traversalPaths[i];
      list <vector<int> > tempList;
      totalCommonPaths.push_back(tempList);
    }
    if(totalCommonPaths[clusterIndex].begin() == totalCommonPaths[clusterIndex].end()){
      totalCommonPaths[clusterIndex].push_back(commonNodesVector);
    }else{
      for(list<vector<int> >::iterator iter = totalCommonPaths[clusterIndex].begin();iter!=totalCommonPaths[clusterIndex].end();iter++){
        vector<int> v2 = findCommonElements(*iter, commonNodesVector);
        if(v2.empty() && *iter != totalCommonPaths[clusterIndex].back()){
          continue;
        }else if(v2.empty() && *iter == totalCommonPaths[clusterIndex].back()){
          totalCommonPaths[clusterIndex].push_back(commonNodesVector);
        }else{
          *iter = v2;
        }
      }
    }
  }
}

/*vector<vector<int> > clusterPaths(vector<vector<int> > mainPaths, vector<vector<int> > traversalPaths){
  vector<vector<int> > pathClusters;
  for(int i = 0;i<traversalPaths.size();i++){
    vector<int> commonNodesVector, tempVector;
    int clusterIndex, maxCommonNodes;
    for(int j=0;j<mainPaths.size();j++){
      vector<int> v1 = findCommonElements(traversalPaths[i],mainPaths[j]);
      if(j==0){
        maxCommonNodes = v1.size();
        commonNodesVector = v1;
        clusterIndex = j;
      }
      else if(v1.size()>maxCommonNodes){
        maxCommonNodes = v1.size();
        commonNodesVector = v1;
        clusterIndex = j;
      }
    }
    tempVector.push_back(clusterIndex);
    for(int j=0;j<commonNodesVector.size();j++){
      tempVector.push_back(commonNodesVector[j]);
    }
    pathClusters.push_back(tempVector);
  }
  return pathClusters;
}*/

vector<int> findCommonElements(vector<int> a, vector<int> b){
  vector<int> result;
  for(vector<int>::iterator it1 = a.begin();it1!=a.end();it1++){
    if(std::find(b.begin(),b.end(),*it1) != b.end())
      result.push_back(*it1);
  }
  return result;
}
