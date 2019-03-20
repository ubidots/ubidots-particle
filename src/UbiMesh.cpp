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
 * Overloaded constructors
 ***************************************************************************/

UbiMesh::UbiMesh(char* token) { _tokenMesh = token; };

void UbiMesh::add(const char* variable_label, float value, const char* context,
                  long unsigned dot_timestamp_seconds,
                  unsigned int dot_timestamp_millis) {
  sprintf(_meshPayload, "");
  // char* _context;
  // context = !NULL ? sprintf(_context, "%s", context) : sprintf(_context, "
  // ");
  sprintf(_meshPayload, "%s|%f|%s|%lu|%d", variable_label, value, context,
          dot_timestamp_seconds, dot_timestamp_millis);
}

/**************************************************************************
 * Mesh handler functions
 ***************************************************************************/

bool UbiMesh::meshPublish(const char* channel, const char* data) {
  return meshPublish(channel, data, 5000);
}

bool UbiMesh::meshPublish(const char* channel, const char* data, int timeout) {
  if (!Mesh.ready()) {
    _MeshReconnect(timeout);
  }

  if (!Mesh.ready()) {
    return false;
  }

  return Mesh.publish(channel, data);
}

bool UbiMesh::meshPublishToUbidots(const char* device_label,
                                   const char* device_name,
                                   IotProtocol iotProtocol) {
  if (strlen(_meshPayload) <= 0) {
    if (_debug) {
      Serial.println(
          "You have not added any variable value, please call the add()"
          "method first");
    }
    return false;
  }
  char payload[256];
  sprintf(payload, "%s|%s|%s", device_label, device_name, _meshPayload);
  return meshPublish(UBIDOTS_MESH_CHANNEL, payload);
}

void UbiMesh::meshLoop() {
  if (!Mesh.ready()) {
    _MeshReconnect(5000);
  }
  Mesh.subscribe(UBIDOTS_MESH_CHANNEL, ubiPublishHandler);
}

/**************************************************************************
 * Other functions
 ***************************************************************************/

/*
  Checks Mesh network connection status
*/

bool UbiMesh::_MeshReconnect(int timeout) {
  int _timeout = 0;

  // Waits up to the set timeout to begin a socket exchange
  while (!Mesh.ready() && _timeout < 5000) {
    Mesh.connect();
    _timeout++;
    delay(1);
  }

  if (!Mesh.ready()) {
    if (_debug) {
      Serial.println(
          "A problem has raised with the device trying to open a Mesh"
          "socket");
    }
    return false;
  }

  return true;
}

/*
  Makes debug messages available
*/

void UbiMesh::setDebug(bool debug) { _debug = debug; }

void UbiMesh::ubiPublishHandler(const char* event, const char* data) {
  Serial.printlnf("event=%s data=%s", event, data ? data : "NULL");
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

  UbiMesh* _protocolInternalMesh = new UbiMesh(_tokenMesh);
  MeshUbi* dots = (MeshUbi*)malloc(sizeof(MeshUbi));
  _protocolInternalMesh->buildDots(meshMap, dots);
  UbiProtocolHandler* _meshCloudHandler =
      new UbiProtocolHandler(_tokenMesh, iotProtocolMesh);
  UbiFlags* flags = new UbiFlags();
  _meshCloudHandler->setDebug(true);
  _meshCloudHandler->add(dots->variableLabel, dots->dotValue, dots->dotContext,
                         dots->dotTimestampSeconds, dots->dotTimestampMillis);
  _meshCloudHandler->send(dots->deviceLabel, dots->deviceName, flags);
  delete _protocolInternalMesh;
  delete _meshCloudHandler;
  free(dots);
}

void UbiMesh::setMeshProtocol(IotProtocol iotProtocol) {
  iotProtocolMesh = iotProtocol;
}

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
    dots->dotContext = meshMap[4];
  }
}

void UbiMesh::_addTimestampToDot(std::map<uint8_t, char*>& meshMap,
                                 MeshUbi* dots) {
  if (meshMap.find(5) != meshMap.end()) {
    meshMap[5] != NULL ? dots->dotTimestampSeconds = atoll(meshMap[5])
                       : dots->dotTimestampSeconds = NULL;
  }

  if (meshMap.find(6) != meshMap.end()) {
    meshMap[6] != NULL ? dots->dotTimestampSeconds = atoll(meshMap[6])
                       : dots->dotTimestampSeconds = NULL;
  }
}