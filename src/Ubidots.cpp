#include "Ubidots.h"
#include "UbiProtocol.h"
#include "UbiTypes.h"
#include "UbiBuilder.h"
#include "UbiConstants.h"

Ubidots::Ubidots(char* token, IotProtocol iot_protocol) {
  UbiBuilder builder(UBIDOTS_SERVER, token, iot_protocol);
  _dots = (Value *)malloc(MAX_VALUES * sizeof(Value));
  _ubiProtocol = builder.builder();
  _token = token;
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

void Ubidots::add(char *variable_label, float value, char *context, long unsigned dot_timestamp_seconds, unsigned int dot_timestamp_millis) {
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

bool Ubidots::send() {
  return send(_default_device_label, _default_device_label);
}

bool Ubidots::send(const char* device_label) {
  return send(device_label, device_label);
}

bool Ubidots::send(const char* device_label, const char* device_name) {
  char* payload = (char *) malloc(sizeof(char) * MAX_BUFFER_SIZE);
  buildTcpPayload(payload, device_label, device_name);
  Serial.println("Sending data...");
  bool result = _ubiProtocol->sendData(device_label, device_name, payload);
  free(payload);
  if (result) {
    _dirty = false;
  }
  return result;
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
    }

    // Adds timestamp milliseconds
    if ((_dots + i)->dot_timestamp_millis != NULL) {
      sprintf(payload, "%s%d", payload, (_dots + i)->dot_timestamp_millis);
    } else {
      sprintf(payload, "%s000", payload);
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
    }

    // Adds timestamp milliseconds
    if ((_dots + i)->dot_timestamp_millis != NULL) {
      sprintf(payload, "%s%d", payload, (_dots + i)->dot_timestamp_millis);
    } else {
      sprintf(payload, "%s000", payload);
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
  Makes debug messages available
*/

void Ubidots::setDebug(bool debug) {
  _debug = debug;
  _ubiProtocol->setDebug(debug);
}