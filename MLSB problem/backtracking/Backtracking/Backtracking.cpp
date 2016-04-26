// Backtracking.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <conio.h>

// #define Size 625	// maximum number of vertices and maximum number of colors

#define Size 200	// maximum number of vertices and maximum number of colors

typedef int Vector[Size];

typedef struct Adj_Item_type {
	int v; // the other vertex
	int c; // color
	struct Adj_Item_type *next; // next Adj item
} Adj_item;

typedef struct Edge_R {
	int u,v;	// vertexes
   //int en;	// number of edges
   struct Edge_R *Next;
} Edge_R, *Edge_T;
typedef int Matrix[Size][Size];

int n,c;	//n=total number of vertexes, c=total number of colours
int Colors[Size];
Matrix Color_Matrix;
Vector Best_Sol;
int Best_Num_Sol;
Edge_T Edges[Size];
Vector Freq;
int Demo;
long int call,Best_call;
FILE *Data_File;
FILE *Results_File;
FILE *Backtrack_File;

Adj_item *Adj[Size]; // Adjacency lists 

Vector MVCA_Sol;

int previous_call;


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
   if (Demo>0){
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


void q_sortGF(int left, int right) {
  	int pivot, index_pivot, l_hold, r_hold;
   Edge_T Edge;
   int Color_support, Freq_support;

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


void List_Colors(){
	int i,j,k;
   Edge_T New_Edge;

   for (i=0; i<c; i++){
		Edges[i]=NULL;
      Freq[i]=0;
   }
	for (i=0; i<n-1; i++){
		for (j=i+1; j<n; j++){
         k=Color_Matrix[i][j]; //k=1+Color_Matrix[i][j];	//pepe convenction, colours from 1 a c
      	if ((k>=0)&&(k<c)){
         	New_Edge=new Edge_R;
         	New_Edge->u=i;
            New_Edge->v=j;
            New_Edge->Next=Edges[k];
            Edges[k]=New_Edge;
            Freq[k]=Freq[k]+1;
         }
      }
   }
   for (k=0; k<c; k++){
   	Colors[k]=k;
   }
   if (Demo>0){
      printf("List Colors:\n");
      for (k=0; k<c; k++){
         printf("%d(%d): ",Colors[k],Freq[k]);
         New_Edge=Edges[k];
      	while (New_Edge!=NULL){
      	  	printf("[%d,%d] ",New_Edge->u,New_Edge->v);
            New_Edge=New_Edge->Next;
         }
      	printf("\n");
      }
      printf("\n");
   	getch();
   }

   if (Demo>1){
   	printf("\ncolor(frequency): ");
		for (i=0; i<c; i++) printf("%d(%d) ",Colors[i],Freq[i]);
		printf("\n");
   	getch();
   }

   q_sortGF(0, c - 1);

   if (Demo>1){
   	printf("\n-- SORTED -- color(frequency): ");
		for (i=0; i<c; i++) printf("%d(%d) ",Colors[i],Freq[i]);
		printf("\n\n");
   	getch();
   }
}


void Print_Summary(	int Labels[], int Num_Comp, 
					int LabelsB[], int Num_Blocks,
					int Sol[], int Num_Sol){
	int i;

   if (Demo>0){
   	printf("\n----> PRINT SUMMARY:");
   }
	printf("\n Vertex: ");
   for (i=0; i<n; i++)	printf("%2d ",i);
   printf("\n Label:  ");
   for (i=0; i<n; i++)	printf("%2d ",Labels[i]);
   printf("\n LabelB: ");
   for (i=0; i<n; i++)	printf("%2d ",LabelsB[i]);
   printf("\n Number of Blocks %d and Connected Components %d;",Num_Blocks,Num_Comp);
   printf("\n Number of colors = %d;",Num_Sol);
   printf("\n S[] : ");
   for (i=0; i<Num_Sol; i++)	// := 1 to Num_Sol
   	printf("%d ",Sol[i]);
   printf("\n Calls: %d;  Best_Calls: %d;\n",call,Best_call);
   printf("\n Best SOLUTION = %d;\n",Best_Num_Sol);
   printf("\n");
	getch();
}


int Try_Add_Color( 
					int sample,
					int Position_Color, 
				   int NumBC,
					int Labels[], int *Num_Comp, 
					int LabelsB[], int *Num_Blocks, 
					int Sol[], int *Num_Sol){
	int i,j,k;
   Edge_T Edge;
   int Labels_support_Add[Size];
   int LabelsB_support_Add[Size];
   int added;
   int Labeli,Labelj,Labelk;
   int app_Num_Comp;
   int app_Num_Blocks;
   int app_Num;
	int Num;

	int path[Size];
   int Queue[Size];
   int q,Qlast;
   Adj_item *a,*aa;
   

   app_Num_Comp = *Num_Comp;
   app_Num_Blocks = *Num_Blocks;
	app_Num  = app_Num_Comp + app_Num_Blocks ;


   for (i=0; i<n; i++)	Labels_support_Add[i]=Labels[i];
   for (i=0; i<n; i++)	LabelsB_support_Add[i]=LabelsB[i];

   //call=call+1;

	if (Demo>0){
		printf(" Num B+C: %d \n",NumBC);
   	printf(" Adding %d, position=%d\n",Colors[Position_Color],Position_Color);
   	Print_Summary(	Labels,*Num_Comp,
	   				   LabelsB,*Num_Blocks,
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
         		k = i ;
      			do{
				  		k = path[k] ;
				  		if(LabelsB[k]!=Labeli) {
							*Num_Blocks = *Num_Blocks - 1;
				  			Labelk = LabelsB[k] ;
				  			for(int q=0; q<n; q++)
				  				if (LabelsB[q]==Labelk)
				  					LabelsB[q]= Labeli ;
							}
						} while (k!=j);
				} // END OF if(label_appB[p][i]!=label_appB[p][j]){
					
      }
      
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
   Num = *Num_Blocks + *Num_Comp;


   if (Demo>1){
   	printf(" Num = %d. ",Num);
   	printf(" Num_Blocks = %d",*Num_Blocks);
   	printf(" Num_Comp   = %d\n",*Num_Comp);
   }
   if (((Num<app_Num)&&(*Num_Sol<Best_Num_Sol-2))
      || ((*Num_Sol==Best_Num_Sol-2)&&(Num<=NumBC)) ) {
      *Num_Sol=*Num_Sol+1;
   	Sol[*Num_Sol-1]=Colors[Position_Color];
      call=call+1;
      added=1;
      if (Demo>0){
         printf("\n --> ADDED COLOR %d, position=%d, --> Num_Sol= %d\n",Colors[Position_Color],Position_Color,*Num_Sol);
   		Print_Summary(Labels,*Num_Comp,LabelsB,*Num_Blocks,Sol,*Num_Sol);
      	printf("----> added=%d, End Adding %d, position=%d\n",added,Colors[Position_Color],Position_Color);
   		getch();
   	}
   }
   else{
      added=0;
      if (Demo>1){
   		if ((Num<app_Num)&&(*Num_Sol<Best_Num_Sol-2))	
		   		printf(" Don't add %d because it doesn't decrease the number of Comps %d +  number of Blocks %d!\n",
		   		Colors[Position_Color],app_Num_Comp, app_Num_Blocks);
         else	if ((*Num_Sol==Best_Num_Sol-2)&&(Num>NumBC))	
		 			printf(" Don't add %d, position=%d, because it doesn't give a solution! \n",
					 Colors[Position_Color],Position_Color);
   		Print_Summary(Labels,*Num_Comp,LabelsB,*Num_Blocks,Sol,*Num_Sol);
      	printf("----> added=%d, End Adding %d, position=%d\n",added,Colors[Position_Color],Position_Color);
   		getch();
   	}
      *Num_Comp=app_Num_Comp;
      *Num_Blocks=app_Num_Blocks;
      Num = *Num_Comp + *Num_Blocks;

      for (i=0; i<n; i++)	Labels[i]=Labels_support_Add[i];
      for (i=0; i<n; i++)	LabelsB[i]=LabelsB_support_Add[i];
      
      // Delete edjes with color "Position_Color"
         if (Demo>1) printf("\n Deleting color %d :",Position_Color);
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
         if (Demo>1) printf("\n Deleted color %d :",Position_Color);


   }

   if (((call%1000)==0)&&(previous_call!=call)){
      previous_call=call;
	  	printf("Sample %d. Best %d. Call %d S[%d] ",sample,Best_Num_Sol,call,*Num_Sol);
   	for (i=0; i<*Num_Sol; i++){	//i := 1 to Num_Sol
   		printf("%d ",Sol[i]);
      }
      printf("\n Blocks: %d. Comp: %d\n",*Num_Blocks,*Num_Comp);
   }

   return added;

}


void Try(int sample,
			int Position_Color, 
			int NumBC,
			int Labels[], int Num_Comp, 
			int LabelsB[], int Num_Blocks, 
			int Sol[], int Num_Sol){
   int k,i;
   int continue_to_try;
   int app_Labels[Size];
   int app_LabelsB[Size];
   
   int Num ;

	if (Demo>0){
   	printf("\nTRY COLOR: %d, position=%d, with Freq %d\n",Colors[Position_Color],Position_Color,Freq[Position_Color]);
   	Print_Summary(Labels,Num_Comp,LabelsB,Num_Blocks,Sol,Num_Sol);
   }
   continue_to_try=Try_Add_Color(sample,
						Position_Color,NumBC,
   					Labels,&Num_Comp,
					LabelsB,&Num_Blocks,
					   Sol,&Num_Sol);
	Num = Num_Comp + Num_Blocks;
   if (Num==NumBC){
    	for (k=0; k<Num_Sol; k++){	// := 1 to Num_Sol
   		Best_Sol[k]=Sol[k];
      }
   	Best_Num_Sol=Num_Sol;
      Best_call=call;
      printf(" %d %d ",Num,NumBC);
      printf(" \n ********************************* BEST SOL %d\n",Best_Num_Sol);
      if (Demo>0){
         Print_Summary(Labels,Num_Comp,LabelsB,Num_Blocks,Sol,Num_Sol);
      }
   }
   else{
      if (continue_to_try==1){
      	for (i=0; i<n; i++)	app_Labels[i]=Labels[i];
      	for (i=0; i<n; i++)	app_LabelsB[i]=LabelsB[i];
        	k=Position_Color+1;
        	while((k<c)&&(Num_Sol<Best_Num_Sol-1)){
 //        	if ((Num_Sol==Best_Num_Sol-2)&&(Freq[k]<(Num_Comp-1)))	break;
            for (i=0; i<n; i++){
         		Labels[i]=app_Labels[i];
         	}
            for (i=0; i<n; i++){
         		LabelsB[i]=app_LabelsB[i];
         	}
            Try(sample,
					k,NumBC,	
					Labels,Num_Comp,
					LabelsB,Num_Blocks,
					Sol,Num_Sol);
            k=k+1;
            if (Demo>1){
            	if(k<c){
            		printf(" \n Continue Inner_Try of %d, position=%d, with the next color %d, position=%d\n",Colors[Position_Color],Position_Color,Colors[k],k);
            	}
            }
         }
      }
   }
   if (Demo>1){
   	if (continue_to_try==0)	printf("----> End Try color %d, position=%d\n",Colors[Position_Color],Position_Color);
      else printf(" \n End Inner_Try of %d, position=%d, with color %d, position=%d\n",Colors[Position_Color],Position_Color,k,Colors[k]);
   	getch();
   }
}


void initial_MVCA(int Demo,int *NumBC, int *Best_Num_Sol){
	int num,numB,numC,i,j,k,p,Add_p;
   int NumComps_app[Size];  // support variable
   int NumBlocks_app[Size];  // support variable
   int temp[Size];
   int label_temp[Size][Size]; // support variable
   int labelB_temp[Size][Size]; // support variable
   Edge_T e;
   int Labeli, Labelj, Labelk;
   int path[Size];
   int Queue[Size];
   int q,Qlast;
   Adj_item *a,*aa;

   if (Demo>0){
   	printf("--- MVCA ---\n\n\n");
   	getch();
   }
   num=n+n;
   numC=n;
   numB=n;
   *Best_Num_Sol = 0;
   Add_p=0;
   for (p=0; p<c; p++){
   	NumComps_app[p]=numC;
   	NumBlocks_app[p]=numB;
      if (Edges[p]==NULL) temp[p]=1;
      else temp[p]=0;
      for (i=0; i<n; i++)	label_temp[p][i]=i;
      for (i=0; i<n; i++)	labelB_temp[p][i]=i;
      if (Demo>1)	printf("position=%d, color=%d, temp[p]=%d\n",p,Colors[p],temp[p]);
   }
   if (Demo==1)	getch();
   for (i=0;i<n;i++)
   Adj[i] = NULL ;
   int improve = 1 ;
   while ((numB!=1) && (improve==1)) {
   	improve = 0 ;
   	for (p=0; p<c; p++) {
         if (Demo>1){
         	printf("position=%d, color=%d, temp[p]=%d\n",p,Colors[p],temp[p]);
            getch();
         }
     		if (temp[p]==1) continue;
        e=Edges[p];
         while (e!=NULL) {
				i=e->u;
   				j=e->v;	
            if (Demo>2)	printf("\ni=%d, j=%d",i,j);
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
         		k = i ;
      			do{
				  		k = path[k] ;
				  		if(labelB_temp[p][k]!=Labeli) {
							NumBlocks_app[p] = NumBlocks_app[p] - 1;
				  			Labelk = labelB_temp[p][k] ;
				  			for(int q=0; q<n; q++)
				  				if (labelB_temp[p][q]==Labelk)
				  					labelB_temp[p][q]= Labeli ;
							}
						} while (k!=j);
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
         if (Demo>1){
         	printf("\n-----> NumBlocks_app[%d]=%d, NumComps_app[%d]=%d\n\n",
			 		p,NumBlocks_app[p],p,NumComps_app[p]);
         	getch();
         	printf("\nOLD TEMP num=%d\n",num);
         	getch();
         }
         
        	if (NumBlocks_app[p]+NumComps_app[p]<num) {
        		improve = 1;
				Add_p=p;
            numC=NumComps_app[p];
				numB=NumBlocks_app[p];
				num=numC+numB;
         }
         
			if (Demo>1){
         	printf("\nTEMPORARYAdd_p=%d, num=%d\n",Add_p,num);
         	if (improve==1)
         		printf("\nWe start again with next color\n");
         	else printf("\n No color to add ");
         	getch();
         }
         
         // Delete edjes with color "p"
         if (Demo>1) printf("\n Deleting color %d :",p);
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
         if (Demo>1) printf("\n Deleted color %d :",p);

         
      }
      if (Demo>1){
      	printf("\n!!!No we scan all the colors: we add the best color\n");
      	getch();
      }
      	//Adding a color:
      if (improve==1){
      	MVCA_Sol[*Best_Num_Sol]=Colors[Add_p];
      	*Best_Num_Sol=*Best_Num_Sol+1;
			
      
      // Add Adjacency items with color Add_p
      
      	e=Edges[Add_p];
      	while (e!=NULL) {
			i=e->u;
   		j=e->v;	
         if (Demo>1)	printf("\ni=%d, j=%d",i,j);
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
      	if (Demo>1){
      		printf("\nPosition=%d, Color added=%d, New num=%d\n\n",Add_p,Colors[Add_p],num);
      		getch();
      	}
      
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

  *NumBC = numC + numB ;	

    if(Demo>=0){
    	printf(" \n MVCA INITIAL SOLUTION S[%d] with NumB+C = %d: ",*Best_Num_Sol,*NumBC);
   	for (i=0; i<*Best_Num_Sol; i++)	// := 1 to Best_Num_Sol
   	printf("%d ",MVCA_Sol[i]);
      printf("\n");
      if (Demo>1) getch();
    }
             
	if (Demo>1) printf("\n Deleting Adj for all color %d :",p);
   for (int i=0; i<n; i++)
      while(Adj[i]!=NULL){
         	a = Adj[i];
				Adj[i] = Adj[i]->next;
         	delete[] a;
		}

}



int _tmain(int argc, _TCHAR* argv[])
{
	int Sample,Samples;
   char answer;
   char ResultsName[40];
	char DataName[40];
   char BacktrackName[40];
   int i,k;
   Vector Labels,LabelsB;
   Vector Sol;
	int Num_Comp,Num_Blocks,Num;
   int Num_Sol;
   int NumBC;
   int value_MVCA,value_EXACT;
   double AvgValue_MVCA,AvgValue_EXACT;
	clock_t u1, u2, u3;
	double u_MVCA,u_EXACT;
	Adj_item *a;
    int MVCA_Num_Sol;


   do{
      for (i=1;i<25;i++) printf("\n");
		// clrscr();
      //textmode(_ORIGMODE);
   		printf("DEMO? \n");
   		printf("(2) : TOTAL \n");
		printf("(1) : YES \n");
		printf("(0) : NO \n");
      printf("\n-> TYPE YOUR CHOICE: ");
      scanf("%d",&Demo);
      printf("\n");
      while ((Demo!=2)&&(Demo!=1)&&(Demo!=0)) {
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
     if(Demo){ 
		 printf("\nNumber of nodes (n)= %d;\nNumber of colours (c)= %d;\n",n,c);
      getch();
	  }
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
		value_MVCA  = 0;
	    value_EXACT = 0;
		u_MVCA  = 0.000;
		u_EXACT = 0.000;
      for (Sample=0; Sample<Samples; Sample++){
         printf("\t\t\t\t\n  ----------------   SAMPLE %d :\n",Sample+1);
//         system("PAUSE");
      	 Read_Data();
         List_Colors();
         call=0;
         Num_Comp=n;
         Num_Blocks = n;
         Num = n+n;
         Num_Sol=0;
         Best_call=0;
   
   		u1=clock();
        
	//Best_Num_Sol=c+1;  //if you want not use the initial MVCA you have to put this instruction and delete Best_Num_Sol=0; and initial_MVCA();
         initial_MVCA(Demo,&NumBC,&MVCA_Num_Sol);
		 u2=clock();
         
         Best_Num_Sol = MVCA_Num_Sol ;


         for (k=0; k<c; k++){ // lui 1 to c
         	if (Demo==1)	printf("\n - - - - - - - - - - - - WE START WITH A NEW INITIAL COLOR: %d \n",k);
//            if ((Num_Sol==Best_Num_Sol-2)&&(Freq[k]<(Num_Comp-1)))	break;
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
	
            Try(Sample,
					k,NumBC,
					Labels,Num_Comp,
					LabelsB,Num_Blocks,
					Sol,Num_Sol);
         }
         u3=clock();
         value_MVCA = value_MVCA + MVCA_Num_Sol;
		 	value_EXACT = value_EXACT + Best_Num_Sol;
			//u = u+ (double(u2 - u1)/1000); seconds
         u_MVCA  = u_MVCA + (double(u2 - u1));  // milliseconds
         u_EXACT = u_EXACT + (double(u3 - u2));  // milliseconds
         
		 if (Demo==1)	printf(" \nEND\n ") ;
        
		   fprintf(Results_File," Sample: %d ",Sample) ;
   		fprintf(Results_File," B: %d ",NumBC-1);
   		fprintf(Results_File," MVCA: %d ",MVCA_Num_Sol);
   		fprintf(Results_File," EXACT: %d ",Best_Num_Sol);
   		fprintf(Results_File," t_MVCA: %f ",double(u2 - u1));
   		fprintf(Results_File," t_EXACT: %f ",double(u3 - u2));
   		fprintf(Results_File," Best_call: %d ",Best_call);
         fprintf(Results_File," with %d total calls.\n",call);
         
		   if(Best_call==0){
         	printf(" \n NOT FOUND A SOLUTION BETTER THAN THE MVCA ONE S[%d]: ",Best_Num_Sol);
   			for (i=0; i<Best_Num_Sol; i++)	// := 1 to Best_Num_Sol
   				printf("%d ",MVCA_Sol[i]);
         	printf(" \n with %d total calls.\n",call);
         	//fprintf(Results_File," -MVCA-Sol[%d] :",Best_Num_Sol);
   			//for (k=0; k<Best_Num_Sol; k++){	// k := 1 to Best_Num_Sol
   			//	fprintf(Results_File,"%d ",MVCA_Sol[k]);
         	//}
         }
         else{
         	printf(" \n FOUND SOLUTION S[%d]: ",Best_Num_Sol);
   			for (i=0; i<Best_Num_Sol; i++)	// := 1 to Best_Num_Sol
   				printf("%d ",Best_Sol[i]);
         	printf(" \n Calls:%d",Best_call);
   			printf(" with %d total calls.\n",call);
         	//fprintf(Results_File," FOUND Sol[%d] :",Best_Num_Sol);
   			//for (k=0; k<Best_Num_Sol; k++){	// k := 1 to Best_Num_Sol
   			//	fprintf(Results_File,"%d ",Best_Sol[k]);
         	//}
   		}
         fprintf(Backtrack_File,"%d\n",Best_Num_Sol);
      } //(* End Sample *)

      AvgValue_MVCA = (value_MVCA +0.0)/Samples;
      AvgValue_EXACT = (value_EXACT +0.0)/Samples;
   	
	printf("\n************************ SPERIMENTALS EXECUTING VALUES: ************************\n\n");
   	printf("\t   MVCA  Average Value: %f",AvgValue_MVCA);
		printf("\t   Average Time (msec): %f\n",(double(u_MVCA/Samples)));
   	printf("\t   EXACT Average Value: %f",AvgValue_EXACT);
		printf("\t   Average Time (msec): %f\n",(double(u_EXACT/Samples)));
   	
	printf("\n\t\tResults saved in the file: \\%s\n",ResultsName);
   	
	fprintf(Results_File,"\n************************ SPERIMENTALS EXECUTING VALUES: ************************\n\n");
   	fprintf(Results_File,"\t   MVCA  Average Value: %f",AvgValue_MVCA);
		fprintf(Results_File,"\t   Average Time (msec): %f\n",(double(u_MVCA/Samples)));
   	fprintf(Results_File,"\t   EXACT Average Value: %f",AvgValue_EXACT);
		fprintf(Results_File,"\t   Average Time (msec): %f\n",(double(u_EXACT/Samples)));
   	
	 fclose(Data_File) ;
	 fclose(Results_File);
    fclose(Backtrack_File);
      printf("\n\n\n\n\n\n\t\t\t   ANOTHER SIMULATION? (y/n): ");
      answer=getch();
      //textmode(C80);
			for (i=1;i<25;i++) printf("\n");
		//clrscr();
   }while (answer=='y'||answer=='Y');

	return 0;
}

