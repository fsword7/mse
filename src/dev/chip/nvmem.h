/*
 * nvmem.h - Non-volatile memory interface package
 *
 *  Created on: Feb 17, 2021
 *      Author: Tim Stark
 */

#pragma once

class nvmInterface : public DeviceInterface
{
public:
	nvmInterface(Device *owner, ctag_t *name = nullptr);
	~nvmInterface();

//	void reset();
//	void load(fs::path &fname);
//	void save(fs::path &fname);
//
//protected:
//	virtual void nvmSetDefault() = 0;
//	virtual void nvmLoad(fs::path &fname) { load(fname); }
//	virtual void nvmSave(fs::path &fname) { save(fname); }
//
//protected:
//	vector<uint8_t []> data;
};
