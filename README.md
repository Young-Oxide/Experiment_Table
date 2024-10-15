## include header:
cmath  
iomanip  
vector  

## contains constant
const double t_095_sqrt_n[11]={0,0,0,2.48,1.59,1.204,1.05,0.926,0.834,0.770,0.715};

## class Var:
static void changeTimes(int TIMES)  
double Average();
double Standard_Deviation();
double Delta_A();误
double Uncertainty();不 
void OriginalData();原  
void ShowAll();
**All Output lines of MarkDown Table**

## class AddVar
have a function : calc(const std::vector<Var*>& OriginalValue, int VarNum)  
used to initialize, no input or output

## Class Table
std::vector<Var*> Vars_;

Table();  
**It will automatically ask the times and Variable numbers**

Table(int VAR_CNT,int EXP_TIMES);  
void Initialize(int VAR_CNT = 3,int EXP_TIMES = 5);  
**Asks the name, unit, \Delta B and values of Variables**

void all_Original_Data();  
**Show all original Datas**

void calc_All();  
**Show the calculated table(in Markdown grammar)**

void add_Var(char* NAME, char* UNIT, double(*FUNCTION)(double* V, int VarNum));  
**add a new variable**

## To get a certian Variable, use <TableObjectName>.Vars_[i]，returning a pointer Var*

## add if U need to output in files, add the following code
Table t1;  
freopen("data.md","w",stdout);  
//main code
fclose(stdout);  
