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



void DhtItr_RISING(void* arg)
{
  DhtReadState* state = (DhtReadState*)arg;
  int t = micros();
  if (state->cursor < sizeof(state->cycles) / sizeof(uint32_t)) {
    state->cycles[state->cursor] = t - state->lastEvent;
  }
  state->lastEvent = t;
  state->cursor++;
}

bool DHTBased::readSensor(int initDelayMicros) {
    // Reset 40 bits of received data to zero.
    memset(data, 0, sizeof(data));

    // Send start signal.  See DHT datasheet for full signal diagram:
    //   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

    // Go into high impedence state to let pull-up raise data line level and
    // start the reading process.
    pinMode(_pin, INPUT_PULLUP);
    delay(1);

    // First set data line low for a period according to sensor type
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(initDelayMicros);

    // hookup interrupts
    uint32_t* cycles;
    DhtReadState state;
    memset(state.cycles, 0, sizeof(state.cycles));
    state.startEvent = micros();
    state.lastEvent = state.startEvent;
    attachInterruptArg(_pin, DhtItr_RISING, &state, CHANGE);
    // End the start signal by setting data line high for 40 microseconds.
    pinMode(_pin, INPUT_PULLUP);

    while (state.cursor < 84) {
      // wait for sensor feedback via interrupts
      vTaskDelay(1 / portTICK_PERIOD_MS);
      if (micros() - state.lastEvent > 5000) {
        //Serial.printf("dht timeout, cursor %d\r\n", state.cursor);
        // detach interrupt before we go, otherwise the interrupt will crash the stack memory as we sent a pointer to stack memory with it
        detachInterrupt(_pin);
        return false;
      }
    }

    detachInterrupt(_pin);

    // Inspect pulses and determine which ones are 0 (high state cycle count < low
    // state cycle count), or 1 (high state cycle count > low state cycle count).
    cycles = &state.cycles[4];
    //Serial.printf("cursor: %d\r\n", state.cursor);
    for (int i = 0; i < 40; ++i) {
      uint32_t lowCycles = cycles[2 * i];
      uint32_t highCycles = cycles[2 * i + 1];
      //Serial.printf("%d/%d ", lowCycles, highCycles);
      data[i / 8] <<= 1;
      // Now compare the low and high cycle times to see if the bit is a 0 or 1.
      if (highCycles > lowCycles) {
        // High cycles are greater than 50us low cycle count, must be a 1.
        data[i / 8] |= 1;
      }
      // Else high cycles are less than (or equal to, a weird case) the 50us low
      // cycle count so this must be a zero.  Nothing needs to be changed in the
      // stored data.
    }
    //Serial.println();

    // Check we read 40 bits and that the checksum matches.
    if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
      return false;
    }
    return true;
}

void DHTBased::calibrate(float temperatureOffset, float humidityOffset)
{
    _temperatureOffset = temperatureOffset;
    _humidtyOffset = humidityOffset;
}

void DHTBased::tareMeasures() {
    temperature += _temperatureOffset;
    humidity += _humidtyOffset;
}

source_updateresult_t Sensor_DHT11::internalUpdate() {
    //pinMode(_pin, OUTPUT);
    if (readSensor(20000)) {
        humidity = data[0] + data[1] * 0.1;
        temperature = data[2];
        if (data[3] & 0x80) {
            temperature = -1 - temperature;
        }
        temperature += (data[3] & 0x0f) * 0.1;
        tareMeasures();
        validate();
        return SOURCE_UPDATE_COMPLETED;
    }
    blackout(1000);
    return SOURCE_UPDATE_FAILED;
}

ACQUIREDRIVER(Sensor_DHT11, PIN pin) {
    _pin = pin.getPin();
    pin.custom(this).every(2 SEC);
}

source_updateresult_t Sensor_DHT21::internalUpdate() {
    if (readSensor(1100)) {
        humidity = (((word)data[0]) << 8 | data[1]) * 0.1;
        temperature = ((word)(data[2] & 0x7F)) << 8 | data[3];
        temperature *= 0.1;
        if (data[2] & 0x80) {
            temperature *= -1;
        }
        tareMeasures();
        validate();
        return SOURCE_UPDATE_COMPLETED;
    }
    blackout(1000);
    return SOURCE_UPDATE_FAILED;
}

ACQUIREDRIVER(Sensor_DHT21, PIN pin) {
    _pin = pin.getPin();
    pin.custom(this).every(3 SEC);
}

source_updateresult_t Sensor_DHT22::internalUpdate() {
    //pinMode(_pin, OUTPUT);
    if (readSensor(1100)) {
        humidity = (((word)data[0]) << 8 | data[1]) * 0.1;
        temperature = ((word)(data[2] & 0x7F)) << 8 | data[3];
        temperature *= 0.1;
        if (data[2] & 0x80) {
            temperature *= -1;
        }
        tareMeasures();
        validate();
        return SOURCE_UPDATE_COMPLETED;
    }
    blackout(1000);
    return SOURCE_UPDATE_FAILED;
}

ACQUIREDRIVER(Sensor_DHT22, PIN pin) {
    _pin = pin.getPin();
    pin.custom(this).every(3 SEC);
}