#include <wx/wxprec.h>//TODO: if i open in my app instance the settings frame when i close app core dumps segmentation fault, deals with MainFrame::OnClose (should hide instead of destroying ?)

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/stdpaths.h>
#include <wx/textfile.h>

#include "Main.hpp"
#include "Settings.hpp"

#include "settings.hpp"

SettingsFrame::SettingsFrame(MainFrame *mainframe)
    : wxFrame(NULL, wxID_ANY, L"Préférences", wxDefaultPosition, wxSize(wxSize(WINDOW_WIDTH, WINDOW_HEIGHT))), mainframe(mainframe), validated(false) {
    SetMinSize(wxSize(WINDOW_WIDTH, WINDOW_HEIGHT));
    settings.SetCount(3);
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString path(pathinfo.GetUserDataDir() + "/preferences.cfg");
    bool empty;
    wxFile file;
    if(wxFile::Exists(path)) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
        file.Open(path);
        empty = false;
        file.Close();
    } else {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Creation de ") + path);
        file.Create(path);
        empty = true;
        file.Write("1\n0\n1");
        file.Close();
    }
    maj = new wxCheckBox(this, IDs::SettingMaj, L"Vérifier les mises à jour au démarrage");
    cmd = new wxCheckBox(this, IDs::SettingCmd, L"Ouvrir une invite de commandes de debug au démarrage");
    splash = new wxCheckBox(this, IDs::SettingSplashScreen, L"Afficher l'icône d'ipms au démarrage");
    if(!empty) {
        wxTextFile fileread(path);
        fileread.Open();
        if(fileread.GetLineCount() >= 2) {
            if(fileread.GetFirstLine() == "1") {
                maj->SetValue(true);
            } else {
                maj->SetValue(false);
            }
            if(fileread.GetNextLine() == "1") {
                maj->SetValue(true);
            } else {
                maj->SetValue(false);
            }
            if(fileread.GetNextLine() == "1") {
                maj->SetValue(true);
            } else {
                maj->SetValue(false);
            }
        } else {
            wxMessageBox(wxString("Pas assez de lignes dans ") + path);
            wxPuts(L"[" RGE L"X" RESET L"] Pas assez de lignes");
        }
        fileread.Close();
    }

    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(maj);
    sizer->Add(cmd);
    sizer->Add(splash);
    sizer->Add(new wxButton(this, IDs::SettingsValider, "Valider"));
    SetSizer(sizer);
    //Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::OnClose, this);
    Bind(wxEVT_CHECKBOX, &SettingsFrame::OnSetMaj, this, IDs::SettingMaj);
    Bind(wxEVT_CHECKBOX, &SettingsFrame::OnSetCmd, this, IDs::SettingCmd);
    Bind(wxEVT_CHECKBOX, &SettingsFrame::OnSetSplashScreen, this, IDs::SettingSplashScreen);
    Bind(wxEVT_BUTTON, &SettingsFrame::OnValider, this, IDs::SettingsValider);
}

void SettingsFrame::OnClose(wxCloseEvent& event) {
    Destroy();
}

void SettingsFrame::OnSetMaj(wxCommandEvent& event) {
    if(event.IsChecked()) {
        wxPuts(L"[" CYN L"/" RESET L"] Maj");
        settings[0] = 1;
    } else {
        wxPuts("L[ ] Maj");
        settings[0] = 0;
    }
}

void SettingsFrame::OnSetCmd(wxCommandEvent& event) {
    if(event.IsChecked()) {
        wxPuts(L"[" CYN L"/" RESET L"] Cmd");
        settings[1] = 1;
    } else {
        wxPuts("L[ ] Cmd");
        settings[1] = 0;
    }
}

void SettingsFrame::OnSetSplashScreen(wxCommandEvent& event) {
    if(event.IsChecked()) {
        wxPuts(L"[" CYN L"/" RESET L"] Icone d'ipms");
        settings[1] = 1;
    } else {
        wxPuts(L"[ ] Icone d'ipms");
        settings[1] = 0;
    }
}

void SettingsFrame::OnValider(wxCommandEvent& event) {
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString path = pathinfo.GetUserDataDir();
    wxMessageDialog *dial = new wxMessageDialog(this, L"Voulez-vous enregistrer ces préférences ?", L"Modification des préférences", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    int retour = dial->ShowModal();
    dial->Destroy();
    if(retour == wxID_YES) {
        wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
        wxString path(pathinfo.GetUserDataDir() + "/preferences.cfg");
        if(wxFile::Exists(path)) {
            wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
            wxFile file(path, wxFile::write);
            file.Write("", 0);
            if(settings[0] == 1) {
                file.Write("1\n");
            } else {
                file.Write("0\n");
            }
            if(settings[1] == 1) {
                file.Write("1\n");
            } else {
                file.Write("0\n");
            }
            if(settings[2] == 1) {
                file.Write("1");
            } else {
                file.Write("0");
            }
            file.Close();
        } else {
            wxMessageBox(L"Fichier inexistant, veuillez quittez et réentrer dans la fenêtre de préférences.");
        }
        wxMessageBox(L"Préférences bien sauvegardées !", L"Info", wxICON_INFORMATION);
    }
    validated = true;
}