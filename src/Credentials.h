#ifndef Credentials_h
#define Credentials_h

#include <string>

struct Credentials
{
    //Declaration for device id number
    static const char device_id;

    //Declarations for WiFi connection
    static const char ssid[];
    static const char pass[];

    //Declarations for Database Instance
    static char hostname[];
    static char user[];
    static char password[];
    static char database_name[];

    //Declarations for query of tables
    static const char device_table_name[];
    static const char plant_table_name[];
    static const char device_proccess_table_name[];
    static const char solar_panel_table_name[];

    //Declarations for query of coloumns
    static const char query_coloumn_name_dt[];
    static const char query_coloumn_name_pt[];

    static const char query_coloumn_name_dpt2[];
    static const char query_coloumn_name_dpt3[];
    static const char query_coloumn_name_dpt4[];
    static const char query_coloumn_name_dpt5[];
    static const char query_coloumn_name_dpt6[];
    static const char query_coloumn_name_dpt7[];
    static const char query_coloumn_name_dpt8[];

    static const char query_coloumn_name_sp1[];
    static const char query_coloumn_name_sp2[];
    static const char query_coloumn_name_sp3[];
    static const char query_coloumn_name_sp4[];

    //Declarations for proccess types
    static char feeding_id;
    static char fretilizingid;
    static char spraying_id;
    static char measurment_id;
    static char lightning_id;

};

//Definition for device id number
const char Credentials::device_id = '1';

//Definitions for WiFi connection
const char Credentials::ssid[] = "Your network SSID";        //network SSID (name)
const char Credentials::pass[] = "Your network password";        //network password

char Credentials::hostname[]     = "Your host name";  //MySQL Server Hostname
char Credentials::user[]         = "Your user name";              // MySQL Server user login username
char Credentials::password[]     = "Your password";          // MySQL Server user login password

//Definition for Database Name
char Credentials::database_name[] = "Your database name";

//Definitions for query of tables
const char Credentials::device_table_name[] = "cihazlar"; //devices
const char Credentials::plant_table_name[] = "bitkiler"; //plants
const char Credentials::device_proccess_table_name[] = "cihaz_islemler"; //device processes
const char Credentials::solar_panel_table_name[] = "panel"; //solar panel

//Definitions for query of coloumns
const char Credentials::query_coloumn_name_dt[] = "idcihazlar"; //id of devices
const char Credentials::query_coloumn_name_pt[] = "id"; //id

const char Credentials::query_coloumn_name_dpt2[] = "idislemturu"; //id of process type
const char Credentials::query_coloumn_name_dpt3[] = "tarih"; //date
const char Credentials::query_coloumn_name_dpt4[] = "saat"; //time
const char Credentials::query_coloumn_name_dpt5[] = "sensor_sicaklik"; //temperature value coloumn from the temperature and relative humidity sensor
const char Credentials::query_coloumn_name_dpt6[] = "sensor_nem_hava"; //relative humidity of air value coloumn from the temperature and relative humidity sensor
const char Credentials::query_coloumn_name_dpt7[] = "sensor_nem_toprak"; //moisture of soil value coloumn from the moisture sensor
const char Credentials::query_coloumn_name_dpt8[] = "sensor_ph"; //ph of soil value coloumn from the ph sensor

const char Credentials::query_coloumn_name_sp1[] = "idcihaz"; //id of device
const char Credentials::query_coloumn_name_sp2[] = "tarih"; //date
const char Credentials::query_coloumn_name_sp3[] = "uretilen_enerji"; //generated energy of solar panel
const char Credentials::query_coloumn_name_sp4[] = "tuketilen_enerji"; //consumed energy of device

//Definitions for proccess types
char Credentials::feeding_id = '1';
char Credentials::fretilizingid = '2';
char Credentials::spraying_id = '3';
char Credentials::measurment_id = '4';
char Credentials::lightning_id = '5';

#endif    //Credentials_h
