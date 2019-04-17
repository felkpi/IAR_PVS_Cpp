/*******************************************************************************
 * file:	FiltersCheb1.h
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	23-July-2015
 * brief:	����� ��� FiltersCheb1.c
 *******************************************************************************/
#ifndef __FILTERSCHEB1_H
#define __FILTERSCHEB1_H

float	df2sos_LPF_100Hz_50kHzSR(float x);			// LPF-������ ����������� DF2SOS 100Hz (������� ������� 50���)
float	df2sos_LPF_500Hz_50kHzSR(float x);			// LPF-������ ����������� DF2SOS 500Hz (������� ������� 50���)
float	df2sos_LPF_1000Hz_50kHzSR(float x);			// LPF-������ ����������� DF2SOS 1000Hz (������� ������� 50���)
float	df2sos_HPF_5Hz_5kHzSR(float x);				// HPF-������ ����������� DF2SOS 5Hz (������� ������� 10���)
float	df2sos_HPF_10Hz_10kHzSR(float x);			// HPF-������ ����������� DF2SOS 10Hz (������� ������� 10���)

#endif /* __FILTERSCHEB1_H */
