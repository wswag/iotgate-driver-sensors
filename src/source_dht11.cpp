#include "source_dht11.h"

Source_DHT11::Source_DHT11(dht11data_t& dst, int pin)
        : _dst(dst), _pin(pin) {
}

source_updateresult_t Source_DHT11::internalUpdate() {
    pinMode(_pin, OUTPUT);
    _dht.read(_pin);
    _dst.temperature = _dht.temperature;
    _dst.humidity = _dht.humidity;
    validate();
    return SOURCE_UPDATE_COMPLETED;
}