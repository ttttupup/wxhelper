#ifndef WXHELPER_HTTP_SERVER_CALLBACK_H_
#define WXHELPER_HTTP_SERVER_CALLBACK_H_
#include <string>

#include "http_server.h"
#include "mongoose.h"


void StartHttpServer(wxhelper::HttpServer *server);

void EventHandler(struct mg_connection *c, int ev, void *ev_data,
                  void *fn_data);
void HandleHttpRequest(struct mg_connection *c, void *ev_data);
void HandleWebsocketRequest(struct mg_connection *c, void *ev_data);
std::string HttpDispatch(struct mg_connection *c, struct mg_http_message *hm);

#endif