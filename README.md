# Waveform Analysis

C++ codes written using the CERN ROOT framework for waveform analysis of muon pulses acquired from an oscilloscope in .csv format

1) ***[CSV_Vector.h](https://github.com/utsav-akhaury/Waveform-Analysis/blob/master/CSV_Vector.h)***     
Reads .csv file into table, exported as a vector of vector of double

2) ***[Waveform_Analysis.h](https://github.com/utsav-akhaury/Waveform-Analysis/blob/master/Waveform_Analysis.h)***      
Includes functions for performing basic operations (finding peak amplitude, extracting timing information based on leading edge discrimination, integrating voltage for charge computation)

3) ***[Main.cpp](https://github.com/utsav-akhaury/Waveform-Analysis/blob/master/Main.cpp)***       
Inputs the data, performs appropriate thresholding and plots corresponding histograms of amplitude & charge distribution

*[Rene Brun and Fons Rademakers, ROOT - An Object Oriented Data Analysis Framework,
Proceedings AIHENP'96 Workshop, Lausanne, Sep. 1996,
Nucl. Inst. & Meth. in Phys. Res. A 389 (1997) 81-86.]*
