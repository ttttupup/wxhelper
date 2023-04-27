#include "pch.h"
#include "config.h"

namespace wxhelper {
Config::Config(/* args */) {}

Config::~Config() {}


void Config::Initialize(){
    port_ = GetPrivateProfileInt("config", "Port", 19088, "./config.ini");
}
int  Config::GetPort(){
    return port_;
}

 } // namespace wxhelper