#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <TMath.h>
#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "TLatex.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include <TStyle.h>
#include <TGraph2D.h>
#include <TRandom.h>
#include <TF2.h>
#include <TH1.h>
#include <string>
#include <vector>
#include <sstream> //istringstream

using namespace std;
/*
 * Reads csv file into table, exported as a vector of vector of doubles.
 * @param inputFileName input file name (full path).
 * @return data as vector of vector of doubles.
 */ 

//______________________________________________________________________________________________________________


/*
 * A class to read data from a csv file.
 */
class CSVReader
{
	std::string fileName;
	std::string delimiter;
 
public:
	CSVReader(std::string filename, std::string delm = ",") :
			fileName(filename), delimiter(delm)
	{ }
 
	// Function to fetch data from a CSV File
	void getData(vector<double> &timestamps, vector<double> &amplitude_list);
};
 
void CSVReader::getData(vector<double> &timestamps, vector<double> &amplitude_list) {
	std::ifstream file(fileName);
	int i=0;
	double time,amplitude;
	;
	
	std::string line = "";
	std::string::size_type sz;
	while (getline(file, line,',')){
		if(i>4){
		time=std::stod (line,&sz);;
		//std::cout << "Value1(" << time << ")\t";   //Comment/Uncomment to print values
		}
		getline(file, line,'\n');
		if(i>4){
		amplitude=std::stod (line,&sz);;
		//std::cout << "Value2(" << amplitude << ")\n";   //Comment/Uncomment to print values
		timestamps.push_back(time);
		amplitude_list.push_back(amplitude);
		}
	i=i+1;
	}
}





