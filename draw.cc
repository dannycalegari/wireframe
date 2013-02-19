/* draw.cc	*/

ipoint point_to_ipoint(point P){	// project 3d point to 2d ipoint
	ipoint p;
	p.x = (int) ((double) (SCREEN/2) + P[0]*200.0);	// perspective: P[0]*(200.0+P[2]*50.0)
	p.y = (int) ((double) (SCREEN/2) + P[1]*200.0);	// perspective: P[1]*(200.0+P[2]*50.0)
	return(p);
};

void draw_edge(point P, point Q){
	ipoint p,q;
	p=point_to_ipoint(P);
	q=point_to_ipoint(Q);
	draw_line(p,q,0);
};

void draw_thin_edge(point P, point Q){
	ipoint p,q;
	p=point_to_ipoint(P);
	q=point_to_ipoint(Q);
	draw_faint_line(p,q,0);
};

void draw_triangle(point P, point Q, point R, point incolor, point outcolor){
	ipoint p,q,r;
	point N;
	int RR,G,B;
	long col;
	
	N=(Q-P)*(R-P);
	N=N/norm(N);
	if(N[2]>0){	// use outcolor; gives numbers between 50 and 200 for each of R G B
		RR=(int) 50.0+(150.0*N[2]*outcolor[0]);
		G=(int) 50.0+(150.0*N[2]*outcolor[1]);
		B=(int) 50.0+(150.0*N[2]*outcolor[2]);
	} else { // use incolor; gives numbers between 50 and 200 for each of R G B
		RR=(int) 50.0-(150.0*N[2]*incolor[0]);
		G=(int) 50.0-(150.0*N[2]*incolor[1]);
		B=(int) 50.0-(150.0*N[2]*incolor[2]);	
	};
	col = (long) ((RR*65536)+(G*256)+B);

	p=point_to_ipoint(P);
	q=point_to_ipoint(Q);
	r=point_to_ipoint(R);
	draw_triangle(p,q,r,col);	
};

void draw_stripy_triangle(point P, point Q, point R, double iheight){
	ipoint p,q,r;
	point N,M;
	int x,z,i;
	double I,s,t;
	
	p=point_to_ipoint(P);
	q=point_to_ipoint(Q);
	r=point_to_ipoint(R);
	draw_triangle(p,q,r,16777215);	// draw white triangle	
	
	// first, sort points
	if(P[2]<Q[2]){
		N=P;
		P=Q;
		Q=N;
	};
	if(P[2]<R[2]){
		N=P;
		P=R;
		R=N;
	};
	if(Q[2]<R[2]){
		N=Q;
		Q=R;
		R=N;
	};
	// now P[2]>=Q[2]>=R[2]
	assert(P[2]>=Q[2]);
	assert(Q[2]>=R[2]);
	
	if(P[2]>=0.0){
		x=(int) (P[2]/iheight);
	} else {
		x=(int) (P[2]/iheight)-1;
	};
	if(R[2]>=0.0){
		z=(int) (R[2]/iheight);
	} else {
		z=(int) (R[2]/iheight)-1;
	};
	for(i=z+1;i<=x;i++){
		I= ((double) i)*iheight;
		assert(R[2]<=I);
		assert(I<=P[2]);
		if(I<=Q[2]){
			s=(I-R[2])/(P[2]-R[2]);	// proportion of the way from R to P
			t=(I-R[2])/(Q[2]-R[2]);	// proportion of the way from R to Q
			N=((1.0-s)*R)+(s*P);
			M=((1.0-t)*R)+(t*Q);
			draw_edge(N,M);
		} else {
			s=(I-R[2])/(P[2]-R[2]);	// proportion of the way from R to P
			t=(I-Q[2])/(P[2]-Q[2]);	// proportion of the way from Q to P
			N=((1.0-s)*R)+(s*P);
			M=((1.0-t)*Q)+(t*P);
			draw_edge(N,M);
		};
	};
};

void draw_stripy_eps_triangle(point P, point Q, point R, double iheight, ofstream &output_file){
	ipoint p,q,r;
	point N,M;
	int x,z,i;
	double I,s,t;
	
	p=point_to_ipoint(P);
	q=point_to_ipoint(Q);
	r=point_to_ipoint(R);
	
	// first, sort points
	if(P[2]<Q[2]){
		N=P;
		P=Q;
		Q=N;
	};
	if(P[2]<R[2]){
		N=P;
		P=R;
		R=N;
	};
	if(Q[2]<R[2]){
		N=Q;
		Q=R;
		R=N;
	};
	// now P[2]>=Q[2]>=R[2]
	assert(P[2]>=Q[2]);
	assert(Q[2]>=R[2]);
	
	if(P[2]>=0.0){
		x=(int) (P[2]/iheight);
	} else {
		x=(int) (P[2]/iheight)-1;
	};
	if(R[2]>=0.0){
		z=(int) (R[2]/iheight);
	} else {
		z=(int) (R[2]/iheight)-1;
	};
	for(i=z+1;i<=x;i++){
		I= ((double) i)*iheight;
		assert(R[2]<=I);
		assert(I<=P[2]);
		if(I<=Q[2]){
			s=(I-R[2])/(P[2]-R[2]);	// proportion of the way from R to P
			t=(I-R[2])/(Q[2]-R[2]);	// proportion of the way from R to Q
			N=((1.0-s)*R)+(s*P);
			M=((1.0-t)*R)+(t*Q);
			output_file << N[0] << " " << -N[1] << " " << M[0] << " " << -M[1] << " l\n";
		} else {
			s=(I-R[2])/(P[2]-R[2]);	// proportion of the way from R to P
			t=(I-Q[2])/(P[2]-Q[2]);	// proportion of the way from Q to P
			N=((1.0-s)*R)+(s*P);
			M=((1.0-t)*Q)+(t*P);
			output_file << N[0] << " " << -N[1] << " " << M[0] << " " << -M[1] << " l\n";
		};
	};
};