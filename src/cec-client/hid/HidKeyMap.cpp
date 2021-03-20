#include "HidKeyMap.h"
#include <fstream>
#include <iterator>
#include <cstdlib>

size_t HidKeyMap::Load(const std::string& db)
{
	keys.clear();
	std::ifstream fin(db);
	if (!fin.is_open())
		throw std::runtime_error("Failed to open \"" + db + "\"");
	std::string inKeyStr;
	std::string outKeyPressedStr;
	std::string outKeyReleasedStr;
	while (fin >> inKeyStr) {
		if (!(fin >> outKeyPressedStr))
			throw std::runtime_error("Failed to read pressed output key code for input key [" + inKeyStr + "]");
		if (!(fin >> outKeyReleasedStr))
			throw std::runtime_error("Failed to read released output key code for input key [" + inKeyStr + "]");
		const auto inKey = HexToBytes(inKeyStr);
		if (inKey.size() != 1)
			throw std::runtime_error("Invalid input key [" + inKeyStr + "] size");
		auto outKeyPressed = HexToBytes(outKeyPressedStr);
		auto outKeyReleased = HexToBytes(outKeyReleasedStr);
		if (!keys.emplace(inKey[0], std::make_pair(std::move(outKeyPressed), std::move(outKeyReleased))).second)
			throw std::runtime_error("Duplicate key [" + inKeyStr + "]");
	}
	return keys.size();
}

const HidKeyMap::Bytes& HidKeyMap::Get(Byte code, bool onPress) const
{
	static const Bytes empty;
	const auto keyIt = keys.find(code);
	if (keyIt == keys.end())
		return empty;
	return onPress ? keyIt->second.first : keyIt->second.second;
}

// static
HidKeyMap::Bytes HidKeyMap::HexToBytes(const std::string& hex)
{
	if (hex.size() % 2 != 0)
		throw std::runtime_error("Invalid size of hex string \"" + hex + "\"");
	Bytes output(hex.size()/2, 0);

	size_t hexPos = 0;
	for (auto& i : output) {
		const auto hexByteStr = hex.substr(hexPos, 2);
		char* hexByteStrEndPos = nullptr;
		i = strtoul(hexByteStr.c_str(), &hexByteStrEndPos, 16);
		if (hexByteStrEndPos != &hexByteStr.c_str()[2])
			throw std::runtime_error("Invalid hex string \"" + hex + "\" [" + std::to_string(hexPos) + "," + std::to_string(hexPos+1) + "]");
		hexPos += 2;
	}

	return output;
}
