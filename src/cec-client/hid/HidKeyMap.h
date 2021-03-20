#ifndef HIDKEYMAP
#define HIDKEYMAP

#include <map>
#include <string>
#include <vector>
#include <cstdint>

class HidKeyMap final
{
public:
	using Byte = uint8_t;
	using Bytes = std::vector<Byte>;

	size_t Load(const std::string&);
	const Bytes& Get(Byte, bool = true) const;

private:
	std::map<uint8_t, std::pair<Bytes, Bytes>> keys;

	static Bytes HexToBytes(const std::string&);
};

#endif
