#include "imemory.hh"

class GameInterface : public MemoryInterface {
protected:
	virtual void ABSTRACT() const {}
	
	void*		process_handle_;	
	uint32_t 	address_money_;
	uint32_t 	address_angels_;

public:
	
	double get_money() const { return read_address_integral<double>(this->address_money_, this->process_handle_); }
	void set_money(double value) const { write_address_integral<double>(this->address_money_, value, this->process_handle_); }
	
	double get_angels() const { return read_address_integral<double>(this->address_angels_, this->process_handle_); }
	void set_angels(double value) const { write_address_integral<double>(this->address_angels_, value, this->process_handle_); }

	GameInterface() {
		uint16_t process_id = processid_fromimage((char*)"adventure-capitalist.exe");
		this->process_handle_ = processhandle_frompid(process_id);
				
		uint32_t module_offset = module_address((char*)"mono.dll", process_id);
		
		this->address_money_ = pointer_chain_resolve(0x001F75EC, module_offset, {0x40, 0x3C, 0xE4, 0x20, 0x18}, this->process_handle_);
		this->address_angels_ = pointer_chain_resolve(0x001F50AC, module_offset, {0x1B4, 0x1C, 0x14, 0x10, 0x258}, this->process_handle_);
	}
};