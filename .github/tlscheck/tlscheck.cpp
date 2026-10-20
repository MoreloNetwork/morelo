// tlscheck <host>...: connects like the updater does (system CA verification) and prints the result. Used by CI.
#include <cstdio>
#include "net/http_client.h"

int main(int argc, char **argv)
{
  mlog_configure("", true);
  mlog_set_log("net.ssl:INFO");
  int failures = 0;
  for (int i = 1; i < argc; ++i)
  {
    epee::net_utils::http::http_simple_client client;
    epee::net_utils::ssl_options_t ssl{epee::net_utils::ssl_support_t::e_ssl_support_enabled};
    client.set_server(argv[i], "443", boost::none, std::move(ssl));
    const bool ok = client.connect(std::chrono::seconds(15));
    std::printf("%s: %s\n", argv[i], ok ? "VERIFIED" : "REJECTED");
    failures += !ok;
  }
  return failures;
}
