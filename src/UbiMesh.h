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

Developed and maintained by Jose Garcia for IoT Services Inc
@jotathebest at github: https://github.com/jotathebest
*/

#ifndef _UbiMesh_H_
#define _UbiMesh_H_

#include <iterator>
#include <map>
#include "Particle.h"
#include "UbiConstants.h"
#include "UbiProtocol.h"
#include "UbiTypes.h"
#include "string.h"

typedef std::map<uint8_t, char*> meshMap;
static IotProtocol iotProtocolMesh = UBI_UDP;
static char* _tokenMesh;
static bool _debugMesh = false;
static int _throttlingTime = 20000;
static unsigned long time_now = millis();

class UbiMesh {
 public:
  explicit UbiMesh(char* token);
  void add(const char* variable_label, float value);
  void add(const char* variable_label, float value, const char* context);
  void add(const char* variable_label, float value, const char* context,
           long unsigned dot_timestamp_seconds);
  void add(const char* variable_label, float value, const char* context,
           long unsigned dot_timestamp_seconds,
           unsigned int dot_timestamp_millis);
  bool meshPublish(const char* channel, const char* data);
  bool meshPublish(const char* channel, const char* data, int timeout);
  bool meshPublishToUbidots();
  bool meshPublishToUbidots(const char* device_label);
  bool meshPublishToUbidots(const char* device_label, const char* device_name);
  void setCloudProtocol(IotProtocol iotProtocol);
  IotProtocol getCloudProtocol();
  void buildDots(std::map<uint8_t, char*>& meshMap, MeshUbi* dots);
#if PLATFORM_ID == PLATFORM_ARGON || PLATFORM_ID == PLATFORM_BORON
  static void ubiPublishHandler(const char* event, const char* data);
  static bool isThrottled();
#endif
  void setDebug(bool debug);
  void meshLoop();
  ~UbiMesh();

 private:
  char* _default_device_label;
  char _meshPayload[256];
  void _addDeviceToDot(std::map<uint8_t, char*>& meshMap, MeshUbi* _dots);
  void _addVariableToDot(std::map<uint8_t, char*>& meshMap, MeshUbi* _dots);
  void _addValueToDot(std::map<uint8_t, char*>& meshMap, MeshUbi* _dots);
  void _addContextToDot(std::map<uint8_t, char*>& meshMap, MeshUbi* _dots);
  void _addTimestampToDot(std::map<uint8_t, char*>& meshMap, MeshUbi* _dots);
  bool _MeshReconnect(int timeout);
};
#endif