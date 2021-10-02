///
/// Starter templated for second baseball problem
/// Solve for the initial speed of the pitch given the initial parameters
/// xend : distance to home plate [18.5] m
/// z0 : height of release of ball [1.4] m
/// theta0 : angle of release above horizontal [1] degree
///
///  Do not change the interface for running the program
///  Fill in the value of vPitch in the print statement with your solution
///  at the end of main()
///

#include "RKn.hpp"
#include "TROOT.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TFile.h"
#include "TStyle.h"
#include "TGClient.h"
#include "TF1.h"
#include "TCanvas.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;


int main(int argc, char **argv){

  // we have 6 initial conditions for this problem
  // y[0] = y[2] = y[4] = 0;  // init x,y,z
  // y[1] = v0*cos(theta0);   // vx  "x is line towards the plate
  // y[3] = 0;                // vy  "y" is measured as left/right divergence from line to plate
  // y[5] = v0*sin(theta0);   // vz  "z" is vertival measure
  vector<double> y0(6);

  bool showPlot=true;    // keep this flag true by default
  // pitches
  // curve ip=0
  // fast ip=1
  // slider ip=2
  // screwball ip=3
  int ip=0;
  int c;
  while ((c = getopt (argc, argv, "p:n")) != -1)
    switch (c) {
    case 'p':
      ip = atoi(optarg);
      break;
    case 'n':
      showPlot=false;
      break;
    }

  TString title;
  if (ip==0){
    cout << "Setting up initial conditions for curveball" << endl;
    //SetupCurve(y0);
  }
  else if (ip==1){
    cout << "Setting up initial conditions for fastball" << endl;
    //SetupFast(y0);
  }
  else if (ip==2){
    cout << "Setting up initial conditions for slider" << endl;
    //SetupSlider(y0);
  }
  else {
      cout << "Setting up initial conditions for screwball" << endl;
      //SetupScrewball(y0);
  }

  TApplication theApp("App", &argc, argv); // init ROOT App for displays

  double xend=60;   // feet
  double yend=0;    // tbd
  double zend=0;    // tbd
  double vxend=0;
  double vyend=0;
  double vzend=0;

  // write code here


  // to compare to the plots in Fitzpatrick, output your results in **feet**
  // do not change these lines
  printf("********************************\n");
  printf("Coordinates when x=60 feet\n");
  printf("(x,y,x) = (%lf,%lf,%lf)\n",xend,yend,zend);
  printf("(vx,vy,vz) = (%lf,%lf,%lf)\n",vxend,vyend,vzend);
  printf("********************************\n");

  // plot the trajectory.  See Fitzpatrick for plot details
  if (showPlot){
    cout << "Press ^c to exit" << endl;
    theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
    theApp.Run();
  }
  
  return 0;
}

