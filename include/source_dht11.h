#ifndef SOURCE_DHT11_H
#define SOURCE_DHT11_H

#include <dht11.h>
#include <datasource.h>

struct dht11data_t {
    int temperature;
    int humidity;
};

class Source_DHT11 : public DataSource {
    private:
        dht11data_t& _dst;
        int _pin;
        dht11 _dht;
    public:
        Source_DHT11(dht11data_t& dst, int pin);
        void internalUpdate() override;
};

#endif /* SOURCE_DHT11_H */
