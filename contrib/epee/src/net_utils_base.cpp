
#include "net/net_utils_base.h"

#include <algorithm>

#include <boost/asio/ip/address_v4.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "string_tools.h"
#include "int-util.h"
#include "net/local_ip.h"

// addresses are kept in network byte order, so the mask has to be too
static inline uint32_t make_ipv4_cidr_mask(const uint8_t mask)
{
	if (mask == 0)
		return 0;
	return SWAP32BE(0xffffffffu << (32 - mask));
}

namespace epee { namespace net_utils
{
	bool ipv4_network_address::equal(const ipv4_network_address& other) const noexcept
	{ return is_same_host(other) && port() == other.port(); }

	bool ipv4_network_address::less(const ipv4_network_address& other) const noexcept
	{ return is_same_host(other) ? port() < other.port() : ip() < other.ip(); }

	std::string ipv4_network_address::str() const
	{ return string_tools::get_ip_string_from_int32(ip()) + ":" + std::to_string(port()); }

	std::string ipv4_network_address::host_str() const { return string_tools::get_ip_string_from_int32(ip()); }
	bool ipv4_network_address::is_loopback() const { return net_utils::is_ip_loopback(ip()); }
	bool ipv4_network_address::is_local() const { return net_utils::is_ip_local(ip()); }

	bool ipv4_network_subnet::equal(const ipv4_network_subnet& other) const noexcept
	{ return is_same_host(other) && m_mask == other.m_mask; }

	bool ipv4_network_subnet::less(const ipv4_network_subnet& other) const noexcept
	{ return subnet() < other.subnet() ? true : (other.subnet() < subnet() ? false : (m_mask < other.m_mask)); }

	uint32_t ipv4_network_subnet::subnet() const noexcept
	{ return m_ip & make_ipv4_cidr_mask(m_mask); }

	std::string ipv4_network_subnet::str() const
	{ return string_tools::get_ip_string_from_int32(subnet()) + "/" + std::to_string(m_mask); }

	std::string ipv4_network_subnet::host_str() const { return string_tools::get_ip_string_from_int32(subnet()) + "/" + std::to_string(m_mask); }
	bool ipv4_network_subnet::is_loopback() const { return net_utils::is_ip_loopback(subnet()); }
	bool ipv4_network_subnet::is_local() const { return net_utils::is_ip_local(subnet()); }
	bool ipv4_network_subnet::matches(const ipv4_network_address &address) const
	{
		return (address.ip() & make_ipv4_cidr_mask(m_mask)) == subnet();
	}

	bool ipv6_network_address::equal(const ipv6_network_address& other) const noexcept
	{ return is_same_host(other) && port() == other.port(); }

	bool ipv6_network_address::less(const ipv6_network_address& other) const noexcept
	{ return is_same_host(other) ? port() < other.port() : m_address < other.m_address; }

	std::string ipv6_network_address::str() const
	{ return std::string("[") + host_str() + "]:" + std::to_string(port()); }

	std::string ipv6_network_address::host_str() const { return m_address.to_string(); }
	bool ipv6_network_address::is_loopback() const { return m_address.is_loopback(); }
	// link-local, the deprecated site-local range and unique local addresses (fc00::/7) never
	// reach us from the internet, treat them like the private IPv4 ranges
	bool ipv6_network_address::is_local() const
	{ return m_address.is_link_local() || m_address.is_site_local() || (m_address.to_bytes()[0] & 0xfe) == 0xfc; }

	bool ipv6_network_subnet::equal(const ipv6_network_subnet& other) const noexcept
	{ return is_same_host(other) && m_mask == other.m_mask; }

	bool ipv6_network_subnet::less(const ipv6_network_subnet& other) const noexcept
	{ return subnet() < other.subnet() ? true : (other.subnet() < subnet() ? false : (m_mask < other.m_mask)); }

	boost::asio::ip::address_v6 ipv6_network_subnet::subnet() const noexcept
	{ return get_ipv6_subnet_address(m_address, m_mask); }

	std::string ipv6_network_subnet::str() const
	{ return subnet().to_string() + "/" + std::to_string(m_mask); }

	std::string ipv6_network_subnet::host_str() const { return str(); }
	bool ipv6_network_subnet::is_loopback() const { return ipv6_network_address{subnet(), 0}.is_loopback(); }
	bool ipv6_network_subnet::is_local() const { return ipv6_network_address{subnet(), 0}.is_local(); }
	bool ipv6_network_subnet::matches(const ipv6_network_address &address) const
	{
		return get_ipv6_subnet_address(address.ip(), m_mask) == subnet();
	}

	boost::optional<ipv4_network_address> get_ipv4_mapped_address(const ipv6_network_address& address)
	{
		const boost::asio::ip::address_v6 ip = address.ip();
		if (!ip.is_v4_mapped())
			return boost::none;

		const boost::asio::ip::address_v4 ipv4 =
			boost::asio::ip::make_address_v4(boost::asio::ip::v4_mapped, ip);
		return ipv4_network_address{SWAP32BE(ipv4.to_uint()), address.port()};
	}

	bool should_group_ipv6_by_prefix(const boost::asio::ip::address_v6& ip)
	{
		return !ip.is_unspecified() && !ip.is_loopback() && !ip.is_multicast() &&
			!ipv6_network_address{ip, 0}.is_local() && !ip.is_v4_mapped();
	}

	boost::asio::ip::address_v6 get_ipv6_subnet_address(const boost::asio::ip::address_v6& ip, const std::size_t prefix_bits)
	{
		boost::asio::ip::address_v6::bytes_type bytes = ip.to_bytes();
		const std::size_t bits = std::min(prefix_bits, std::size_t{128});
		if (bits < 128)
		{
			const std::size_t full_bytes = bits / 8;
			const std::size_t remainder_bits = bits % 8;
			if (remainder_bits != 0)
				bytes[full_bytes] &= uint8_t(0xffu << (8 - remainder_bits));
			std::fill(bytes.begin() + full_bytes + (remainder_bits != 0), bytes.end(), 0);
		}
		return boost::asio::ip::address_v6(bytes);
	}


	bool network_address::equal(const network_address& other) const
	{
		// clang typeid workaround
		network_address::interface const* const self_ = self.get();
		network_address::interface const* const other_self = other.self.get();
		if (self_ == other_self) return true;
		if (!self_ || !other_self) return false;
		if (typeid(*self_) != typeid(*other_self)) return false;
		return self_->equal(*other_self);
	}

	bool network_address::less(const network_address& other) const
	{
		// clang typeid workaround
		network_address::interface const* const self_ = self.get();
		network_address::interface const* const other_self = other.self.get();
		if (self_ == other_self) return false;
		if (!self_ || !other_self) return self == nullptr;
		if (typeid(*self_) != typeid(*other_self))
			return self_->get_type_id() < other_self->get_type_id();
		return self_->less(*other_self);
	}

	bool network_address::is_same_host(const network_address& other) const
	{
		// clang typeid workaround
		network_address::interface const* const self_ = self.get();
		network_address::interface const* const other_self = other.self.get();
		if (self_ == other_self) return true;
		if (!self_ || !other_self) return false;
		if (typeid(*self_) != typeid(*other_self)) return false;
		return self_->is_same_host(*other_self);
	}

	boost::optional<ipv4_network_address> get_ipv4_mapped_address(const network_address& address)
	{
		if (address.get_type_id() != ipv6_network_address::get_type_id())
			return boost::none;
		return get_ipv4_mapped_address(address.as<const ipv6_network_address>());
	}

	std::string get_host_limit_key(const network_address& address)
	{
		if (address.get_type_id() == ipv6_network_address::get_type_id())
		{
			const boost::asio::ip::address_v6 ip = address.as<const ipv6_network_address>().ip();
			if (should_group_ipv6_by_prefix(ip))
				return ipv6_network_subnet{ip, 64}.str();
		}
		return address.host_str();
	}

  std::string print_connection_context(const connection_context_base& ctx)
  {
    std::stringstream ss;
    ss << ctx.m_remote_address.str() << " " << ctx.m_connection_id << (ctx.m_is_income ? " INC":" OUT");
    return ss.str();
  }

  std::string print_connection_context_short(const connection_context_base& ctx)
  {
    std::stringstream ss;
    ss << ctx.m_remote_address.str() << (ctx.m_is_income ? " INC":" OUT");
    return ss.str();
  }

  const char* zone_to_string(zone value) noexcept
  {
    switch (value)
    {
    case zone::public_:
      return "public";
    case zone::i2p:
      return "i2p";
    case zone::tor:
      return "tor";
    default:
      break;
    }
    return "invalid";
  }

  zone zone_from_string(const boost::string_ref value) noexcept
  {
    if (value == "public")
      return zone::public_;
    if (value == "i2p")
      return zone::i2p;
    if (value == "tor")
      return zone::tor;
    return zone::invalid;
  }
}}
