#include <cstdint>


struct myIn_addr
{
    uint32_t s_addr; //IPv4 in big-endian 
};

struct mySockaddr_in{
    uint16_t sin_family; // AF_INET
    uint16_t sin_port; // port in big-endian
    myIn_addr sin_addr;// IPv4 
    unsigned char sin_zero[8]; // padding — must be present, kernel expects 16-byte total size
    //(the addres of where it needs to go kinda)
};
