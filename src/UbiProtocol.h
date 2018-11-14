#ifndef _UbiProtocol_H_
#define _UbiProtocol_H_

#include "UbiTypes.h"
class UbiProtocol {
  public:
    virtual bool sendData(const char* device_label, const char* device_name, char* payload, Ubi_flags* flags) = 0;
    virtual float get(const char* device_label, const char* variable_label) = 0;
    virtual void setDebug(bool debug) = 0;
};

#endif