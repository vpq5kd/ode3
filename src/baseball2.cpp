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
#include "TAxis.h"
#include "TLine.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
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
	double v = sqrt((vx*vx) + (vy * vy) + (vz * vz));
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
	double v = sqrt((vx*vx) + (vy * vy) + (vz * vz));
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
	double v = sqrt((vx*vx) + (vy * vy) + (vz * vz));
	double fv = 0.0039 + (0.0058)/(1 + exp((v-35)/5));
	return (-p->g)-(fv*v*vz)-(p->B*p->w*vx*sin(p->phi));
}
double f_stop(double x, const vector<double> &y, void *params){
	(void) x;
	if (y[0] >= 18.44) return 1;
	return 0;
}

vector<TGraph> throw_baseball(double theta, double w, double phi){
    Params pars;
    vector<double> y0(6);
    pars.g = 9.81;
    pars.w = w * (M_PI/30);
    pars.phi = phi * M_PI/180;
    pars.B = 4.1e-4;
    void *p_par = (void *) &pars;

    double v0 = 37.9984;
    theta = theta * M_PI/180;
    double h = 1.4e-4;
    double steps = 1/h;

    y0[0] = 0.0;
    y0[1] = v0*cos(theta);
    y0[2] = 0.0;
    y0[3] = 0.0;
    y0[4] = 0.0;
    y0[5] = v0*sin(theta);

    vector<pfunc_t> fn = {fx,fvx, fy, fvy, fz, fvz};
    
    double x0 = 0;

    return RK4SolveN(fn, y0, steps, x0, 18.5, p_par, f_stop);
}
int main(int argc, char **argv){

  // we have 6 initial conditions for this problem
  // y[0] = y[2] = y[4] = 0;  // init x,y,z
  // y[1] = v0*cos(theta0);   // vx  "x is line towards the plate
  // y[3] = 0;                // vy  "y" is measured as left/right divergence from line to plate
  // y[5] = v0*sin(theta0);   // vz  "z" is vertival measure

  bool showPlot=true;
  // pitches
  // slider ip=0
  // curve ip=1
  // screwball ip=2
  // fast ip=3
  int ip=0;    // default pitch
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

  vector<TGraph> tgN;
  string fig_name;
  if (ip==0){
    cout << "Setting up initial conditions for slider" << endl;
    //SetupSlider(y0);
    tgN = throw_baseball(1, 1800, 0);   
    fig_name = "Slider";
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

    double x_final, y_final, z_final, vx_final, vy_final, vz_final, xx,xvx,xy,xvy, xz, xvz;
    tgN[0].GetPoint(tgN[0].GetN()-1, xx, x_final);
    tgN[1].GetPoint(tgN[1].GetN()-1, xvx, vx_final);
    tgN[2].GetPoint(tgN[2].GetN()-1, xy, y_final);
    tgN[3].GetPoint(tgN[3].GetN()-1, xvy, vy_final);
    tgN[4].GetPoint(tgN[4].GetN()-1, xz, z_final);
    tgN[5].GetPoint(tgN[5].GetN()-1, xvz, vz_final);


  double feet_multiplier = 3.28084;
  double mph_multiplier = 2.23694;  
  double xend=x_final*feet_multiplier;   // feet
  double yend=y_final*feet_multiplier;    // tbd
  double zend = z_final*feet_multiplier;    // tbd
  double vxend=vx_final*mph_multiplier;
  double vyend=vy_final*mph_multiplier;
  double vzend=vz_final*mph_multiplier;

  TGraph zx;
  int n = tgN[0].GetN();
  for (int i = 0; i<n; i++){
  	double t1,x,t2,z;
	tgN[0].GetPoint(i, t1,x);
	tgN[4].GetPoint(i, t2,z);
	zx.SetPoint(i,x*feet_multiplier,z*feet_multiplier);
  }

  TGraph yx;
  for (int i = 0; i<n; i++){
  	double t1,x,t2,y;
	tgN[0].GetPoint(i, t1,x);
	tgN[2].GetPoint(i, t2,y);
	yx.SetPoint(i,x*feet_multiplier,y*feet_multiplier);
  }
  TLine *x_end = new TLine(xend,-4,xend,2);
  // write code here
    // to compare to  plots in Fitzpatrick, output your results in **feet**
  // do not change these lines
  printf("********************************\n");
  printf("Coordinates when x=60 feet\n");
  printf("(x,y,z) = (%lf,%lf,%lf)\n",xend,yend,zend);
  printf("(vx,vy,vz) = (%lf,%lf,%lf)\n",vxend,vyend,vzend);
  printf("********************************\n");

  // plot the trajectory.  See Fitzpatrick for plot details
  if (showPlot){
    cout << "Press ^c to exit" << endl;
    theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
    TCanvas *c1 = new TCanvas("c1", "outputs" , 800, 600);
    string title = fig_name + "; x (ft); y (ft) / z (ft)";
    zx.SetTitle(title.c_str());
    zx.Draw("AL");
    zx.GetYaxis()->SetRangeUser(-4,2);
    yx.SetLineStyle(2);
    yx.Draw("L SAME");
    x_end->SetLineColor(kGray);
    x_end->Draw("same");
    string filename = fig_name +".png";
    c1->SaveAs(filename.c_str());
    theApp.Run();
  }
  
  return 0;
}

