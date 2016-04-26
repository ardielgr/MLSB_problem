// Backtracking for the Minimum Labeling Block (MLP) problem by Pepe
// including MVCA and GRASP

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <conio.h>

// #define Size 625	// maximum number of vertices and maximum number of colors

#define Size 100	// maximum number of vertices and maximum number of colors
#define Size2 10000	// maximum number of vertices and maximum number of colors

typedef int Vector[Size];

typedef struct Adj_Item_type {
	int v; // the other vertex
	int c; // color
	struct Adj_Item_type *next; // next Adj item
} Adj_item;

typedef struct Edge_R {
	int u,v;	// vertices
    //int en;	// number of edges
   struct Edge_R *Next;
} Edge_R, *Edge_T;

typedef int Matrix[Size][Size];

int n,c;	// n = total number of vertices, c = total number of colours
int Colors[Size]; // To reorganize the array of colours by Quick Sort
Matrix Color_Matrix;
Vector Best_Sol;   // Vector with colours in the Best Sol
int Best_Num_Sol;  // Size of the Best Sol
Edge_T Edges[Size]; // List of edges with each colour
Vector Freq;        // Vector with the "frequency" of each colour; number of edges
Adj_item *Adj[Size]; // Adjacency lists for each vertex

Vector MVCA_Sol;

int Demo;

long int call,Best_call;
int previous_call;

FILE *Data_File;
FILE *Results_File;
FILE *Backtrack_File;




int ReadNumber(FILE *fp) {
	char buf[10];
	char c;
	int i,nn;
	int num;
	
	c=fgetc(fp);
	i=0;
	while (!feof(fp) && (c<'0' || c>'9')) c=fgetc(fp);
	while (!feof(fp) && (c>='0' && c<='9')) {
		buf[i]=c;
      c=fgetc(fp);
      i++;
	}
	num=0;
	nn=i;
	for (i=0; i<nn; i++) {
		num=num*10+(int)(buf[i]-'0');
	}

   return num;
}


void Read_Data(){
	int i,j,k;

   for (i=0; i<n-1; i++){	//my convenction, vertexes da from a n-1
   	for(j=i+1; j<n; j++){	//my convenction, vertexes da from a n-1
      	k=ReadNumber(Data_File);
         Color_Matrix[i][j]=k;
       }
   }
   if (Demo==1){
   	printf("Color_Matrix:\n");
   	for (i=0; i<n-1; i++){
   		for(j=i+1; j<n; j++){
      		printf("%d ",Color_Matrix[i][j]);
      	}
      	printf("\n");
   	}
   	printf("\n");
      getch();
   }
}


// Quick Sort of colours by their frequency in vector Color
void q_sortGF(int left, int right) {  
  	int pivot, index_pivot, l_hold, r_hold;
   Edge_T Edge;
   int Color_support, Freq_support;

   for (int k=0; k<c; k++){
   	Colors[k]=k;
   }

   	l_hold = left;
	r_hold = right;
   	index_pivot=right;

   	Edge=Edges[index_pivot];
   	Color_support=Colors[index_pivot];
   	Freq_support=Freq[index_pivot];

   	pivot = Freq[index_pivot];

   	while (left < right){
	   	while ((Freq[left] >= pivot) && (left < right))	left++;
    	if (right != left){
         	Colors[right]=Colors[left];
         	Edges[right]=Edges[left];
         	Freq[right]=Freq[left];
         	right--;
    	}
      	while ((Freq[right] <= pivot) && (left < right))	right--;
    	if (right != left){
         	Colors[left]=Colors[right];
      		Edges[left]=Edges[right];
         	Freq[left]=Freq[right];
         	left++;
    	}
  	}
   //Colors[right]=Colors[index_pivot];
   //Edges[right]=Edges[index_pivot];
   //Freq[right]=Freq[index_pivot];

   	Colors[right]=Color_support;
   	Edges[right]=Edge;
   	Freq[right]=Freq_support;

   	pivot = right;
   	left = l_hold;
  	right = r_hold;
   /*
   int k;
   if (Demo==1){
   	printf("\n");
   	printf("\nSort List: ");
   	printf("\ncolor(frequency): ");
		for (k=0; k<cn; k++) printf("%d(%d) ",L[k].c,L[k].en);
		printf("\n\n");
   	getch();
   }
   */
   	if (right > pivot){
   		q_sortGF(pivot+1, right);
   	}
   	if (left < pivot)	{
   		q_sortGF(left, pivot-1);
	}	
}


// Gets the list of edges for each colour and compute frequencies
void List_Colors(){  //
	int i,j,k;
   Edge_T New_Edge;

   for (i=0; i<c; i++){
	Edges[i] = NULL;
    Freq[i] = 0;
   }
   for (i=0; i<n-1; i++){
	for (j=i+1; j<n; j++){
        k = Color_Matrix[i][j]; // k = 1 + Color_Matrix[i][j];	if colors from 1 to c
    	if ( (k>=0) && (k<c) ){
         	New_Edge = new Edge_R;
         	New_Edge->u = i;
            New_Edge->v = j;
            New_Edge->Next = Edges[k];
            Edges[k] = New_Edge;
            Freq[k] = Freq[k] + 1;
        }
    }
   }

   if (Demo==1){
      printf("List Colors:\n");
      for (k=0; k<c; k++){
    	printf("%d(%d): ",k,Freq[k]);
        New_Edge=Edges[k];
    	while (New_Edge!=NULL){
      	  	printf("[%d,%d] ",New_Edge->u,New_Edge->v);
            New_Edge = New_Edge->Next;
        }
      	printf("\n");
      }
      printf("\n");
   	getch();
   }

   if (Demo==1){
   	printf("\ncolor(frequency): ");
	for (i=0; i<c; i++) printf("%d(%d) ",i,Freq[i]);
	printf("\n");
   	getch();
   }

   q_sortGF(0, c - 1);

   if (Demo==1){
   	printf("\n-- SORTED -- color(frequency): ");
	for (i=0; i<c; i++) printf("%d(%d) ",Colors[i],Freq[i]);
	printf("\n\n");
   	getch();
   }
}


void Print_Summary(	
					int Labels[], int Num_Comp, 
					int LabelsB[], int Num_Blocks,
					int Labels2[], int Num_VBlocks,
					int Sol[], int Num_Sol){
	int i,j;

   if (Demo==1){
   	printf("\n----> PRINT SUMMARY:");
   }
   printf("\n Best SOLUTION = %d;\n",Best_Num_Sol);
   printf("\n Number of colors = %d;",Num_Sol);
   printf("\n S[%d] : ",Num_Sol);  for (i=0; i<Num_Sol; i++) printf("%d ",Sol[i]);
   
   printf("\n Number of VBlocks: %d ",Num_VBlocks);
   printf("\n Number of Blocks: %d ",Num_Blocks);
   printf("\n Number of Connected Components %d;",Num_Comp);
   
   printf("\n Vertex: "); for (i=0; i<n; i++)	printf("%d ",i);
   printf("\n Label:  ");  for (i=0; i<n; i++)	printf("%d ",Labels[i]);
   printf("\n LabelB: ");  for (i=0; i<n; i++)	printf("%d ",LabelsB[i]);
   
   printf("\n Labels2: ");  
   	for (i=0; i<n; i++){
   		printf("\n     %d: ",i);  
   		for(j=0; j<n; j++)
      		printf("%d ",Labels2[i*n+j]);
      	}
    printf("\n");
   
   printf("\n Calls: %d;  Best_Calls: %d;\n",call,Best_call);
   printf("\n");
	getch();
}


int Try_Add_Color(	int Position_Color, 
					int Labels[], int *Num_Comp, 
					int LabelsB[], int *Num_Blocks, 
					int Labels2[], int *Num_VBlocks,
					int Sol[], int *Num_Sol){
	int i,j,k;
   	Edge_T Edge;
   	int Labels_support_Add[Size];
   	int LabelsB_support_Add[Size];
   	int Labels2_support_Add[Size2];
   	
	int added;
   	
	int Labeli,Labelj,Labelk;
	int Label2i,Label2j,Label2k;
   	int app_Num_Comp;
   	int app_Num_Blocks;
   	int app_Num_VBlocks;

	int path[Size];
   	int Queue[Size];
   	int q,Qlast;
   	Adj_item *a,*aa;
   

   app_Num_Comp = *Num_Comp;
   app_Num_Blocks = *Num_Blocks;
   app_Num_VBlocks = *Num_VBlocks;
      
   for (i=0; i<n; i++)	Labels_support_Add[i]=Labels[i];
   for (i=0; i<n; i++)	LabelsB_support_Add[i]=LabelsB[i];
   for (i=0; i<n; i++)	for (j=0; i<n; j++)	Labels2_support_Add[i*n+j]=Labels2[i*n+j];


   //call=call+1;

	if (Demo==1){
   		printf(" Adding %d, position=%d\n",Colors[Position_Color],Position_Color);
   		Print_Summary(	Labels,*Num_Comp,
	   					LabelsB,*Num_Blocks,
	   					Labels2,*Num_VBlocks,
	   					Sol,*Num_Sol);
   }
   Edge=Edges[Position_Color];
   while (Edge!=NULL){
	  i=Edge->u;
   	  j=Edge->v;
   		   		
      if (Labels[i]!=Labels[j]){
      	*Num_Comp=*Num_Comp-1;
         Labeli=Labels[i];
         Labelj=Labels[j];
         for (k=0; k<n; k++){	// := 1 to N
         	if (Labels[k]==Labelj)
            	   Labels[k]=Labeli;
      	}
      	Labels2[i*n+j]=i*n+j;
      }
      else {																							 
		if(LabelsB[i]!=LabelsB[j]) {
				// BFS for the shortest path from i to j
      	   for (k=0; k<n; k++)	
         		path[k]=n;
         	path[j]=j;
         	Queue[0]=j;
         	q = 0 ;
         	Qlast = 0 ;
         	do{
         		a = Adj[Queue[q]];
         		while (a!=NULL){
         			if (path[a->v]==n){
         				path[a->v]=Queue[q];
         				Qlast++;
         				Queue[Qlast]=a->v;
					}
         			a=a->next ;
				}
         			q++;
			} while (path[i]==n);
					// End of the BFS
         		// The path is i, path[i], path[path[i]], ..., j
         		
         		// Traverse the path from i to j joining the blocks found
      		Labeli = LabelsB[i];
      		Label2i = Labels2[k*n+path[k]] ;
         	k = i ;
      		do{
      			if (Labels2[k*n+path[k]] != Label2i){
					*Num_VBlocks = *Num_VBlocks - 1;
      				Label2k = Labels2[k*n+path[k]];
				  	for(int q=0; q<n; q++)
				  		for(int qq=0; qq<n; qq++)
				  		if (Labels2[q*n+qq]==Label2k)
				  			Labels2[q*n+qq]= Label2i ;
      			}
      			
				k = path[k] ;
				if(LabelsB[k]!=Labeli) {
					*Num_Blocks = *Num_Blocks - 1;
				  	Labelk = LabelsB[k] ;
				  	for(int q=0; q<n; q++)
				  		if (LabelsB[q]==Labelk)
				  			LabelsB[q]= Labeli ;
				}
			} while (k!=j);
			Labels2[i*n+j]=Label2k;
		} // END OF if(label_appB[p][i]!=label_appB[p][j]){
					
      } // END OF ELSE
      
    a=new Adj_item;
			a->v=i;
        	a->c=Position_Color;
        	a->next=Adj[j];
        	Adj[j]=a;
        
    a=new Adj_item;
			a->v=j;
        	a->c=Position_Color;
        	a->next=Adj[i];
        	Adj[i]=a;
              		
   	Edge=Edge->Next;
   }

   if (Demo==1){
   	printf(" Num_VBlocks = %d\n",*Num_VBlocks);
   	printf(" Num_Blocks = %d\n",*Num_Blocks);
   	printf(" Num_Comp   = %d\n",*Num_Comp);
   }
   if (((*Num_VBlocks<app_Num_VBlocks)&&(*Num_Sol<Best_Num_Sol-2))
      || ((*Num_Sol==Best_Num_Sol-2)&&(*Num_VBlocks==1)) ) {
      *Num_Sol=*Num_Sol+1;
   	Sol[*Num_Sol-1]=Colors[Position_Color];
      call=call+1;
      added=1;
      if (Demo==1){
         printf("\n --> ADDED COLOR %d, position=%d, --> Num_Sol= %d\n",Colors[Position_Color],Position_Color,*Num_Sol);
   		Print_Summary(	Labels,*Num_Comp,
		   				LabelsB,*Num_Blocks,
						Labels2,*Num_VBlocks,
						Sol,*Num_Sol);
      	printf("----> added=%d, End Adding %d, position=%d\n",added,Colors[Position_Color],Position_Color);
   		getch();
   	}
   }
   else{
      added=0;
      if (Demo==1){
   		if ((*Num_VBlocks<app_Num_VBlocks)&&(*Num_Sol<Best_Num_Sol-2))	
		   		printf(" Don't add %d because it doesn't decrease the number of Blocks %d!\n",
		   		Colors[Position_Color],app_Num_Blocks);
         else	if ((*Num_Sol==Best_Num_Sol-2)&&(*Num_VBlocks!=1))	
		 			printf(" Don't add %d, position=%d, because it doesn't give a solution! \n",
					 Colors[Position_Color],Position_Color);
   		Print_Summary(	Labels,*Num_Comp,
		   				LabelsB,*Num_Blocks,
		   				Labels2,*Num_VBlocks,
		   				Sol,*Num_Sol);
      	printf("----> added=%d, End Adding %d, position=%d\n",added,Colors[Position_Color],Position_Color);
   		getch();
   	}
      *Num_Comp=app_Num_Comp;
      *Num_Blocks=app_Num_Blocks;
      *Num_VBlocks=app_Num_VBlocks;
      for (i=0; i<n; i++)	Labels[i]=Labels_support_Add[i];
      for (i=0; i<n; i++)	LabelsB[i]=LabelsB_support_Add[i];
      for (i=0; i<n; i++)	for (j=0; j<n; j++) 	Labels2[i*n+j]=Labels2_support_Add[i*n+j];
      
      // Delete edjes with color "Position_Color"
         if (Demo==1) printf("\n Deleting color %d :",Position_Color);
         for (int i=0; i<n; i++){
         	while(Adj[i]!=NULL){
         		if (Adj[i]->c == Position_Color) {
         			a = Adj[i];
						Adj[i] = Adj[i]->next;
         	  		delete[] a;
						}
					else break;
					}
				if (Adj[i]!=NULL){
         		a = Adj[i] ;
         		while((a->next)!=NULL){
         			if ((a->next)->c == Position_Color) {
         				aa = a->next ;
							a->next = aa->next ;
         	  			delete[] aa;
         	  			}
						else 
							a=a->next;
						}
					}
				}
         if (Demo==1) printf("\n Deleted color %d :",Position_Color);


   }

   if (((call%1000)==0)&&(previous_call!=call)){
      previous_call=call;
	  	printf("call %d S[%d] ",call,*Num_Sol);
   	for (i=0; i<*Num_Sol; i++){	//i := 1 to Num_Sol
   		printf("%d ",Sol[i]);
      }
      printf("\n Blocks: %d. Comp: %d\n",*Num_Blocks,*Num_Comp);
   }

   return added;

}


void Try(int Position_Color, 
			int Labels[], int Num_Comp, 
			int LabelsB[], int Num_Blocks, 
			int Labels2[], int Num_VBlocks, 
			int Sol[], int Num_Sol){
   int k,i,j;
   int continue_to_try;
   int app_Labels[Size];
   int app_LabelsB[Size];
   int app_Labels2[Size2];

	if (Demo==1){
   	printf("\nTRY COLOR: %d, position=%d, with Freq %d\n",Colors[Position_Color],Position_Color,Freq[Position_Color]);
   	Print_Summary(	Labels,Num_Comp,
	   				LabelsB,Num_Blocks,
	   				Labels2,Num_VBlocks,
	   				Sol,Num_Sol);
   }
   continue_to_try=Try_Add_Color(Position_Color,
   						Labels,&Num_Comp,
						LabelsB,&Num_Blocks,
					   	Labels2,&Num_VBlocks,
					   	Sol,&Num_Sol);
   if (Num_VBlocks==1){
    	for (k=0; k<Num_Sol; k++){	// := 1 to Num_Sol
   		Best_Sol[k]=Sol[k];
      }
   	Best_Num_Sol=Num_Sol;
      Best_call=call;
      printf(" \n ********************************* BEST SOL %d\n",Best_Num_Sol);
      if (Demo==1){
   		Print_Summary(	Labels,Num_Comp,
	   					LabelsB,Num_Blocks,
	   					Labels2,Num_VBlocks,
	   					Sol,Num_Sol);
      }
   }
   else{
      if (continue_to_try==1){
      	for (i=0; i<n; i++)	app_Labels[i]=Labels[i];
      	for (i=0; i<n; i++)	app_LabelsB[i]=LabelsB[i];
        for (i=0; i<n; i++)	for (j=0; j<n; j++)	app_Labels2[i*n+j]=Labels2[i*n+j];
        	k=Position_Color+1;
        	while((k<c)&&(Num_Sol<Best_Num_Sol-1)){
         		if ((Num_Sol==Best_Num_Sol-2)&&(Freq[k]<(Num_Comp-1)))	break;
            	for (i=0; i<n; i++) Labels[i]=app_Labels[i];
         		for (i=0; i<n; i++) LabelsB[i]=app_LabelsB[i];
         	    for (i=0; i<n; i++) for (j=0; j<n; j++) Labels2[i*n+j]=app_LabelsB[i*n+j];
         	    Try(k,	
					Labels,Num_Comp,
					LabelsB,Num_Blocks,
					Labels2,Num_VBlocks,
					Sol,Num_Sol);
            k=k+1;
            if (Demo==1){
            	if(k<c){
            		printf(" \n Continue Inner_Try of %d, position=%d, with the next color %d, position=%d\n",Colors[Position_Color],Position_Color,Colors[k],k);
            	}
            }
         }
      }
   }
   if (Demo==1){
   	if (continue_to_try==0)	printf("----> End Try color %d, position=%d\n",Colors[Position_Color],Position_Color);
      else printf(" \n End Inner_Try of %d, position=%d, with color %d, position=%d\n",Colors[Position_Color],Position_Color,k,Colors[k]);
   	getch();
   }
}


void initial_MVCA(int Demo){
	int num,numB,numC,i,j,k,p,Add_p;
   int NumComps_app[Size];  // support variable
   int NumBlocks_app[Size];  // support variable
   int NumVBlocks_app[Size];  // support variable
   int temp[Size];
   int label_temp[Size][Size]; // support variable
   int labelB_temp[Size][Size]; // support variable
   int label2_temp[Size][Size2]; // support variable
   Edge_T e;
   int Labeli, Labelj, Labelk;
   int path[Size];
   int Queue[Size];
   int q,Qlast;
   Adj_item *a,*aa;

   if (Demo==1){
   	printf("--- MVCA ---\n\n\n");
   	getch();
   }
   num=n+n;
   numC=n;
   numB=n;
   numVB=n
   Add_p=0;
   for (p=0; p<c; p++){
   	NumComps_app[p]=numC;
   	NumBlocks_app[p]=numB;
    NumVBlocks_app[p]=numVB;
      if (Edges[p]==NULL) temp[p]=1;
      else temp[p]=0;
      for (i=0; i<n; i++)	label_temp[p][i]=i;
      for (i=0; i<n; i++)	labelB_temp[p][i]=i;
      for (i=0; i<n; i++)	for (j=0; j<n; j++)	label2_temp[p][i]*n+j=i*n+j;
      if (Demo==1)	printf("position=%d, color=%d, temp[p]=%d\n",p,Colors[p],temp[p]);
   }
   if (Demo==1)	getch();
   for (i=0;i<n;i++)
   Adj[i] = NULL ;
   while (numB!=1) {
   	for (p=0; p<c; p++) {
         if (Demo==1){
         	printf("position=%d, color=%d, temp[p]=%d\n",p,Colors[p],temp[p]);
            getch();
         }
     		if (temp[p]==1) continue;
        e=Edges[p];
         while (e!=NULL) {
				i=e->u;
   				j=e->v;	
            if (Demo==1)	printf("\ni=%d, j=%d",i,j);
            if (i==j) {
        			e=e->Next;
         		continue;
      		}
            if (label_temp[p][i]!=label_temp[p][j]){
      			NumComps_app[p]=NumComps_app[p]-1;
         		Labeli=label_temp[p][i];
         		Labelj=label_temp[p][j];
         		for (k=0; k<n; k++){	// := 1 to N
         			if (label_temp[p][k]==Labelj)
            	   	label_temp[p][k]=Labeli;
               }
               Lebels2_temp[p][i*n+j]=i*n+j;
            }
           	else 
				if(labelB_temp[p][i]!=labelB_temp[p][j]) {
					
					// BFS for the shortest path from i to j
      			for (k=0; k<n; k++)	
         			path[k]=n;
         		path[j]=j;
         		Queue[0]=j;
         		q = 0 ;
         		Qlast = 0 ;
         		do{
         			a = Adj[Queue[q]];
         			while (a!=NULL){
         				if (path[a->v]==n){
         					path[a->v]=Queue[q];
         					Qlast++;
         					Queue[Qlast]=a->v;
								}
         				a=a->next ;
							}
         				q++;
						} while (path[i]==n);
					// End of the BFS
         		// The path is i, path[i], path[path[i]], ..., j
         		
         		// Traverse the path from i to j joining the blocks found
      			Labeli = labelB_temp[p][i];
      			      		Label2i = Labels2[k*n+path[k]] ;
         		k = i ;
      			do{
      				if (Labels2_temp[p][k*n+path[k]] != Label2i){
					*Num_VBlocks = *Num_VBlocks - 1;
      				Label2k = Labels2_temp[k*n+path[k]];
				  	for(int q=0; q<n; q++)
				  		for(int qq=0; qq<n; qq++)
				  		if (Labels2_temp[p][q*n+qq]==Label2k)
				  			Labels2_temp[p][q*n+qq]= Label2i ;
      				}	
      			}
      			

				  		k = path[k] ;
				  		if(labelB_temp[p][k]!=Labeli) {
							NumBlocks_app[p] = NumBlocks_app[p] - 1;
				  			Labelk = labelB_temp[p][k] ;
				  			for(int q=0; q<n; q++)
				  				if (labelB_temp[p][q]==Labelk)
				  					labelB_temp[p][q]= Labeli ;
							}
						} while (k!=j);
				Labels2_temp[p][i*n+j]=Label2k;
				} // END OF if(label_appB[p][i]!=label_appB[p][j]){

/*            if (Demo==1){
            	printf("e->next\n\n\n");
            	getch();
            }
*/
            
            
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
              		
  
            
      		e=e->Next;
         }
         if (Demo==1){
         	printf("\n-----> NumVBlocks_app[%d]=%d, NumBlocks_app[%d]=%d, NumComps_app[%d]=%d\n\n",
			 		p,NumVBlocks_app[p],NumBlocks_app[p],p,NumComps_app[p]);
         	getch();
         	printf("\nOLD TEMP num=%d\n",num);
         	getch();
         }
        	if (NumVBlocks_app[p]+NumComps_app[p]<num) {
				Add_p=p;
            numC=NumComps_app[p];
				numB=NumBlocks_app[p];
				num=numC+numB;
         }
         if (Demo==1){
         	printf("\nTEMPORARYAdd_p=%d, num=%d\n",Add_p,num);
         	printf("\nWe start again with next color\n");
         	getch();
         }
         
         // Delete edjes with color "p"
         if (Demo==1) printf("\n Deleting color %d :",p);
         for (int i=0; i<n; i++){
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
         if (Demo==1) printf("\n Deleted color %d :",p);

         
      }
      if (Demo==1){
      	printf("\n!!!No we scan all the colors: we add the best color\n");
      	getch();
      }
      	//Adding a color:
      MVCA_Sol[Best_Num_Sol]=Colors[Add_p];
      Best_Num_Sol=Best_Num_Sol+1;
      
      // Add Adjacency items with color Add_p
      
      e=Edges[Add_p];
      while (e!=NULL) {
			i=e->u;
   		j=e->v;	
         if (Demo==1)	printf("\ni=%d, j=%d",i,j);
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
      	e=e->Next;
  			}
      
      temp[Add_p]=1;
      if (Demo==1){
      	printf("\nPosition=%d, Color added=%d, New num=%d\n\n",Add_p,Colors[Add_p],num);
      	getch();
      }
      for (j=0; j<c; j++){
      	NumComps_app[j]=numC;
      	NumBlocks_app[j]=numB;
         if (temp[j]==1)	continue;
      	for (i=0; i<n; i++) {
      		label_temp[j][i]=label_temp[Add_p][i];
      		labelB_temp[j][i]=labelB_temp[Add_p][i];
         }
      }
    }
    if(Demo==1){
    	printf(" \n MVCA INITIAL SOLUTION S[%d]: ",Best_Num_Sol);
   	for (i=0; i<Best_Num_Sol; i++)	// := 1 to Best_Num_Sol
   	printf("%d ",MVCA_Sol[i]);
      printf("\n");
      getch();
    }
             
	if (Demo==1) printf("\n Deleting Adj for all color %d :",p);
   for (int i=0; i<n; i++)
      while(Adj[i]!=NULL){
         	a = Adj[i];
				Adj[i] = Adj[i]->next;
         	delete[] a;
		}
	
}


//-------------------------------- MAIN:-------------------------------------//

int main() {
   int Sample,Samples;
   char answer;
   char ResultsName[40];
	char DataName[40];
   char BacktrackName[40];
   int i,k;
   Vector Labels,LabelsB,Labels2;
   Vector Sol;
	int Num_Comp,Num_Blocks,Num_VBlocks;
   int Num_Sol;
   int value;
   double AvgValue;
	clock_t u1, u2;
	double u;
	Adj_item *a;

   do{
      for (i=1;i<25;i++) printf("\n");
		// clrscr();
      //textmode(_ORIGMODE);
   	printf("DEMO? \n");
   	printf("(1) : YES \n");
		printf("(0) : NO \n");
      printf("\n-> TYPE YOUR CHOICE: ");
      scanf("%d",&Demo);
      printf("\n");
      while ((Demo!=1)&&(Demo!=0)) {
      	for (i=1;i<25;i++) printf("\n");
      	//clrscr();
      	printf("!!! WRONG ANSWER !!! Demo: %d\n",Demo);
			printf("YOU MUST TYPE 1 OR 0!!! PLEASE, TRY AGAIN.");
      	printf("\n\n\n\n\nPress a key to continue ...");
			getch();
			for (i=1;i<25;i++) printf("\n");
			// clrscr();
      	printf(" Demo? \n");
   		printf(" (1) : YES \n");
			printf(" (0) : NO \n");
         printf("\n-> TYPE YOUR CHOICE: ");
   		scanf("%d",&Demo);
         printf("\n");
   	}
      printf("Data File Name: ");
		scanf("%s",DataName);
      Data_File=fopen(DataName,"rt");
		while (Data_File==NULL) {
      	for (i=1;i<25;i++) printf("\n");
			//clrscr();
      	printf("!!! DANGER !!! CANNOT OPEN FILE: %s\n",DataName);
			printf("THE FILE DOESN'T EXIST! PLEASE, TRY AGAIN.");
      	printf("\n\n\n\n\nPress a key to continue ...");
			getch();
			for (i=1;i<25;i++) printf("\n");
			// clrscr();
      	printf("Data File Name: ");
			scanf("%s",DataName);
      	Data_File=fopen(DataName,"rt");
   	}
      printf("Number of Samples: ");
   	scanf("%d",&Samples);
      n=ReadNumber(Data_File);
   	if (n>Size){
   		      for (i=1;i<25;i++) printf("\n");
			//clrscr();
      	printf("!!! DANGER !!! CANNOT OPEN FILE: %s\n",DataName);
			printf("THE TOTAL NUMBER OF VERTICES IS %d AND IT EXCEEDS THE MAXIMUM LIMIT OF %d!\n",n,Size);
      	printf("\n\n\n\n\nPress a key to continue ...");
			getch();
      	fclose(Data_File);
			      for (i=1;i<25;i++) printf("\n");
			//clrscr();
      	return 0;
   	}
		c=ReadNumber(Data_File);
   	if (c>Size){
   		      for (i=1;i<25;i++) printf("\n");
			// clrscr();
      	printf("!!! DANGER !!! CANNOT OPEN FILE: %s\n",DataName);
			printf("THE TOTAL NUMBER OF COLOURS IS %d AND IT EXCEEDS THE MAXIMUM LIMIT OF %d!\n",c,Size);
      	printf("\n\n\n\n\nPress a key to continue ...");
			getch();
      	fclose(Data_File);
			      for (i=1;i<25;i++) printf("\n");
			//clrscr();
      	return 0;
   	}
      printf("\nNumber of nodes (n)= %d;\nNumber of colours (c)= %d;\n",n,c);
      getch();
      printf("\n==================================== RESULTS ==================================\n\n");
   	printf("\n");
      if (Demo==1){
         sprintf(ResultsName,"%c%c_DEMObacktracking_%d_%d.txt",DataName[0],DataName[1],n,c);
      }
      else{
      	sprintf(ResultsName,"%c%c_BACKTRACKING_%d_%d.txt",DataName[0],DataName[1],n,c);

      }
   	Results_File=fopen(ResultsName,"wt");
   	if (Results_File==NULL) {
   		printf("CANNOT OPEN FILE: %s\n",ResultsName);
      	printf("\n\n\n\nPress a key to continue ...");
			getch();
			      for (i=1;i<25;i++) printf("\n");
			//clrscr();
			return 0;
		}
   	fprintf(Results_File,"%d %d\n\n",n,c);
      sprintf(BacktrackName,"back%d%c%c%d.txt",n,DataName[0],DataName[1],c);
      Backtrack_File=fopen(BacktrackName,"wt");
      value=0;
		u=0.000;
      for (Sample=0; Sample<Samples; Sample++){
         printf("\t\t\t\t\n  ----------------   SAMPLE %d :\n",Sample+1);
         system("PAUSE");
      	u1=clock();
         Read_Data();
         List_Colors();
         call=0;
         Num_Comp=n;
         Num_Blocks = n;
         Num_Sol=0;
         Best_call=0;
         Best_Num_Sol=0;	//Best_Num_Sol=c+1;  //if you want not use the initial MVCA you have to put this instruction and delete Best_Num_Sol=0; and initial_MVCA();
         initial_MVCA(Demo);

         for (k=0; k<c; k++){ // lui 1 to c
         	if (Demo==1)	printf("\n - - - - - - - - - - - - WE START WITH A NEW COLOR: %d \n",k);
            if ((Num_Sol==Best_Num_Sol-2)&&(Freq[k]<(Num_Comp-1)))	break;
            for (i=0; i<n; i++)	Labels[i]=i;
            for (i=0; i<n; i++)	LabelsB[i]=i;
            // Delete Adj 
            if (Demo==1) printf("\n Deleting Adj for all colors");
   			for (int i=0; i<n; i++)
      			while(Adj[i]!=NULL){
         			a = Adj[i];
						Adj[i] = Adj[i]->next;
         			delete[] a;
					}
	
            Try(k,
				Labels,Num_Comp,
				LabelsB,Num_Blocks,
				Labels2,Num_VBlocks,
				Sol,Num_Sol);
         }
         u2=clock();
         value=value+Best_Num_Sol;
			//u = u+ (double(u2 - u1)/1000); seconds
         u = u+ (double(u2 - u1));  // milliseconds
         if (Demo==1)	printf(" \nEND\n ") ;
         if(Best_call==0){
         	printf(" \n NOT FOUND A SOLUTION BETTER THAN THE MVCA ONE S[%d]: ",Best_Num_Sol);
   			for (i=0; i<Best_Num_Sol; i++)	// := 1 to Best_Num_Sol
   				printf("%d ",MVCA_Sol[i]);
         	printf(" \n with %d total calls.\n",call);
         	fprintf(Results_File," Sample %d:",Sample) ;
   			fprintf(Results_File," -MVCA-Sol[%d] :",Best_Num_Sol);
   			for (k=0; k<Best_Num_Sol; k++){	// k := 1 to Best_Num_Sol
   				fprintf(Results_File,"%d ",MVCA_Sol[k]);
         	}
   			fprintf(Results_File," with %d total calls.\n",call);
         }
         else{
         	printf(" \n FOUND SOLUTION S[%d]: ",Best_Num_Sol);
   			for (i=0; i<Best_Num_Sol; i++)	// := 1 to Best_Num_Sol
   				printf("%d ",Best_Sol[i]);
         	printf(" \n Calls:%d",Best_call);
   			printf(" with %d total calls.\n",call);
         	fprintf(Results_File," Sample %d:",Sample) ;
   			fprintf(Results_File," FOUND Sol[%d] :",Best_Num_Sol);
   			for (k=0; k<Best_Num_Sol; k++){	// k := 1 to Best_Num_Sol
   				fprintf(Results_File,"%d ",Best_Sol[k]);
         	}
   			fprintf(Results_File," Calls:%d",Best_call);
         	fprintf(Results_File," with %d total calls.\n",call);
         }
         fprintf(Backtrack_File,"%d\n",Best_Num_Sol);
      } //(* End Sample *)

      AvgValue=(value+0.0)/Samples;
   	printf("\n************************ SPERIMENTALS EXECUTING VALUES: ************************\n\n");
   	printf("\t\t\t   Average Value: %f\n",AvgValue);
		printf("\t\t\t   Average Time (msec): %f\n",(double(u/Samples)));
   	printf("\n\t\tResults saved in the file: \\%s\n",ResultsName);
   	fprintf(Results_File,"\n************************ SPERIMENTALS EXECUTING VALUES: ************************\n\n");
   	fprintf(Results_File,"\t\t\t   Average Value: %f\n",AvgValue);
		fprintf(Results_File,"\t\t\t   Average Time (msec): %f\n",(double(u/Samples)));
   	fclose(Data_File) ;
		fclose(Results_File);
      fclose(Backtrack_File);
      printf("\n\n\n\n\n\n\t\t\t   ANOTHER SIMULATION? (y/n): ");
      answer=getch();
      //textmode(C80);
			for (i=1;i<25;i++) printf("\n");
		//clrscr();
   }while (answer=='y'||answer=='Y');
}


