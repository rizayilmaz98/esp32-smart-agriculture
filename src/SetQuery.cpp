#include "SetQuery.h"

SetQuery::SetQuery(){}
SetQuery::~SetQuery(){}

String SetQuery::setDeviceInfoQuery()
{
  return String("SELECT * FROM ") + String(Credentials::database_name) + String(".") + String(Credentials::device_table_name) + String(" WHERE ") 
                                  + String(Credentials::query_coloumn_name_dt) + String(" = ") + String(Credentials::device_id);
}

String SetQuery::setDeviceParametersQuery()
{
  return String("SELECT * FROM ") + String(Credentials::database_name) + String(".") + String(Credentials::plant_table_name) + String(" WHERE ") 
                                  + String(Credentials::query_coloumn_name_pt) + String(" = ");
}

String SetQuery::setInsertDeviceProccessQuery()
{
  return String("INSERT INTO ") + String(Credentials::database_name) + String(".") + String(Credentials::device_proccess_table_name) + String(" (")
                                + String(Credentials::query_coloumn_name_dt) + String(", ") + String(Credentials::query_coloumn_name_dpt2) + String(", ") 
                                + String(Credentials::query_coloumn_name_dpt3) + String(", ") + String(Credentials::query_coloumn_name_dpt4) + String(", ") 
                                + String(Credentials::query_coloumn_name_dpt5) + String(", ") + String(Credentials::query_coloumn_name_dpt6) + String(", ") 
                                + String(Credentials::query_coloumn_name_dpt7) + String(", ") + String(Credentials::query_coloumn_name_dpt8) 
                                + String(") VALUES (%s, %s, '%s', '%s', %s, %s, %s, %s)");
}

String SetQuery::setPanelProccessQuery()
{
  return String("INSERT INTO ") + String(Credentials::database_name) + String(".") + String(Credentials::solar_panel_table_name) + String(" (")
                                + String(Credentials::query_coloumn_name_sp1) + String(", ") + String(Credentials::query_coloumn_name_sp2) 
                                + String(", ") + String(Credentials::query_coloumn_name_sp3) + String(", ") + String(Credentials::query_coloumn_name_sp4) 
                                + String(") VALUES (%s, '%s', %s, %s)");
}