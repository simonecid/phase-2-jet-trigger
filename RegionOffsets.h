#ifndef __REGION_OFFSET_H__
#define __REGION_OFFSET_H__

#include "HLS/HistogramSettings.h"

unsigned char returnBarrelEtaOffset(unsigned char regionId);

unsigned char returnBarrelPhiOffset(unsigned char regionId);

unsigned char returnTKHGEtaOffset(unsigned char regionId);

unsigned char returnTKHGPhiOffset(unsigned char regionId);

unsigned char returnHGEtaOffset(unsigned char regionId);

unsigned char returnHGPhiOffset(unsigned char regionId);

unsigned char returnHFEtaOffset(unsigned char regionId);

unsigned char returnHFPhiOffset(unsigned char regionId);

#endif