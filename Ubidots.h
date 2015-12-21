#ifndef __Ubidots_H_
#define __Ubidots_H_

#include "application.h"
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"


class Ubidots {
	public:
		Ubidots(String token);
		boolean connect();
		int Send();
		void save_value(String varibleName, String varialbeValue);
		void save_value(String varibleName, String varialbeValue, String context);

		String info;
		boolean flag;
		String ID = Spark.deviceID();

	private:

}
#endif
