/* algebra.cc	*/

point operator+(point U, point V){
	point W;
	int i;
	for(i=0;i<3;i++){
		W[i]=U[i]+V[i];
	};
	return(W);
};

point operator-(point U, point V){
	point W;
	int i;
	for(i=0;i<3;i++){
		W[i]=U[i]-V[i];
	};
	return(W);
};

point operator*(point U, point V){	// cross product
	point W;
	W[0]=U[1]*V[2]-U[2]*V[1];
	W[1]=-U[0]*V[2]+U[2]*V[0];
	W[2]=U[0]*V[1]-U[1]*V[0];
	return(W);
};

double dot_product(point U, point V){	// dot product
	return(U[0]*V[0] + U[1]*V[1] + U[2]*V[2]);
};

double norm(point P){
	return(sqrt(P[0]*P[0]+P[1]*P[1]+P[2]*P[2]));
};

double distance(point P, point Q){
	return(norm(Q-P));
};

double area(point P, point Q, point R){
	return(norm((Q-P)*(R-P))/2.0);
};

mat operator*(mat M, mat N){
	mat O;
	int i,j,k;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			O[i][j]=0.0;
			for(k=0;k<3;k++){
				O[i][j]=O[i][j]+(M[i][k]*N[k][j]);
			};
		};
	};
	return(O);
};

point operator*(mat M, point U){
	point V;
	int i,j;
	for(i=0;i<3;i++){
		V[i]=0;
		for(j=0;j<3;j++){
			V[i]=V[i]+(M[i][j]*U[j]);
		};
	};
	return(V);
};

point operator*(point U, double d){
	point V;
	int i;
	for(i=0;i<3;i++){
		V[i]=U[i]*d;
	};
	return(V);
};

point operator*(double d, point U){
	return(U*d);
};

point operator/(point U, double d){
	return(U*(1.0/d));
};

mat XRmat(double A){
	mat M;
	M[0][0]=1.0;
	M[0][1]=0.0;
	M[0][2]=0.0;
	M[1][0]=0.0;
	M[1][1]=cos(A);
	M[1][2]=sin(A);
	M[2][0]=0.0;
	M[2][1]=-sin(A);
	M[2][2]=cos(A);
	return(M);
};

mat YRmat(double A){
	mat M;
	M[0][0]=cos(A);
	M[0][1]=0.0;
	M[0][2]=sin(A);
	M[1][0]=0.0;
	M[1][1]=1.0;
	M[1][2]=0.0;
	M[2][0]=-sin(A);
	M[2][1]=0.0;
	M[2][2]=cos(A);
	return(M);
};

mat ZRmat(double A){
	mat M;
	M[0][0]=cos(A);
	M[0][1]=sin(A);
	M[0][2]=0.0;
	M[1][0]=-sin(A);
	M[1][1]=cos(A);
	M[1][2]=0.0;
	M[2][0]=0.0;
	M[2][1]=0.0;
	M[2][2]=1.0;
	return(M);
};

mat Dmat(double t){
	mat M;
	M[0][0]=t;
	M[0][1]=0.0;
	M[0][2]=0.0;
	M[1][0]=0.0;
	M[1][1]=t;
	M[1][2]=0.0;
	M[2][0]=0.0;
	M[2][1]=0.0;
	M[2][2]=t;
	return(M);
};


point newpoint(double x, double y, double z){
	point V;
	V[0]=x;
	V[1]=y;
	V[2]=z;
	return(V);
};

point random_point(){
	point V;
	V[0]=(((double) (rand() % 200))/100.0)-1.0;
	V[1]=(((double) (rand() % 200))/100.0)-1.0;
	V[2]=(((double) (rand() % 200))/100.0)-1.0;
	return(V);
};
