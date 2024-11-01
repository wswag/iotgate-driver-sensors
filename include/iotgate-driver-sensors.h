#ifndef IOTGATE_DRIVER_SENSORS_H
#define IOTGATE_DRIVER_SENSORS_H

#include <iotgate.h>

struct DhtReadState {
  uint32_t cycles[84];
  uint32_t startEvent;
  uint32_t lastEvent;
  int cursor = 0;
};

class DHTBased : public DataSource {
    protected:
        uint8_t data[5];
        uint8_t _pin = 0;

        bool readSensor(int initDelayMicros);
    public:
        // data
        float temperature;
        float humidity;

        PUBLISHERDRIVERDEF;
};

class Sensor_DHT11 : public DHTBased {
    private:
        source_updateresult_t internalUpdate() override;
    public:
        ACQUIREDRIVERDEF(PIN pin);
};

class Sensor_DHT21 : public DHTBased {
    protected:
        source_updateresult_t internalUpdate() override;
    public:
        ACQUIREDRIVERDEF(PIN pin);
};

class Sensor_DHT22 : public DHTBased {
    protected:
        source_updateresult_t internalUpdate() override;
    public:
        ACQUIREDRIVERDEF(PIN pin);
};



#endif /* IOTGATE_DRIVER_SENSORS_H */
