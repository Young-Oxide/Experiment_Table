#ifndef EXP_TABLE_TABLE_H
#define EXP_TABLE_TABLE_H

//version 1.3.1

#include<iostream>
#include<cmath>
#include<iomanip>
#include<vector>
#include<cstring>

const double t_095_sqrt_n[11]={0,0,0,2.48,1.59,1.204,1.05,0.926,0.834,0.770,0.715};

class Var{
	friend class AddVar;
protected:
    static int VarTimes_;
	char* Name_;
	char* Unit_;
    double DeltaB_;
	double* Value_;
	virtual double partial_derivative(const std::vector<Var*>& V, int Var_i, int VarNum){return 0.0;}
public:
	Var();
	Var(char* NAME, char* UNIT, double DeltaB = 0);
    virtual ~Var();
    static void changeTimes(int TIMES){ Var::VarTimes_ = TIMES;	}
    
	double Average();
	double Standard_Deviation();
	double Delta_A();
	virtual double Uncertainty();
	void OriginalData();
	virtual void ShowAll();
};
class AddVar : public Var{
	double Ucr_;//Uncertainty
	double (*func)(double* V, int VarNum);
	double partial_derivative(const std::vector<Var*>& V, int Var_i, int VarNum);
public:
	AddVar(char* NAME, char* UNIT,int VarNum, double(*FUNCTION)(double* V, int VarNum));
	~AddVar();
	void calc(const std::vector<Var*>& OriginalValue, int VarNum);
	double Uncertainty();
	void ShowAll();
};

class Table{
	int ExpTimes_;
	int VarCnt_;
public:
	std::vector<Var*> Vars_;
	//NEVER USE THIS! SB STD!
	Table();
	Table(int VAR_CNT,int EXP_TIMES);
	~Table();
	void Initialize(int VAR_CNT = 3,int EXP_TIMES = 5);
	void all_Original_Data();
	void calc_All();
	void add_Var(char* NAME, char* UNIT, double(*FUNCTION)(double* V, int VarNum));
};

int Var::VarTimes_ = 0;

Var::Var(){//initialize a sigle new var
	std::cout<<"Input Name, Unit, DeltaB (separate with \\Space or \\Enter):\n";
	this->Name_ = new char[40];
	this->Unit_ = new char[10];
	std::cin>>this->Name_>>this->Unit_>>this->DeltaB_;
//	std::cout<<"input: "<<Name_<<" "<<Unit_<<" "<<DeltaB_<<"\n";
	this->Value_ = new double[VarTimes_];
	if(this->DeltaB_ == -1){
		std::cout<<"Input the single value of "<<this->Name_<<":";
		double tempValue;
		std::cin>>tempValue;
		
		std::cout<<"Input real Uncertainty:";
		std::cin>>this->DeltaB_;
		
		for(int i=0;i<VarTimes_;i++) this->Value_[i] = tempValue;
		
		return ;
	}
	std::cout<<"Input the Values of all "<<VarTimes_<<" experiments:\n";
	for(int i=0;i<VarTimes_;i++){
		std::cin>>this->Value_[i];
	}
}
Var::Var(char* NAME,char* UNIT,double DELTA_B){//create an empty Var, Value_ not calculated.
	this->Name_ = new char[40];
	this->Unit_ = new char[10];
	strcpy(Name_, NAME);
	strcpy(Unit_, UNIT);
	this->DeltaB_ = DELTA_B;
	this->Value_ = new double[VarTimes_];
}
Var::~Var(){
//	std::cout<<"Var desconstructor called.\n";
//	this->OriginalData();
    if(Name_) delete Name_;
    if(Unit_) delete Unit_;
    if(Value_) delete Value_;
}
double Var::Average(){
	double ans=0;
	for(int i=0;i<VarTimes_;i++)
		ans+=this->Value_[i];
	ans/=VarTimes_;
	return ans;
}
double Var::Standard_Deviation(){
    if(VarTimes_==1) return this->Value_[0];
    double ans = 0;
    for(int i = 0 ;i < VarTimes_ ;i++)
        ans += pow(this->Value_[i] - this->Average(),2);
    ans /= (VarTimes_ - 1);
    return sqrt(ans);
}
double Var::Delta_A(){
    return t_095_sqrt_n[VarTimes_] * this->Standard_Deviation();
}
double Var::Uncertainty(){
//	if(this->DeltaB_ == -1) return 0;
    return sqrt(pow(this->Delta_A(),2)+pow(this->DeltaB_,2));
}
void Var::OriginalData(){
	std::cout<<"| $";
	std::cout<<this->Name_<<"/"<<this->Unit_<<" $|";
	for(int i=0;i<VarTimes_;i++)
		std::cout<<" $"<<this->Value_[i]<<"$ |";
	std::cout<<"\n";
}
void Var::ShowAll(){
	std::cout<<"| $"<<Name_<<"/"<<Unit_<<"$ |";
	std::cout<<" "<<this->Average()<<" | ";
	std::cout<<" "<<this->Standard_Deviation()<<" | ";
	std::cout<<" "<<this->Delta_A()<<" | ";
	if(this->DeltaB_!=-1) std::cout<<" "<<this->DeltaB_<<" | ";
	else std::cout<<" 0 |";
	std::cout<<" "<<this->Uncertainty()<<" | ";
	std::cout<<" "<<this->Uncertainty() * 100/ this->Average()<<"$\\\%$ |\n";
}

AddVar::AddVar(char* NAME, char* UNIT,int VarNum, double(*FUNCTION)(double* V,int VarNum)):Var(NAME,UNIT){
	func = FUNCTION;
}
AddVar::~AddVar(){//auto call ~Var
}
void AddVar::calc(const std::vector<Var*>& OriginalValue,int VarNum){
	
	for(int t = 0; t < VarTimes_; t++){//calc for Exp.t
	
		double* temp=new double[VarNum];
		//store all Vars' Value at Exp.t
		for(int i=0;i<VarNum;i++)
			temp[i] = OriginalValue[i]->Value_[t];
		
		this->Value_[t] = func(temp, VarNum);
		
		delete temp;
	}
	//calc uncertianty
	double sum=0;
	for(int i = 0; i < VarNum; i++)
		sum+=pow(this->partial_derivative(OriginalValue,i,VarNum) *
		 OriginalValue[i]->Uncertainty() / OriginalValue[i]->Average(),2);
	this->Ucr_ = sqrt(sum);
	this->OriginalData();
}
double AddVar::partial_derivative(const std::vector<Var*>& V, int Var_i,int VarNum){//dF/d{x_i}|x=x\bar

	double* tempA=new double[VarNum];
	//store all tempVars' Value at Exp.t in tempD 
	for(int i=0;i<VarNum;i++)
		tempA[i] = V[i]->Average();
	double F0 = func(tempA, VarNum);

	const double stepLength = 0.00001;
	tempA[Var_i]+=stepLength;
	double F1 = func(tempA, VarNum);
	
	delete tempA;
	
	return (F1 - F0)/stepLength;
}
double AddVar::Uncertainty(){
	return Ucr_ * this->Average();
}
void AddVar::ShowAll(){
	std::cout<<"| $"<<Name_<<"/"<<Unit_<<"$ |";
	std::cout<<" "<<this->Average()<<" | ";
	std::cout<<" "<<this->Standard_Deviation()<<" | ";
	std::cout<<" "<<"---"<<" | ";
	std::cout<<" "<<"---"<<" | ";
	std::cout<<" "<<this->Ucr_ * this->Average()<<" | ";
	std::cout<<" "<<this->Ucr_ * 100<<"$\\\%$ |\n";
}

Table::Table(){
	std::cout<<"Input VarCount and ExperimentTimes:";
	int VC,ET;
	std::cin >> VC >> ET;
	this->Initialize(VC, ET);
}
Table::~Table(){//delete all var
//	std::cout<<"table decon!\n";
	for(std::vector<Var*>::iterator i = Vars_.begin(); i < Vars_.end(); i++)
		delete (*i);
	Vars_.clear();
}
Table::Table(int VAR_CNT,int EXP_TIMES){
	this->Initialize(VAR_CNT, EXP_TIMES);
}
void Table::Initialize(int VAR_CNT,int EXP_TIMES){
		
	//initialize all constants
	VarCnt_ = VAR_CNT;
	ExpTimes_ = EXP_TIMES;
	Var::changeTimes(EXP_TIMES);
    
	//set standard output
	std::cout<<std::fixed<<std::setprecision(3);
	
	//initialize Vars_
	if(!Vars_.empty()) Vars_.clear();
	for(int i = 0; i < VarCnt_; i++){
        std::cout<<"New Variable No "<<i+1<<": ";
		Var* temp = new Var();
		Vars_.push_back(temp);
//		std::cout<<"Var pushed\n";
	}
//	std::cout<<"Init fin!";
}
void Table::all_Original_Data(){
	//first line
	std::cout<<"\n|"<<"    "<<"|";
	for(int t=0;t<ExpTimes_;t++) std::cout<<t+1<<"|";
	//second line
	std::cout<<"\n|----|";
	for(int t=0;t<ExpTimes_;t++) std::cout<<"----|";
	std::cout<<"\n";
	
	for(std::vector<Var*>::iterator i = Vars_.begin(); i < Vars_.end() ; i++)
		(*i)->OriginalData();
    std::cout<<"\n";
}
void Table::calc_All(){
	std::cout<<"|   | Average | $\\sigma$ | $\\Delta A$ | $\\Delta B$ | $u$ | $u_r$ |\n";
    std::cout<<"|----| ---- | ---- | ---- | ---- | ---- |---- |\n";
	
	for(std::vector<Var*>::iterator i = Vars_.begin(); i<Vars_.end(); i++)
		(*i)->ShowAll();
		
	std::cout<<"\n";
}
void Table::add_Var(char* NAME, char* UNIT, double(*FUNCTION)(double* V,int VarNum)){
	AddVar* temp = new AddVar(NAME,UNIT,VarCnt_,FUNCTION);
	temp->calc(Vars_, VarCnt_);
	Vars_.push_back(temp);
	VarCnt_++;
}
#endif //EXP_TABLE_TABLE_H
