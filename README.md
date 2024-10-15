## include header:
``` cpp
#include<cmath>  
#include<iomanip>  
#include<vector>
```  

## contains constant
``` cpp
const double t_095_sqrt_n[11]={0,0,0,2.48,1.59,1.204,1.05,0.926,0.834,0.770,0.715};
``` 

## class Var:
``` cpp
static void changeTimes(int TIMES);
double Average();  
double Standard_Deviation();  
double Delta_A();  
double Uncertainty();   
void OriginalData();   
void ShowAll();  
```

**All Output lines of MarkDown Table**

## class AddVar
have a function : calc(const std::vector<Var*>& OriginalValue, int VarNum)  
used to initialize, no input or output

## Class Table
``` cpp
std::vector<Var*> Vars_;

Table();  
//It will automatically ask the times and Variable numbers

Table(int VAR_CNT,int EXP_TIMES);  
void Initialize(int VAR_CNT = 3,int EXP_TIMES = 5);  
//Asks the name, unit, \Delta B and values of Variables  
//if input DeltaB as -1, consider the variable is only measured once

void all_Original_Data();  
//Show all original Datas

void calc_All();  
//Show the calculated table(in Markdown grammar)

void add_Var(char* NAME, char* UNIT, double(*FUNCTION)(double* V, int VarNum));  
//add a new variable
```

## To get a certian Variable, use `<TableObjectName>Vars_[i]` returning a pointer `Var*` 

## add if U need to output in files, add the following code
``` cpp
Table t1;  
freopen("data.md","w",stdout);  
//main code
fclose(stdout);  
```
