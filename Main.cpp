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
#include "CSV_Vector.h"
#include "Waveform_Analysis.h"

int main(){

int nofpulses = 12809;
string path = "/Enter path here";
string outfolder = "/Enter outfolder here";

vector<double> ctr_vect;
vector<double> ctr_cfd_vect;
vector<double> charge_c3_vect;
vector<double> charge_c4_vect;
vector<double> peak_c3_vect;
vector<double> peak_c4_vect;

for(int i=0;i<nofpulses;i++){ //This loops through each pulse.

	string file_with_path;
	/*Reading the Channel 4 and storing into timestamps_c4, amplitude_list_c4 */
	file_with_path=F_w_p(path, "rawpulse", i, 4);
	//cout<<"Channel 4 File :"<<file_with_path<<"\n";   //Uncomment to print out the file names used; 
	CSVReader readerc4(file_with_path);
	vector<double> timestamps_c4, amplitude_list_c4;
	readerc4.getData(timestamps_c4,amplitude_list_c4);
	//cout<< "Number of Samples: " <<timestamps_c4.size() <<"\n";   //Uncomment to print out the number of time points. 

	/*Reading the Channel 3 and storing into timestamps_c3, amplitude_list_c3*/
	file_with_path=F_w_p(path, "rawpulse", i, 3);
	cout<<"Channel 3 File: "<<file_with_path<<"\n"; //Uncomment to print out the file names used;
	CSVReader readerc3(file_with_path);
	vector<double> timestamps_c3, amplitude_list_c3;
	readerc3.getData(timestamps_c3,amplitude_list_c3);
	//cout<< "Number of Samples: " <<timestamps_c3.size() <<"\n"; //Uncomment to print out the number of time points.

	//Peak detecting C3 and C4
	double peak_c4=Peak(amplitude_list_c4, 1);
	double peak_c3=Peak(amplitude_list_c3, 1);
	cout<<"pulse n: "<< peak_c3<<"  "<< peak_c4<<"\n";

	if(peak_c3<-0.005 && peak_c4<-0.005){
		peak_c3_vect.push_back(peak_c3);
		peak_c4_vect.push_back(peak_c4);
		cout<<"\n ble \n";
		/*Leading Edge Discriminating C3 and C4 with CFD and filling into histogram ctr_hist*/
		int time_index_c4=Thr_x_ind(amplitude_list_c4, peak_c4*0.5);
		int time_index_c3=Thr_x_ind(amplitude_list_c3, peak_c3*0.5);	
		cout<<time_index_c4-time_index_c3;
		ctr_cfd_vect.push_back(timestamps_c4[time_index_c4]-timestamps_c3[time_index_c3]);
		
		/*Leading Edge Discriminating C3 and C4 with fixed threshold and filling into histogram ctr_hist*/
		time_index_c4=Thr_x_ind(amplitude_list_c4, -0.003);
		time_index_c3=Thr_x_ind(amplitude_list_c3, -0.003);
		ctr_vect.push_back(timestamps_c4[time_index_c4]-timestamps_c3[time_index_c3]);

		/*Calling the Charge_ped function to get the charge of C3 and C4*/
		double delta_t=timestamps_c4[5]-timestamps_c4[4];//Just find delta_t by taking difference of any adjacent elements in the timestamps_cn
		charge_c3_vect.push_back(Charge_ped(amplitude_list_c3, delta_t, 0, 50.00));
		charge_c4_vect.push_back(Charge_ped(amplitude_list_c4, delta_t, 0, 50.00));
		//delta_t is 5e-11 in the sep29data
	}
	
	else cout<<"At least one of the two signals did not cross the given threshold.";
	
	/*Plotting the waveforms and saving into separate files
	TCanvas *c1 = new TCanvas("c1","Pulse",0,0,600,400);
	c1->cd();
	//Plotting Channel 4
	TGraph *ch4= new TGraph();
	for(int i=0; i < timestamps_c4.size(); i++){
   		ch4->SetPoint(i,timestamps_c4[i],amplitude_list_c4[i]);
	}
	ch4->Draw("");
	//Plotting Channel 3
	TGraph *ch3= new TGraph();
	for(int i=0; i < timestamps_c3.size(); i++){
   		ch3->SetPoint(i,timestamps_c3[i],amplitude_list_c3[i]);
	}
	ch3->SetLineColor(kBlue);
	ch3->Draw("same");
		
	string outname;
	if(peak_c3<-0.003 && peak_c4<-0.003) outname=outfolder+to_string(i)+".png";
	else outname=outfolder+"didnttrigger/"+to_string(i)+".png";
	c1->SaveAs(outname.c_str());
	/**/
	
}

float ctr_mean=Sum(ctr_vect,0)/ctr_vect.size();
float charge_c3_mean=Sum(charge_c3_vect,0)/charge_c3_vect.size();
float charge_c4_mean=Sum(charge_c4_vect,0)/charge_c4_vect.size();
float peak_c3_mean=Sum(peak_c3_vect,0)/peak_c3_vect.size();
float peak_c4_mean=Sum(peak_c4_vect,0)/peak_c4_vect.size();

TH1F *ctr_hist = new TH1F("Timing_Resolution", "; x}; #occurences", 100, ctr_mean-5E-9, ctr_mean+5E-9); //Histogram to store timing differences (Mean+-5ns)
TH1F *ctr_cfd_hist = new TH1F("Timing_Resolution - CFD", "; x}; #occurences", 100, ctr_mean-5E-9, ctr_mean+5E-9); //Histogram to store timing differences with CFD (Mean+-5ns)
TH1F *charge_c3_hist = new TH1F("Charge C3", "; x}; #occurences", 50, -10E-16,-10E-10); //Histogram to store Charge C3
TH1F *charge_c4_hist = new TH1F("Charge C4", "; x}; #occurences", 50, -10E-16,-10E-10); //Histogram to store Charge C4
TH1F *peak_c3_hist = new TH1F("Peak C3", "; x}; #occurences", 50, peak_c3_mean-0.005, peak_c3_mean+0.005); //Histogram to store Charge C3
TH1F *peak_c4_hist = new TH1F("Peak C4", "; x}; #occurences", 50, peak_c4_mean-0.005, peak_c4_mean+0.005); //Histogram to store Charge C3
TH2F *charge_c3_timing_hist2d = new TH2F("Charge C3 vs Timing", "CHARGE C3 VS TIMING", 500, ctr_mean-5E-9, ctr_mean+5E-9, 500,-4E-12,4E-12);
TH2F *charge_c4_timing_hist2d = new TH2F("Charge C4 vs Timing", "CHARGE C3 VS TIMING", 500, ctr_mean-5E-9, ctr_mean+5E-9, 500,-4E-12,4E-12);
TH2F *peak_c3_timing_hist2d = new TH2F("Peak C3 vs Timing", "Peak C3 VS TIMING", 500, ctr_mean-5E-9, ctr_mean+5E-9, 500,-0.05,0);
TH2F *peak_c4_timing_hist2d = new TH2F("Peak C4 vs Timing", "Peak C4 VS TIMING", 500, ctr_mean-5E-9, ctr_mean+5E-9, 500,-0.05,0);
//Note: Make C3-C4 (Or Peak3-Peak4), and Low[C3/P3, C4/P4]

for(int i=0; i<ctr_vect.size(); i++){
	ctr_hist->Fill(ctr_vect[i]);
	ctr_cfd_hist->Fill(ctr_cfd_vect[i]);
	charge_c3_hist->Fill(charge_c3_vect[i]);
	charge_c4_hist->Fill(charge_c4_vect[i]);
	peak_c3_hist->Fill(peak_c3_vect[i]);
	peak_c4_hist->Fill(peak_c4_vect[i]);
	charge_c3_timing_hist2d->Fill(ctr_cfd_vect[i], charge_c3_vect[i]);
	charge_c4_timing_hist2d->Fill(ctr_cfd_vect[i], charge_c4_vect[i]);
	peak_c3_timing_hist2d->Fill(ctr_cfd_vect[i], peak_c3_vect[i]);
	peak_c4_timing_hist2d->Fill(ctr_cfd_vect[i], peak_c4_vect[i]);
}

//
int bin1 = ctr_hist->FindFirstBinAbove(ctr_hist->GetMaximum()/2);
int bin2 = ctr_hist->FindLastBinAbove(ctr_hist->GetMaximum()/2);
double fwhm1 = ctr_hist->GetBinCenter(bin2) - ctr_hist->GetBinCenter(bin1);
cout<<"FWHM 1 (without CFD) is: "<<fwhm1<<"\n";
//

int bin11 = ctr_hist->FindFirstBinAbove(ctr_cfd_hist->GetMaximum()/2);
int bin22 = ctr_cfd_hist->FindLastBinAbove(ctr_cfd_hist->GetMaximum()/2);
double fwhm2 = ctr_cfd_hist->GetBinCenter(bin22) - ctr_cfd_hist->GetBinCenter(bin11);
cout<<"FWHM 2 (with CFD) is: "<<fwhm2<<"\n";
//

TCanvas* c1 = new TCanvas("c1", "c1", 700, 400);

ctr_hist->SetLineColor(kBlue);
ctr_hist->SetLineWidth(3);
ctr_hist->Draw("");

ctr_cfd_hist->SetLineColor(kRed);
ctr_cfd_hist->SetLineWidth(3);
ctr_cfd_hist->Draw("same");
c1->SaveAs("Timing.eps");
c1->Clear("");

charge_c3_hist->Draw();
c1->SaveAs("Charge_C3.eps");

charge_c4_hist->Draw();
c1->SaveAs("Charge_C4.eps");

peak_c3_hist->Draw();
c1->SaveAs("Peak_C3.eps");

peak_c4_hist->Draw();
c1->SaveAs("Peak_C3.eps");

charge_c3_timing_hist2d->Draw("colz");
c1->SaveAs("Charge_C3_Timing.eps");

charge_c4_timing_hist2d->Draw("colz");
c1->SaveAs("Charge_C4_Timing.eps");

peak_c3_timing_hist2d->Draw("colz");
c1->SaveAs("Peak_C3_Timing.eps");

peak_c4_timing_hist2d->Draw("colz");
c1->SaveAs("Peak_C4_Timing.eps");

} //END OF MAIN()

