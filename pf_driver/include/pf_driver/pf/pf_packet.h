#pragma once

#include <vector>
#include <ros/serialization.h>
#include "pf_driver/PFR2000Header.h"
#include "pf_driver/PFR2300Header.h"

class PFPacketReader;

class PFPacket
{
public:
    pf_driver::PFHeader header;
    std::vector<uint32_t> distance;
    std::vector<uint16_t> amplitude;

    virtual size_t get_size() = 0;
    virtual std::tuple<uint16_t, uint32_t, uint16_t> read_header(ros::serialization::IStream& stream) = 0;
    virtual void get_type(char *p_type) = 0;
    virtual void read_with(PFPacketReader& reader){}

    int find_packet_start(uint8_t* buf, size_t buf_len);
    bool parse_buf(uint8_t* buf, size_t buf_len, size_t &remainder, size_t &p_size);
};

class PFR2000Packet : public PFPacket
{
public:
    virtual size_t get_size()
    {
        return ros::serialization::serializationLength(header);
    }

    virtual std::tuple<uint16_t, uint32_t, uint16_t> read_header(ros::serialization::IStream& stream)
    {
        ros::serialization::Serializer<pf_driver::PFR2000Header>::read(stream, header);
        return std::tuple<uint16_t, uint32_t, uint16_t>(header.header.header_size, header.header.packet_size, header.num_points_packet);
    }

    virtual void read_with(PFPacketReader& reader);

    pf_driver::PFR2000Header header;
};

class PFR2000Packet_A : public PFR2000Packet
{
public:
    // virtual void read_with(PFPacketReader& reader);
    virtual void get_type(char *c)
    {
        c[0] = 0x41;
        c[1] = 0x00;
    }
};

class PFR2000Packet_B : public PFR2000Packet
{
    virtual void get_type(char *c)
    {
        c[0] = 0x42;
        c[1] = 0x00;
    }
};

class PFR2000Packet_C : public PFR2000Packet
{
    virtual void get_type(char *c)
    {
        c[0] = 0x43;
        c[1] = 0x00;
    }
};

class PFR2300Packet : public PFPacket
{
public:
    virtual size_t get_size()
    {
        return ros::serialization::serializationLength(header);
    }

    virtual std::tuple<uint16_t, uint32_t, uint16_t> read_header(ros::serialization::IStream& stream)
    {
        ros::serialization::Serializer<pf_driver::PFR2300Header>::read(stream, header);
        return std::tuple<uint16_t, uint32_t, uint16_t>(header.header.header_size, header.header.packet_size, header.num_points_packet);
    }

    virtual void read_with(PFPacketReader& reader);

    pf_driver::PFR2300Header header;
};

//0x3143
class PFR2300Packet_C1 : public PFR2300Packet
{
    virtual void get_type(char *c)
    {
        c[0] = 0x43;
        c[1] = 0x31;
    }
};