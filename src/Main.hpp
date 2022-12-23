#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <wx/wx.h>
#include <wx/file.h>
#include <wx/socket.h>

#include "NouveauContact.hpp"

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void OnNouveauContact(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnImportContact(wxCommandEvent& event);
    void AfficherMenuPrincipal(wxCommandEvent& event);
    void AfficherMenuPrincipal();
    void VerifierMessageRecu();

    wxMenu *m_menuFile;
    wxMenu *m_menuContact;
    wxMenu *m_menuHelp;
    wxMenuBar *m_menuBar;

    wxGridSizer *m_sizerMenuPrincipal;
    wxStaticText *m_infoNoCtc;

    wxSocketClient *m_sock;
};

#endif // MAIN_H_INCLUDED