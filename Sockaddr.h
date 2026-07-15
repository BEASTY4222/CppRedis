#include <cstdint>

struct sockaddr_in{
    uint16_t sin_family; // AF_INET
    uint16_t sin_port; // port in big-endian
    struct in_addr sin_addr;// IPv4 
    //(the addres of where it needs to go kinda)
};

struct in_addr
{
    uint32_t s_addr; //IPv4 in big-endian 
};
