/*	wireframe.cc	

	tool to quickly produce nice .eps figures of 3d rendered
	surfaces with boundary, for inclusion in math papers

	Version 0.02: February 10, 2013

	Copyright Danny Calegari

	released under the terms of the GNU GPL
*/

// standard libraries to include

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
//#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>
#include <tr1/array>
#include <complex>
#include <assert.h>

using namespace std;
using std::tr1::array;

// preprocessor definitions

#define PI 		3.14159265358979
#define TWOPI	6.28318530717959
#define SCREEN	900
#define ivec		vector< int >				// vector of ints
#define imat		vector< ivec >				// vector of vectors of ints
#define point		array < double,3 >			// 3d point
#define mat			array < point,3 >			// 3d matrix
#define vpoint		vector< point >				// vector of 3d points

// global constants

#include "graphics.cc"
#include "algebra.cc"
#include "sort.cc"
#include "draw.cc"
#include "mesh.cc"
#include "generate.cc"
#include "input_output.cc"
#include "interface.cc"

int main(int argc, char *argv[]){ 

	mesh M;
	ifstream input_file;
	string T,S = "";
	
	cout << "Welcome to wireframe!\n";
	cout << "Press [f] to toggle between solid and wireframe.\n";
	cout << "Press [v] to toggle between verbose/silent.\n";
	cout << "Press [i] to toggle between shade/isobar.\n";
	cout << "Press [1/2] to make isobars sparser/denser.\n";
	cout << "Press [s] to subdivide and smooth mesh.\n";
	cout << "Press [r] to retriangulate to shorten edges.\n";
	cout << "Press [c] to do curvature flow.\n";
	cout << "Press [t] to trim pyramids.\n";
	cout << "Press arrow keys to rotate mesh.\n";
	cout << "Press [+/-] to make bigger/smaller.\n";
	cout << "Press [e] for .eps output.\n";
	cout << "Press [w] to write mesh to file.\n";
	cout << "Press [p] to write in povray format.\n";
	cout << "Press [q] to quit.\n\n";
	
	if(argc<=2){	
		cout << "Usage: ./wireframe -w <filename.wire> or ./wireframe -g <filename.grf> \n";
		return(0);
	} else {
		T=argv[1];
		if(T=="-w"){
			S=std::string(argv[2]);
			input_file.open(S.c_str(), std::fstream::in);
			M.read_mesh(input_file);
		} else if(T=="-g"){
			S=std::string(argv[2]);
			input_file.open(S.c_str(), std::fstream::in);
			M.generate_mesh(input_file);
		};
	};
	
	M.generate_face_list();
	M.compute_normals();
	
	setup_graphics();
	M.draw_mesh();
	XFlush(display);
	
	M.interface();

	return(0);
};
