#ifndef Credentials_h
#define Credentials_h

//Declaration for device id number
extern const char device_id;

//Declarations for WiFi connection
extern const char ssid[];
extern const char pass[];

//Declarations for Database Instance
extern char hostname[];
extern char user[];
extern char password[];
extern char database_name[];

//Declarations for query of tables
extern const char device_table_name[];
extern const char plant_table_name[];
extern const char device_proccess_table_name[];
extern const char solar_panel_table_name[];

//Declarations for query of coloumns
extern const char query_coloumn_name_dt[];
extern const char query_coloumn_name_pt[];

extern const char query_coloumn_name_dpt2[];
extern const char query_coloumn_name_dpt3[];
extern const char query_coloumn_name_dpt4[];
extern const char query_coloumn_name_dpt5[];
extern const char query_coloumn_name_dpt6[];
extern const char query_coloumn_name_dpt7[];
extern const char query_coloumn_name_dpt8[];

extern const char query_coloumn_name_sp1[];
extern const char query_coloumn_name_sp2[];
extern const char query_coloumn_name_sp3[];
extern const char query_coloumn_name_sp4[];

//Declarations for proccess types
extern char feeding_id;
extern char fretilizingid;
extern char spraying_id;
extern char measurment_id;
extern char lightning_id;

#endif    //Credentials_h
