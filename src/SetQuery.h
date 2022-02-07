#ifndef _SETQUERY_H_
#define _SETQUERY_H_

#include <Arduino.h>
#include <string>

#include "Credentials.h"

class SetQuery
{
public:
    SetQuery();
    ~SetQuery();

    static String setDeviceInfoQuery();
    static String setDeviceParametersQuery();
    static String setInsertDeviceProccessQuery();
    static String setPanelProccessQuery();
};

#endif