/*********************************************************************
 * File			:	BPNetwork.h
 * Author		:	TianJiao Mao
 * Date			:	2011.11.14
 * Description	:	This file defines a class that represent a 3-layer 
 *					BP Network which can be used to deal with classify 
 *					problems.
 ********************************************************************/

#ifndef _BPNETWORK_H_
#define _BPNETWORK_H_


/**
 *	BP神经网络类，表示一个三层BP网络
 */
class BPNetwork
{
	typedef double (*Func)(double x);	//定义一个函数指针类型
public:
	BPNetwork();
	~BPNetwork();
	
	/**创建BP神经网络**/
	bool Create(int nIn, int nHide, int nOut);	
	
	/**设置学习速率**/
	void SetLearningSpeed(double speed);
	
	/**设置节点激活函数**/
	void SetActiveFunc(Func f);
	
	/**设置节点激活函数的导数**/
	void SetActiveFuncD(Func f);
	
	/**设置输出层神经元输出的最大值和最小值(激活函数的值域)**/
	void SetMaxMinOutput(double max, double min);
	
	/**训练BP神经网络**/
	int Train(double **input, double **output, int size);
	
	/**使用训练好的网络进行分类**/
	int Classify(double *input);
	
	/**申请二维双精度数组**/
	static double **Alloc2DArray(int m, int n);
	
private:
	/**获得一个-1到1的随机数**/
	double GetRandom();		
	
	/**设定目标向量**/
	void SetTargetOutput(double *pTarget);
	
	/**随机初始化连接矩阵权值**/
	void RandWeights(double **W, int m, int n);
	
	/**随机初始化一维阈值数组**/
	void RandThreshold(double *th, int m);
	
	/**前向计算**/
	void CalculateForward();
	
	/**计算输出层和隐藏层误差**/
	void CalculateDelta();
	
	/**调整连接矩阵权值**/
	void AdjustWeights();
	
	/**调整隐藏层与输出层阈值**/
	void AdjustThreshold();
	
	/**判断分类结果是否正确**/
	bool IsCorrect(double *output);

private:
	int m_nNodeIn;			//输入层节点数目
	int m_nNodeHide;		//隐藏层节点数目
	int m_nNodeOut;			//输出层节点数目
	double *m_theta;		//隐节点的阈值数组
	double *m_gamma;		//输出节点的阈值数组
	double m_speed;			//学习速率
	double m_maxOut;		//输出神经元输出的最大值
	double m_minOut	;		//输出神经元输出的最小值

	double *m_inputUnits;	//输入层神经元的值
	double *m_hideUnits;	//隐藏层神经元的输出值
	double *m_outputUnits;	//输出层神经元的输出值
	double *m_hideUnitsIn;	//隐藏层神经元的输入值
	double *m_outputUnitsIn;//输出层神经元的输入值
	
	double *m_target;		//目标向量
	
	double *m_deltaHide;	//隐藏层误差
	double *m_deltaOut;		//输出层误差
	
	double **m_wInToHide;	//输入层到隐藏层的连接权值矩阵
	double **m_wHideToOut;	//隐藏层到输出层的连接权值矩阵
			
	Func m_pActiveFunc;		//激活函数
	Func m_pActiveFuncD;	//激活函数的导数

};
#endif //#ifndef _BPNETWORK_H_