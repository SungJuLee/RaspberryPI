/*
 * json_msg.c
 *
 *  Created on: 2015. 11. 20.
 *      Author: LEE
 */

#include <json/json.h>
#include <stdio.h>
#include <string.h>
#include "poll.c"

char msg[1024];

void sendMsg(json_object *jobj, int obj) {
	char * poststr = json_object_to_json_string(jobj);
	switch (obj) {
	case 1: // Server
		sprintf(msg, "POST / HTTP/1.1\r\n"
				"Host: %s\r\n"
				"Connection: keep-alive\r\n"
				"Content-Type: Application/json\r\n"
				"Content-Length: %d\r\n\r\n"
				"%s\r\n", "http://sweyoon.ivyro.net/msg.php", strlen(poststr),
				poststr);
		break;
	case 2: // IVI
		sprintf(msg, "POST / HTTP/1.1\r\n"
				"Host: %s\r\n"
				"Connection: keep-alive\r\n"
				"Content-Type: Application/json\r\n"
				"Content-Length: %d\r\n\r\n"
				"%s\r\n", "http://yul1006.ivyro.net/msg.php", strlen(poststr),
				poststr);
		break;
	}
}

void Msg300() {
	json_object *jobj, *flag;

	jobj = json_object_new_objet();
	flag = json_objet_new_string("300");
	json_object_object_add(jobj, "flag", flag);

	sendMsg(jobj, 1);
}

void Msg301(char* authorizationresult, char* userid, char* vehiclecode) {
	json_object *jobj, *flag, *obj1, *obj2, *obj3;

	jobj = json_object_new_object();

	flag = json_object_new_string("301");
	obj1 = json_object_new_string(authorizationresult);
	obj2 = json_object_new_string(userid);
	obj3 = json_object_new_string(vehiclecode);

	json_object_object_add(jobj, "flag", flag);
	json_object_object_add(jobj, "authorization_result", obj1);
	json_object_object_add(jobj, "user_id", obj2);
	json_object_object_add(jobj, "vehicle_code", obj3);

	sendMsg(jobj, 1);
}

void Msg302(char* endconnection) {
	json_object *jobj, *flag, *obj1;

	jobj = json_object_new_object();

	flag = json_object_new_string("302");
	obj1 = json_object_new_string(endconnection);

	json_object_object_add(jobj, "flag", flag);
	json_object_object_add(jobj, "end_connection", obj1);

	sendMsg(jobj, 1);
}

void Msg303(char* battery, char* temperature) {
	json_object *jobj, *flag, *obj1, *obj2;

	jobj = json_object_new_object();

	flag = json_object_new_string("303");
	obj1 = json_object_new_string(battery);
	obj2 = json_object_new_string(temperature);

	json_object_object_add(jobj, "flag", flag);
	json_object_object_add(jobj, "battery", obj1);
	json_object_object_add(jobj, "temperature", obj2);

	sendMsg(jobj, 1);
}

void Msg399(char* errMsg) {
	json_object *jobj, *flag, *obj1;

	jobj = json_object_new_object();

	flag = json_object_new_string("399");
	obj1 = json_object_new_string(errMsg);

	json_object_object_add(jobj, "flag", flag);
	json_object_object_add(jobj, "error", obj1);

	sendMsg(jobj, 1);
}

void Msg501(char* authorizationuserid) {
	json_object *jobj, *flag, *obj1;

	jobj = json_object_new_object();

	flag = json_object_new_string("501");
	if(strcmp(authorizationuserid,authorization_user_id) == 0)
		printf("Success!");
	else printf("User Different!");
	obj1 = json_object_new_string(authorization_user_id);

	json_object_object_add(jobj, "flag", flag);
	json_object_object_add(jobj, "authorization_user_id", obj1);

	sendMsg(jobj, 2);
}

void Msg502(char* departure_time, char* destination, char* target_temperature, char* keeping_time) {
	json_object *jobj, *flag, *obj1, *obj2, *obj3, *obj4;

	jobj = json_object_new_object();

	flag = json_object_new_string("502");
	obj1 = json_object_new_string(departure_time);
	obj2 = json_object_new_string(destination);
	obj3 = json_object_new_string(target_temperature);
	obj4 = json_object_new_string(keeping_time);

	json_object_object_add(jobj, "flag", flag);
	json_object_object_add(jobj, "departure_time", obj1);
	json_object_object_add(jobj, "destination", obj2);
	json_object_object_add(jobj, "target_temperature", obj3);
	json_object_object_add(jobj, "keeping_time", obj4);

	sendMsg(jobj, 2);
}

void Msg599(char* errMsg) {
	json_object *jobj, *flag, *obj1;

	jobj = json_object_new_object();

	flag = json_object_new_string("599");
	obj1 = json_object_new_string(errMsg);

	json_object_object_add(jobj, "flag", flag);
	json_object_object_add(jobj, "error", obj1);

	sendMsg(jobj, 2);
}

void Msg401(char *buffer) {
	json_objet *myobj, *dval;
	myobj = json_tokener_parse(buffer);

	dval = json_object_object_get(myobj, "user_id");
	strcpy(authorization_user_id, json_object_get_string(dval));

	Msg501();
}

void Msg402(char *buffer) {
	json_objet *myobj, *dval;
	char departure_time[10];
	char destination[20];
	char target_temperature[10];
	char keeping_time[10];
	myobj = json_tokener_parse(buffer);

	dval = json_object_object_get(myobj, "departure_time");
	strncpy(departure_time, json_object_get_string(dval),
			strlen(json_object_get_string(dval)));
	dval = json_object_object_get(myobj, "destination");
	strncpy(destination, json_object_get_string(dval),
			strlen(json_object_get_string(dval)));
	dval = json_object_object_get(myobj, "target_temperature");
	strncpy(target_temperature, json_object_get_string(dval),
			strlen(json_object_get_string(dval)));
	dval = json_object_object_get(myobj, "user_id");
	strncpy(authorization_user_id, json_object_get_string(dval),
			strlen(json_object_get_string(dval)));

	Msg502(departure_time, destination, target_temperature, keeping_time);
}

void Msg601(char* buffer) {
	json_object *myobj, *dval;
	char authorizationresult[10];
	char userid[10];
	char vehiclecode[12];
	myobj = json_tokener_parse(buffer);

	dval = json_object_objct_get(myobj, "authorization_result");
	strncpy(authorizationresult, json_objcet_get_string(dval),
			strlen(json_object_get_string(dval)));
	dval = json_object_objct_get(myobj, "user_id");
	strncpy(userid, json_objcet_get_string(dval),
			strlen(json_object_get_string(dval)));
	dval = json_object_objct_get(myobj, "vehicle_code");
	strncpy(vehiclecode, json_objcet_get_string(dval),
			strlen(json_object_get_string(dval)));

	if (strcmp(vehiclecode, vehicle_code) == 0)
		printf("success!");

	Msg301(authorizationresult, userid, vehiclecode)
}

void Msg602(char* buffer) {
	json_object *myobj, *dval;
	char endconnection[10];
	myobj = json_tokener_parse(buffer);

	dval = json_object_objct_get(myobj, "end_connection");
	strncpy(endconnection, json_objcet_get_string(dval),
			strlen(json_object_get_string(dval)));

	Msg302(endconnection);
}

void Msg603(char* buffer) {
	json_object *myobj, *dval;
	char battery[10];
	char temperature[10];
	myobj = json_tokener_parse(buffer);

	dval = json_object_objct_get(myobj, "battery");
	strncpy(battery, json_objcet_get_string(dval),
			strlen(json_object_get_string(dval)));
	dval = json_object_objct_get(myobj, "temperature");
	strncpy(temperature, json_objcet_get_string(dval),
			strlen(json_object_get_string(dval)));

	Msg302(battery, temperature);
}
