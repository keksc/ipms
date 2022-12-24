#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/snglinst.h>

#include "App.hpp"
#include "Main.hpp"

bool IpmsApp::OnInit() {
    m_checker = new wxSingleInstanceChecker();
    if(m_checker->IsAnotherRunning()) {
        wxLogError("Une autre instance de ce programme est en cours d'execution, annulation.");
        delete m_checker;
        m_checker = nullptr;

        return false;
    }
    MainFrame *frame = new MainFrame();
    frame->Show(true);
    return true;
}
int IpmsApp::OnExit() {
    delete m_checker;
    return 0;
}