/* generate.cc code to generate mesh for surface by taking the boundary of a
	thickened neighborhood of a planar embedded graph */

int reverse_ej(int i, int j, imat GRF){ 	
	// returns l such that if GRF[i][j]=k then GRF[k][l]=i
	int k,l,L;
	k=GRF[i][j];
	L=-1;
	if(k!=-1){	// if this is not a boundary edge
		for(l=0;l<(int) GRF[k].size();l++){
			if(GRF[k][l]==i){
				L=l;
			};
		};
	};
	return(L);
};

void mesh::generate_mesh(ifstream &input_file){	// generate mesh by thickening planar fatgraph
	ADJ.resize(0);
	LOC.resize(0);
	ivec I;
	
	imat GRF;		// planar graph with junctions and endpoints
	vpoint GLC;		// location of vertices of GRF
	
	double theta1,theta2,theta,phi,x,y,z;	
	int i,j,J,k,K,l,L,m,v;
	
	double edge_thick, inner_thick;
	
	edge_thick=0.3;		// default parameters in case not specified
	inner_thick=0.3;
	incolor=newpoint(1.0,1.0,0.8);
	outcolor=newpoint(0.8,0.8,1.0);	// default colors in case not specified
	

	imat W;			// labels of new vertices in ADJ associated to GRF
	
	GRF.resize(0);	// initialize
	GLC.resize(0);
	
	/* read GRF and GLC from <filename.grf> file */
	input_file >> v;	// number of vertices;
	for(i=0;i<v;i++){
		input_file >> j;	// valence
		I.resize(0);
		for(k=0;k<j;k++){
			input_file >> l;
			I.push_back(l);
		};
		GRF.push_back(I);
		input_file >> x >> y >> z;
		GLC.push_back(newpoint(x,y,z));
	};
	input_file >> edge_thick;
	input_file >> inner_thick;
	for(i=0;i<3;i++){
		input_file >> incolor[i];
	};
	for(i=0;i<3;i++){
		input_file >> outcolor[i];
	};	
	input_file.close();
	
	/* determine W; i.e. labels for vertices in ADJ associated to vertices of GRF */
	W=GRF;	
	v=0;
	for(i=0;i<(int) GRF.size();i++){
		for(j=0;j<(int) GRF[i].size();j++){		// new vertices in the x-y plane
			W[i][j]=v;
			v++;
		};
		W[i].push_back(v);		// new vertex with z>0
		v++;
		W[i].push_back(v);		// new vertex with z<0
		v++;
	};
	
	/* add the ADJ vertices and their LOC one by one */
	for(i=0;i<(int) GRF.size();i++){
		// special case: boundary vertex
		if((int) GRF[i].size()==2 && (GRF[i][0]==-1 || GRF[i][1]==-1)){
			if(GRF[i][1]==-1){	// normalize so that GRF[i][0]=-1
				GRF[i][1]=GRF[i][0];
				GRF[i][0]=-1;
			};
			k=GRF[i][1];
			l=reverse_ej(i,1,GRF);
			L=(l-1+(int) GRF[k].size()) % (int) GRF[k].size();
			
			I.resize(0);					// new boundary right vertex
			I.push_back(W[i][GRF[i].size()+1]);	
			I.push_back(W[k][l]);
			I.push_back(W[k][GRF[k].size()]);
			I.push_back(W[i][GRF[i].size()]);
			I.push_back(-1);
			ADJ.push_back(I);		
			
			theta1=atan2(GLC[k][1]-GLC[i][1],GLC[k][0]-GLC[i][0]);
			phi=theta1-(PI/2.0);
			LOC.push_back(GLC[i]+newpoint(edge_thick*cos(phi),edge_thick*sin(phi),0.0));			

			I.resize(0);					// new boundary left vertex
			I.push_back(W[i][GRF[i].size()]);	
			I.push_back(W[k][L]);
			I.push_back(W[k][GRF[k].size()+1]);
			I.push_back(W[i][GRF[i].size()+1]);
			I.push_back(-1);
			ADJ.push_back(I);		
			
			theta1=atan2(GLC[k][1]-GLC[i][1],GLC[k][0]-GLC[i][0]);
			phi=theta1+(PI/2.0);
			LOC.push_back(GLC[i]+newpoint(edge_thick*cos(phi),edge_thick*sin(phi),0.0));	
			
			I.resize(0);					// new boundary top vertex
			I.push_back(W[i][0]);	
			I.push_back(W[k][GRF[k].size()]);
			I.push_back(W[k][L]);
			I.push_back(W[i][1]);
			I.push_back(-1);
			ADJ.push_back(I);
			LOC.push_back(GLC[i]+newpoint(0.0,0.0,edge_thick));
			
			I.resize(0);					// new boundary bottom vertex
			I.push_back(W[i][1]);
			I.push_back(W[k][GRF[k].size()+1]);
			I.push_back(W[k][l]);
			I.push_back(W[i][0]);
			I.push_back(-1);
			ADJ.push_back(I);
			LOC.push_back(GLC[i]+newpoint(0.0,0.0,-edge_thick));
		} else {
		
		
		// generic case: interior vertex
		
		for(j=0;j<(int) GRF[i].size();j++){
			I.resize(0);							// new ordinary side vertex
			I.push_back(W[i][GRF[i].size()]);			
			k=GRF[i][j];
			l=reverse_ej(i,j,GRF);
			L=(l-1+ (int) GRF[k].size()) % (int) GRF[k].size();
			I.push_back(W[k][L]);
			I.push_back(W[k][GRF[k].size()+1]);
			I.push_back(W[i][GRF[i].size()+1]);
			J=(j+1) % (int) GRF[i].size();
			K=GRF[i][J];
			m=reverse_ej(i,J,GRF);
			I.push_back(W[K][m]);
			I.push_back(W[K][GRF[K].size()]);
			ADJ.push_back(I);						
			
			theta1=atan2(GLC[k][1]-GLC[i][1],GLC[k][0]-GLC[i][0]);
			theta2=atan2(GLC[K][1]-GLC[i][1],GLC[K][0]-GLC[i][0]);
			theta=((theta2-theta1)/2.0);
			if(theta<0.0){
				theta=theta+PI;
			};
			if(theta>PI){
				theta=theta-PI;
			};
			phi=theta1+theta;
			LOC.push_back(GLC[i]+newpoint(inner_thick*cos(phi),inner_thick*sin(phi),0.0));			
		};
		I.resize(0);								// new ordinary top vertex
		for(j=0;j<(int) GRF[i].size();j++){
			k=GRF[i][j];
			l=reverse_ej(i,j,GRF);
			L=(l-1+ (int) GRF[k].size()) % (int) GRF[k].size();
			I.push_back(W[k][GRF[k].size()]);
			I.push_back(W[k][L]);
			I.push_back(W[i][j]);
		};
		ADJ.push_back(I);		
		LOC.push_back(GLC[i]+newpoint(0.0,0.0,inner_thick));
		
		I.resize(0);								// new ordinary bottom vertex
		for(j=(int) GRF[i].size()-1;j>=0;j--){
			k=GRF[i][j];
			l=reverse_ej(i,j,GRF);
			I.push_back(W[i][j]);
			I.push_back(W[k][GRF[k].size()+1]);
			I.push_back(W[k][l]);
		};
		ADJ.push_back(I);
		LOC.push_back(GLC[i]+newpoint(0.0,0.0,-inner_thick));
		
		};
	};
	
	WIRE=true;	// nicer default
	verbose=true; // nicer default
	isobar=false; // nicer default
	iheight=0.01; // nice default
	
	if(verbose){
		cout << "Generated mesh from planar graph. " << ADJ.size() << " vertices. \n";
	};
};