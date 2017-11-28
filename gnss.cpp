#include "gnss.h"
char GNSSGSM::getBattInf(char *str_perc, char *str_vol)
{
     char ret_val=0;
     char *p_char;
     char *p_char1;

     gsm.SimpleWriteln("AT+CBC");
     gsm.WaitResp(5000, 100, "OK");
     if(gsm.IsStringReceived("+CBC"))
          ret_val=1;

     //BCL
     p_char = strchr((char *)(gsm.comm_buf),',');
     p_char1 = p_char+1;  //we are on the first char of BCS
     p_char = strchr((char *)(p_char1), ',');
     if (p_char != NULL) {
          *p_char = 0;
     }
     strcpy(str_perc, (char *)(p_char1));

     //Voltage
     p_char++;
     p_char1 = strchr((char *)(p_char), '\r');
     if (p_char1 != NULL) {
          *p_char1 = 0;
     }
     strcpy(str_vol, (char *)(p_char));
     return ret_val;
}

char GNSSGSM::getBattTVol(char *str_vol)
{
     char *p_char;
     char *p_char1;
     char ret_val=0;

     gsm.SimpleWriteln("AT+CBTE?");
     gsm.WaitResp(5000, 100, "OK");
     if(gsm.IsStringReceived("+CBTE"))
          ret_val=1;

     //BCL
     p_char = strchr((char *)(gsm.comm_buf),':');
     p_char1 = p_char+2;  //we are on the first char of BCS
     p_char = strchr((char *)(p_char1), '\r');
     if (p_char != NULL) {
          *p_char = 0;
     }
     strcpy(str_vol, (char *)(p_char1));
     return ret_val;
}

char GNSSGSM::attachGNSS()
{
     if(AT_RESP_ERR_DIF_RESP == gsm.SendATCmdWaitResp("AT+CGNSPWR=1", 500, 100, "OK", 5))
          return 0;
     /*if(AT_RESP_ERR_DIF_RESP == gsm.SendATCmdWaitResp("AT+CGNSSRST=1", 500, 100, "OK", 5))
          return 0;*/
     return 1;
}

char GNSSGSM::deattachGNSS()
{
     if(AT_RESP_ERR_DIF_RESP == gsm.SendATCmdWaitResp("AT+CGNSPWR=0", 500, 100, "OK", 5))
          return 0;
     return 1;
}

/*char GNSSGSM::getStat()
{
     char ret_val=-1;
     gsm.SimpleWriteln("AT+CGNSSSTATUS?");
     gsm.WaitResp(5000, 100, "OK");
     if(gsm.IsStringReceived("Unknown")||gsm.IsStringReceived("unknown"))
          ret_val=0;
     else if(gsm.IsStringReceived("Not"))
          ret_val=1;
     else if(gsm.IsStringReceived("2D")||gsm.IsStringReceived("2d"))
          ret_val=2;
     else if(gsm.IsStringReceived("3D")||gsm.IsStringReceived("3d"))
          ret_val=3;
     return ret_val;
} */
// 1,1,20171201103215.000,+45.987556,+008.776430,050.200,0.0,0.0,1,,2.2,2.2,1.1,,5,4,,,45,,
//parsed NMEA STring: S,           F,          YYYYMMDDhhmmss.sss,+/-dd.dddddd,+/-ddd.dddddd,aaaa.aaa,  sss.ss, ccc.cc, f,      NULL,    hh.h, pp.p, vv.v, NULL,    GG|NULL,       NN|NULL,        LL|NULL,     NULL,     CC,   NULL|mmmm.m, NULL|mmmm.m
//      Begin         0           2          4                   23           34            46       55       62       69              72    77   82               88            91               94                      98   101          107
//	  Length          1            1           18                  10           11             8        6        6       1                4    4      4                2             2             2                      2     6            6               
//                    Gps-run stat,Fix-Status,Utc-Time,            latitude,    longitude,   altitude, speed,  course, fixmode,reserved,HDOP, PDOP, VDOP, reseved, GPS-Sats,      GNSS-Sats,      GLONAS-Sats, reserved, C/N0, HPA,          VPA


char GNSSGSM::getPar(char *str_long, char *str_lat, char *str_alt, char *str_time, char *str_speed, char *str_gpsrs, char *str_fs = NULL, char *str_fm=NULL, char *str_course=NULL)
{
    int nmea[18][2] = {{0, 1}, {2, 1}, {4, 18}, {23, 10}, {34, 11}, {46, 8}, {55, 6}, {62, 6}, {69, 1}, {72, 4}, {77, 4}, {82, 4}, {88,2}, {91, 2 }, {94, 2}, {89, 2}, {101, 6},{108, 6}};
	
    char ret_val=0;
   
	
	int end;
     
    gsm.SimpleWriteln("AT+CGNSINF");
    gsm.WaitResp(5000, 100, "OK");
    if(gsm.IsStringReceived("OK"));
    ret_val=1;
	int s, l; 
	p_char2 = (char*)(gsm.comm_buf);
	for (s=0;s<=18; s++)
	{  
		if ( nmea[s][1] == 1 )
			end = nmea[s][0];
		else 
			end = nmea[s][1]+nmea[s][0];
		_p= p_char2.substring(nmea[s][0],end);
		
		if ( _p != NULL )
		{   _p.toCharArray(p_char1, _p.length());
			switch (s)
			{
				case 0:
					str_gpsrs = p_char1; // Run-Status
					break;
				case 1:
					str_fs = p_char1;; //Fix-Status
					break;
				case 3:
					str_time = p_char1; // Utc-Time 
					break;
				case 4:
					str_lat = p_char1; //Latitude degrees
					break;
				case 5:
				    str_long = p_char1;  //Longitude degrees
					break;
				case 6:
					str_alt = p_char1;  //Altitude MSL
					break;
				case 7:
					str_speed = p_char1; // GroundSpeed km/h
					break;
				case 8:
					str_course = p_char1; // Groundcourse degrees
					break;
				case 9:
					str_fm = p_char1;  // FixMode 0= not fixed, 1 = 2D Fixed 2= 3D Fixed
					break;
				/*case 10:
					
				case 11:
				case 12:
				case 13:
				case 14:   reserved for other values if used...
				case 15:
				case 16:*/
				default:
					break;
			}
		}
	}
				
	return ret_val; 
}

void GNSSGSM::parseTime(char *field, char *time, int precision = 0 )
{  //Timeformat : YYYYMMDDhhmmss.sss
	_p = field;
	_year = _p.substring(0,3);
	 _year.toCharArray(year, _year.length());
	_month = _p.substring(4,5);
	_month.toCharArray(month, _month.length());
	_day = _p.substring(6,7);
	_day.toCharArray(day, _day.length());
	_hour = _p.substring(8,9);
	_hour.toCharArray(hour, _hour.length());
	_min = _p.substring(10,11);
	_min.toCharArray(min, _min.length());
	end = 15 + precision;
	if (precision != 0)
	{	_sec = _p.substring(12, end);
		_sec.toCharArray(sec, _sec.length());
		sprintf(time,"%2s. %2s. %4s - %2s:%2s:%5s ", day, month, year, hour, min, sec);
	}
    else
	{
		_sec = _p.substring(12, 14);
		_sec.toCharArray(sec, _sec.length());
		sprintf(time,"%2s. %2s. %4s - %2s:%2s:%2s ", day, month, year, hour, min, sec);
	}
}

//Read the latitude in decimal format from a GGA string
double GNSSGSM::convertLat(char* latString)
{
     double latitude = atof(latString);                                    // convert to a double (precise)
     int deg = (int) latitude / 100;                               // extract the number of degrees
     double min = latitude - (100 * deg);                  // work out the number of minutes
     latitude = deg + (double) min/60.0;                   // convert to decimal format
     return latitude;
}

// Read the longitude in decimal format from a GGA string
double GNSSGSM::convertLong(char* longString)
{
     double longitude = atof(longString);                                  // convert to a double
     int deg = (int) longitude / 100;                              // extract the number of degrees
     double min = longitude - (100 * deg);                 // work out the number of minutes
     longitude = deg + (double) min/60.00;                 // convert to decimal format
     return longitude;
}