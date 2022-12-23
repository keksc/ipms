#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "App.hpp"
#include "Main.hpp"

bool IpmsApp::OnInit() {
    MainFrame *frame = new MainFrame();
    frame->Show(true);
    return true;
}
