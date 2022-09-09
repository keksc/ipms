#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "App.h"
#include "Main.h"

bool IpmsApp::OnInit()
{
    MainFrame *frame = new MainFrame();
    frame->Show(true);
    return true;
}
