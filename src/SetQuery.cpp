#include "SetQuery.h"

SetQuery::SetQuery(){}
SetQuery::~SetQuery(){}

String SetQuery::setDeviceInfoQuery()
{
  return String("SELECT * FROM ") + String(database_name) + String(".") + String(device_table_name) + String(" WHERE ") 
                                  + String(query_coloumn_name_dt) + String(" = ") + String(device_id);
}

String SetQuery::setDeviceParametersQuery()
{
  return String("SELECT * FROM ") + String(database_name) + String(".") + String(plant_table_name) + String(" WHERE ") 
                                  + String(query_coloumn_name_pt) + String(" = ");
}

String SetQuery::setInsertDeviceProccessQuery()
{
  return String("INSERT INTO ") + String(database_name) + String(".") + String(device_proccess_table_name) + String(" (")
                                + String(query_coloumn_name_dt) + String(", ") + String(query_coloumn_name_dpt2) + String(", ") 
                                + String(query_coloumn_name_dpt3) + String(", ") + String(query_coloumn_name_dpt4) + String(", ") 
                                + String(query_coloumn_name_dpt5) + String(", ") + String(query_coloumn_name_dpt6) + String(", ") 
                                + String(query_coloumn_name_dpt7) + String(", ") + String(query_coloumn_name_dpt8) 
                                + String(") VALUES (%s, %s, '%s', '%s', %s, %s, %s, %s)");
}

String SetQuery::setPanelProccessQuery()
{
  return String("INSERT INTO ") + String(database_name) + String(".") + String(solar_panel_table_name) + String(" (")
                                + String(query_coloumn_name_sp1) + String(", ") + String(query_coloumn_name_sp2) 
                                + String(", ") + String(query_coloumn_name_sp3) + String(", ") + String(query_coloumn_name_sp4) 
                                + String(") VALUES (%s, '%s', %s, %s)");
}