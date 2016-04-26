//	Solve MLST by MVCA

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <iostream.h>
#include <conio.h>

#define VN 500 // maximum number of vertices
#define CN 625 // maximum number of colors

int *Temporary_Vector;
int dim_Temp;
int Demo;

typedef struct edge_type {
	int u; // one vertex
	int v; // the other vertex
	int c; // color
struct edge_type *next; // next edge
} edge;


typedef struct color_type {
	int c; // color
   //int freq; // the net frequency of the color
	int en; // the number of edges
   edge *root; // point to a list of edges
} color;


class Graph {
   public: color L[CN]; // list of colors
   int vn; // number of vertices
   int cn; // number of colors
   int sol_cn; // number of colors
   void q_sortGF(int left, int right);	//quick sort
   //int backup_cn;
   //private: int backup_C[CN];
   public: int label[VN]; // from pepe
   int NumComps;
   public: int ReadNumber(FILE *fp);
	void InitGraph(FILE *fp);
	void InitEmptyGraph(Graph G);
   void PrintResultsToFile(FILE *results);
	void PrintGraph(); // Print this graph
	void PrintSolution(); // Print the feasible_solution
	public: void AddColor(Graph G, int c);
	//void RemoveColor(Graph G, int c);
   //public: int NumComponents(); // Find the number of connected components of the graph
	void ClearAll();
	void RemoveAllColors();
	void my_MVCA(Graph G); // heuristic
   void PostOptimization(); // Post Optimization phase
   void RemoveUselessColor(int x);
	//private: void SortByGeneral(int numb_col); // Sort by the general frequency
};


int Graph::ReadNumber(FILE *fp) {
	char buf[10];
	char c;
	int i,n;
	int num;

   c=fgetc(fp);
	i=0;
	while (!feof(fp) && (c<'0' || c>'9')) c=fgetc(fp);
	while (!feof(fp) && (c>='0' && c<='9')) {
		buf[i]=c;
      c=fgetc(fp);
      i++;
	}
	//buf[i]='\0';
	num=0;
	n=i;
	for (i=0; i<n; i++) {
		num=num*10+(int)(buf[i]-'0');
	}

   return num;
}


void Graph::InitEmptyGraph(Graph G) {
	int i;

	vn=G.vn;
   cn=G.cn;
   sol_cn=0;
   for (i=0; i<vn; i++) {
		label[i]=i;
	}
   NumComps=vn;

	for (i=0; i<cn; i++) {
		L[i].en=0;
      L[i].c=i;
      L[i].root=NULL;
   }
}


void Graph::InitGraph(FILE *fp) {
	int i,j,k;
	edge *e;

   sol_cn=cn;
   for (i=0; i<vn; i++) {
		label[i]=i;
   }
	for (i=0; i<cn; i++) {
		L[i].en=0;
      L[i].c=i;
      L[i].root=NULL;
   }
					// Add edges in the empty graph:
	for (i=0; i<vn-1; i++) {
		for (j=i+1; j<vn; j++) {
			//if (i==j) continue;
			k=ReadNumber(fp);
			if (k==cn) continue;
					// Add color k for edge (i,j):
			e=new edge;
			e->u=i;
         e->v=j;
         e->c=k;
         e->next=L[k].root;
         L[k].root=e;
         L[k].en++;
		}
	}

   if (Demo==1){
   	printf("\ncolor(frequency): ");
		for (i=0; i<cn; i++) printf("%d(%d) ",L[i].c,L[i].en);
		printf("\n");
   	getch();
   }
   
   //q_sortGF(0, cn - 1);

   if (Demo==1){
   	printf("\n-- SORTED -- color(frequency): ");
		for (i=0; i<cn; i++) printf("%d(%d) ",L[i].c,L[i].en);
		printf("\n\n");
   	getch();
   }
}


void Graph::q_sortGF(int left, int right) {
  	int pivot, index_pivot, l_hold, r_hold;
   color L_support;

   l_hold = left;
  	r_hold = right;
   index_pivot=right;
   L_support=L[index_pivot];
   pivot = L[index_pivot].en;
   while (left < right){
   	while ((L[left].en >= pivot) && (left < right))	left++;
    	if (right != left){
         L[right] = L[left];
         right--;
    	}
      while ((L[right].en <= pivot) && (left < right))	right--;
    	if (right != left){
      	L[left] = L[right];
      	left++;
    	}
  	}
   L[right] = L_support;
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


void Graph::my_MVCA(Graph G) {
	int num,i,j,p,k,Add_p;
	int flag;
	int temp[CN];
   edge *e;
   int label_app[CN][VN]; // support variable
   int Labeli,Labelj;
   int app_Num_Comp[VN];

   Temporary_Vector=new int[G.cn];

   if (Demo==1){
   	printf("--- MVCA ---\n\n\n");
   	getch();
   }
   if (sol_cn!=0) RemoveAllColors();
   num=vn;
   //Add_p=0;
   flag=1;
	for (p=0; p<G.cn; p++){
      if (G.L[p].root==NULL) temp[p]=1;
      else temp[p]=0;
      if (Demo==1)	printf("temp[%d]=%d\n",p,temp[p]);
   }
   if (Demo==1)	getch();
   while (flag==1) {
      dim_Temp=0;
   	for (p=0; p<G.cn; p++) {
         if (Demo==1)	printf("temp[%d]=%d\n",p,temp[p]);
     		if (temp[p]==1) continue;
         for (i=0; i<vn; i++) {
      		label_app[p][i]=label[i];
            if (Demo==1)	printf("label_app[%d][%d]=%d, label[%d]=%d\n",p,i,label_app[p][i],i,label[i]);
         }
         app_Num_Comp[p]=NumComps;
         e=G.L[p].root;
         while (e!=NULL) {
				i=e->u;
         	j=e->v;
            if (Demo==1)	printf("\ni=%d, j=%d\n",i,j);
            if (i==j) {
        			e=e->next;
         		continue;
      		}
      		if(label_app[p][i]!=label_app[p][j]){
            	app_Num_Comp[p]=app_Num_Comp[p]-1;
         		Labeli=label_app[p][i];
         		Labelj=label_app[p][j];
         		for (k=0; k<vn; k++){	// := 1 to N
         			if (label_app[p][k]==Labelj)	label_app[p][k]=Labeli;
         		}
      		}
            if (Demo==1){
            	printf("e->next\n\n\n");
            	getch();
            }
      		e=e->next;
         }
         if (Demo==1){
         	getch();
         	printf("\nOLD TEMP num=%d\n",num);
         	getch();
         }
        	if (app_Num_Comp[p]<num) {
            Temporary_Vector[0]=p;
            dim_Temp=1;
				//Add_p=p;
            num=app_Num_Comp[p];
            if (Demo==1){
            	printf("\nNEW TEMP num=%d\n",num);
         		getch();
            }
         }
         else{
         	if ((app_Num_Comp[p]==num)&&(dim_Temp!=0)) {
               if (Demo==1)	printf("\nTHIS COLOR HAS THE SAME NUMBER OF CONN.COMPS. NEW ENTRY IN TEMPORARY VECTOR!\n");
            	Temporary_Vector[dim_Temp]=p;
         		dim_Temp++;
            }
         }
         if (Demo==1){
            printf("\nTEMPORARY VECTOR: ");
            for (i=0; i<dim_Temp; i++) {
            	printf("%d ",Temporary_Vector[i]);
            }
         	printf("\nWe start again with another color\n");
         	getch();
         }
      }
      if (Demo==1){
      	printf("\n!!!No we scan all the colors: we add the best color\n");
      	getch();
      }
      Add_p=Temporary_Vector[rand()%dim_Temp];
      AddColor(G,Add_p);
      temp[Add_p]=1;
      NumComps=num;
      if (Demo==1){
      	printf("\nPosition of the color added=%d, color added=%d, New num=%d\n",Add_p,G.L[Add_p].c,num);
      	getch();
      }
      for (i=0; i<vn; i++) {
         label[i]=label_app[Add_p][i];
         if (Demo==1)	printf("label[%d]=%d ",i,label[i]);
      }
      if (NumComps==1) flag=0;
      if (Demo==1){
      	printf("\nflag=%d\n",flag);
      	getch();
      }
   }
}


void Graph::PostOptimization(){
	int i,j,p,k;
	int flag;
	int x;
   edge *e;
   int Labeli,Labelj;
   int rem=0;

   if (sol_cn!=0){
      if (Demo==1){
         printf("\n--------------- SOLUTION ");
   		printf("\ncolor(frequency): ");
			for (i=0; i<sol_cn; i++) printf("%d(%d) ",L[i].c,L[i].en);
			printf("\n");
   		getch();
   	}
      x=sol_cn-2;
      //x=0;
      if (Demo==1){
      	printf("\nStart of the Post Optimization Phase\n");
         printf("\nWe start with the last color: %d\n",L[x].c);
      }
   	flag=1;
      while (flag==1) {
			for (i=0; i<vn; i++) {
				label[i]=i;
			}
   		NumComps=vn;
         for (p=0; p<sol_cn; p++) {
           	if (p==x) continue;
            e=L[p].root;
            while (e!=NULL) {
					i=e->u;
         		j=e->v;
            	if (i==j) {
        				e=e->next;
         			continue;
      			}
      			if(label[i]!=label[j]){
            		NumComps=NumComps-1;
         			Labeli=label[i];
         			Labelj=label[j];
         			for (k=0; k<vn; k++){	// := 1 to N
         				if (label[k]==Labelj)	label[k]=Labeli;
         			}
      			}
            	e=e->next;
         	}
         }
         if (Demo==1)	printf("\nNumComps=%d\n",NumComps);
         if (NumComps==1){
           	if (Demo==1){
            	printf("\n *-*-* IT IS POSSIBLE TO DELETE COLOR %d FROM THE SOLUTION!\n",L[x].c);
            	rem++;
            }
            RemoveUselessColor(x);
         }
         else{
           	if (Demo==1)	printf("\nIT IS NOT POSSIBLE TO DELETE COLOR %d FROM THE SOLUTION!\n",L[x].c);
         }
         if (x==0){
         //if (x>=sol_cn-2){
         	flag=0;
            if (Demo==1){
            	if (rem!=0){
            		printf("\n --- Total removed=%d\n",rem);
               	PrintSolution();
               }
            }
         }
         else x=x-1;
         //else x=x+1;
         if (Demo==1){
            if (flag==1) printf("\nnew x=%d; We start again with another color: %d\n",x,L[x].c);
         	getch();
         }
      }
   }
   else{
   	printf("\n!!! DANGER: THE SOLUTION IS EMPTY !!!\n");
		printf("IMPOSSIBLE TO APPLY THE POST OPTIMIZATION PHASE\n");
      printf("\nPress a key to continue ...\n");
		getch();
   }
}


void Graph::RemoveAllColors() {
	int i;

   sol_cn=0;
	for (i=0; i<cn; i++) {
		L[i].c=cn;
      L[i].en=0;
		L[i].root=NULL;
   }
   for (i=0; i<vn; i++) {
   	label[i]=i;
   }
   NumComps=vn;
}


void Graph::AddColor(Graph G, int p) {
   L[sol_cn].root=G.L[p].root;
   L[sol_cn].c=G.L[p].c;
   L[sol_cn].en=G.L[p].en;
   sol_cn++;
}


void Graph::RemoveUselessColor(int x) {
   int p;

   sol_cn--;
   for (p=x; p<sol_cn; p++) {
   	L[p]=L[p+1];
   }
   if (Demo==1)	PrintSolution();
}

/*
void Graph::RemoveColor(Graph G, int p) {
   int k;

   if (L[cn-1].c==G.L[p].c){
      L[cn-1].root=NULL;
		L[cn-1].en=0;
      L[cn-1].c=cn; //cn means no color
		cn--;
   }
   else{
   	printf("\nError in removing! ");
      G.ClearAll();
		for (k=0; k<cn; k++) {
			L[k].c=cn;
      	L[k].en=0;
			L[k].root=NULL;
		}
   	cn=0;
      printf("\nPress a key to continue ...");
		getch();
      clrscr();
      return;
   }
} */


void Graph::ClearAll() {
	int i;
	edge *e,*e1;

	for (i=0; i<cn; i++) {
		L[i].c=cn;
      L[i].en=0;
		e=L[i].root;
		while (e!=NULL) {
			e1=e;
         e=e->next;
			delete e1;
		}
		L[i].root=NULL;
   }
   cn=0;
   sol_cn=0;
}


void Graph::PrintSolution() {
	int p;

   printf("Number of colors of the found solution: %d\n",sol_cn);
	printf("SOLUTION: ");
	for (p=0; p<sol_cn; p++) {
		if (L[p].root!=NULL) printf("%d ",L[p].c);
	}
	printf("\n");
}


void Graph::PrintGraph() {
	int i;
	edge *e;

	printf("GRAPH ----------------------------------------\n");
   printf("position) color(freq): (arcs)\n");
	for (i=0; i<sol_cn; i++) {
		if (L[i].root==NULL) continue;
		printf("%d) %d(%d): ",i,L[i].c,L[i].en);
		e=L[i].root;
		while (e!=NULL) {
			printf("(%d,%d) ",e->u,e->v);
			e=e->next;
		}
		printf("\n");
	}
   printf("\n");
   printf("===============================================================================\n\n");
   printf("\n");
}


void Graph::PrintResultsToFile(FILE *results) {
	int i;
   int p;
	edge *e;

   fprintf(results,"Number of colors of the found solution: %d\n",sol_cn);
   fprintf(results,"SOLUTION: ");
	for (p=0; p<sol_cn; p++) {
		if (L[p].root!=NULL) fprintf(results,"%d ",L[p].c);
	}
	fprintf(results,"\n");
   fprintf(results,"GRAPH ----------------------------------------\n");
   fprintf(results,"position) color(freq): (arcs)\n");
	for (i=0; i<sol_cn; i++) {
		if (L[i].root==NULL) continue;
		fprintf(results,"%d) %d(%d): ",i,L[i].c,L[i].en);
		e=L[i].root;
		while (e!=NULL) {
			fprintf(results,"(%d,%d) ",e->u,e->v);
			e=e->next;
		}
		fprintf(results,"\n");
	}
   fprintf(results,"\n");
   fprintf(results,"===============================================================================\n\n");
   fprintf(results,"\n");
}


//-------------------------------- MAIN:-------------------------------------//

void main() {
   FILE *fp;
   FILE *results;
   FILE *heuristic;
   Graph G,H;
	int k;
   int gn;
	int value;
   char answer;
	char filename[40];
   char buffer[40];
   char vector[40];
   double AvgValue;
	clock_t u1, u2;
	double u;

   do{
      clrscr();
      textmode(_ORIGMODE);
      srand(1);
      printf("DEMO? \n");
   	printf("(1) : YES \n");
		printf("(0) : NO \n");
      printf("\n-> TYPE YOUR CHOICE: ");
      scanf("%d",&Demo);
      printf("\n");
      while ((Demo!=1)&&(Demo!=0)) {
      	clrscr();
      	printf("!!! WRONG ANSWER !!! Demo: %d\n",Demo);
			printf("YOU MUST TYPE 1 OR 0!!! PLEASE, TRY AGAIN.");
      	printf("\n\n\n\n\nPress a key to continue ...");
			getch();
			clrscr();
      	printf(" Demo? \n");
   		printf(" (1) : YES \n");
			printf(" (0) : NO \n");
         printf("\n-> TYPE YOUR CHOICE: ");
   		scanf("%d",&Demo);
         printf("\n");
   	}

		printf("Filename: ");
		scanf("%s",filename);
      fp=fopen(filename,"rt");
		while (fp==NULL) {
      	clrscr();
      	printf("!!! DANGER !!! CANNOT OPEN FILE: %s\n",filename);
			printf("THE FILE DOESN'T EXIST! PLEASE, TRY AGAIN.");
      	printf("\n\n\n\n\nPress a key to continue ...");
			getch();
			clrscr();
         printf("Filename: ");
			scanf("%s",filename);
         fp=fopen(filename,"rt");
      }
      if (Demo==1){
   		gn=1;
		}else{
      	if (Demo==0){
				printf("Number of samples: ");
				scanf("%d",&gn);
         }
      }
      printf("\n==================================== RESULTS ==================================\n\n");
   	printf("\n");
   	G.vn=G.ReadNumber(fp);
   	if (G.vn>VN){
   		clrscr();
      	printf("!!! DANGER !!! CANNOT OPEN FILE: %s\n",filename);
			printf("THE TOTAL NUMBER OF VERTICES IS %d AND IT EXCEEDS THE MAXIMUM LIMIT OF %d!\n",G.vn,VN);
      	printf("\n\n\n\n\nPress a key to continue ...");
			getch();
      	fclose(fp);
			clrscr();
      	return;
   	}
		G.cn=G.ReadNumber(fp);
   	if (G.cn>CN){
   		clrscr();
      	printf("!!! DANGER !!! CANNOT OPEN FILE: %s\n",filename);
			printf("THE TOTAL NUMBER OF COLOURS IS %d AND IT EXCEEDS THE MAXIMUM LIMIT OF %d!\n",G.cn,CN);
      	printf("\n\n\n\n\nPress a key to continue ...");
			getch();
      	fclose(fp);
			clrscr();
      	return;
   	}
      sprintf(buffer,"%c%c_results_MVCA_%d_%d.txt",filename[0],filename[1],G.vn,G.cn);
      results=fopen(buffer,"wt");
   	if (results==NULL) {
   		printf("CANNOT OPEN FILE: %s\n",buffer);
      	printf("\n\n\n\nPress a key to continue ...");
			getch();
			clrscr();
			return;
		}
   	fprintf(results,"%d %d\n\n",G.vn,G.cn);
   	H.InitEmptyGraph(G);
      sprintf(vector,"heur%d%c%c%d.txt",G.vn,filename[0],filename[1],G.cn);
      heuristic=fopen(vector,"wt");
      value=0;
		u=0.000;
		for (k=0; k<gn; k++) {
			u1=clock();
			G.InitGraph(fp);
      	H.my_MVCA(G);
         H.PostOptimization();
      	u2=clock();
      	printf("\t\t\t\t    SAMPLE %d \n",k+1);
      	fprintf(results,"\t\t\t\t    SAMPLE %d \n",k+1);
      	H.PrintSolution();
      	H.PrintGraph();
      	H.PrintResultsToFile(results);
         fprintf(heuristic,"%d\n",H.sol_cn);
      	value=value+H.sol_cn;
         // u = u+ (double(u2 - u1)/1000); //sec
			u = u+ (double(u2 - u1));   //msec
   	}

		AvgValue=(value+0.0)/gn;
   	printf("\n************************ SPERIMENTALS EXECUTING VALUES: ************************\n\n");
   	printf("\t\t\t   Average Value: %f\n",AvgValue);
		printf("\t\t\t   Average Time (msec): %f\n",(double(u/gn)));
   	printf("\n\t\tResults saved in the file: \\%s\n",buffer);
   	fprintf(results,"\n************************ SPERIMENTALS EXECUTING VALUES: ************************\n\n");
   	fprintf(results,"\t\t\t   Average Value: %f\n",AvgValue);
		fprintf(results,"\t\t\t   Average Time (msec): %f\n",(double(u/gn)));
   	fclose(fp);
   	fclose(results);
      fclose(heuristic);

   	G.ClearAll();
		for (k=0; k<H.cn; k++) {
			H.L[k].c=H.cn;
      	H.L[k].en=0;
			H.L[k].root=NULL;
		}
   	H.cn=0;
   	H.sol_cn=0;
      delete Temporary_Vector;

		printf("\n\n\n\n\n\n\t\t\t   ANOTHER SIMULATION? (y/n): ");
	   answer = getch();
      textmode(C80);
      clrscr();
   }while (answer=='y'||answer=='Y');
}

