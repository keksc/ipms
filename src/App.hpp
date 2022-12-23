#include "wx/wx.h"

#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

class IpmsApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(IpmsApp);

#endif // APP_H_INCLUDED