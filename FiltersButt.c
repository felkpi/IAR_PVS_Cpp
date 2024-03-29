/*******************************************************************************
 * file:	FiltrsButt.c
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	23-July-2015
 * brief:	�������� ������� �����������
 *******************************************************************************/
#include "FiltersButt.h"

float df2sos_BPF_5_500Hz_10kHzSR(float x)
{
//	const float	b[5][3] = {	1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1};
	const float	a[5][3] = {	1, -1.6326274871826172, 0.78175699710845947,
							1, -1.9985390901565552, 0.99854469299316406,
							1, -1.9961402416229248, 0.99614602327346802,
							1, -1.3890095949172974, 0.51420724391937256,
							1, -1.6548442840576172, 0.65566790103912354};
	const float	g[5] = {0.19212333858013153, 0.19212333858013153,
						0.1770528256893158,  0.1770528256893158,
						0.17216603457927704};
	
	// ���������� �������
	static float w[5][3];

	for (int i = 0; i < 5; i++)
	{
		w[i][2] = w[i][1];
		w[i][1] = w[i][0];
		w[i][0] = x * g[i] - w[i][1] * a[i][1] - w[i][2] * a[i][2];
		x = w[i][0] - w[i][2];
	}
	return x;
}

float df2sos_BPF_10_1000Hz_10kHzSR(float x)
{
	// ������������ ������� 10Hz...1kHz (5 ������ SOS-II)
//	const float	b[5][3] = {	1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1};
	const float	a[5][3] = {	1, -1.1286489963531494, 0.63718229532241821,
							1, -1.9970685243606567,	0.99709117412567139,
							1, -1.9922854900360107, 0.99230831861495972,
							1, -0.87604826688766479, 0.26481443643569946,
							1, -1.3992478847503662, 0.40210747718811035};
	const float	g[5] = {0.35469123721122742, 0.35469123721122742, 0.31168174743652344, 0.31168174743652344, 0.29894626140594482};
	
	// ���������� �������
	static float w[5][3];

	for (int i = 0; i < 5; i++)
	{
		w[i][2] = w[i][1];
		w[i][1] = w[i][0];
		w[i][0] = x * g[i] - w[i][1] * a[i][1] - w[i][2] * a[i][2];
		x = w[i][0] - w[i][2];
	}
	return x;
}

float df2sos_BPF_20_1000Hz_20kHzSR(float x)
{
	// ������������ ������� 20Hz...1kHz (6 ������ SOS-II)
//	const float	b[6][3] = {	1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1};
	const float	a[6][3] = {	1, -1.6524360179901123, 0.81164228916168213,
							1, -1.9976265430450439, 0.99764800071716309,
							1, -1.993446946144104,  0.99346888065338135,
							1, -1.4229068756103516, 0.55735838413238525,
							1, -1.9908925294876099, 0.99091482162475586,
							1, -1.3183649778366089, 0.44040101766586304};
	const float	g[6] = {0.19750736653804779, 0.19750736653804779, 0.18300464749336243,
						0.18300464749336243, 0.17595990002155304, 0.17595990002155304};
	
	// ���������� �������
	static float w[6][3];

	for (int i = 0; i < 6; i++)
	{
		w[i][2] = w[i][1];
		w[i][1] = w[i][0];
		w[i][0] = x * g[i] - w[i][1] * a[i][1] - w[i][2] * a[i][2];
		x = w[i][0] - w[i][2];
	}
	return x;
}

float df2sos_BPF_5_500Hz_50kHzSR(float x)
{
// ������������ ������� 5Hz...500Hz (5 ������ SOS-II)
//	const float	b[5][3] = {	1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1};
	const float	a[5][3] = {	1, -1.9439172744750977, 0.95061343908309937,
							1, -1.9997085332870483, 0.99970877170562744,
							1, -1.99922776222229,   0.99922794103622437,
							1, -1.8693208694458008, 0.87567782402038574,
							1, -1.9206901788711548, 0.92072808742523193};
	const float	g[5] = {0.040721502155065536, 0.040721502155065536, 0.039927292615175247, 0.039927292615175247, 0.039635967463254929};
	
	// ���������� �������
	static float w[5][3];

	for (int i = 0; i < 5; i++)
	{
		w[i][2] = w[i][1];
		w[i][1] = w[i][0];
		w[i][0] = x * g[i] - w[i][1] * a[i][1] - w[i][2] * a[i][2];
		x = w[i][0] - w[i][2];
	}
	return x;
}

float df2sos_BPF_10_1000Hz_50kHzSR(float x)
{
// ������������ ������� 10Hz...1kHz (5 ������ SOS-II)
//	const float	b[5][3] = {	1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1,
//							1,0,-1};
	const float	a[5][3] = {	1, -1.8779246807098389, 0.90398681163787842,
							1, -1.999416708946228,  0.99941766262054443,
							1, -1.9984556436538696, 0.99845653772354126,
							1, -1.7428719997406006, 0.76674926280975342,
							1, -1.8472180366516113, 0.84736412763595581};
	const float	g[5] = {0.080331102013587952, 0.080331102013587952, 
						0.077366642653942108, 0.077366642653942108,
						0.076317943632602692};
	
	// ���������� �������
	static float w[5][3];

	for (int i = 0; i < 5; i++)
	{
		w[i][2] = w[i][1];
		w[i][1] = w[i][0];
		w[i][0] = x * g[i] - w[i][1] * a[i][1] - w[i][2] * a[i][2];
		x = w[i][0] - w[i][2];
	}
	return x;
}

float df2sos_LPF_100Hz_50kHzSR(float x)
{
// ������������ LPF-������� 100Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,2,1,
//							1,2,1,
//							1,2,1};
	const float	a[3][3] = {	1, -1.9913831949234009, 0.99164485931396484,
							1, -1.9770774841308594, 0.97733724117279053,
							1, -1.9689112901687622, 0.9691699743270874};
	const float	g[3] = {0.00006540995673276484, 0.00006494006811408326, 0.000064671832660678774};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] + w[0][1] + w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] + w[1][1] + w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] + w[2][1] + w[2][1] + w[2][2];
	return x;
}

float df2sos_LPF_500Hz_50kHzSR(float x)
{
// ������������ LPF-������� 500Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,2,1,
//							1,2,1,
//							1,2,1};
	const float	a[3][3] = {	1, -1.9493236541748047, 0.95654189586639404,
							1, -1.8786201477050781, 0.88557666540145874,
							1, -1.8400870561599731, 0.8469008207321167};
	const float	g[3] = {0.0018045627512037754, 0.0017391098663210869, 0.0017034383490681648};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] + w[0][1] + w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] + w[1][1] + w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] + w[2][1] + w[2][1] + w[2][2];
	return x;
}

float df2sos_LPF_800Hz_50kHzSR(float x)
{
// ������������ LPF-������� 800Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,2,1,
//							1,2,1,
//							1,2,1};
	const float	a[3][3] = {	1, -1.9133098125457764, 0.93151843547821045,
							1, -1.8061904907226562, 0.82337969541549683,
							1, -1.7496356964111328, 0.76628667116165161};
	const float	g[3] = {0.0045521622523665428, 0.0042973030358552933, 0.004162747412919998};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] + w[0][1] + w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] + w[1][1] + w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] + w[2][1] + w[2][1] + w[2][2];
	return x;
}

float df2sos_LPF_1000Hz_50kHzSR(float x)
{
// ������������ LPF-������� 1kHz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,2,1,
//							1,2,1,
//							1,2,1};
	const float	a[3][3] = {	1, -1.8871375322341919, 0.9153016209602356,
							1, -1.758173942565918, 0.78441327810287476,
							1, -1.6914380788803101, 0.71668142080307007};
	const float	g[3] = {0.0070410142652690411, 0.0065598436631262302, 0.0063108485192060471};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] + w[0][1] + w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] + w[1][1] + w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] + w[2][1] + w[2][1] + w[2][2];
	return x;
}

float df2sos_HPF_5Hz_5kHzSR(float x)
{
// ������������ HPF-������� 5Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,-2,1,
//							1,-2,1,
//							1,-2,1};
	const float	a[3][3] = {	1, -1.9976041316986084, 0.99762523174285889,
							1, -1.9935042858123779, 0.99352526664733887,
							1, -1.9911447763442993, 0.99116581678390503};
	const float	g[3] = {0.99880731105804443, 0.9967573881149292, 0.99557769298553467};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] - w[0][1] - w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] - w[1][1] - w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] - w[2][1] - w[2][1] + w[2][2];
	return x;
}

float df2sos_HPF_10Hz_5kHzSR(float x)
{
// ������������ HPF-������� 5Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,-2,1,
//							1,-2,1,
//							1,-2,1};
	const float	a[3][3] = {	1, -1.9951719045639038, 0.99525606632232666,
							1, -1.9870085716247559, 0.98709243535995483,
							1, -1.9823257923126221, 0.98240947723388672};
	const float	g[3] = {0.99760699272155762, 0.99352526664733887, 0.9911838173866272};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] - w[0][1] - w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] - w[1][1] - w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] - w[2][1] - w[2][1] + w[2][2];
	return x;
}

float df2sos_HPF_5Hz_10kHzSR(float x)
{
// ������������ HPF-������� 5Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,-2,1,
//							1,-2,1,
//							1,-2,1};
	const float	a[3][3] = {	1, -1.9988065958023071, 0.99881190061569214,
							1, -1.996752142906189, 0.9967573881149292,
							1, -1.9955679178237915, 0.99557310342788696};
	const float	g[3] = {0.99940460920333862, 0.99837738275527954, 0.99778527021408081};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] - w[0][1] - w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] - w[1][1] - w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] - w[2][1] - w[2][1] + w[2][2];
	return x;
}

float df2sos_HPF_10Hz_10kHzSR(float x)
{
// ������������ HPF-������� 10Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,-2,1,
//							1,-2,1,
//							1,-2,1};
	const float	a[3][3] = {	1, -1.9976041316986084, 0.99762523174285889,
							1, -1.9935042858123779, 0.99352526664733887,
							1, -1.9911447763442993, 0.99116581678390503};
	const float	g[3] = {0.99880731105804443, 0.9967573881149292, 0.99557769298553467};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] - w[0][1] - w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] - w[1][1] - w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] - w[2][1] - w[2][1] + w[2][2];
	return x;
}

float df2sos_HPF_5Hz_50kHzSR(float x)
{
// ������������ HPF-������� 5Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,-2,1,
//							1,-2,1,
//							1,-2,1};
	const float	a[3][3] = {	1, -1.9997537136077881, 0.99975395202636719,
							1, -1.9993277788162231, 0.99932801723480225,
							1, -1.9990819692611694, 0.99908214807510376};
	const float	g[3] = {0.99987691640853882, 0.99966394901275635, 0.99954104423522949};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] - w[0][1] - w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] - w[1][1] - w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] - w[2][1] - w[2][1] + w[2][2];
	return x;
}

float df2sos_HPF_10Hz_50kHzSR(float x)
{
// ������������ HPF-������� 10Hz (3 ������ SOS-II)
//	const float	b[3][3] = {	1,-2,1,
//							1,-2,1,
//							1,-2,1};
	const float	a[3][3] = {	1, -1.9994851350784302, 0.99948614835739136,
							1, -1.9985957145690918, 0.99859672784805298,
							1, -1.9980826377868652, 0.99808359146118164};
	const float	g[3] = {0.99974280595779419, 0.999298095703125, 0.99904155731201172};
	// ���������� �������
	static float w[3][3];
	// ������ ������
		w[0][2] = w[0][1];
		w[0][1] = w[0][0];
		w[0][0] = x * g[0] - w[0][1] * a[0][1] - w[0][2] * a[0][2];
		x = w[0][0] - w[0][1] - w[0][1] + w[0][2];
	// ������ ������
		w[1][2] = w[1][1];
		w[1][1] = w[1][0];
		w[1][0] = x * g[1] - w[1][1] * a[1][1] - w[1][2] * a[1][2];
		x = w[1][0] - w[1][1] - w[1][1] + w[1][2];
	// ������ ������
		w[2][2] = w[2][1];
		w[2][1] = w[2][0];
		w[2][0] = x * g[2] - w[2][1] * a[2][1] - w[2][2] * a[2][2];
		x = w[2][0] - w[2][1] - w[2][1] + w[2][2];
	return x;
}
