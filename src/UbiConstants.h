#ifndef _UbiConstants_H_
#define _UbiConstants_H_

#include "UbiTypes.h"
#include "Particle.h"

const char * const UBIDOTS_SERVER = "industrial.api.ubidots.com";
const char * const USER_AGENT = "UbidotsParticle/3.0";
const int UBIDOTS_HTTP_PORT = 80;
const int UBIDOTS_TCP_PORT = 9012;
const uint8_t MAX_VALUES = 10;
const float ERROR_VALUE = -3.4028235E+8;
const int MAX_BUFFER_SIZE = 700;
static UbiServer UBI_INDUSTRIAL = "industrial.api.ubidots.com";
static UbiServer UBI_EDUCATIONAL = "things.ubidots.com";

#endif