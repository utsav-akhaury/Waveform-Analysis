#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <TMath.h>
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
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
 
double Sum(vector<double> ampl_list, int index){
//This function returns the sum of elements in ampl_list till index, if index==0 sums the entire ampl_list
//Note that there is a huge difference between index=0 and index=1, but not so much between index=1 and index=2!
	int count;
	double sum=0;
	if(index==0) count =ampl_list.size();
	else count = index;
	for(int i=0; i < count; i++){
		sum+=ampl_list[i];
	}
	return sum;
}

double Peak(vector<double> ampl_list, int p_n){
//This function returns the peak value of the elements in the ampl_list.
//p_n==0 looks for positive peak, p_n==1  looks for negative peak.
	double peak=ampl_list[0];
	if(p_n==0){
	for(int i=1; i < ampl_list.size(); i++){//Note that this loops starts from the 2nd element.
		if(peak<ampl_list[i]) peak=ampl_list[i];
	}
	}
	if(p_n==1){
	for(int i=1; i < ampl_list.size(); i++){//Note that this loops starts from the 2nd element.
		if(peak>ampl_list[i]) peak=ampl_list[i];
	}
	}
	return peak;
}

double Charge_ped(vector<double> ampl_list, double delta_t, int pedestal_index, double resistance){
//Integrate, subtract pedestal//
//This function outputs the charge (integrate the voltage, subtracts the pedestal).
//If pedestal_index==0, pedastal is not subtracted.
	double charge=0, pedestal=0;
	charge=Sum(ampl_list,0);
	if(pedestal_index!=0){
		pedestal=Sum(ampl_list,pedestal_index);
		pedestal=pedestal*float(ampl_list.size())/float(pedestal_index);
		charge=charge-pedestal;
	}
	charge=charge*delta_t/resistance;
	return charge;
}

int Thr_x_ind(vector<double> ampl_list, float threshold){
//Threshold Crossing Index//
//This function extracts timing information based on leading edge discrimination.
	int index=33333;//This value is returned if the signal never crosses the threshold.
	for(int i=0; i < ampl_list.size(); i++){
   		if(ampl_list[i]<threshold){
   			index=i;
   			break;
   		}
	}
	return index;
}

string F_w_p(string path, string prefix, int i, int ch_no){
	//Code to get the filename
	string file_with_path;
	if(i<10) file_with_path= path + "C"+to_string(ch_no)+prefix+"0000"+to_string(i)+".csv";
	else if(i<100) file_with_path= path + "C"+to_string(ch_no)+prefix+"000"+to_string(i)+".csv";
	else if(i<1000) file_with_path= path + "C"+to_string(ch_no)+prefix+"00"+to_string(i)+".csv";
	else if(i<10000) file_with_path= path + "C"+to_string(ch_no)+prefix+"0"+to_string(i)+".csv";
	else file_with_path= path + "C"+to_string(ch_no)+prefix+to_string(i)+".csv";
	return file_with_path;
}

