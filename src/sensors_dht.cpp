#include "iotgate-driver-sensors.h"

PUBLISHERDRIVER(DHTBased) {
    PUBLISH_DEVICENODE(humidity).onChange().every(1 MIN).addMeta()
        PRESET_HUMIDITY_SENSOR_PERC
        .withLowerBound(0)
        .withUpperBound(100);

    PUBLISH_DEVICENODE(temperature).onChange().every(1 MIN).addMeta()
        PRESET_TEMPERATURE_SENSOR_DEGC
        .withLowerBound(-40)
        .withUpperBound(120);
}

source_updateresult_t DHTBased::internalUpdate() {
    //pinMode(_pin, OUTPUT);
    sensors_event_t event_hum, event_temp;
    auto dht = DHT_Unified(_pin, _sensorType);
    dht.begin();
    dht.humidity().getEvent(&event_hum);
    dht.temperature().getEvent(&event_temp);
    humidity = event_hum.relative_humidity;
    temperature = event_temp.temperature;
    validate();
    return SOURCE_UPDATE_COMPLETED;
}

ACQUIREDRIVER(Sensor_DHT11, PIN pin) {
    _pin = pin.getPin();
    _sensorType = DHT11;
    pin.custom(this).every(2 SEC);
}

ACQUIREDRIVER(Sensor_DHT21, PIN pin) {
    _pin = pin.getPin();
    _sensorType = DHT21;
    pin.custom(this).every(3 SEC);
}

ACQUIREDRIVER(Sensor_DHT22, PIN pin) {
    _pin = pin.getPin();
    _sensorType = DHT22;
    pin.custom(this).every(3 SEC);
}