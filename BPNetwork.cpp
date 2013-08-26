/********************************************************************
 * File			:	BPNetwork.cpp
 * Author		:	Tianjiao Mao
 * Date			:	2011.11.15
 * Description	:	This file implements the BPNetwork class which is
 *					defined in "BPNetwork.h".
 *******************************************************************/
 
#include "BPNetwork.h"
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;
 
BPNetwork::BPNetwork()
{
	m_inputUnits = NULL;
	m_hideUnits = NULL;
	m_outputUnits = NULL;
	m_hideUnitsIn = NULL;
	m_outputUnitsIn = NULL;
	m_deltaHide = NULL;
	m_deltaOut = NULL;
	m_target = NULL;
	m_theta = NULL;
	m_gamma = NULL;
	m_wInToHide = NULL;
	m_wHideToOut = NULL;
	m_pActiveFunc = NULL;
	m_pActiveFuncD = NULL;
	m_maxOut = 1.0;
	m_minOut = 0;
	m_speed = 0.1;
}
 
BPNetwork::~BPNetwork()
{
	/*释放所有动态申请的内存空间*/
	if (m_inputUnits != NULL)
		delete []m_inputUnits;
	if (m_hideUnitsIn != NULL)
		delete []m_hideUnitsIn;
	if (m_outputUnitsIn != NULL)
		delete []m_outputUnitsIn;
	if (m_hideUnits != NULL)
		delete []m_hideUnits;
	if (m_outputUnits != NULL)
		delete []m_outputUnits;
	if (m_target != NULL)
		delete []m_target;
	if (m_gamma != NULL)
		delete []m_gamma;
	if (m_theta != NULL)
		delete []m_theta;
	if (m_deltaHide != NULL)
		delete []m_deltaHide;
	if (m_deltaOut != NULL)
		delete []m_deltaOut;
	if (m_wInToHide != NULL)
	{
		for (int i = 0; i < m_nNodeIn; i++)
			delete []m_wInToHide[i];
		
		delete []m_wInToHide;
	}
	if (m_wHideToOut != NULL)
	{
		for (int i = 0; i < m_nNodeHide; i++)
			delete []m_wHideToOut[i];
		
		delete []m_wHideToOut;
	}
}
 
/**获得一个-1到1的随机数**/
double BPNetwork::GetRandom()
{
	return ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
}


/**设置输出层神经元输出的最大值和最小值
 * 其中(min, max)应为节点激活函数的值域
 */
void BPNetwork::SetMaxMinOutput(double max, double min)
{
	m_maxOut = max;
	m_minOut = min;
}

/**设置节点激活函数**/
void BPNetwork::SetActiveFunc(Func f)
{
	m_pActiveFunc = f;
}

/**设置节点激活函数的导数**/
void BPNetwork::SetActiveFuncD(Func f)
{
	m_pActiveFuncD = f;
}


/**随机初始化连接矩阵权值
 * 输入：	W	权值矩阵指针
 * 			m	矩阵行数
 *			n	矩阵列数
 */
void BPNetwork::RandWeights(double **W, int m, int n)
{
	srand(time(NULL));
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			W[i][j] = GetRandom();
		}
	}
}

/**用(0,1)的随机值初始化一维阈值数组
 * 输入：	th	数组指针
 * 			m	数组长度
 */
void BPNetwork::RandThreshold(double *th, int m)
{
	srand(time(NULL));
	for (int i = 0; i < m; i++)
	{
		th[i] = (double)rand() / (double)RAND_MAX;
	}
}

/**设置学习速率**/
void BPNetwork::SetLearningSpeed(double speed)
{
	m_speed = speed;
}	
 
/**创建BP网络,为数组动态分配内存，并随机初始化连接权值矩阵
 * 输入:	nIn		输入层节点数
 *			nHide	隐藏层节点数
 *			nOut	输出层节点数
 */ 
bool BPNetwork::Create(int nIn, int nHide, int nOut)
{
	m_nNodeIn = nIn;
	m_nNodeHide = nHide;
	m_nNodeOut = nOut;

	m_inputUnits = new double[nIn];
	m_hideUnits = new double[nHide];
	m_outputUnits = new double[nOut];
	
	m_hideUnitsIn = new double[nHide];
	m_outputUnitsIn = new double[nOut];
	
	m_target = new double[nOut];

	m_deltaHide = new double[nHide];
	m_deltaOut = new double[nOut];
	
	m_theta = new double[nHide];
	m_gamma = new double[nOut];
	
	m_wInToHide = Alloc2DArray(nIn, nHide);
	m_wHideToOut = Alloc2DArray(nHide, nOut);
	
	/*将各个数组清零*/
	memset(m_inputUnits, 0, sizeof(double) * nIn);
	memset(m_hideUnitsIn, 0, sizeof(double) * nHide);
	memset(m_outputUnitsIn, 0, sizeof(double) * nOut);
	memset(m_hideUnits, 0, sizeof(double) * nHide);
	memset(m_outputUnits, 0, sizeof(double) * nOut);
	memset(m_target, 0, sizeof(double) * nOut);
	memset(m_deltaHide, 0, sizeof(double) * nHide);
	memset(m_deltaOut, 0, sizeof(double) * nOut);
	
	/*随机初始化连接矩阵权值*/
	RandWeights(m_wInToHide, nIn, nHide);
	RandWeights(m_wHideToOut, nHide, nOut);
	
	/*随机初始化隐藏层和输出层阈值*/
	RandThreshold(m_theta, nHide);
	RandThreshold(m_gamma, nOut);
	
	return true;
}

/**申请二维双精度数组
 * 输入：	m	数组行数
 *			n	数组列数
 */
double** BPNetwork::Alloc2DArray(int m, int n)
{
	double **pMatrix;
	pMatrix = new double*[m];
	
	for(int i = 0; i < m; i++)
	{
		pMatrix[i] = new double[n];
		memset(pMatrix[i], 0, sizeof(double) * n);
	}
	return pMatrix;
}


/**前向计算
 * 输入：	input	输入向量
 */
void BPNetwork::CalculateForward()
{
	memset(m_hideUnitsIn, 0, sizeof(double) * m_nNodeHide);
	memset(m_outputUnitsIn, 0, sizeof(double) * m_nNodeOut);
	
	/*计算隐藏层神经元的激活值*/
	for (int i = 0; i < m_nNodeHide; i++)
	{
		for (int j = 0; j < m_nNodeIn; j++)
		{
			/*计算加权输入*/
			m_hideUnitsIn[i] += m_wInToHide[j][i] * m_inputUnits[j];
		}
		/*计算激活值*/
		m_hideUnitsIn[i] += m_theta[i];	//加上隐藏层阈值
		m_hideUnits[i] = m_pActiveFunc(m_hideUnitsIn[i]); //计算激活值
	}
	
	
	/*计算输出层激活值*/
	for (int k = 0; k < m_nNodeOut; k++)
	{
		for (int i = 0; i < m_nNodeHide; i++)
		{
			m_outputUnitsIn[k] = m_wHideToOut[i][k] * m_hideUnits[i];
		}
		m_outputUnitsIn[k] += m_gamma[k];
		m_outputUnits[k] = m_pActiveFunc(m_outputUnitsIn[k]);
	}
}

/**计算输出层和隐藏层误差**/
void BPNetwork::CalculateDelta()
{
	/*计算输出层误差*/
	for (int i = 0; i < m_nNodeOut; i++)
	{
		m_deltaOut[i] = m_pActiveFuncD(m_outputUnitsIn[i])
						* (m_target[i] - m_outputUnits[i]);
	}
	
	/*计算隐藏层误差*/
	for (int j = 0; j < m_nNodeHide; j++)
	{
		m_deltaHide[j] = 0;
		for (int k = 0; k < m_nNodeOut; k++)
		{
			m_deltaHide[j] += m_wHideToOut[j][k] * m_deltaOut[k];
		}
		m_deltaHide[j] *= m_pActiveFuncD(m_hideUnitsIn[j]);	
	}
}

/**调整连接矩阵权值**/
void BPNetwork::AdjustWeights()
{
	/*调整隐藏层到输出层连接矩阵权值*/
	for (int i = 0; i < m_nNodeHide; i++)
	{
		for (int j = 0; j < m_nNodeOut; j++)
		{
			m_wHideToOut[i][j] += m_speed * m_hideUnits[i] * m_deltaOut[j];
		}
	}
	
	/*调整输入层到隐藏层连接矩阵权值*/
	for (int k = 0; k < m_nNodeIn; k++)
	{
		for (int j = 0; j < m_nNodeHide; j++)
		{
			m_wInToHide[k][j] += m_speed * m_inputUnits[k] * m_deltaHide[j];
		}
	}
}

/**调整隐藏层与输出层阈值**/
void BPNetwork::AdjustThreshold()
{
	for (int i = 0; i < m_nNodeOut; i++)
		m_gamma[i] += m_speed * m_deltaOut[i];
		
	for (int j = 0; j < m_nNodeHide; j++)
		m_theta[j] += m_speed * m_deltaHide[j];
}

/**判断分类结果是否正确
 * 输入：	output	预期输出结果
 */
bool BPNetwork::IsCorrect(double *output)
{
	int max = 0;		//输出最大节点所在位置
	
	/*分析输出结果（找到输出最大的节点，作为输出结果）*/
	for (int i = 1; i < m_nNodeOut; i++)
	{
		if (m_outputUnits[max] < m_outputUnits[i])
		{
			max = i;
		}
	}

	/*标准输出向量形式为(1,0,0···),(0,1,0···),(0,0,1···),
	其中 '1' 所在一维作为分类结果*/
	if (output[max] == 1.0)
		return true;
	
	return false;
}

/**设定目标向量
 * 输入： pTarget	目标向量
 * 说明：输入的目标向量形式为(1,0,0…),(0,1,0…),(0,0,1…)……,
 *       经过转化变为(m_maxOut,m_minOut,m_minOut…),(m_minOut,m_maxOut,m_minOut…)……
 *       以便更好地学习.
 */
void BPNetwork::SetTargetOutput(double *pTarget)
{
	int n;
	for (int i = 0; i < m_nNodeOut; i++)
	{
		if (pTarget[i] == 1.0)
			n = i;
	}
	for (int j = 0; j < m_nNodeOut; j++)
	{
		m_target[j] = m_minOut;
	}
	m_target[n] = m_maxOut;
}


/**训练BP神经网络
 * 输入：	input	输入集
 * 			output	目标输出集
 *			size	训练集的大小
 * 返回值：	count	迭代次数
 */
int BPNetwork::Train(double **input, double **output, int size)
{
	bool isDone = false;
	int count = 0;	//迭代次数
	while (!isDone)
	{
		isDone = true;
		count++;
		for (int i = 0; i < size; i++)
		{
			/*将输入向量直接传入输入神经元*/
			memcpy(m_inputUnits, input[i], sizeof(double) * m_nNodeIn);
			/*设定目标向量*/
			SetTargetOutput(output[i]);
			//memcpy(m_target, output[i], sizeof(double) * m_nNodeOut);
			
			
			/*进行前向计算*/
			CalculateForward();
			
			/*如果输出结果不正确，继续训练*/
			if (!IsCorrect(output[i]))
			{
				isDone = false;
				
				/*计算误差*/
				CalculateDelta();
				
				/*调整连接矩阵权值*/
				AdjustWeights();
				
				/*调整隐藏层与输出层阈值*/
				AdjustThreshold();
			}	
		}
	}
	return count;
}


/**使用训练好的网络进行分类
 * 输入：input	特征向量
 * 返回值：	0	种类为setosa
 *			1	种类为versicolor
 * 			2	种类为virginica
 *			>2	其他
 */
int BPNetwork::Classify(double *input)
{
	int kind = 0;
	
	/*将输入向量直接传入输入神经元*/
	memcpy(m_inputUnits, input, sizeof(double) * m_nNodeIn);
	
	/*进行前向计算*/
	CalculateForward();
	
	/*分析输出结果（找到输出最大的节点，作为输出结果）*/
	for (int i = 0; i < m_nNodeOut - 1; i++)
	{
		if (m_outputUnits[i] < m_outputUnits[i + 1])
		{
			kind = i + 1;
		}
	}
		
	return kind;
}