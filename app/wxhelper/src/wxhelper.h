
#ifndef WXHELPER_WXHELPER_H_
#define WXHELPER_WXHELPER_H_
#include <Windows.h>
#include "singleton.h"
namespace wxhelper {
    class WxHelper  : public base::Singleton<WxHelper>{

    public:
    void init(HMODULE module);
    void finally();
    };
}
#endif