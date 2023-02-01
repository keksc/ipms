#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/sckipc.h>

#include "Main.hpp"

#include "settings.hpp"

void MainFrame::Connect() {
    wxIPV4address adr;
    adr.Hostname(ipToSend->c_str());
    delete ipToSend;
    adr.Service(PORT);

    Socket = new wxSocketClient();

    Socket->SetEventHandler(*this, IDs::Socket);
    Socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    Socket->Notify(TRUE);

    Socket->Connect(adr, false);
    wxPuts("Connexion au serveur...");

    return;
}
void MainFrame::OnSocketEvent(wxSocketEvent &event) {
    wxSocketBase *Sock = event.GetSocket();
    char buffer[1024*8];

    switch(event.GetSocketEvent()) {
    case wxSOCKET_CONNECTION: {
        wxPuts("Connexion reussie");
        Sock->Write(dataToSend->c_str(), dataToSend->length());
        wxPuts(wxString("    Envoye ") + *dataToSend);
        delete dataToSend;

        break;
    }

    case wxSOCKET_INPUT: {
        wxPuts("Donnees recues");
        Sock->Read(buffer, sizeof(buffer));
        wxPuts(wxString("    Recu ") + wxString(buffer));
        wxString strBuf(buffer);
        MessageRecu(&strBuf);
        break;
    }

    case wxSOCKET_LOST: {
        wxPuts("Connexion perdue");
        Sock->Destroy();
        break;
    }

    default: {
        break;
    }
    }
}

void MainFrame::SrvStart() {
    wxIPV4address adr;
    adr.Service(PORT);

    m_server = new wxSocketServer(adr);


    m_server->SetEventHandler(*this, IDs::Server);
    m_server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_server->Notify(true);

    if(!m_server->Ok()) {
        wxMessageBox("Le serveur na pas pu demarrer\nAttendez un peu puis redemarrez ipms\n");

        return;
    }

    wxPuts(wxT("Le serveur a demarre\n"));

    return;
}
void MainFrame::OnServerEvent(wxSocketEvent &event) {
    switch (event.GetSocketEvent()) {
    case wxSOCKET_CONNECTION: {
        wxPuts("Connexion entrante\n");

        wxSocketBase *Sock = m_server->Accept(true);

        if (Sock == NULL) {
            wxPuts("Erreur: na pas pu accepter la connexion\n");
            return;
        }

        wxPuts("    Connexion acceptee\n");
        Sock->SetEventHandler(*this, IDs::SrvSock);
        Sock->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_OUTPUT_FLAG | wxSOCKET_LOST_FLAG);
        Sock->Notify(true);
        break;
    }
    default: {
        break;
    }
    }
}