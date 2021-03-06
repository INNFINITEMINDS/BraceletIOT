#ifndef _PARSER_H
#define _PARSER_H

#include "Logs_Container.h"

// Prints floats into the stream with an accuracy of 6 digits.
// Used in location type.
Stream& operator<<(Stream& stream, float val) {
	stream.print(String(val, 6));
}

/**
 * serializes LogRecord to stream.
 * output format is according to README.md
 */
Stream& operator<<(Stream& stream, const LogRecord& record) {
	stream.print(F("<"));
	stream.print(record.type);
	stream.print(",");
	stream.print(record.timestamp.time);
	stream.print(",");
	stream.print(record.timestamp.id);
	stream.print(",");

	switch (record.type) {
	case tag_scan:
		stream.print(record.data.tagId);
		break;
	case mobile_device_id:
		stream.print(record.data.mobileIdData);
		break;
	case app_new_data:
		stream.print(record.data.rawData);
		break;
	case app_update_data:
		stream.print(record.data.UpdateRecord.ts.time);
		stream.print(",");
		stream.print(record.data.UpdateRecord.ts.id);
		stream.print(",");
		stream.print(record.data.UpdateRecord.data);
		break;
	case app_headquarter_communication:
		stream.print(record.data.rawData);
		break;
	case blood_pressure:
		stream.print(record.data.rawData);
		break;
	case app_command:
		stream.print(record.data.rawData);
		break;
	case app_delete_data:
		stream.print(record.data.UpdateRecord.ts.time);
		stream.print(",");
		stream.print(record.data.UpdateRecord.ts.id);
		stream.print(",");
		stream.print(record.data.UpdateRecord.data);
		break;
	case app_location_lat:
		stream << ((float)record.data.location) / LOCATION_FACTOR;
		break;
	case app_location_lon:
		stream << ((float)record.data.location) / LOCATION_FACTOR;
		break;
	case app_soldier_status:
		stream.print(record.data.rawData);
		break;
	case custom:
		stream.print(record.data.rawData);
		break;
	case soldier_id:
		stream.print(record.data.rawData);
		break;
	default:
		stream.print(record.data.rawData);
		break;
	}

	stream.print(F(">"));
	return stream;
}

/**
 * deserializes LogRecord from stream.
 * input format is according to README.md
 * returns true iff success.
 */
bool operator >> (Stream& stream, LogRecord& record) {
	if (stream.read() != '<') {
		return false;
	}

	Data_Type type = (Data_Type)stream.parseInt();
	if (stream.read() != ',') {
		return false;
	}

	Data data;
	switch (type) {
	case tag_scan:
		data.tagId = stream.parseInt();
		break;
	case mobile_device_id:
		data.mobileIdData = stream.parseInt();
		break;
	case app_new_data:
		data.rawData = stream.parseInt();
		break;
	case app_update_data:
		data.UpdateRecord.ts.time = stream.parseInt();
		if (stream.read() != ',') return false;
		data.UpdateRecord.ts.id = stream.parseInt();
		if (stream.read() != ',') return false;
		data.UpdateRecord.data = stream.parseInt();
		break;
	case app_headquarter_communication:
		data.rawData = stream.parseInt();
		break;
	case blood_pressure:
		data.rawData = stream.parseInt();
		break;
	case app_command:
		data.rawData = stream.parseInt();
		break;
	case app_delete_data:
		data.UpdateRecord.ts.time = stream.parseInt();
		if (stream.read() != ',') return false;
		data.UpdateRecord.ts.id = stream.parseInt();
		if (stream.read() != ',') return false;
		data.UpdateRecord.data = stream.parseInt();
		break;
	case app_location_lat:
		float lat;
		lat = stream.parseFloat();
		data.location = (long)(lat * LOCATION_FACTOR);
		break;
	case app_location_lon:
		float lon;
		lon = stream.parseFloat();
		data.location = (long)(lon * LOCATION_FACTOR);
		break;
	case app_soldier_status:
		data.rawData = stream.parseInt();
		break;
	case custom:
		data.rawData = stream.parseInt();
		break;
	case get_db:
		//No need to do anything, nothing is saved on the device.
		stream.parseInt(); //just removing it from the stream;
		break;
	case soldier_id:
		data.rawData = stream.parseInt();
		break;
	default:
		return false;
		break;
	}

	if (stream.read() != '>') {
		return false;
	}

	record = LogRecord(type, data);
	return true;
}

/**
 * serializes container to stream.
 */
Stream& operator<<(Stream& stream, const LogsContainer& container) {
	stream.print('[');
	for (int i = 0; i < container.size; i++) {
		stream << container.records[i];
	}
	stream.println("]");
}

#endif
