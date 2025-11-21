///
/// Starter template for first baseball problem
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

struct Params {
  double g;   // acceleration [m/s^2]
  double m;   // mass of object [kg], nb proj. In vacuum funcs do not depend on the mass
  double d;   // m diameter of ball
  double b;   // b,c params for air resistance
  double c;
};
  double fx(double x, const vector<double> &y, void *params){
  	(void) x;
	return y[1];
  }

  double fvx(double x, const vector<double> &y, void *params){
	Params *p = (Params*) params;
	double vx = y[1], vz = y[3];
	double v = sqrt(vx*vx + vz*vz);
	double Fd = p->b*v + p->c*v*v;
	return -(Fd*vx)/(p->m*v);	
  }

  double fz(double x, const vector<double> &y, void *params){
  	(void) x; 
	return y[3];
  }

  double fvz(double x, const vector<double> &y, void *params){
  	Params *p = (Params *) params;
	double vx = y[1], vz = y[3]; 
	double v = sqrt(vx*vx + vz*vz);
	double Fd = p->b*v +p->c*v*v;
	return -p->g -(Fd*vz)/(p->m*v);
  }

  double f_stop(double x, const vector<double> &y, void *params){
  	if (y[0] >= 18.5) return 1;
	return 0;
  }

  double test_v0(double v0, double z0, double theta0, void * params){
  	vector<pfunc_t> fn = {fx, fvx, fz, fvz}; 
	double theta = theta0 * M_PI/180;
  	vector<double> y0(4);
 	y0[0] = 0.0;
  	y0[1] = v0*cos(theta);
  	y0[2] = z0;
  	y0[3] = v0*sin(theta);
	double x0 = 0; 
	int steps = 100;
	RK4SolveN(fn, y0,steps, x0, 5.0, params, f_stop);
	return y0[2];
  }


int main(int argc, char **argv){

  // examples of parameters
  Params pars;
  pars.g=9.81;
  pars.m=0.145;    
  pars.d=0.0075;   
  pars.b=1.6e-4;  
  pars.c=0.25;
  void *p_par = (void*) &pars;

  double xend=18.5;       // meters to plate
  double z0=1.4;             // height of release [m]
  double theta0=1;         // angle of velocity at release (degrees)
                                      // convert to radians before using!
  bool showPlot=false;    // keep this flag false by default
  
  // allow changing the parameters from the command line
  int c;
  while ((c = getopt (argc, argv, "x:z:t:p")) != -1)
    switch (c) {
    case 'x':
      xend = atof(optarg);
      break;
    case 'z':
      z0 = atof(optarg);
      break;
    case 't':
      theta0 = atof(optarg);
      break;
    case 'p':
      showPlot=true;
      break;
    case '?':
      fprintf (stderr, "Unknown option `%c'.\n", optopt);
    }
  TApplication theApp("App", &argc, argv); // init ROOT App for displays
  double low  = 20.0;
  double high = 60.0;
  double v0_test = 0;
  for (int i = 0; i < 100; i++){
  	v0_test = 0.5*(high+low);
	double z_test = test_v0(v0_test, z0, theta0, p_par);
	if (z_test > 0.9) high = v0_test;
	else low = v0_test;
  
  }

  double vPitch = v0_test;   // m/s of pitch needed to land in strike zone at 0.9 meters
  // write code to solve for vPitch here



  // do not change these lines
  printf("********************************\n");
  printf("(xend,z0,theta0) = (%lf,%lf,%lf)\n",xend,z0,theta0);
  printf("v_pitch = %lf m/s\n",vPitch);
  printf("********************************\n");

  if (showPlot){
    cout << "Press ^c to exit" << endl;
    theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
    theApp.Run();
  }
  
  return 0;
}

