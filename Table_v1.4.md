# 包含库
``` cpp
cmath
iomanip
vector
```

# 内置常量
``` cpp
const double t_095_sqrt_n[11]={0,0,0,2.48,1.59,1.204,1.05,0.926,0.834,0.770,0.715};
```

# 类Var拥有私静态成员 VarTimes_
``` cpp
static void changeTimes(int TIMES)
double Average();平均数
double Standard_Deviation();标准差
double Delta_A();误差A
double Uncertainty();不确定度
void OriginalData();原始数据
void ShowAll();计算结果，用于Table::calc_All()
//这些函数的输出都是无表头的markdown格式
``` 

## 派生出AddVar类，拥有相同功能
增加 `calc(const std::vector<Var*>& OriginalValue, int VarNum)` 用于初始化，无返回值和输出  
新添加函数 `void AllPartial(const std::vector<Var*>& V,int VarNum)` ,用于输出对应于此变量的所有偏导数

# 类Table拥有一个公有的vector对象，存储了各个变量的数据
``` cpp
std::vector<Var*> Vars_;	  

Table();  
//会自动询问变量的数量和实验次数

Table(int VAR_CNT,int EXP_TIMES);  
void Initialize(int VAR_CNT = 3,int EXP_TIMES = 5);  
//会自动询问所有变量的信息  
//依次是变量的名称单位$\Delta B$，以及每一次实验的数据  
//如果输入的$\Delta B$为-1，则认为是只测量一次的变量   
//名称最大40字符，单位最大10字符，支持markdown语法  

void all_Original_Data();  
//显示所有的原始数据  

void calc_All();  
//显示计算后的表格，markdown格式  

void add_Var(char* NAME, char* UNIT, double(*FUNCTION)(double* V, int VarNum));  
//添加名为NAME/UNIT的变量，使用FUNCIOTION进行计算，V的下标对应原始变量的下标  
```

**如果需要调用某个特定变量，请使用 `TableObjectName.Vars_[i]` , 返回的是一个 `Var*` 类型的指针**  

## 如果需要输出在文档中，自行添加
``` cpp
Table t1;
freopen("data.md","w",stdout);
//代码主体
fclose(stdout);
```
