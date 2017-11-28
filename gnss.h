
/***********************
gnss.h 
(c) by Christoph Schultz <ic-schultz@gmx.de>
derived from gps.h

For usage with SM808 GSM-GNSS chip 
Hardware >= V2.01
and Firmware/Software >= 1418B01SIM808M32
Findout with AT + CGMM for Modelversion
and AT + CGMR for Softwareversion.

****************************/









#ifndef _GNSS_H_
#define _GNSS_H_

#include "SIM900.h"

class GNSSGSM 
{
public:
     char getBattInf(char *str_perc, char *str_vol);
     char getBattTVol(char *str_vol);
     char attachGNSS();
     char deattachGNSS();
     //char getStat();
	 char getPar(char *str_long, char *str_lat, char *str_alt, char *str_time, char *str_speed, char *str_gpsrs, char *str_fs = NULL, char *str_fm=NULL, char *str_course=NULL);     
	 void parseTime(char *field, char *time, int precision = 0);
     double convertLat(char*);
     double convertLong(char*);
	 char * year;
	 char * month;
	 char * day;
	 char * hour;
	 char * min;
	 char * sec;
	 
	 
private:
    String p_char2;
	String _p;
	String _year;
	String _month;
	 String _day;
	String _hour;
	String _min; 
	 String _sec;
	char * p_char1;
	
	int end;
};

#endif
