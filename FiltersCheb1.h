/*******************************************************************************
 * file:	FiltersCheb1.h
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	23-July-2015
 * brief:	’едер дл€ FiltersCheb1.c
 *******************************************************************************/
#ifndef __FILTERSCHEB1_H
#define __FILTERSCHEB1_H

float	df2sos_LPF_100Hz_50kHzSR(float x);			// LPF-фильтр Ѕаттерворта DF2SOS 100Hz (частота выборки 50к√ц)
float	df2sos_LPF_500Hz_50kHzSR(float x);			// LPF-фильтр Ѕаттерворта DF2SOS 500Hz (частота выборки 50к√ц)
float	df2sos_LPF_1000Hz_50kHzSR(float x);			// LPF-фильтр Ѕаттерворта DF2SOS 1000Hz (частота выборки 50к√ц)
float	df2sos_HPF_5Hz_5kHzSR(float x);				// HPF-фильтр Ѕаттерворта DF2SOS 5Hz (частота выборки 10к√ц)
float	df2sos_HPF_10Hz_10kHzSR(float x);			// HPF-фильтр Ѕаттерворта DF2SOS 10Hz (частота выборки 10к√ц)

#endif /* __FILTERSCHEB1_H */
