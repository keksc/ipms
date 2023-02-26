#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/snglinst.h>
#include <wx/splash.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>

#include "App.hpp"
#include "Main.hpp"

#include "settings.hpp"

#include "res/icon.xpm"

bool IpmsApp::OnInit() {
    m_checker = new wxSingleInstanceChecker();
    if(m_checker->IsAnotherRunning()) {
        wxLogError(L"Une autre instance de ce programme est en cours d'execution, annulation.");
        delete m_checker;
        m_checker = nullptr;

        return false;
    }
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString path(pathinfo.GetUserDataDir() + L"/preferences.cfg");
    wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
    wxFileConfig config("IPMS", wxEmptyString, path, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
    config.Read("icone", &m_checkIconState, 1);
    wxPuts(L"[" VRT L"-" RESET L"] Affichage de l'icone");
    if(m_checkIconState) {
        wxBitmap bitmap(icon);
        if (bitmap.IsOk()) {
            splash = new wxSplashScreen(bitmap,wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT, 1000, NULL, -1,  wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE|wxSTAY_ON_TOP);
        }
        for(int i=0; i<4; i++) {
            wxYield();
            wxMilliSleep(250);
        }
        splash->Close();
    }
    frame = new MainFrame();
    frame->Show(true);
    return true;
}

int IpmsApp::OnExit() {
    delete m_checker;
    return 0;
}