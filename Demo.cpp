#include <iostream>
#include "Table.h"

using namespace std;

double calcR(double* V,int VarNum){
	return V[0]/V[1];
}

int main(){
	Table t1(2,3);
	
	t1.add_Var("R=\\frac{U}{I}","\\Omega",calcR);

	//If you want to output in files, do not comment the code below
	// freopen("data.markdown","w",stdout);
	t1.all_Original_Data();
	t1.calc_All();
	// fclose(stdout);
	// freopen("CON","w",stdout);
	cout<<"Output finished!\n";
	return 0;
}
