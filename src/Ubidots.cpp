/*
Copyright (c) 2013-2018 Ubidots.
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Developed and maintained by Jose Garcia for IoT Services Inc
@jotathebest at github: https://github.com/jotathebest
*/

#include "Ubidots.h"
#include "UbiProtocol.h"
#include "UbiTypes.h"
#include "UbiBuilder.h"
#include "UbiConstants.h"

/**************************************************************************
* Overloaded constructors
***************************************************************************/

Ubidots::Ubidots(char* token, IotProtocol iot_protocol) {
  builder(token, UBI_INDUSTRIAL, iot_protocol);
}

Ubidots::Ubidots(char* token, UbiServer server, IotProtocol iot_protocol) {
  builder(token, server, iot_protocol);
}

void Ubidots::builder(char* token, UbiServer server, IotProtocol iot_protocol) {
  _iot_protocol = iot_protocol;
  UbiBuilder builder(server, token, _iot_protocol);
  _dots = (Value *)malloc(MAX_VALUES * sizeof(Value));
  _context = (ContextUbi *)malloc(MAX_VALUES * sizeof(ContextUbi));
  _ubiProtocol = builder.builder();
  _token = token;
  _current_value=0;
  _current_context=0;
  String particle_id_str = System.deviceID();
  _default_device_label = new char[particle_id_str.length() + 1];
  strcpy(_default_device_label, particle_id_str.c_str());
}

/***************************************************************************
FUNCTIONS TO SEND DATA
***************************************************************************/

/**
 * Add a value of variable to save
 * @arg variable_label [Mandatory] variable label where the dot will be stored
 * @arg value [Mandatory] Dot value
 * @arg context [optional] Dot context to store. Default NULL
 * @arg dot_timestamp_seconds [optional] Dot timestamp in seconds, usefull for datalogger. Default NULL
 * @arg dot_timestamp_millis [optional] Dot timestamp in millis to add to dot_timestamp_seconds, usefull for datalogger.
 */

void Ubidots::add(char *variable_label, float value) {
  return add(variable_label, value, NULL, NULL, NULL);
}

void Ubidots::add(char *variable_label, float value, char *context) {
  return add(variable_label, value, context, NULL, NULL);
}

void Ubidots::add(char *variable_label, float value, char *context, long unsigned dot_timestamp_seconds) {
  return add(variable_label, value, context, dot_timestamp_seconds, NULL);
}

void Ubidots::add(char *variable_label, float value, char *context, unsigned long dot_timestamp_seconds, unsigned int dot_timestamp_millis) {
  _dirty = true;
  (_dots + _current_value)->variable_label = variable_label;
  (_dots + _current_value)->dot_value = value;
  (_dots + _current_value)->dot_context = context;
  (_dots + _current_value)->dot_timestamp_seconds = dot_timestamp_seconds;
  (_dots + _current_value)->dot_timestamp_millis = dot_timestamp_millis;
  _current_value++;
  if (_current_value > MAX_VALUES) {
    if (_debug) {
      Serial.println(F("You are sending more than the maximum of consecutive variables"));
    }
    _current_value = MAX_VALUES;
  }
}

/**
 * Sends data to Ubidots
 * @arg device_label [Mandatory] device label where the dot will be stored
 * @arg device_name [optional] Name of the device to be created (supported only for TCP/UDP)
 * @arg flags [Optional] Particle publish flags for webhooks
 */

bool Ubidots::send() {
  UbiFlags *flags = new UbiFlags();
  return send(_default_device_label, _default_device_label, flags);
}

bool Ubidots::send(const char* device_label) {
  UbiFlags *flags = new UbiFlags();
  return send(device_label, device_label, flags);
}

bool Ubidots::send(const char* device_label, const char* device_name) {
  UbiFlags *flags = new UbiFlags();
  return send(device_label, device_name, flags);
}

bool Ubidots::send(const char* device_label, PublishFlags flag) {
  UbiFlags *flags = new UbiFlags();
  flags->particle_flag = flag;
  return send(device_label, device_label, flags);
}

bool Ubidots::send(const char* device_label, const char* device_name, UbiFlags* flags) {
  // Builds the payload
  char* payload = (char *) malloc(sizeof(char) * MAX_BUFFER_SIZE);
  if (_iot_protocol == UBI_TCP || _iot_protocol == UBI_UDP) {
    buildTcpPayload(payload, device_label, device_name);
  } else {
    buildHttpPayload(payload);
  }

  // Sends data
  if (_debug) {
    Serial.println("Sending data...");
  }

  bool result = _ubiProtocol->sendData(device_label, device_name, payload, flags);
  free(payload);
  delete flags;
  if (result) {
    _dirty = false;
    _current_value = 0;
  }

  _current_value = 0;
  return result;
}

float Ubidots::get(const char* device_label, const char* variable_label) {
  if (_iot_protocol == UBI_UDP || _iot_protocol == UBI_PARTICLE) {
    Serial.println("ERROR, data retrieval is only supported using TCP or HTTP protocols");
    return ERROR_VALUE;
  }

  float value = ERROR_VALUE;

  value = _ubiProtocol->get(device_label, variable_label);

  return value;
}


/**
 * Builds the HTTP payload to send and saves it to the input char pointer.
 * @payload [Mandatory] char payload pointer to store the built structure.
 * @timestamp_global [Optional] If set, it will be used for any dot without timestamp.
 */

void Ubidots::buildHttpPayload(char* payload) {
  /* Builds the payload */
  sprintf(payload, "{");

  for (uint8_t i = 0; i < _current_value;) {
    sprintf(payload, "%s\"%s\":{\"value\":%f", payload, (_dots + i)->variable_label, (_dots + i)->dot_value);

    // Adds timestamp seconds
    if ((_dots + i)->dot_timestamp_seconds != NULL) {
      sprintf(payload, "%s,\"timestamp\":%lu", payload, (_dots + i)->dot_timestamp_seconds);
      // Adds timestamp milliseconds
      if ((_dots + i)->dot_timestamp_millis != NULL) {
        char milliseconds[3];
        int timestamp_millis = (_dots + i)->dot_timestamp_millis;
        uint8_t units = timestamp_millis % 10;
        uint8_t dec = (timestamp_millis / 10) % 10;
        uint8_t hund = (timestamp_millis / 100) % 10;
        sprintf(milliseconds, "%d%d%d", hund, dec, units);
        sprintf(payload, "%s%s", payload, milliseconds);
      } else {
        sprintf(payload, "%s000", payload);
      }
    }

    // Adds dot context
    if ((_dots + i)->dot_context != NULL) {
      sprintf(payload, "%s,\"context\": {%s}", payload, (_dots + i)->dot_context);
    }

    sprintf(payload, "%s}", payload);
    i++;

    if (i < _current_value) {
      sprintf(payload, "%s,", payload);
    } else {
      sprintf(payload, "%s}", payload);
      _current_value = 0;
    }
  }

  if (_debug) {
    Serial.println("----------");
    Serial.println("payload:");
    Serial.println(payload);
    Serial.println("----------");
    Serial.println("");
  }

}

/**
 * Builds the TCP payload to send and saves it to the input char pointer.
 * @payload [Mandatory] char payload pointer to store the built structure.
 * @timestamp_global [Optional] If set, it will be used for any dot without timestamp.
 */

void Ubidots::buildTcpPayload(char* payload, const char* device_label, const char* device_name) {
  sprintf(payload, "");
  sprintf(payload, "%s|POST|%s|", USER_AGENT, _token);
  sprintf(payload, "%s%s:%s", payload, device_label, device_name);

  sprintf(payload, "%s=>", payload);
  for (uint8_t i = 0; i < _current_value;) {
    sprintf(payload, "%s%s:%f", payload, (_dots + i)->variable_label, (_dots + i)->dot_value);

    // Adds dot context
    if ((_dots + i)->dot_context != NULL) {
      sprintf(payload, "%s$%s", payload, (_dots + i)->dot_context);
    }

    // Adds timestamp seconds
    if ((_dots + i)->dot_timestamp_seconds != NULL) {
      sprintf(payload, "%s@%lu", payload, (_dots + i)->dot_timestamp_seconds);
      // Adds timestamp milliseconds
      if ((_dots + i)->dot_timestamp_millis != NULL) {
        char milliseconds[3];
        int timestamp_millis = (_dots + i)->dot_timestamp_millis;
        uint8_t units = timestamp_millis % 10;
        uint8_t dec = (timestamp_millis / 10) % 10;
        uint8_t hund = (timestamp_millis / 100) % 10;
        sprintf(milliseconds, "%d%d%d", hund, dec, units);
        sprintf(payload, "%s%s", payload, milliseconds);
      } else {
        sprintf(payload, "%s000", payload);
      }
    }

    i++;

    if (i < _current_value) {
      sprintf(payload, "%s,", payload);
    } else {
      sprintf(payload, "%s|end", payload);
      _current_value = 0;
    }
  }

  if (_debug) {
    Serial.println("----------");
    Serial.println("payload:");
    Serial.println(payload);
    Serial.println("----------");
    Serial.println("");
  }

}

/*
* Adds to the context structure values to retrieve later it easily by the user
*/

void Ubidots::addContext(char *key_label, char *key_value) {
  (_context + _current_context)->key_label = key_label;
  (_context + _current_context)->key_value = key_value;
  _current_context++;
  if (_current_context >= MAX_VALUES) {
      Serial.println(F("You are adding more than the maximum of consecutive key-values pairs"));
    _current_context = MAX_VALUES;
  }
}

/*
* Retrieves the actual stored context properly formatted
*/

void Ubidots::getContext(char* context_result) {
  // TCP context type
  if (_iot_protocol == UBI_TCP || _iot_protocol == UBI_UDP) {
    sprintf(context_result, "");
    for (uint8_t i = 0; i < _current_context;) {
      sprintf(context_result, "%s%s=%s", context_result, (_context + i)->key_label, (_context + i)->key_value);
      i++;
      if (i < _current_context) {
        sprintf(context_result, "%s$", context_result);
      } else {
        sprintf(context_result, "%s", context_result);
        _current_context = 0;
      }
    }
  }

  // HTTP context type
  if (_iot_protocol == UBI_PARTICLE || _iot_protocol == UBI_HTTP) {
    sprintf(context_result, "");
    for (uint8_t i = 0; i < _current_context;) {
      sprintf(context_result, "%s\"%s\":\"%s\"", context_result, (_context + i)->key_label, (_context + i)->key_value);
      i++;
      if (i < _current_context) {
        sprintf(context_result, "%s,", context_result);
      } else {
        sprintf(context_result, "%s", context_result);
        _current_context = 0;
      }
    }
  }

}

/*
  Makes debug messages available
*/

void Ubidots::setDebug(bool debug) {
  _debug = debug;
  _ubiProtocol->setDebug(debug);
}