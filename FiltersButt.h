/*******************************************************************************
 * file:	FiltersButt.h
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	23-July-2015
 * brief:	����� ��� FiltersButt.c
 *******************************************************************************/
#ifndef __FILTERSBUTT_H
#define __FILTERSBUTT_H

float	df2sos_BPF_5_500Hz_10kHzSR(float x);		// BPF-������ ����������� DF2SOS 5-500Hz (������� ������� 10���)
float	df2sos_BPF_10_1000Hz_10kHzSR(float x);		// BPF-������ ����������� DF2SOS 10-1000Hz (������� ������� 10���)
float	df2sos_BPF_20_1000Hz_20kHzSR(float x);		// BPF-������ ����������� DF2SOS 20-1000Hz (������� ������� 20���)
float	df2sos_BPF_5_500Hz_50kHzSR(float x);		// BPF-������ ����������� DF2SOS 5-500Hz (������� ������� 50���)
float	df2sos_BPF_10_1000Hz_50kHzSR(float x);		// BPF-������ ����������� DF2SOS 10-1000Hz (������� ������� 50���)
float	df2sos_LPF_100Hz_50kHzSR(float x);			// LPF-������ ����������� DF2SOS 100Hz (������� ������� 50���)
float	df2sos_LPF_500Hz_50kHzSR(float x);			// LPF-������ ����������� DF2SOS 500Hz (������� ������� 50���)
float	df2sos_LPF_800Hz_50kHzSR(float x);			// LPF-������ ����������� DF2SOS 800Hz (������� ������� 50���)
float	df2sos_LPF_1000Hz_50kHzSR(float x);			// LPF-������ ����������� DF2SOS 1000Hz (������� ������� 50���)
float	df2sos_HPF_5Hz_5kHzSR(float x);				// HPF-������ ����������� DF2SOS 5Hz (������� ������� 5���)
float	df2sos_HPF_10Hz_5kHzSR(float x);			// HPF-������ ����������� DF2SOS 10Hz (������� ������� 5���)
float	df2sos_HPF_5Hz_10kHzSR(float x);			// HPF-������ ����������� DF2SOS 5Hz (������� ������� 10���)
float	df2sos_HPF_10Hz_10kHzSR(float x);			// HPF-������ ����������� DF2SOS 10Hz (������� ������� 10���)
float	df2sos_HPF_5Hz_50kHzSR(float x);			// HPF-������ ����������� DF2SOS 5Hz (������� ������� 50���)
float	df2sos_HPF_10Hz_50kHzSR(float x);			// HPF-������ ����������� DF2SOS 10Hz (������� ������� 50���)

#endif /* __FILTERSBUTT_H */
