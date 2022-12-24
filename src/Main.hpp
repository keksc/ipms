#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

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

    void Connect(wxCommandEvent& event);
    void OnSocketEvent(wxSocketEvent &event);
    void SrvStart(wxCommandEvent& event);
    void OnServerEvent(wxSocketEvent &event);

    wxMenu *m_menuFile;
    wxMenu *m_menuContact;
    wxMenu *m_menuHelp;
    wxMenuBar *m_menuBar;

    wxMenu *m_menuSocket;

    wxGridSizer *m_sizerMenuPrincipal;
    wxStaticText *m_infoNoCtc;

    wxSocketServer *m_server;
};

#endif // MAIN_HPP_INCLUDED