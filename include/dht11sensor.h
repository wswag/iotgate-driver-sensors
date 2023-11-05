#ifndef DHT11_H
#define DHT11_H

#include <iotgate.h>
#include <source_dht11.h>

class DHT11 {
    public:
        
        // data
        dht11data_t environment;

        ACQUIREDRIVERDEF(PIN pin);
        PUBLISHERDRIVERDEF;
};

#endif /* DHT11_H */
