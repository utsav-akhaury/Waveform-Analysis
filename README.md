# Waveform Analysis

C++ codes written using the CERN ROOT framework for waveform analysis of muon pulses acquired from an oscilloscope in .csv format

1) ***CSV_Vector.h*** &nbsp;- &nbsp;Reads .csv file into table, exported as a vector of vector of double

2) ***Waveform_Analysis.h*** &nbsp;- &nbsp;Includes functions for performing basic operations (finding peak amplitude, extracting timing information based on leading edge discrimination, integrating voltage for charge computation)

3) ***Main.cpp*** &nbsp;- &nbsp;Inputs the data, performs appropriate thersholding and plots corresponding histograms of amplitude & charge distribution

*[Rene Brun and Fons Rademakers, ROOT - An Object Oriented Data Analysis Framework,
Proceedings AIHENP'96 Workshop, Lausanne, Sep. 1996,
Nucl. Inst. & Meth. in Phys. Res. A 389 (1997) 81-86.]*
