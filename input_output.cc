/* input_output.cc */

void mesh::read_mesh(ifstream &input_file){
	
	/* format of <filename.wire> is as follows:
	number of vertices
	valence of vertex 0		list of vertices adjacent to vertex 0	location of vertex 0
	valence of vertex 1		list of vertices adjacent to vertex 1	location of vertex 1
	and so on.	*/
	
	ivec I;
	point p;
	int i,j,k,vertices,valence;
	
	ADJ.resize(0);	// initialize vertex list data
	LOC.resize(0);	// initialize vertex location data
	
	input_file >> vertices;
	for(i=0;i<vertices;i++){
		I.resize(0);
		input_file >> valence;
		for(j=0;j<valence;j++){
			input_file >> k;
			I.push_back(k);
		};
		ADJ.push_back(I);
		input_file >> p[0];
		input_file >> p[1];
		input_file >> p[2];
		LOC.push_back(p);
	};
	incolor=newpoint(1.0,1.0,0.8);
	outcolor=newpoint(0.8,0.8,1.0);	// default colors in case not specified
	for(i=0;i<3;i++){
		input_file >> incolor[i];
	};
	for(i=0;i<3;i++){
		input_file >> outcolor[i];
	};
	
	WIRE=true;	// nicer default
	verbose=true; // nicer default
	isobar=false; // nicer default
	iheight=0.01; // nice default
	if(verbose){
		cout << "Read mesh from file. " << vertices << " vertices.\n";
	};
};

void mesh::eps_draw(){	// write mesh to .eps file
	string S;
	ofstream output_file;
	int i,j,k,l;
	double t;
	point N;
	
	cout << "Enter name of file to write to: ";
	getline(cin, S, '\n');
	cout << "Writing to file " << S << "\n";
	output_file.open(S.c_str());
	output_file << "%!PS-Adobe-2.0 EPSF-2.0 \n";
	output_file << "%%BoundingBox: 0 0 500 500 \n";
	output_file << "gsave \n";
	output_file << "100 100 scale \n";
	output_file << "1 100 div setlinewidth \n";
	output_file << "2.5 2.5 translate \n";
	if(isobar==false){	// standard shaded mode
		output_file << "/t {9 dict begin \n";
		output_file << "   /c3 exch def \n";
		output_file << "   /c2 exch def \n";
		output_file << "   /c1 exch def \n";
		output_file << "   /y3 exch def \n";
		output_file << "   /x3 exch def \n";
		output_file << "   /y2 exch def \n";
		output_file << "   /x2 exch def \n";
		output_file << "   /y1 exch def \n";
		output_file << "   /x1 exch def \n";
		output_file << "   gsave \n";
		output_file << "   c1 c2 c3 setrgbcolor \n";
		output_file << "   newpath \n";
		output_file << "   x1 y1 moveto \n";
		output_file << "   x2 y2 lineto \n";
		output_file << "   x3 y3 lineto \n";
		output_file << "   closepath \n";
		output_file << "   fill \n";
		output_file << "   stroke \n";
		output_file << "   grestore \n";
		output_file << "end} def \n";
	
		sort_faces();
		for(i=0;i<(int) FAC.size();i++){
			j=FAC[i][0];
			k=FAC[i][1];
			l=FAC[i][2];
			output_file << LOC[j][0] << " " << -LOC[j][1] << " ";
			output_file << LOC[k][0] << " " << -LOC[k][1] << " ";
			output_file << LOC[l][0] << " " << -LOC[l][1] << " ";
			N = (LOC[k]-LOC[j])*(LOC[l]-LOC[k]);
			N=N/norm(N);
			t=N[2];
			if(t>=0.0){
				output_file << 0.2+(0.6*t*outcolor[0]) << " " << 0.2+(0.6*t*outcolor[1]) << " " << 0.2+(0.6*t*outcolor[2]) << " t\n";
			} else {
				output_file << 0.2-(0.6*t*incolor[0]) << " " << 0.2-(0.6*t*incolor[1]) << " " << 0.2-(0.6*t*incolor[2]) << " t\n";
			};
		};
	} else {	// isobar mode
		output_file << "/t {6 dict begin \n";
		output_file << "   /y3 exch def \n";
		output_file << "   /x3 exch def \n";
		output_file << "   /y2 exch def \n";
		output_file << "   /x2 exch def \n";
		output_file << "   /y1 exch def \n";
		output_file << "   /x1 exch def \n";
		output_file << "   gsave \n";
		output_file << "   1 setgray \n";
		output_file << "   newpath \n";
		output_file << "   x1 y1 moveto \n";
		output_file << "   x2 y2 lineto \n";
		output_file << "   x3 y3 lineto \n";
		output_file << "   closepath \n";
		output_file << "   fill \n";
		output_file << "   stroke \n";
		output_file << "   grestore \n";
		output_file << "end} def \n";	
		output_file << "/l {4 dict begin \n";
		output_file << "	/y2 exch def \n";
		output_file << "	/x2 exch def \n";
		output_file << "	/y1 exch def \n";
		output_file << "	/x1 exch def \n";
		output_file << "	newpath \n";
		output_file << "	x1 y1 moveto \n";
		output_file << "	x2 y2 lineto \n";
		output_file << "	stroke \n";
		output_file << "end} def \n";

		sort_faces();
		for(i=0;i<(int) FAC.size();i++){
			j=FAC[i][0];
			k=FAC[i][1];
			l=FAC[i][2];
			output_file << LOC[j][0] << " " << -LOC[j][1] << " ";
			output_file << LOC[k][0] << " " << -LOC[k][1] << " ";
			output_file << LOC[l][0] << " " << -LOC[l][1] << " t\n";
			draw_stripy_eps_triangle(LOC[j],LOC[k],LOC[l],iheight,output_file);
		};
	};
	
	output_file << "grestore \n";
	output_file << "%eof \n";
	output_file.close();
	
	cout << "wrote to file.\n";
};

void mesh::write_mesh_to_cout(){	// write mesh to cout
	int i,j;
	for(i=0;i<(int) ADJ.size();i++){
		cout << "vertex " << i << "   ";
		for(j=0;j<(int) ADJ[i].size();j++){
			cout << ADJ[i][j] << " ";
		};
		cout << "  " << LOC[i][0] << " " << LOC[i][1] << " " << LOC[i][2] << "\n";
	};
	cout << "incolor: ";
	for(i=0;i<3;i++){
		cout << incolor[i] << " ";
	};
	cout << "outcolor: ";
	for(i=0;i<3;i++){
		cout << outcolor[i] << " ";
	};
	cout << "\n";
};
