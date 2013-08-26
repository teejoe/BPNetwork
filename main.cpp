#include "BPNetwork.h"
#include <memory.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;


/*训练输入和输出数据（共75组，每组4个特征输入）*/
double **trainInput;
double **trainOutput;
	
/*测试输入和输出数据*/
double testInput[75][4];
int testOutput[75];		//测试数据的实际输出
int testTarget[75];		//测试数据的目标输出

/**激活函数和其对应的导函数**/
double ActiveFunc1(double x)		//s型函数
{
	return 1.0 / (1.0 + exp(-x));
}
double ActiveFuncD1(double x)
{
	return exp(-x) / pow(1.0 + exp(-x), 2);
}
double ActiveFunc2(double x)		//正弦函数
{
	return sin(x);
}
double ActiveFuncD2(double x)
{
	return cos(x);
}
double ActiveFunc3(double x)		//双曲正切函数
{
	return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}
double ActiveFuncD3(double x)
{
	return 4.0 / pow(exp(x) + exp(-x), 2);
}
double ActiveFunc4(double x)		//反正切函数
{
	return atan(x);
}
double ActiveFuncD4(double x)
{
	return 1.0 / (1.0 + x * x);
}
////////////////////////////////////////////////////////////////////


/**读取训练数据**/
bool readTrainData(char *filePath)
{
	if (freopen(filePath, "r", stdin) == NULL)
		return false;
	string strKind;
	for (int i = 0; i < 75; i++)
	{

		/*读取输入向量*/
		for (int j = 0; j < 4; j++)
		{
			cin>>trainInput[i][j];
		}
		
		/*读取输出向量(根据iris花种类判定，输出向量为
		(1, 0, 0), (0, 1, 0), (0, 0, 1)三者之一)*/
		cin>>strKind;
		if (strKind == "setosa")
		{
			trainOutput[i][0] = 1.0;
		}
		else if(strKind == "versicolor")
		{
			trainOutput[i][1] = 1.0;
		}
		else if (strKind == "virginica")
		{
			trainOutput[i][2] = 1.0;
		}
	}
	fclose(stdin);
	
	return true;
}

/**读取测试数据**/
bool readTestData(char *filePath)
{
	if (freopen(filePath, "r", stdin) == NULL)
		return false;
	string strKind;
	for (int k = 0; k < 75; k++)
	{
		for (int i = 0; i < 4; i++)
			cin>>testInput[k][i];
		
		cin>>strKind;
		if (strKind == "setosa")
			testTarget[k] = 0;
		else if(strKind == "versicolor")
			testTarget[k] = 1;
		else if (strKind == "virginica")
			testTarget[k] = 2;
		
	}
	fclose(stdin);
	
	return true;
}

/**写出分类数据**/
bool writeOutput(char *filePath)
{
	int nError = 0;
	cout<<"分类结果:"<<endl;
	for (int j = 0; j < 75; j++)
	{
		if (testOutput[j] != testTarget[j])
			nError++;
		
		switch (testOutput[j])
		{
		case 0:
			cout<<"setosa"<<endl;
			break;
		case 1:
			cout<<"versicolor"<<endl;
			break;
		case 2:
			cout<<"virginica"<<endl;
			break;
		}
	}
	cout<<"\n\n识别错误: "<<nError<<"个"<<endl;
	
	return true;
}

int main(int argc, char **argv)
{
	int nHide;		//隐节点个数
	double speed;	//学习速率
	int nFunc;		//激活函数种类
	
	trainInput = BPNetwork::Alloc2DArray(75, 4);
	trainOutput = BPNetwork::Alloc2DArray(75, 3);;
	
	cout<<"请选选择激活函数:"<<endl;
	cout<<"1. s型函数  2. 正弦函数  3. 双曲正切函数  4. 反正切函数"<<endl;
	cin>>nFunc;
	cout<<"请输入隐节点个数：";
	cin>>nHide;
	cout<<"\n请输入学习速率(0~1.0):";
	cin>>speed;

	/*读取训练数据*/
	if (!readTrainData("train.dat"))
	{
		cout<<"读取训练数据失败！"<<endl;
		return -1;
	}
	
	/*读取测试数据*/
	if (!readTestData("test.dat"))
	{
		cout<<"读取测试数据失败!"<<endl;
		return -1;
	}
	
	/*创建神经网络*/
	BPNetwork bpn;
	bpn.Create(4, nHide, 3);
	bpn.SetLearningSpeed(speed);
	switch (nFunc)	//选择激活函数
	{
	case 1:
		bpn.SetActiveFunc(ActiveFunc1);
		bpn.SetActiveFuncD(ActiveFuncD1);
		bpn.SetMaxMinOutput(1.0, 0.0);
		break;
	case 2:
		bpn.SetActiveFunc(ActiveFunc2);
		bpn.SetActiveFuncD(ActiveFuncD2);
		bpn.SetMaxMinOutput(1.0, -1.0);
		break;
	case 3:
		bpn.SetActiveFunc(ActiveFunc3);
		bpn.SetActiveFuncD(ActiveFuncD3);
		bpn.SetMaxMinOutput(1.0, -1.0);
		break;
	case 4:
		bpn.SetActiveFunc(ActiveFunc4);
		bpn.SetActiveFuncD(ActiveFuncD4);
		bpn.SetMaxMinOutput(1.57, -1.57);
		break;
	default:
		break;
	}
	
	/*训练BP神经网络*/
	cout<<"正在学习中，请稍后..."<<endl;
	int count = bpn.Train(trainInput, trainOutput, 75);
	
	/*测试BP神经网络*/
	for (int k = 0; k < 75; k++)
	{
		testOutput[k] = bpn.Classify(testInput[k]);
	}
	writeOutput("testclassify.dat");
	cout<<"迭代次数: "<<count<<endl;
	
	return 0;
}








