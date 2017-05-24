#include "kalmanFilter.h"
kalmanFilter::kalmanFilter(action_matrix & Atemp, action_matrix & Htemp, action_matrix & Ltemp, action_matrix & Qtemp, action_matrix & Rtemp, action_matrix & Ptemp, action_matrix & Xtemp)
{
	Aa = Atemp;
	H = Htemp;
	L = Ltemp;
	Q = Qtemp;
	R = Rtemp;
	P = Ptemp;
	X = Xtemp;

	K = action_matrix(X.get_row(),H.get_row());
}

action_matrix kalmanFilter::estimateState(action_matrix& Z,action_matrix& U)
{
	//cout<<"X(k-1):"<<endl;
	//cout<<X;
	//
	//cout<<"A(k):"<<endl;
	//cout<<Aa;
	//
	//cout<<"U"<<endl;
	//cout<<U;
	//
	//cout<<"L"<<endl;
	//cout<<L;
	//
	//cout<<"Q"<<endl;
	//cout<<Q;
	//
	//cout<<"R"<<endl;
	//cout<<R;
	//
	//cout<<"H"<<endl;
	//cout<<H;
	//
	//cout<<"P(k-1):"<<endl;
	//cout<<P;
	
	X = Aa*X+U;  //得到预测的状态矩阵X（K|K-1）
	
	//cout<<"X(k|k-1):"<<endl;
	//cout<<X;
	
	P = Aa*P * (!Aa) + L*Q * (!L);  //得到预测误差的协方差矩阵
	
	//cout<<"P(k|k-1):"<<endl;
	//cout<<P;

	K = P * (!H)*(~(H*P * (!H) + R));
	
	//cout<<"K:"<<endl;
	//cout<<K;
	
	X = X + K*(Z - H*X);
	
	//cout<<"X(k):"<<endl;
	//cout<<X;
	
	P = (action_matrix(K.get_row(), H.get_column(), MATRIX_I) - K*H)*P;
	
	//cout<<"P(k):"<<endl;
	//cout<<P;
	
	return X;
}

fadingKalmanFilter::fadingKalmanFilter(action_matrix& Atemp, action_matrix& Htemp, action_matrix& Ltemp, 
																			 action_matrix& Qtemp, action_matrix& Rtemp, action_matrix& Ptemp,
																			 action_matrix& Xtemp,	     	 uint32_t len)
 :kalmanFilter(Atemp,Htemp,Ltemp,Qtemp,Rtemp,Ptemp,Xtemp),len(len)
{
	r=new action_matrix[len];
	K = action_matrix(X.get_row(),H.get_row());
}
fadingKalmanFilter::~fadingKalmanFilter()
{
	delete[] r;
}
action_matrix fadingKalmanFilter::estimateState(action_matrix& Z,action_matrix& U)
{
	X=Aa*X+U;
	
	//更新新息序列
	for(uint32_t count=0;count<len-1;count++)
	{
		r[count]=r[count+1];
	}
	r[len-1]=Z-H*X;
	
	float factorP=0.0;
	if(r[0].get_row()!=0&&r[0].get_column()!=0)
	{
		//更新计算新息的协方差矩阵
		action_matrix Cr=r[0]*(!r[0]);
		for(uint32_t count=1;count<len;count++)
		{
			Cr=Cr+r[count]*(!r[count]);
		}
		Cr=Cr/static_cast<float>(len-1);
		
		
		factorP=tr(Cr-H*L*Q*(!L)*(!H)-R)/tr(H*Aa*P*(!Aa)*(!H));
		if(factorP<1)
			factorP=1;
	}
	else
	{
		factorP=1;
	}
	
	P=factorP*Aa*P*(!Aa)+L*Q*(!L);
	K=P*(!H)*(~(H*P*(!H)+R));
	X=X+K*(Z-H*X);
	P=(action_matrix(K.get_row(),H.get_column(),MATRIX_I)-K*H)*P; 
	return X;
}
