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
#include "UbiConstants.h"
#include "UbiProtocol.h"
#include "UbiTypes.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

Ubidots::Ubidots(char* token, IotProtocol iotProtocol) {
  _builder(token, UBI_INDUSTRIAL, iotProtocol);
}

Ubidots::Ubidots(char* token, UbiServer server, IotProtocol iotProtocol) {
  _builder(token, server, iotProtocol);
}

void Ubidots::_builder(char* token, UbiServer server, IotProtocol iotProtocol) {
  _iotProtocol = iotProtocol;
  _context = (ContextUbi*)malloc(MAX_VALUES * sizeof(ContextUbi));

// Electron and photon do not support Mesh
#if PLATFORM_ID != PLATFORM_PHOTON_DEV &&        \
    PLATFORM_ID != PLATFORM_PHOTON_PRODUCTION && \
    PLATFORM_ID != PLATFORM_ELECTRON_PRODUCTION
  _protocolMesh = new UbiMesh(token);
#endif

// Only non-xenon devices may have cloud communication protocols
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_PHOTON_DEV ||                               \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION ||                        \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  if (_iotProtocol != UBI_MESH) {
    _cloudProtocol = new UbiProtocolHandler(token, server, iotProtocol);
  } else {
    // Default is TCP
    _cloudProtocol = new UbiProtocolHandler(token, server, UBI_TCP);
  }
#endif
}

/**************************************************************************
 * Destructor
 ***************************************************************************/

Ubidots::~Ubidots() {
  free(_context);
#if PLATFORM_ID != PLATFORM_PHOTON_DEV &&        \
    PLATFORM_ID != PLATFORM_PHOTON_PRODUCTION && \
    PLATFORM_ID != PLATFORM_ELECTRON_PRODUCTION
  delete _protocolMesh;
#elif PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_PHOTON_DEV ||                               \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION ||                        \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  delete _cloudProtocol;
#endif
}

/***************************************************************************
FUNCTIONS TO SEND DATA
***************************************************************************/

/**
 * Add a value of variable to save
 * @arg variable_label [Mandatory] variable label where the dot will be stored
 * @arg value [Mandatory] Dot value
 * @arg context [optional] Dot context to store. Default NULL
 * @arg dot_timestamp_seconds [optional] Dot timestamp in seconds, usefull for
 * datalogger. Default NULL
 * @arg dot_timestamp_millis [optional] Dot timestamp in millis to add to
 * dot_timestamp_seconds, usefull for datalogger.
 */

void Ubidots::add(char* variable_label, float value) {
  add(variable_label, value, NULL, NULL, NULL);
}

void Ubidots::add(char* variable_label, float value, char* context) {
  add(variable_label, value, context, NULL, NULL);
}

void Ubidots::add(char* variable_label, float value, char* context,
                  long unsigned dot_timestamp_seconds) {
  add(variable_label, value, context, dot_timestamp_seconds, NULL);
}

void Ubidots::add(char* variable_label, float value, char* context,
                  long unsigned dot_timestamp_seconds,
                  unsigned int dot_timestamp_millis) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON
  if (_iotProtocol == UBI_MESH) {
    _protocolMesh->add(variable_label, value, context, dot_timestamp_seconds,
                       dot_timestamp_millis);
  } else {
    _cloudProtocol->add(variable_label, value, context, dot_timestamp_seconds,
                        dot_timestamp_millis);
  }

#elif PLATFORM_ID == PLATFORM_PHOTON_DEV ||      \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION || \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  if (_iotProtocol != UBI_MESH) {
    _cloudProtocol->add(variable_label, value, context, dot_timestamp_seconds,
                        dot_timestamp_millis);
  } else {
    Serial.println(
        "[WARNING] please choose a different cloud protocol of Mesh in the "
        "Ubidots constructor");
  }
// Xenon only supports Mesh cloud protocol
#elif PLATFORM_ID == PLATFORM_XENON || PLATFORM_ID == PLATFORM_XENON_SOM
  _protocolMesh->add(variable_label, value, context, dot_timestamp_seconds,
                     dot_timestamp_millis);
#else
  Serial.println("[WARNING] Your board does not support the add() method");
#endif
}

bool Ubidots::meshPublishToUbidots() {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_XENON || PLATFORM_ID == PLATFORM_XENON_SOM
  return _protocolMesh->meshPublishToUbidots();
#else
  Serial.println(
      "[WARNING] Your board does not support the meshPublishToUbidots() "
      "method");
  return false;
#endif
}

bool Ubidots::meshPublishToUbidots(const char* device_label) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_XENON || PLATFORM_ID == PLATFORM_XENON_SOM
  return _protocolMesh->meshPublishToUbidots(device_label);
#else
  Serial.println(
      "[WARNING] Your board does not support the meshPublishToUbidots() "
      "method");
  return false;
#endif
}

bool Ubidots::meshPublishToUbidots(const char* device_label,
                                   const char* device_name) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_XENON || PLATFORM_ID == PLATFORM_XENON_SOM
  return _protocolMesh->meshPublishToUbidots(device_label, device_name);
#else
  Serial.println(
      "[WARNING] Your board does not support the meshPublishToUbidots() "
      "method");
  return false;
#endif
}

void Ubidots::meshLoop() {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON
  _protocolMesh->meshLoop();
#else
  Serial.println(
      "[WARNING] Your board does not support the meshLoop() "
      "method");
#endif
}

void Ubidots::setCloudProtocol(IotProtocol iotProtocol) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON
  _protocolMesh->setCloudProtocol(iotProtocol);
#else
  Serial.println(
      "[WARNING] Your board does not support the setCloudProtocol() "
      "method");
#endif
}

/**
 * Sends data to Ubidots
 * @arg device_label [Mandatory] device label where the dot will be stored
 * @arg device_name [optional] Name of the device to be created (supported only
 * for TCP/UDP)
 * @arg flags [Optional] Particle publish flags for webhooks
 */

bool Ubidots::send() {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_PHOTON_DEV ||                             \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION ||                      \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  if (_iotProtocol != UBI_MESH) {
    return _cloudProtocol->send();
  } else {
    Serial.println(
        "[WARNING] The Mesh Protocol is not supported in this device for "
        "sending data");
    return false;
  }
#else
  Serial.println(
      "[WARNING] Your board does not support the send() "
      "method");
#endif
}

bool Ubidots::send(const char* device_label) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_PHOTON_DEV ||                             \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION ||                      \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  return _cloudProtocol->send(device_label);
#else
  Serial.println(
      "[WARNING] Your board does not support the send() "
      "method");
  return false;
#endif
}

bool Ubidots::send(const char* device_label, const char* device_name) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_PHOTON_DEV ||                             \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION ||                      \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  return _cloudProtocol->send(device_label, device_name);
#else
  Serial.println(
      "[WARNING] Your board does not support the send() "
      "method");
  return false;
#endif
}

bool Ubidots::send(const char* device_label, PublishFlags flag) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_PHOTON_DEV ||                             \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION ||                      \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  return _cloudProtocol->send(device_label, flag);
#else
  Serial.println(
      "[WARNING] Your board does not support the send() "
      "method");
  return false;
#endif
}

bool Ubidots::send(const char* device_label, const char* device_name,
                   UbiFlags* flags) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_PHOTON_DEV ||                             \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION ||                      \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  return _cloudProtocol->send(device_label, device_name, flags);
#else
  Serial.println(
      "[WARNING] Your board does not support the send() "
      "method");
  return false;
#endif
}

float Ubidots::get(const char* device_label, const char* variable_label) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_PHOTON_DEV ||                             \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION ||                      \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  if (_iotProtocol != UBI_MESH) {
    _cloudProtocol->get(device_label, variable_label);
  } else {
    Serial.println(
        "[Warning] To retrieve data, please set a valid cloud protocol in the "
        "Ubidots constructor");
    return ERROR_VALUE;
  }
#else
  Serial.println("[Warning] Your board does not support the get() method");
  return ERROR_VALUE;
#endif
}

void Ubidots::setDebug(bool debug) {
  _debug = debug;
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON
  if (_iotProtocol != UBI_MESH) {
    _cloudProtocol->setDebug(debug);
  } else {
    _protocolMesh->setDebug(debug);
  }
#elif PLATFORM_ID == PLATFORM_PHOTON_DEV ||      \
    PLATFORM_ID == PLATFORM_PHOTON_PRODUCTION || \
    PLATFORM_ID == PLATFORM_ELECTRON_PRODUCTION
  _cloudProtocol->setDebug(debug);
#elif PLATFORM_ID == PLATFORM_XENON || PLATFORM_ID == PLATFORM_XENON_SOM
  _protocolMesh->setDebug(debug);
#endif
}

/*
 * Adds to the context structure values to retrieve later it easily by the user
 */

void Ubidots::addContext(char* key_label, char* key_value) {
  (_context + _current_context)->key_label = key_label;
  (_context + _current_context)->key_value = key_value;
  _current_context++;
  if (_current_context >= MAX_VALUES) {
    Serial.println(
        F("You are adding more than the maximum of consecutive key-values "
          "pairs"));
    _current_context = MAX_VALUES;
  }
}

/*
 * Retrieves the actual stored context properly formatted
 */

void Ubidots::getContext(char* context_result) {
  getContext(context_result, _iotProtocol);
}

void Ubidots::getContext(char* context_result, IotProtocol iotProtocol) {
  // TCP context type
  if (iotProtocol == UBI_TCP || iotProtocol == UBI_UDP) {
    sprintf(context_result, "");
    for (uint8_t i = 0; i < _current_context;) {
      sprintf(context_result, "%s%s=%s", context_result,
              (_context + i)->key_label, (_context + i)->key_value);
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
  if (iotProtocol == UBI_PARTICLE || iotProtocol == UBI_HTTP) {
    sprintf(context_result, "");
    for (uint8_t i = 0; i < _current_context;) {
      sprintf(context_result, "%s\"%s\":\"%s\"", context_result,
              (_context + i)->key_label, (_context + i)->key_value);
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