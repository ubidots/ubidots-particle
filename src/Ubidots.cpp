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
#include "UbiBuilder.h"
#include "UbiConstants.h"
#include "UbiProtocol.h"
#include "UbiProtocolHandler.h"
#include "UbiTypes.h"
#include "Ubidots.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

Ubidots::Ubidots(char* token, UbiServer server, IotProtocol iot_protocol) {
  _iot_protocol = iot_protocol;
#if iot_protocol != UBI_MESH
  _protocol = new UbiProtocolHandler(token, server, iot_protocol);
#else
  _protocol = new UbiMesh();
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
#if _iot_protocol != UBI_MESH
  _protocol->add(variable_label, value, context, dot_timestamp_seconds,
                 dot_timestamp_millis);
#endif
}

void Ubidots::setDebug(bool debug) {
  _debug = debug;
  _protocol->setDebug(debug);
}