/*******************************************************************************
 * file:	FiltersButt.h
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	23-July-2015
 * brief:	’едер дл€ FiltersButt.c
 *******************************************************************************/
#ifndef __FILTERSBUTT_H
#define __FILTERSBUTT_H

float	df2sos_BPF_5_500Hz_10kHzSR(float x);		// BPF-фильтр Ѕаттерворта DF2SOS 5-500Hz (частота выборки 10к√ц)
float	df2sos_BPF_10_1000Hz_10kHzSR(float x);		// BPF-фильтр Ѕаттерворта DF2SOS 10-1000Hz (частота выборки 10к√ц)
float	df2sos_BPF_20_1000Hz_20kHzSR(float x);		// BPF-фильтр Ѕаттерворта DF2SOS 20-1000Hz (частота выборки 20к√ц)
float	df2sos_BPF_5_500Hz_50kHzSR(float x);		// BPF-фильтр Ѕаттерворта DF2SOS 5-500Hz (частота выборки 50к√ц)
float	df2sos_BPF_10_1000Hz_50kHzSR(float x);		// BPF-фильтр Ѕаттерворта DF2SOS 10-1000Hz (частота выборки 50к√ц)
float	df2sos_LPF_100Hz_50kHzSR(float x);			// LPF-фильтр Ѕаттерворта DF2SOS 100Hz (частота выборки 50к√ц)
float	df2sos_LPF_500Hz_50kHzSR(float x);			// LPF-фильтр Ѕаттерворта DF2SOS 500Hz (частота выборки 50к√ц)
float	df2sos_LPF_800Hz_50kHzSR(float x);			// LPF-фильтр Ѕаттерворта DF2SOS 800Hz (частота выборки 50к√ц)
float	df2sos_LPF_1000Hz_50kHzSR(float x);			// LPF-фильтр Ѕаттерворта DF2SOS 1000Hz (частота выборки 50к√ц)
float	df2sos_HPF_5Hz_5kHzSR(float x);				// HPF-фильтр Ѕаттерворта DF2SOS 5Hz (частота выборки 5к√ц)
float	df2sos_HPF_10Hz_5kHzSR(float x);			// HPF-фильтр Ѕаттерворта DF2SOS 10Hz (частота выборки 5к√ц)
float	df2sos_HPF_5Hz_10kHzSR(float x);			// HPF-фильтр Ѕаттерворта DF2SOS 5Hz (частота выборки 10к√ц)
float	df2sos_HPF_10Hz_10kHzSR(float x);			// HPF-фильтр Ѕаттерворта DF2SOS 10Hz (частота выборки 10к√ц)
float	df2sos_HPF_5Hz_50kHzSR(float x);			// HPF-фильтр Ѕаттерворта DF2SOS 5Hz (частота выборки 50к√ц)
float	df2sos_HPF_10Hz_50kHzSR(float x);			// HPF-фильтр Ѕаттерворта DF2SOS 10Hz (частота выборки 50к√ц)

#endif /* __FILTERSBUTT_H */
