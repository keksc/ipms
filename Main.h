#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <wx/wx.h>
#include <wx/file.h>
#include <map>

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    void OnNouveauContact(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNouveauContactEntrer(wxCommandEvent& event);
    void OnImportContact(wxCommandEvent& event);

    wxMenu *m_menuFile;
    wxMenu *m_menuContact;
    wxMenu *m_menuHelp;
    wxMenuBar *m_menuBar;

    wxGridSizer *m_sizerNouveauContact;
    wxButton *m_btnEntrerNouveauContact;
    wxTextCtrl *m_nomNouveauContact;
    wxTextCtrl *m_prenomNouveauContact;
    wxTextCtrl *m_IPNouveauContact;

    bool printed_menuNouveauContact;

    std::map<wxString, wxString> config;
};

#endif // MAIN_H_INCLUDED