#include <iostream>
#include <string>
#include <map>

#include "../../source/trainer.h"

class Adcap_Trainer : public Trainer {
private: 
	/* Base address for mono.dll */
	unsigned __int64 m_baddr_mono;

	/* Discovered dynamic addresses for in-game values */
	unsigned __int64 m_addr_money;
	unsigned __int64 m_addr_angels;
	unsigned __int64 m_addr_lemonade_stands;

public:
	Adcap_Trainer() {
		m_window_title = (char*)"Adcap!";
		trainer_setup_();

		m_baddr_mono = discover_base_address_((char*)"mono.dll", m_process_id);

		m_addr_money = discover_dynamic_address_(0x001F75EC, {0x40,  0x3C,  0xE4, 0x20, 0x18 }, m_baddr_mono);
		m_addr_angels = discover_dynamic_address_(0x001F50AC, {0x1B4, 0x1C,  0x14, 0x10, 0x258}, m_baddr_mono);
		m_addr_lemonade_stands = discover_dynamic_address_(0x001F7964, {0x30,  0x17C, 0x14, 0x38, 0x18}, m_baddr_mono);
	}

	double GetMoney() const { return read_address_data<double>(m_addr_money); }
	void SetMoney(double data) const { write_address_data<double>(m_addr_money, data); }

	double GetAngles() const { return read_address_data<double>(m_addr_angels); }
	void SetAngels(double data) const { write_address_data<double>(m_addr_angels, data); }

	double GetLemonadeStands() const { return read_address_data<double>(m_addr_lemonade_stands); }
	void SetLemonadeStands(double data) const { write_address_data<double>(m_addr_lemonade_stands, data); }
};