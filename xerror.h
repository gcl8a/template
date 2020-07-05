//modified 02/28/01

#ifndef __TEMPLATE_XERROR_H
#define __TEMPLATE_XERROR_H

#include <string.h>

using namespace std;

class XError {
protected:
    char _zError[256];
public:

    XError(const char* pzError) {
        strncpy(_zError, pzError, 255);
    }

    virtual char* Why(void) {
        return _zError;
    }

    void Raise(void) {
        throw XError(*this);
    }
};

#endif
