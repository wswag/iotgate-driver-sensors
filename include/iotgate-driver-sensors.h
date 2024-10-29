#ifndef DHT11_H
#define DHT11_H

#include <iotgate.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class DHTBased : public DataSource {
    protected:
        uint8_t _sensorType = DHT11;
        uint8_t _pin = 0;
        source_updateresult_t internalUpdate() override;
    public:
        // data
        float temperature;
        float humidity;

        PUBLISHERDRIVERDEF;
};

class Sensor_DHT11 : public DHTBased {
    public:
        ACQUIREDRIVERDEF(PIN pin);
};

class Sensor_DHT21 : public DHTBased {
    public:
        ACQUIREDRIVERDEF(PIN pin);
};

class Sensor_DHT22 : public DHTBased {
    public:
        ACQUIREDRIVERDEF(PIN pin);
};

#endif /* DHT11_H */
