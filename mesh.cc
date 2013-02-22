/* mesh.cc */

class mesh{   //	triangular mesh

	/* mesh is stored in vertex-vertex format; i.e. each vertex is an
	ordered list of adjacent vertices; edges and faces are 
	defined implicitly and computed from the data by functions.	
	A vertex is on the boundary if one of its ``adjacent vertices'' is -1.
	*/
	
	public:
  		// data defining mesh
  		imat	ADJ;		// vertex adjacency	list
  		vpoint	LOC;		// location of vertices in space
  		vpoint  NOR;		// unit normals to vertices
  		imat	FAC;		// face list
  		bool	WIRE;		// wire or solid?
  		bool 	verbose;	// how much information?
  		bool	isobar;		// isobar or shade?
  		double	iheight;	// vertical space between isobars (defaults to 0.1)
  		point	incolor;	// color of interior faces
  		point	outcolor;	// color of exterior faces
  		
  		// function to generate mesh with prescribed topology
  		void generate_mesh(ifstream &input_file);	// generate mesh by thickening graph
//  		void generate_better_mesh(ifstream &input_file);	// alternate method to generate mesh
  		
  		// functions to read mesh from file
  		void read_mesh(ifstream &input_file);	// read mesh from file
  		
  		// functions to write mesh to screen or file
  		void write_mesh_to_cout();	// write mesh to cout
  		void write_mesh_to_file();	// write mesh to file
		void write_povray_file();	// write mesh in povray format
  		
  		// functions to determine combinatorics
  		int reverse_edge(int i,int j);	// inverse adjacency data
  		void generate_face_list();	// determine face list from vertex list
  		void sort_faces();	// order face list by height
  		int boundary_edge(int v);	// which edge points to boundary, if any?
  		
  		// functions to modify mesh
  		void linear_transformation(mat M);
  		void compute_normals();
  		void subdivide();	
  		void curvature_flow();
  		void trim_pyramids();
  		
  		// functions to draw mesh
  		void draw_mesh();	// draw mesh in some way
  		void wire_draw();	// draw edges of mesh
  		void face_draw();	// draw triangles
  		void eps_draw();	// draw triangles to .eps file
  		
  		// interface function
  		void interface();	// interact with mouse/arrows/keyboard
};

int mesh::reverse_edge(int i, int j){ 
	// returns l such that if ADJ[i][j]=k then ADJ[k][l]=i
	int k,l,L;
	k=ADJ[i][j];
	L=-1;
	if(k!=-1){	// if this is not a boundary edge
		for(l=0;l<(int) ADJ[k].size();l++){
			if(ADJ[k][l]==i){
				L=l;
			};
		};
	};
	return(L);
};

int mesh::boundary_edge(int v){		
	// which edge points to boundary, if any? returns -1 if none.
	int i,e;
	e=-1;
	for(i=0;i<(int) ADJ[v].size();i++){
		if(ADJ[v][i]==-1){
			e=i;
		};
	};
	return(e);
};

void mesh::generate_face_list(){
	/* generate list of faces (they should all be triangles) from
	a vertex-vertex description of the mesh. */
	
	ivec I;
	int i,j,k,l,L,m,n,N;

	FAC.resize(0);	// initialize face list data
	
	for(i=0;i<(int) ADJ.size();i++){	// for each vertex
		for(j=0;j<(int) ADJ[i].size();j++){	// for each outgoing edge
			if(ADJ[i][j]!=-1 && ADJ[i][(j-1+(int) ADJ[i].size()) % (int) ADJ[i].size()]!=-1){	// if this is not a boundary face
				I.resize(0);
				k=ADJ[i][j];
				l=reverse_edge(i,j);	
				assert (ADJ[k][l]==i); // should have ADJ[k][l]==i
				L=(l+1)%((int) ADJ[k].size());	// L=l+1 cyclically
				m=ADJ[k][L];
				n=reverse_edge(k,L);	
				assert (ADJ[m][n]==k); // should have ADJ[m][n]=k
				N=(n+1)%((int) ADJ[m].size());	// N=n+1 cyclically
				assert (ADJ[m][N]==i); // should have ADJ[m][N]=i
				if(i<k && i<m){	// only add face to list once
					I.push_back(i);
					I.push_back(k);
					I.push_back(m);
					FAC.push_back(I);
				};
			};
		};
	};
	if(verbose){
		cout << "Generated faces. " << FAC.size() << " faces.\n";
	};
};

void mesh::linear_transformation(mat M){
	/* apply a fixed linear transformation (in the form of a matrix)
	to every vertex in the mesh.	*/
	
	int i;
	for(i=0;i<(int) LOC.size();i++){
		LOC[i]=M*(LOC[i]);	
		NOR[i]=M*(NOR[i]);
	};
};

void mesh::compute_normals(){
	point P,N;
	int i,j,k,J,K;
	
	NOR.resize(0);	// initialize normal list data
	for(i=0;i<(int) ADJ.size();i++){	// for each vertex
		for(j=0;j<3;j++){	// initialize P
			P[j]=0.0;
		};
		for(j=0;j<(int) ADJ[i].size();j++){		// for each adjacent vertex
			k=ADJ[i][j];
			J=(j+1)%((int) ADJ[i].size());
			K=ADJ[i][J];
			if(k!=-1 && K!=-1){		// no contribution from boundary
				N=(LOC[K]-LOC[i])*(LOC[k]-LOC[i]);
				N=N/norm(N);
				P=P+N;
			};
		};
		P=P/norm(P);
		NOR.push_back(P);
	};
	if(verbose){
		cout << "Computed vertex normals. \n";
	};
};

void mesh::subdivide(){
	/* Function subdivides mesh, putting one new vertex at the midpoint of every edge.
	Then mesh is smoothed by pushing new vertices along normals (average of normals at
	the two endpoints of the edge) to make the result more spherical. */
	
	imat EDJ,NEW_ADJ;
	vpoint NEW_LOC,NEW_NOR;
	point X,Y,Zi,Wi,Zk,Wk,Z;
	ivec I;
	EDJ=ADJ;
	int i,j,k,l;
	int edges,verts;
	edges=0;
	verts=(int) ADJ.size();
	for(i=0;i<(int) ADJ.size();i++){
		for(j=0;j<(int) ADJ[i].size();j++){
			k=ADJ[i][j];
			if(k!=-1){
				if(i<k){
					EDJ[i][j]=edges+verts;
					edges++;
				} else {
					l=reverse_edge(i,j);
					assert(ADJ[k][l]==i);
					EDJ[i][j]=EDJ[k][l];
				};
			} else {
				EDJ[i][j]=-1;
			};
		};
	};
	
	/* At this point edges = number of edges in initial triangulation
	and for every i,j we have EDJ[i][j] = the edge number. This
	is a number between verts and verts+edges-1.
	
	For ADJ[i][j]=-1 (i.e. boundary edge) we have EDJ[i][j]=-1. */
	
	NEW_ADJ.resize(0);	// initialize
	NEW_LOC.resize(0);
	NEW_NOR.resize(0);
	
	for(i=0;i<(int) ADJ.size();i++){	// new vertices corresponding to old vertices
		I.resize(0);	// initialize
		for(j=0;j<(int) ADJ[i].size();j++){
			if(ADJ[i][j]!=-1){	// not boundary edge
				I.push_back(EDJ[i][j]);
			} else {	// boundary edge
				I.push_back(-1);
			};
		};
		NEW_ADJ.push_back(I);
		NEW_LOC.push_back(LOC[i]);
		NEW_NOR.push_back(NOR[i]);
	};
	
	for(i=0;i<(int) ADJ.size();i++){	// new vertices corresponding to old edges
		for(j=0;j<(int) ADJ[i].size();j++){
			I.resize(0);	// initialize
			k=ADJ[i][j];
			if(i<k){	// is this the first time we've seen this edge?
			
				/* special case: edge is boundary */	
				if(ADJ[i][(j-1+(int) ADJ[i].size()) % (int) ADJ[i].size()]==-1){ // boundary on right
					l=reverse_edge(i,j);
					I.push_back(i);
					I.push_back(-1);
					I.push_back(k);
					I.push_back(EDJ[k][(l-1 + EDJ[k].size()) % EDJ[k].size()]);
					I.push_back(EDJ[i][(j+1) % EDJ[i].size()]);
					NEW_ADJ.push_back(I);	

					X=(LOC[i]+LOC[k])/2.0;	// midpoint of two vertices
					Y=(NOR[i]+NOR[k])/2.0;	// average of two normals
					Y=Y/norm(Y);
					Zi=X+(dot_product(LOC[i]-X,Y)*Y);
					Wi=Zi+((dot_product(LOC[i]-Zi,NOR[i])*Y)/(2.0+(dot_product(NOR[i],Y)-1.0)));
					Zk=X+(dot_product(LOC[k]-X,Y)*Y);
					Wk=Zk+((dot_product(LOC[k]-Zk,NOR[k])*Y)/(2.0+(dot_product(NOR[k],Y)-1.0)));
					Z=(Wi+Wk)/2.0;
					NEW_LOC.push_back(Z);	// push in direction of average normal
					NEW_NOR.push_back(Y);
				} else if(ADJ[i][(j+1) % (int) ADJ[i].size()]==-1){ // boundary on left
					l=reverse_edge(i,j);
					I.push_back(i);
					I.push_back(EDJ[i][(j-1 + EDJ[i].size()) % EDJ[i].size()]);
					I.push_back(EDJ[k][(l+1) % EDJ[k].size()]);
					I.push_back(k);
					I.push_back(-1);
					NEW_ADJ.push_back(I);	

					X=(LOC[i]+LOC[k])/2.0;	// midpoint of two vertices
					Y=(NOR[i]+NOR[k])/2.0;	// average of two normals
					Y=Y/norm(Y);
					Zi=X+(dot_product(LOC[i]-X,Y)*Y);
					Wi=Zi+((dot_product(LOC[i]-Zi,NOR[i])*Y)/(2.0+(dot_product(NOR[i],Y)-1.0)));
					Zk=X+(dot_product(LOC[k]-X,Y)*Y);
					Wk=Zk+((dot_product(LOC[k]-Zk,NOR[k])*Y)/(2.0+(dot_product(NOR[k],Y)-1.0)));
					Z=(Wi+Wk)/2.0;
					NEW_LOC.push_back(Z);	// push in direction of average normal
					NEW_NOR.push_back(Y);
				} else {
				
				/* general case */
					l=reverse_edge(i,j);
					I.push_back(i);
					I.push_back(EDJ[i][(j-1 + EDJ[i].size()) % EDJ[i].size()]);
					I.push_back(EDJ[k][(l+1) % EDJ[k].size()]);
					I.push_back(k);
					I.push_back(EDJ[k][(l-1 + EDJ[k].size()) % EDJ[k].size()]);
					I.push_back(EDJ[i][(j+1) % EDJ[i].size()]);
					NEW_ADJ.push_back(I);
				
				// messy ad hoc formula to figure out how far to deform new vertex
					X=(LOC[i]+LOC[k])/2.0;	// midpoint of two vertices
					Y=(NOR[i]+NOR[k])/2.0;	// average of two normals
					Y=Y/norm(Y);
					Zi=X+(dot_product(LOC[i]-X,Y)*Y);
					Wi=Zi+((dot_product(LOC[i]-Zi,NOR[i])*Y)/(2.0+(dot_product(NOR[i],Y)-1.0)));
					Zk=X+(dot_product(LOC[k]-X,Y)*Y);
					Wk=Zk+((dot_product(LOC[k]-Zk,NOR[k])*Y)/(2.0+(dot_product(NOR[k],Y)-1.0)));
					Z=(Wi+Wk)/2.0;
					NEW_LOC.push_back(Z);	// push in direction of average normal
					NEW_NOR.push_back(Y);
				};
			};			// else edge has already been added
		};	
	};
	
	ADJ=NEW_ADJ;
	LOC=NEW_LOC;
	NOR=NEW_NOR;

	if(verbose){
		cout << "Subdividision complete. Now " << verts+edges << " vertices\n";
	};
//	assert(verts+edges==(int) ADJ.size());
	generate_face_list();
};

void mesh::trim_pyramids(){	// cut off all isolated 3-valent vertices

	imat NEW_ADJ;
	vpoint NEW_LOC,NEW_NOR;
	int i,j,k,found_vertex,verts;
	bool potential_example, found_isolated;
	ivec I;
	
	verts=(int) ADJ.size();
	
	found_isolated=false;
	i=0;
	potential_example=false;

	while(found_isolated==false){
		if((int) ADJ[i].size()==3){	// found a 3-valent vertex
			potential_example=true;
			for(j=0;j<3;j++){
				if((int) ADJ[ADJ[i][j]].size()==3){
					potential_example=false;
				};
			};
		};
		if(potential_example==true){
			found_isolated=true;
			found_vertex=i;
		};
		if(found_isolated==false){
			i++;
		};
		if(i==verts){
			break;
		};
	};
	if(found_isolated==true){
		i=found_vertex;
		if(verbose){
			cout << "Found isolated 3-valent vertex; it's vertex " << i << " which has valence " << ADJ[i].size() << ".\n";
			cout << "Trimming vertex.\n";
		};
		NEW_ADJ.resize(0);
		NEW_LOC.resize(0);
		NEW_NOR.resize(0);
		for(j=0;j<verts;j++){
			if(j!=i){	// is this a vertex to be added?
				I.resize(0);
				for(k=0;k<(int) ADJ[j].size();k++){		// let I = ADJ[j] with i removed and j>i diminshed by 1
					if(ADJ[j][k]<i){
						I.push_back(ADJ[j][k]);
					} else if(ADJ[j][k]>i){
						I.push_back(ADJ[j][k]-1);
					};
				};
				NEW_ADJ.push_back(I);
				NEW_LOC.push_back(LOC[j]);
				NEW_NOR.push_back(NOR[j]);
			};
		};
		ADJ=NEW_ADJ;
		LOC=NEW_LOC;
		NOR=NEW_NOR;
		
		if(verbose){
			cout << "Pyramid trim complete. Now " << ADJ.size() << " vertices\n";
		};
		generate_face_list();
	} else {
		if(verbose){
			cout << "Found no isolated 3-valent vertex.\n";
		};
	};
};

void mesh::curvature_flow(){	
	/* Based on the ``shrinkwrapping'' method of Kobbelt-Vorsatz-Labsik-Seidel
	replaces each vertex with a weighted average of itself and its 
	neighbors; neighbor weights are proportional to the average edge 
	length of edges incident to them. This has the effect that edge lengths
	tend to normalize (still, singularities can develop).
	
	For boundaries, just take a weighted average of two boundary 
	neighbors. This makes boundaries ``stiff''. */
	
	int i,j,k,l,valence;
	double v;
	double t,T;
	vpoint NEW_LOC;
	point P,Q;
	
	NEW_LOC=LOC;
	
	for(i=0;i<(int) ADJ.size();i++){	// for each vertex
		if(boundary_edge(i)>=0){	// if we are a boundary edge
			j=ADJ[i][(boundary_edge(i)-1+(int) ADJ[i].size()) % (int) ADJ[i].size()];
			k=ADJ[i][(boundary_edge(i)+1) % (int) ADJ[i].size()];
			NEW_LOC[i]=(LOC[i]*0.9)+(LOC[j]*0.05)+(LOC[k]*0.05);
		} else {
			P=LOC[i];
			Q=newpoint(0.0,0.0,0.0);	// initialize Q
			T=0.0;	
			for(j=0;j<(int) ADJ[i].size();j++){
				k=ADJ[i][j];
				valence=(int) ADJ[k].size();
				v=(double) valence;
				if(boundary_edge(k)>=0){
					v=v-1.0;
				};
				t=0.0;
				for(l=0;l<valence;l++){
					if(ADJ[k][l]!=-1){
						t=t+norm(LOC[k]-LOC[ADJ[k][l]]);
					};
				};
				t=t/v;	// average edge length at vertex k
				Q=Q+(LOC[k]*t);
				T=T+t;
			};
			Q=Q/T;
			NEW_LOC[i]=(P*0.5)+(Q*0.5);
		};
	};
	LOC=NEW_LOC;

	compute_normals();
};

void mesh::draw_mesh(){
	if(WIRE){
		wire_draw();
	} else {
		face_draw();
	};
};

void mesh::wire_draw(){
	int i,j,k;
	for(i=0;i<(int) ADJ.size();i++){
		for(j=0;j<(int) ADJ[i].size();j++){
			k=ADJ[i][j];
			if(k>i){	// only draw edge once
				draw_edge(LOC[i],LOC[k]);
			};
		};
//		draw_thin_edge(LOC[i],LOC[i]+NOR[i]*0.1);	// draw normals (a bit silly)
	};
};

void mesh::sort_faces(){
	vector < pair < int, double > > height_list;
	pair <int,double> single_height;
	int i;
	imat NEW_FAC;
	
	height_list.resize(0);
	for(i=0;i<(int) FAC.size();i++){
		single_height.first=i;
		single_height.second=(LOC[FAC[i][0]][2]/3.0) + (LOC[FAC[i][1]][2]/3.0) + (LOC[FAC[i][2]][2]/3.0);
		height_list.push_back(single_height);
	};
	
	height_list=sort(height_list);	// quicksort by z coordinates
	NEW_FAC.resize(0);
	for(i=0;i<(int) height_list.size();i++){
		NEW_FAC.push_back(FAC[height_list[i].first]);
	};
	FAC=NEW_FAC;
};

void mesh::face_draw(){
	int i,j,k,l;
	sort_faces();
	for(i=0;i<(int) FAC.size();i++){
		j=FAC[i][0];
		k=FAC[i][1];
		l=FAC[i][2];
		if(isobar==true){
			draw_stripy_triangle(LOC[j],LOC[k],LOC[l],iheight);
		} else {
			draw_triangle(LOC[j],LOC[k],LOC[l],incolor,outcolor);
		};
	};
};