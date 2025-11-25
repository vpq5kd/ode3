///
/// Starter template for second baseball problem
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

struct Params{
	double B;
	double phi;
	double w; 
	double g;
};
double fx(double x, const vector<double> &y, void *params){
  	(void) x; 
	return y[1];
}
double fvx(double x, const  vector<double> &y, void *params){
	Params *p = (Params*) params;
	(void) x;
	double vx = y[1], vy = y[3], vz = y[5];
	double v = sqrt(vx*vx + vy * vy + vz * vz);
	double fv = 0.0039 + (0.0058)/(1 + exp((v-35)/5));
	return (-fv*v*vx) + p->B*p->w*((vz*sin(p->phi))-(vy*cos(p->phi)));
}
double fy(double x, const vector<double> &y, void *params){
  	(void) x; 
	return y[3];
}
double fvy(double x, const vector<double> &y, void *params){
  	Params * p = (Params*) params;
	(void) x;
        double vx = y[1], vy = y[3], vz = y[5];
	double v = sqrt(vx*vx + vy * vy + vz * vz);
	double fv = 0.0039 + (0.0058)/(1 + exp((v-35)/5));
	return (-fv*v*vy) + (p->B*p->w*vx*cos(p->phi));
}
double fz(double x, const vector<double> &y, void *params){
	(void) x;
	return y[5]; 
}
double fvz(double x, const vector<double> &y, void * params){
	Params * p = (Params*) params;
	(void) x;
        double vx = y[1], vy = y[3], vz = y[5];
	double v = sqrt(vx*vx + vy * vy + vz * vz);
	double fv = 0.0039 + (0.0058)/(1 + exp((v-35)/5));
	return (-p->g)-(fv*v*vz)-(p->B*p->w*vx*sin(p->phi));
}
double f_stop(double x, const vector<double> &y, void *params){
	(void) x;
	if (y[0] >= 18.47) return 1;
	return 0;
}

int main(int argc, char **argv){

  // we have 6 initial conditions for this problem
  // y[0] = y[2] = y[4] = 0;  // init x,y,z
  // y[1] = v0*cos(theta0);   // vx  "x is line towards the plate
  // y[3] = 0;                // vy  "y" is measured as left/right divergence from line to plate
  // y[5] = v0*sin(theta0);   // vz  "z" is vertival measure
  vector<double> y0(6);

  bool showPlot=false;
  // pitches
  // slider ip=0
  // curve ip=1
  // screwball ip=2
  // fast ip=3
  int ip=1;    // default pitch
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
    cout << "Setting up initial conditions for slider" << endl;
    //SetupSlider(y0);
    Params pars;
    pars.g = 9.81;
    pars.w = 1800;
    pars.phi = 0;
    pars.B = 4.1e-4;
    void *p_par = (void *) &pars;

    double v0 = 37.9984;
    double theta = 1 * M_PI/180;
    double h = 1.4e-4;
    double steps = 1/h;

    y0[0] = 0.0;
    y0[1] = v0*cos(theta);
    y0[2] = 0.0;
    y0[3] = 0.0;
    y0[4] = 0.0;
    y0[5] = v0*sin(theta);

    vector<pfunc_t> fn = {fx,fy,fz,fvx,fvy,fvz};
    
    double x0 = 0;

    auto tgN = RK4SolveN(fn, y0, steps, x0, 20.0, p_par, f_stop);
    double vx_final, vy_final, vz_final, xx,xy,xz;

    tgN[1].GetPoint(tgN[1].GetN()-1, xx, vx_final);
    tgN[3].GetPoint(tgN[3].GetN()-1, xy, vy_final);
    tgN[5].GetPoint(tgN[5].GetN()-1, xz, vz_final);

    double speed = sqrt((vx_final*vx_final) * (vy_final*vy_final) * (vz_final*vz_final);
    cout << "slider speed = " << speed << "(m/s)" << endl;

  }
  else if (ip==1){
    cout << "Setting up initial conditions for curveball" << endl;
    //SetupCurve(y0);
  }
  else if (ip==2){
    cout << "Setting up initial conditions for screwball" << endl;
    //SetupScrewball(y0);
  }
  else {
    cout << "Setting up initial conditions for fastball" << endl;
    //SetupFastball(y0);
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

