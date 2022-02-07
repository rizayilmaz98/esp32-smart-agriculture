#ifndef DefaultDevice_h
#define DefaultDevice_h

#include <Arduino.h>
#include <string>

struct DefaultDevice
{
    //Definitions of default device parameters when the server or Wi-Fi connection is failed
    static const String default_user_id;
    static const String default_plant_id;
    static const String default_soil_id;
    static const String default_location;
    static const String default_ph;
    static const String default_feeding_time;

    static const String default_ideal_temp_min;
    static const String default_ideal_temp_max;

    static const String default_nonideal_temp_min;
    static const String default_nonideal_temp_max;

    static const String default_hum_min;
    static const String default_hum_max;

    static const String default_ph_min;
    static const String default_ph_max;
};

const String DefaultDevice::default_user_id{"1"};
const String DefaultDevice::default_plant_id{"1"};
const String DefaultDevice::default_soil_id{"4"};
const String DefaultDevice::default_location{"Istanbul"};
const String DefaultDevice::default_ph{"6.50"};
const String DefaultDevice::default_feeding_time{"11:00:00"};

const String DefaultDevice::default_ideal_temp_min = "18.00";
const String DefaultDevice::default_ideal_temp_max = "27.00";

const String DefaultDevice::default_nonideal_temp_min = "10.00";
const String DefaultDevice::default_nonideal_temp_max = "35.00";

const String DefaultDevice::default_hum_min = "50.00";
const String DefaultDevice::default_hum_max = "60.00";

const String DefaultDevice::default_ph_min = "5.50";
const String DefaultDevice::default_ph_max = "7.00";

#endif //DefaultDevice.h
