#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <wx/wx.h>
#include <wx/file.h>
#include <wx/socket.h>
#include "wx/richtext/richtextctrl.h"

#include "NouveauContact.hpp"
class DiscussionFrame;
class SettingsFrame;

class MainFrame : public wxFrame {
public:
    MainFrame();
    void Envoyer(wxRichTextCtrl *text, wxString ip);
private:
    void OnNouveauContact(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnImportContact(wxCommandEvent& event);
    void OnListBoxEvent(wxCommandEvent& event);
    void OnPreferences(wxCommandEvent& event);
    void AfficherMenuPrincipal();
    void CreateConfFolders();
    void MessageRecu(wxString *buffer);

    void Connect();
    void OnSocketEvent(wxSocketEvent& event);
    void SrvStart();
    void OnServerEvent(wxSocketEvent& event);

    wxMenu *m_menuFile;
    wxMenu *m_menuContact;
    wxMenu *m_menuHelp;
    wxMenuBar *m_menuBar;
    wxString *myPublicAddr;
    wxString *dataToSend;
    wxString *ipToSend;

    wxSocketClient *Socket;

    wxMenu *m_menuSocket;

    wxBoxSizer *m_sizerMenuPrincipal;
    wxStaticText *m_infoNoCtc;

    wxSocketServer *m_server;

    wxListBox *m_listBox;

    wxVector<DiscussionFrame*> discFrames;

    bool m_printedListBox;

    SettingsFrame *settingsFrame;
};

#endif // MAIN_HPP_INCLUDED