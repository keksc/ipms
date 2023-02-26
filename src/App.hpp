#include "wx/wx.h"

#include <wx/snglinst.h>

#include "Main.hpp"

#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

class IpmsApp : public wxApp {
public:
    virtual bool OnInit();
    virtual int OnExit();
private:
    wxSingleInstanceChecker* m_checker;
    wxSplashScreen* splash;
    MainFrame *frame;
    int m_checkUpdatesState = 0;
    int m_checkDebugState = 0;
    int m_checkIconState = 0;
};

wxIMPLEMENT_APP(IpmsApp);

#endif // APP_HPP_INCLUDED