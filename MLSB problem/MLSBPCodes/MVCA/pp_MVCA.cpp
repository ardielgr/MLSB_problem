void Graph::pp_MVCA(Graph G) {
	int num,numC,numB;
	int i,j,p,k,Add_p;
	int flag;
	int temp[CN];
   edge *e;
   Adj_item *a,*aa;
   int label_app[CN][VN]; // support variable
   int label_appB[CN][VN]; // support variable
   int Labeli,Labelj,Labelk;
   int app_Num_Comp[CN];
   int app_Num_Blocks[CN];
   int path[vn];
   int Queue[vn];
   int q,Qlast;
   int ii; // index for the "for" sentence

   Temporary_Vector=new int[G.cn];

   if (sol_cn!=0) RemoveAllColors();
   
   numC = vn ;
   numB = vn ;
   num = numC + numB ; 

   //Add_p=0;
   flag=1;
	for (p=0; p<G.cn; p++)
      if (G.L[p].root==NULL) 
			temp[p]=1;
      else temp[p]=0;
      
	for (int i=0; i<vn; i++) 
		Adj[i]=NULL ;
		
   while (flag==1) {
		dim_Temp=0;    
   	for (p=0; p<G.cn; p++) {

     	if (temp[p]==1) continue;
      for (i=0; i<vn; i++) {
      	label_app[p][i]=label[i];
      	label_appB[p][i]=labelB[i];
			}
         
      app_Num_Comp[p]=NumComps;
      app_Num_Blocks[p]=NumBlocks;

		e=G.L[p].root;
      while (e!=NULL) {
			i=e->u;
         j=e->v;
      	if (i==j) {
       		e=e->next;
         	continue;
				}
      		
      	if(label_app[p][i]!=label_app[p][j]) {
            app_Num_Comp[p]=app_Num_Comp[p]-1;
         	Labeli=label_app[p][i];
         	Labelj=label_app[p][j];
         	for (k=0; k<vn; k++) {	
					if (label_app[p][k]==Labelj)	
		 				label_app[p][k]=Labeli;
         		}
      		}
      	else {
				if(label_appB[p][i]!=label_appB[p][j]) {

					// BFS for the shortest path from i to j
      			for (k=0; k<vn; k++)	
         			path[k]=vn;
         		path[j]=j;
         		Queue[0]=j;
         		q = 0 ;
         		Qlast = 0 ;
         		do{
         			a = Adj[Queue[q]];
         			while (a!=NULL){
         				if (path[a->v]==vn){
         					path[a->v]=Queue[q];
         					Qlast++;
         					Queue[Qlast]=a->v;
								}
         				a=a->next ;
							}
         				q++;
						} while (path[i]==vn);
					// End of the BFS
         		// The path is i, path[i], path[path[i]], ..., j
         		         		
         		// Traverse the path from i to j joining the blocks found
      			Labeli = label_appB[p][i];
         		k = i ;
      			do{
				  		k = path[k] ;
				  		if(label_appB[p][k]!=Labeli) {
							app_Num_Blocks[p] = app_Num_Blocks[p] - 1;
				  			Labelk = label_appB[p][k] ;
				  			for(int q=0; q<vn; q++)
				  				if (label_appB[p][q]==Labelk)
				  					label_appB[p][q]= Labeli ;
							}
						} while (k!=j);
				   if (Demo==1) printf(" Num of Blocks down to: %d",app_Num_Blocks[p]) ;
      			} // END OF if(label_appB[p][i]!=label_appB[p][j]){
      		} // END of ELSE
      		
      	a=new Adj_item;
			a->v=i;
        	a->c=p;
        	a->next=Adj[j];
        	Adj[j]=a;
        
      	a=new Adj_item;
			a->v=j;
        	a->c=p;
        	a->next=Adj[i];
        	Adj[i]=a;
              		
      	e=e->next;
         } // END of WHILE e!= NULL
         
      if (app_Num_Comp[p] + app_Num_Blocks[p]<num) {
         Temporary_Vector[0]=p;
         dim_Temp=1;
				//Add_p=p;
         numC = app_Num_Comp[p] ;
         numB = app_Num_Blocks[p] ;
         num = numC + numB;
         }
      else{
         if (((app_Num_Comp[p] + app_Num_Blocks[p])==num)&&(dim_Temp!=0)) {
            	Temporary_Vector[dim_Temp]=p;
         		dim_Temp++;
            }
         }
         
      // Deleting color p from Adjacency lists
         for (int i=0; i<vn; i++){
         	while(Adj[i]!=NULL){
         		if (Adj[i]->c == p) {
         			a = Adj[i];
						Adj[i] = Adj[i]->next;
         	  		delete[] a;
						}
					else break;
					}
				if (Adj[i]!=NULL){
         		a = Adj[i] ;
         		while((a->next)!=NULL){
         			if ((a->next)->c == p) {
         				aa = a->next ;
							a->next = aa->next ;
         	  			delete[] aa;
         	  			}
						else 
							a=a->next;
						}
					}
				}
      } // End of for p

   Add_p=Temporary_Vector[rand()%dim_Temp];
   AddColor(G,Add_p);
   temp[Add_p]=1;
   numC = app_Num_Comp[Add_p] ;
   numB = app_Num_Blocks[Add_p] ;   
   NumComps=numC;
   NumBlocks=numB;

   for (i=0; i<vn; i++) {
      label[i]=label_app[Add_p][i];
      labelB[i]=label_appB[Add_p][i];
   }
      
	e=G.L[Add_p].root;
   while (e!=NULL) {
		i=e->u;
      j=e->v;
      if (i==j) {
       	e=e->next;
         continue;
      	}      		
      a=new Adj_item;
		a->v=i;
      a->c=Add_p;
      a->next=Adj[j];
      Adj[j]=a;
        
      a=new Adj_item;
		a->v=j;
   	a->c=Add_p;
      a->next=Adj[i];
      Adj[i]=a;
      e = e->next; 
	}      
      if (NumBlocks==1) flag=0;
	} // END OF WHILE flag == 1
}
