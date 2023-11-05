#include "dht11sensor.h"

ACQUIREDRIVER(DHT11, PIN pin) {
    pin.custom(new Source_DHT11(environment, pin.getPin())).every(1 SEC);
}

PUBLISHERDRIVER(DHT11) {
    PUBLISH_DEVICENODE(environment.humidity).onChange().addMeta()
        PRESET_HUMIDITY_SENSOR_PERC
        .withLowerBound(0)
        .withUpperBound(100)
        .withProperty("SensorType", "DHT11");

    PUBLISH_DEVICENODE(environment.temperature).onChange().addMeta()
        PRESET_TEMPERATURE_SENSOR_DEGC
        .withLowerBound(0)
        .withUpperBound(50)
        .withProperty("SensorType", "DHT11");
}
