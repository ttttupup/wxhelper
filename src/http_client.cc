#include "pch.h"
#include "http_client.h"
namespace wxhelper {

 void HttpClient::SendRequest(std::string content) {
    struct mg_mgr mgr;
    Data data ;
    data.done = false;
    data.post_data = content;
    mg_mgr_init(&mgr);
    mg_http_connect(&mgr, url_.c_str(), OnHttpEvent, &data);
    while (!data.done){
        mg_mgr_poll(&mgr, 500);
    }
    mg_mgr_free(&mgr);
}


 void HttpClient::OnHttpEvent(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  const char * s_url = GetInstance().url_.c_str();
  // 注意：这里必须用指针，不能按值拷贝 Data —— 拷贝出来的是独立副本，
  // 后面对 data->done 的赋值只会修改副本，SendRequest 里 while(!data->done)
  // 永远等不到 true，调用线程会被永久阻塞在 mg_mgr_poll 死循环里。
  Data *data = (Data *)fn_data;
  if (ev == MG_EV_OPEN) {
    // Connection created. Store request expiration time in c->data.
    // 这个时间戳之后会在 MG_EV_CONNECT 里重置，这里先给连接阶段一个保底超时。
    *(uint64_t *) c->data = mg_millis() + GetInstance().timeout_;
  } else if (ev == MG_EV_POLL) {
    // 之前这里只在 c->is_connecting || c->is_resolving 时才判断超时，
    // 导致 TCP 连上之后如果对端一直不回响应，会永久阻塞在这条连接上
    // （而 SendRequest 是同步等待 data->done 的，相当于把调用线程也一起卡死）。
    // 这里去掉这个限制，让"已连接但响应迟迟不来"同样会被判定为超时并主动断开。
    if (mg_millis() > *(uint64_t *) c->data) {
      mg_error(c, "Request timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    // 连接建立后重新计时，给"等待响应"阶段一个完整的超时窗口，
    // 避免域名解析/TCP 握手耗时过长把等待响应的预算提前吃掉。
    *(uint64_t *) c->data = mg_millis() + GetInstance().timeout_;

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
              mg_url_uri(s_url), (int) host.len,
              host.ptr, content_length);
   mg_send(c, data->post_data.c_str(), content_length);
  } else if (ev == MG_EV_HTTP_MSG) {
    // Response is received. Print it
    #ifdef _DEBUG
      struct mg_http_message *hm = (struct mg_http_message *) ev_data;
      printf("%.*s", (int) hm->message.len, hm->message.ptr);
    #endif
    c->is_closing = 1;         // Tell mongoose to close this connection
    data->done = true;  // Tell event loop to stops
  } else if (ev == MG_EV_ERROR) {
    data->done = true;  // Error, tell event loop to stop
  }
}

void HttpClient::SetConfig(std::string url,uint64_t timeout){
  url_=url;
  timeout_=timeout;
}

}  // namespace wxhelper