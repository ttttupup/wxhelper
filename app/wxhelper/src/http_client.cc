#include "http_client.h"

namespace http {

HttpClient::HttpClient(std::string url, int timeout)
    : url_(url), timeout_(timeout) {}

void HttpClient::SendRequest(const std::string &content) {
  struct mg_mgr mgr;
  Data data;
  data.done = false;
  data.post_data = content;
  data.url = url_;
  data.timeout = timeout_;
  mg_mgr_init(&mgr);
  mg_http_connect(&mgr, url_.c_str(), &HttpClient::OnHttpEvent, &data);
  while (!data.done) {
    mg_mgr_poll(&mgr, 500);
  }
  mg_mgr_free(&mgr);
  data.done = false;
}

void HttpClient::OnHttpEvent(struct mg_connection *c, int ev, void *ev_data,
                             void *fn_data) {
 
  Data *data = (Data *)fn_data;
  const char *s_url = data->url.c_str();
  int timeout = data->timeout;
  if (ev == MG_EV_OPEN) {
    // Connection created. Store connect expiration time in c->data
    *(uint64_t *)c->data = mg_millis() + timeout;
  } else if (ev == MG_EV_POLL) {
    if (mg_millis() > *(uint64_t *)c->data &&
        (c->is_connecting || c->is_resolving)) {
      mg_error(c, "Connect timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    struct mg_str host = mg_url_host(s_url);
    if (mg_url_is_ssl(s_url)) {
      // no implement
    }
    // Send request
    size_t content_length = data->post_data.size();
    mg_printf(c,
              "POST %s HTTP/1.0\r\n"
              "Host: %.*s\r\n"
              "Content-Type: application/json\r\n"
              "Content-Length: %d\r\n"
              "\r\n",
              mg_url_uri(s_url), (int)host.len, host.ptr, content_length);
    mg_send(c, data->post_data.c_str(), content_length);
  } else if (ev == MG_EV_HTTP_MSG) {
// Response is received. Print it
#ifdef _DEBUG
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    printf("%.*s", (int)hm->message.len, hm->message.ptr);
#endif
    // c->is_closing = 1;  // Tell mongoose to close this connection
    c->is_draining = 1;
    data->done = true;  // Tell event loop to stops
  } else if (ev == MG_EV_ERROR) {
    data->done = true;  // Error, tell event loop to stop
  } else if (ev == MG_EV_CLOSE) {
    if (!data->done) {
      data->done = true;
    }
  } else if (ev == MG_EV_HTTP_CHUNK) {
    mg_error(c, "http chunk no implement");
    c->is_closing = 1;
    data->done = true;
  }
}

}  // namespace http