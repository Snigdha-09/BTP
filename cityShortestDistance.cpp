#include<iostream>
#include<cmath>
#include<utility>
#include<fstream>
#include<vector>
#define INF 100000000000
#define N -1
using namespace std;

class CityData{
	public:
	vector<pair<double,double> > coordinates;
	vector<pair<int,int> > edges;
	vector<vector<double> > distanceArray;
	vector<vector<double> > pathArray;
	CityData(string pointsFile, string edgesFile);
	void getShortestDistances();
	void printDistanceArrayToFile();
	void printPathArrayToFile();
	private: int noOfCoordinates;
	double getDistance(int endpoint1, int endpoint2);
	void getCityData(string fileName, int inputDataType);
	void printDataToFile(vector<vector<double> > array,string fileName);
};

CityData::CityData(string pointsFileName, string edgesFileName)
{
	this->getCityData(pointsFileName,0);
	this->getCityData(edgesFileName,1);
	this->noOfCoordinates = coordinates.size();
	for(int i=0;i<noOfCoordinates;i++){
		vector<double> temp1,temp2;
		for(int j=0;j<noOfCoordinates;j++){
			i==j? temp1.push_back(0):temp1.push_back(INF);
			temp2.push_back(N);
		}
		distanceArray.push_back(temp1);
		pathArray.push_back(temp2);
	}

	for(int i=0;i<edges.size();i++){
		double distance = this->getDistance(edges[i].first,edges[i].second);
		distanceArray[edges[i].first-1][edges[i].second-1] = distance;
		distanceArray[edges[i].second-1][edges[i].first-1] = distance;
		pathArray[edges[i].first-1][edges[i].second-1] = edges[i].first-1;
		pathArray[edges[i].second-1][edges[i].first-1] = edges[i].second-1;
	}
}

void CityData::getCityData(string fileName, int inputDataType){
	double endPoint1, endPoint2;
	ifstream dataFile;
	dataFile.open(fileName.c_str());
	if(dataFile.is_open()){
		while (true){
			dataFile>>endPoint1;
			if(dataFile.eof())
				break;
			dataFile>>endPoint2;
			if(inputDataType == 0)
				coordinates.push_back(make_pair(endPoint1, endPoint2));
			else
				edges.push_back(make_pair(endPoint1, endPoint2));
		}
	}else cout<<"Couldn't open the file"<<endl;
}

void CityData::getShortestDistances(){
	for(int k=0;k<noOfCoordinates;k++)
		for(int i=0;i<noOfCoordinates;i++)
			for(int j=0;j<noOfCoordinates;j++){
				if(distanceArray[i][k] == INF || distanceArray[k][j] == INF)
					continue;
				if(distanceArray[i][j]>distanceArray[i][k]+distanceArray[k][j]){
					distanceArray[i][j] = distanceArray[i][k] + distanceArray[k][j];
					pathArray[i][j] = pathArray[k][j];
				}
			}
}

void CityData::printDataToFile(vector<vector<double> > array,string fileName){
	ofstream datafile;
	datafile.open(fileName.c_str());
	for(int i=0;i<noOfCoordinates;i++){
		for(int j=0;j<noOfCoordinates;j++){
			array[i][j]==INF ? (datafile<<"INF\t"):(array[i][j]==N ? (datafile<<"N\t"):(datafile<<array[i][j]<<"\t"));
		}
		datafile<<endl;
	}
}

void CityData::printDistanceArrayToFile(){
	string fileName = "distanceData.txt";
	this->printDataToFile(distanceArray,fileName);
}

void CityData::printPathArrayToFile(){
	string fileName = "pathData.txt";
	this->printDataToFile(pathArray,fileName);
}

double CityData::getDistance(int endpoint1, int endpoint2){
	return sqrt(pow((coordinates[endpoint2].first-coordinates[endpoint1].first),2)+pow((coordinates[endpoint2].second-coordinates[endpoint1].second),2));
}

int main()
{
	string pointsFileName, edgesFileName;
	cout<<"Enter the names of the points and edges files"<<endl;
	cin>>pointsFileName>>edgesFileName;
	CityData newCity(pointsFileName,edgesFileName);
	newCity.getShortestDistances();
	newCity.printDistanceArrayToFile();
	newCity.printPathArrayToFile();
	return 0;
}
