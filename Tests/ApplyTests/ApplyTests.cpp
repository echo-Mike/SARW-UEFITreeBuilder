#include <iostream>
#include <cstdio>


#define PROJ_UINT64_FORMAT(value) "%llu (%#.16llX)", (value), (value)
#define PROJ_UINT32_FORMAT(value) "%u (%#.8X)", (value), (value)
#define PROJ_UINT16_FORMAT(value) "%hu (%#.4hX)", (value), (value)
#define PROJ_UINT8_FORMAT(value) "%hhu (%#.2hhX)", (value), (value)

#define PROJ_INT64_FORMAT(value) "%lld (%#.16llX)", (value), (value)
#define PROJ_INT32_FORMAT(value) "%d (%#.8X)", (value), (value)
#define PROJ_INT16_FORMAT(value) "%hd (%#.4hX)", (value), (value)
#define PROJ_INT8_FORMAT(value) "%hhd (%#.2hhX)", (value), (value)

#define PROJ_16BYTE_FORMAT(...) "%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX", __VA_ARGS__

#define PROJ_SNPRINTF(buffer, format, ...) ::std::snprintf(buffer, sizeof(buffer), format( __VA_ARGS__ ) )

int main(void)
{
	char strBuffer[1024];
	std::uint64_t a = 0xff00ff00ff00ff00;
	std::uint32_t b = 0xff00ff00;
	std::uint16_t c = 0xff00;
	std::uint8_t d = 0xff;
	PROJ_SNPRINTF(strBuffer, PROJ_16BYTE_FORMAT, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
	std::cout << strBuffer << '\n';
	
	return 1;
}