/*
Copyright (c) 2013-2020 Ubidots.
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

#include "UbiParticle.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

UbiParticle::UbiParticle(const char* host, const int port, const char* user_agent, const char* token) {
  _host = host;
  _user_agent = user_agent;
  _token = token;
}

/**************************************************************************
 * Destructor
 ***************************************************************************/

UbiParticle::~UbiParticle() {
  delete[] _host;
  delete[] _user_agent;
  delete[] _token;
}

/*
 * Wrapper of the Publish() method from particle to trigger webhooks
 */

bool UbiParticle::sendData(const char* device_label, const char* device_name, char* payload, UbiFlags* flags) {
  return Particle.publish(device_label, payload, flags->particle_flag);
}

float UbiParticle::get(const char* device_label, const char* variable_label) {
  // Retrieving values is not supported using Particle webhooks
  return ERROR_VALUE;
}

void UbiParticle::setDebug(bool debug) { _debug = debug; }

tcpMap UbiParticle::getMultipleValues(const char* deviceLabel, const char* variableLabels) {
  tcpMap results;
  return results;
}
