/* sort.cc	quicksort implementation for vector of <int,double> sorted by second index */

vector < pair < int, double > > sort(vector < pair < int, double > > H){
	vector < pair < int, double > > L, G;
	pair < int, double > P;
	int i,j;
	
	if(H.size()<=1){	// base case of induction
		return(H);
	} else {
		i=(int) H.size();
		P=H[i/2];	// choose a pivot
		L.resize(0);
		G.resize(0);
		for(j=0;j<(int) H.size();j++){	// for each element of H
			if(j!=i/2){		// not the pivot!
				if(H[j].second<=P.second){
					L.push_back(H[j]);
				} else {
					G.push_back(H[j]);
				};
			};
		};
		L=sort(L);	// sort left sublist
		G=sort(G);	// sort right sublist
		L.push_back(P);	// L becomes L+P
		L.insert( L.end(), G.begin(), G.end() );	// L+P becomes L+P+G
		return(L);	// return L+P+G
	};
};