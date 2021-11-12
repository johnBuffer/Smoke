#pragma once

#include <vector>


constexpr float MAX_RANGE = 10.0f;


struct DNA
{
	using byte = uint8_t;

	DNA(uint64_t bits_count)
		: code(bits_count / 8u + bool(bits_count % 8 && bits_count > 8))
	{}

	template<typename T>
	T get(uint64_t offset) const
	{
		T result;
		const uint64_t dna_offset = offset * sizeof(T);
		memcpy(&result, &code[dna_offset], sizeof(T));
		return result;
	}

	template<typename T>
	void set(uint64_t offset, const T& value)
	{
		//const float checked_value = clamp(-MAX_RANGE, MAX_RANGE, value);
		const uint64_t dna_offset = offset * sizeof(T);
		memcpy(&code[dna_offset], &value, sizeof(T));
	}

	uint64_t getBytesCount() const
	{
		return code.size();
	}

	template<typename T>
	uint64_t getElementsCount() const
	{
		return code.size() / sizeof(T);
	}

	bool operator==(const DNA& other) const
	{
		const uint64_t code_length = getBytesCount();
		if (other.getBytesCount() != code_length) {
			return false;
		}

		for (uint64_t i(0); i < code_length; ++i) {
			if (code[i] != other.code[i]) {
				return false;
			}
		}

		return true;
	}

	std::vector<byte> code;
};
