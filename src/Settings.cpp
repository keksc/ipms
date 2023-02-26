#include <wx/wx.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

#include "Main.hpp"

#include "Settings.hpp"

#include "settings.hpp"

SettingsFrame::SettingsFrame(MainFrame* parent)
    : wxFrame(parent, wxID_ANY, "Settings") {
    // Create the checkbox controls
    m_checkUpdates = new wxCheckBox(this, wxID_ANY, L"Vérifier les mises à jour au démarrage");
    m_checkDebug = new wxCheckBox(this, wxID_ANY, L"Ouvrir une console de debug au démarrage");
    m_checkIcon = new wxCheckBox(this, wxID_ANY, L"Montrer l'icône d'ipms au démarrage");

    // Add a sizer to arrange the controls vertically
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_checkUpdates, wxSizerFlags().Border(wxALL, 5));
    sizer->Add(m_checkDebug, wxSizerFlags().Border(wxALL, 5));
    sizer->Add(m_checkIcon, wxSizerFlags().Border(wxALL, 5));

    // Add a "Validate" button to save the settings
    auto button = new wxButton(this, wxID_ANY, "Validate");
    sizer->Add(button, wxSizerFlags().Center().Border(wxALL, 5));

    // Load the preferences from preferences.cfg
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString path(pathinfo.GetUserDataDir() + L"/preferences.cfg");
    wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
    wxFileConfig config("ipms", wxEmptyString, path, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
    config.Read("checkmaj", &m_checkUpdatesState, 1);
    config.Read("console", &m_checkDebugState, 1);
    config.Read("icone", &m_checkIconState, 1);

    // Set the state of the checkboxes based on the preferences
    m_checkUpdates->SetValue(m_checkUpdatesState);
    m_checkDebug->SetValue(m_checkDebugState);
    m_checkIcon->SetValue(m_checkIconState);

    SetSizerAndFit(sizer);

    // Bind the button to the OnValidate method
    button->Bind(wxEVT_BUTTON, &SettingsFrame::OnValidate, this);
    Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::OnClose, this);
}

void SettingsFrame::OnValidate(wxCommandEvent& event) {
    // Save the state of the checkboxes to preferences.cfg
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString path(pathinfo.GetUserDataDir() + L"/preferences.cfg");
    wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
    wxFileConfig config("ipms", wxEmptyString, path, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
    config.Write("checkmaj", m_checkUpdates->GetValue());
    config.Write("console", m_checkDebug->GetValue());
    config.Write("icone", m_checkIcon->GetValue());
    wxPuts(L"[" VRT L"-" RESET L"] Preferences enregistrees");

    // Close the dialog
    wxPuts(L"[" VRT L"-" RESET L"] Cachage de la fenetre de preferences");
    Hide();
}

void SettingsFrame::OnClose(wxCloseEvent& event) {
    wxPuts(L"[" VRT L"-" RESET L"] Reset des checkbox");
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString path(pathinfo.GetUserDataDir() + L"/preferences.cfg");
    wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
    wxFileConfig config("ipms", wxEmptyString, path, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
    config.Read("checkmaj", &m_checkUpdatesState, 1);
    config.Read("console", &m_checkDebugState, 1);
    config.Read("icone", &m_checkIconState, 1);

    m_checkUpdates->SetValue(m_checkUpdatesState);
    m_checkDebug->SetValue(m_checkDebugState);
    m_checkIcon->SetValue(m_checkIconState);
    wxPuts(L"[" VRT L"-" RESET L"] Cachage de la fenetre de preferences");
    Hide();
}