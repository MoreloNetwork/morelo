#include "net/net_helper.h"

#include <vector>
#include <boost/asio/connect.hpp>

namespace epee
{
namespace net_utils
{
	boost::unique_future<boost::asio::ip::tcp::socket>
	direct_connect::operator()(const std::string& addr, const std::string& port, boost::asio::steady_timer& timeout) const
	{
		// Get a list of endpoints corresponding to the server name.
		//////////////////////////////////////////////////////////////////////////
		boost::asio::ip::tcp::resolver resolver(GET_IO_SERVICE(timeout));
		const auto results = resolver.resolve(addr, port, boost::asio::ip::tcp::resolver::canonical_name);
		if(results.empty()) // Documentation states that successful call is guaranteed to be non-empty
			throw boost::system::system_error{boost::asio::error::fault, "Failed to resolve " + addr};

		// tried in turn, IPv4 first: a hostname with both records keeps connecting over IPv4 but
		// still works when only its IPv6 address answers
		std::vector<boost::asio::ip::tcp::endpoint> endpoints;
		for(const auto& entry : results)
			if(entry.endpoint().address().is_v4())
				endpoints.push_back(entry.endpoint());
		for(const auto& entry : results)
			if(!entry.endpoint().address().is_v4())
				endpoints.push_back(entry.endpoint());

		//////////////////////////////////////////////////////////////////////////

		struct new_connection
		{
			boost::promise<boost::asio::ip::tcp::socket> result_;
			boost::asio::ip::tcp::socket socket_;

			explicit new_connection(boost::asio::io_context& io_service)
			  : result_(), socket_(io_service)
			{}
		};

		const auto shared = std::make_shared<new_connection>(GET_IO_SERVICE(timeout));
		timeout.async_wait([shared] (boost::system::error_code error)
		{
			if (error != boost::system::errc::operation_canceled && shared && shared->socket_.is_open())
			{
				shared->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
				shared->socket_.close();
			}
		});
		boost::asio::async_connect(shared->socket_, endpoints, [shared] (boost::system::error_code error, const boost::asio::ip::tcp::endpoint&)
		{
			if (shared)
			{
				if (error)
					shared->result_.set_exception(boost::system::system_error{error});
				else
					shared->result_.set_value(std::move(shared->socket_));
			}
		});
		return shared->result_.get_future();
	}
}
}
