/*
Copyright (c) 2013-2019 Ubidots.
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

Developed and maintained by Jose Garcia for Ubidots Inc
@jotathebest at github: https://github.com/jotathebest
*/

#include "UbiMesh.h"
#include "UbiProtocolHandler.h"

/**************************************************************************
 * Static Functions
 ***************************************************************************/
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON
bool UbiMesh::isThrottled() {
  // Simple throttling routine for TCP and HTTP
  bool throttled = true;
  if (iotProtocolMesh == UBI_HTTP || iotProtocolMesh == UBI_TCP) {
    if (millis() - time_now > _throttlingTime) {
      throttled = false;
      time_now = millis();
    }
  }

  if (iotProtocolMesh == UBI_UDP || iotProtocolMesh == UBI_PARTICLE) {
    throttled = false;
  }

  return throttled;
}

void UbiMesh::ubiPublishHandler(const char* event, const char* data) {
  bool throttled = isThrottled();

  if (throttled && _debugMesh) {
    Serial.println(
        "[WARNING] your sample time reached the throttling of 20 s, please "
        "set a higher sample time or use UDP or Particle Webhooks to send "
        "data");
  } else {
    uint8_t i = 0;
    char* _data = const_cast<char*>(data);
    char* pch;
    const char _meshDelimiter[2] = "|";
    pch = strtok(_data, _meshDelimiter);

    std::map<uint8_t, char*> meshMap;

    while (pch != NULL) {
      meshMap.insert(std::pair<uint8_t, char*>(i, pch));
      i++;
      pch = strtok(NULL, _meshDelimiter);
    }

    MeshUbi* dots = (MeshUbi*)malloc(sizeof(MeshUbi));
    UbiMesh* _protocolInternalMesh = new UbiMesh(_tokenMesh);
    UbiProtocolHandler* _meshCloudHandler =
        new UbiProtocolHandler(_tokenMesh, iotProtocolMesh);
    _protocolInternalMesh->buildDots(meshMap, dots);
    _meshCloudHandler->setDebug(true);
    _meshCloudHandler->add(dots->variableLabel, dots->dotValue,
                           dots->dotContext, dots->dotTimestampSeconds,
                           dots->dotTimestampMillis);
    _meshCloudHandler->send(dots->deviceLabel, dots->deviceName);
    delete _meshCloudHandler;
    delete _protocolInternalMesh;
    free(dots);
  }
}
#endif

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

UbiMesh::UbiMesh(char* token) {
  _tokenMesh = token;
  String particle_id_str = System.deviceID();
  _default_device_label = new char[particle_id_str.length() + 1];
  strcpy(_default_device_label, particle_id_str.c_str());
};

/**************************************************************************
 * Destructor
 ***************************************************************************/

UbiMesh::~UbiMesh() { delete[] _default_device_label; }

/**************************************************************************
 * Overloaded Values Add functions
 ***************************************************************************/

void UbiMesh::add(const char* variable_label, float value) {
  add(variable_label, value, NULL, NULL, NULL);
}
void UbiMesh::add(const char* variable_label, float value,
                  const char* context) {
  add(variable_label, value, context, NULL, NULL);
}
void UbiMesh::add(const char* variable_label, float value, const char* context,
                  long unsigned dot_timestamp_seconds) {
  add(variable_label, value, context, dot_timestamp_seconds, NULL);
}

void UbiMesh::add(const char* variable_label, float value, const char* context,
                  long unsigned dot_timestamp_seconds,
                  unsigned int dot_timestamp_millis) {
  char _context[100];
  char _dot_timestamp_seconds[11];
  char _dot_timestamp_millis[4];

  if (context != NULL && strlen(context) > 100) {
    Serial.println("[WARNING] Your Mesh context length must be lower than 100");
  }

  context != NULL ? sprintf(_context, "%s", context)
                  : sprintf(_context, "%s", " ");

  dot_timestamp_seconds != NULL
      ? sprintf(_dot_timestamp_seconds, "%lu", dot_timestamp_seconds)
      : sprintf(_dot_timestamp_seconds, "%s", " ");

  dot_timestamp_millis != NULL
      ? sprintf(_dot_timestamp_millis, "%d", dot_timestamp_millis)
      : sprintf(_dot_timestamp_millis, "%s", " ");

  sprintf(_meshPayload, "%s|%f|%s|%s|%s", variable_label, value, _context,
          _dot_timestamp_seconds, _dot_timestamp_millis);
}

/**************************************************************************
 * Mesh handler functions
 ***************************************************************************/

bool UbiMesh::meshPublish(const char* channel, const char* data) {
  return meshPublish(channel, data, 5000);
}

bool UbiMesh::meshPublish(const char* channel, const char* data, int timeout) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_XENON || PLATFORM_ID == PLATFORM_XENON_SOM
  if (!Mesh.ready()) {
    _MeshReconnect(timeout);
  }
  if (!Mesh.ready()) {
    return false;
  }

  bool result = Mesh.publish(channel, data);
  return result;
#else
  return false;
#endif
}

bool UbiMesh::meshPublishToUbidots() {
  return meshPublishToUbidots(_default_device_label);
}

bool UbiMesh::meshPublishToUbidots(const char* device_label) {
  return meshPublishToUbidots(device_label, device_label);
}

bool UbiMesh::meshPublishToUbidots(const char* device_label,
                                   const char* device_name) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_XENON || PLATFORM_ID == PLATFORM_XENON_SOM
  if (strlen(_meshPayload) <= 0) {
    if (_debugMesh) {
      Serial.println(
          "You have not added any variable value, please call the add()"
          "method first");
    }
    return false;
  }
  char payload[256];
  sprintf(payload, "%s|%s|%s", device_label, device_name, _meshPayload);
  return meshPublish(UBIDOTS_MESH_CHANNEL, payload);
#else
  Serial.println(
      "[WARNING] Your device does not support to publish using Mesh");
#endif
}

void UbiMesh::meshLoop() {
// Only non Xenon Devices should subscribe to the Ubidots events channel
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON
  if (!Mesh.ready()) {
    _MeshReconnect(5000);
  }
  Mesh.subscribe(UBIDOTS_MESH_CHANNEL, ubiPublishHandler);
#else
  Serial.println(
      "[Warning] The meshLoop() method does not implement any routine in your "
      "device");
#endif
}

/**************************************************************************
 * Other functions
 ***************************************************************************/

/*
  Checks Mesh network connection status
*/

bool UbiMesh::_MeshReconnect(int timeout) {
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON || \
    PLATFORM_ID == PLATFORM_XENON || PLATFORM_ID == PLATFORM_XENON_SOM
  int _timeout = 0;

  // Waits up to the set timeout to begin a socket exchange
  while (!Mesh.ready() && _timeout < 5000) {
    Mesh.connect();
    _timeout++;
    delay(1);
  }

  if (!Mesh.ready()) {
    if (_debugMesh) {
      Serial.println(
          "A problem has raised with the device trying to open a Mesh"
          "socket");
    }
    return false;
  }

  return true;
#else
  return false;
#endif
}

/*
  Makes debug messages available
*/

void UbiMesh::setDebug(bool debug) { _debugMesh = debug; }

/*
  Sets the cloud protocol to be used by the Mesh gateway
*/

void UbiMesh::setCloudProtocol(IotProtocol iotProtocol) {
  iotProtocolMesh = iotProtocol;
}

/*
  Gets the actual cloud protocol to used by the Mesh gateway
*/

IotProtocol UbiMesh::getCloudProtocol() { return iotProtocolMesh; }

void UbiMesh::buildDots(std::map<uint8_t, char*>& meshMap, MeshUbi* dots) {
  _addDeviceToDot(meshMap, dots);
  _addVariableToDot(meshMap, dots);
  _addValueToDot(meshMap, dots);
  _addContextToDot(meshMap, dots);
  _addTimestampToDot(meshMap, dots);
}

void UbiMesh::_addDeviceToDot(std::map<uint8_t, char*>& meshMap,
                              MeshUbi* dots) {
  if (meshMap.find(0) != meshMap.end()) {
    dots->deviceLabel = meshMap[0];
  }
  if (meshMap.find(1) != meshMap.end()) {
    dots->deviceName = meshMap[1];
  }
}

void UbiMesh::_addVariableToDot(std::map<uint8_t, char*>& meshMap,
                                MeshUbi* dots) {
  if (meshMap.find(2) != meshMap.end()) {
    dots->variableLabel = meshMap[2];
  }
}

void UbiMesh::_addValueToDot(std::map<uint8_t, char*>& meshMap, MeshUbi* dots) {
  if (meshMap.find(3) != meshMap.end()) {
    dots->dotValue = atof(meshMap[3]);
  }
}

void UbiMesh::_addContextToDot(std::map<uint8_t, char*>& meshMap,
                               MeshUbi* dots) {
  if (meshMap.find(4) != meshMap.end()) {
    strcmp(meshMap[4], " ") != 0 ? dots->dotContext = meshMap[4]
                                 : dots->dotContext = NULL;
  }
}

void UbiMesh::_addTimestampToDot(std::map<uint8_t, char*>& meshMap,
                                 MeshUbi* dots) {
  if (meshMap.find(5) != meshMap.end()) {
    strcmp(meshMap[5], " ") != 0 ? dots->dotTimestampSeconds = atoll(meshMap[5])
                                 : dots->dotTimestampSeconds = NULL;
  }

  if (meshMap.find(6) != meshMap.end()) {
    strcmp(meshMap[6], " ") != 0 ? dots->dotTimestampMillis = atoll(meshMap[6])
                                 : dots->dotTimestampMillis = NULL;
  }
}