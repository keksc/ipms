#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <wx/wx.h>
#include <wx/file.h>
#include <wx/socket.h>

#include "NouveauContact.hpp"
#include "Discussion.hpp"

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void OnNouveauContact(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnImportContact(wxCommandEvent& event);
    void OnListBoxEvent(wxCommandEvent& event);
    void AfficherMenuPrincipal(wxCommandEvent& event);
    void OnResize(wxSizeEvent& event);
    void AfficherMenuPrincipal();
    void CreateConfFolders();
    wxArrayInt GetWinSize();

    void Connect(wxCommandEvent& event);
    void OnSocketEvent(wxSocketEvent &event);
    void SrvStart();
    void OnServerEvent(wxSocketEvent &event);

    wxMenu *m_menuFile;
    wxMenu *m_menuContact;
    wxMenu *m_menuHelp;
    wxMenuBar *m_menuBar;

    wxMenu *m_menuSocket;

    wxGridSizer *m_sizerMenuPrincipal;
    wxStaticText *m_infoNoCtc;

    wxSocketServer *m_server;

    wxListBox *m_listBox;

    wxVector<DiscussionFrame*> discFrames;

    bool m_printedListBox;
};

#endif // MAIN_HPP_INCLUDED