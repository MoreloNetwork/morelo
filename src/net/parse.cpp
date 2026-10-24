// Copyright (c) 2018-2019, The Arqma Project
// Copyright (c) 2018, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "parse.h"

#include <algorithm>
#include <boost/asio/ip/address_v6.hpp>

#include "net/tor_address.h"
#include "net/i2p_address.h"
#include "string_tools.h"

namespace net
{
    void get_network_address_host_and_port(const std::string& address, std::string& host, std::string& port)
    {
        // IPv6 address with a port: "[addr:addr:...:addr]:port"
        if (address.find(']') != std::string::npos)
        {
            host = address.substr(1, address.rfind(']') - 1);
            if ((host.size() + 2) < address.size())
                port = address.substr(address.rfind(':') + 1);
        }
        // bare IPv6 address without a port: "addr:addr:...:addr"
        else if (std::count(address.begin(), address.end(), ':') >= 2)
        {
            host = address;
        }
        // IPv4, Tor, I2P address or hostname with an optional ":port"
        else
        {
            host = address.substr(0, address.rfind(':'));
            if (host.size() < address.size())
                port = address.substr(host.size() + 1);
        }
    }

    expect<epee::net_utils::network_address>
    get_network_address(const boost::string_ref address, const std::uint16_t default_port)
    {
        std::string host_str;
        std::string port_str;
        get_network_address_host_and_port(std::string{address}, host_str, port_str);
        const boost::string_ref host{host_str};

        if (host.empty())
            return make_error_code(net::error::invalid_host);
        if (host.ends_with(".onion"))
            return tor_address::make(address, default_port);
        if (host.ends_with(".i2p"))
            return i2p_address::make(address, default_port);

        std::uint16_t port = default_port;
        if (!port_str.empty())
        {
            if (!epee::string_tools::get_xtype_from_string(port, port_str))
                return make_error_code(net::error::invalid_port);
        }

        boost::system::error_code ec;
        const boost::asio::ip::address_v6 v6 = boost::asio::ip::make_address_v6(host_str, ec);
        if (!ec)
        {
            const epee::net_utils::ipv6_network_address v6_address{v6, port};
            // an IPv4-mapped address names an IPv4 host, keep a single representation of it
            const boost::optional<epee::net_utils::ipv4_network_address> mapped =
                epee::net_utils::get_ipv4_mapped_address(v6_address);
            if (mapped)
                return {*mapped};
            return {v6_address};
        }

        std::uint32_t ip = 0;
        if (epee::string_tools::get_ip_int32_from_string(ip, host_str))
            return {epee::net_utils::ipv4_network_address{ip, port}};
        return make_error_code(net::error::unsupported_address);
    }

    expect<epee::net_utils::ipv4_network_subnet>
    get_ipv4_subnet_address(const boost::string_ref address, bool allow_implicit_32)
    {
        uint32_t mask = 32;
        const boost::string_ref::size_type slash = address.find_first_of('/');
        if (slash != boost::string_ref::npos)
        {
            if (!epee::string_tools::get_xtype_from_string(mask, std::string{address.substr(slash + 1)}))
                return make_error_code(net::error::invalid_mask);
            if (mask > 32)
                return make_error_code(net::error::invalid_mask);
        }
        else if (!allow_implicit_32)
            return make_error_code(net::error::invalid_mask);

        std::uint32_t ip = 0;
        boost::string_ref S(address.data(), slash != boost::string_ref::npos ? slash : address.size());
        if (!epee::string_tools::get_ip_int32_from_string(ip, std::string(S)))
            return make_error_code(net::error::invalid_host);

        return {epee::net_utils::ipv4_network_subnet{ip, (uint8_t)mask}};
    }

    expect<epee::net_utils::ipv6_network_subnet>
    get_ipv6_subnet_address(const boost::string_ref address, bool allow_implicit_128)
    {
        uint32_t mask = 128;
        const boost::string_ref::size_type slash = address.find_first_of('/');
        if (slash != boost::string_ref::npos)
        {
            if (!epee::string_tools::get_xtype_from_string(mask, std::string{address.substr(slash + 1)}))
                return make_error_code(net::error::invalid_mask);
            if (mask > 128)
                return make_error_code(net::error::invalid_mask);
        }
        else if (!allow_implicit_128)
            return make_error_code(net::error::invalid_mask);

        boost::string_ref host = address.substr(0, slash);
        if (host.size() >= 2 && host.front() == '[' && host.back() == ']')
            host = host.substr(1, host.size() - 2);

        boost::system::error_code ec;
        const boost::asio::ip::address_v6 ip = boost::asio::ip::make_address_v6(std::string{host}, ec);
        // IPv4 hosts are never seen as ::ffff:a.b.c.d, such a subnet would match nothing
        if (ec || ip.is_v4_mapped())
            return make_error_code(net::error::invalid_host);

        return {epee::net_utils::ipv6_network_subnet{ip, (uint8_t)mask}};
    }
}
