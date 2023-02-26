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
    wxPuts(L"[" VRT L"-" RESET L"] Connexion au serveur...");

    return;
}
void MainFrame::OnSocketEvent(wxSocketEvent &event) {
    wxSocketBase *Sock = event.GetSocket();
    char buffer[1024*8];

    switch(event.GetSocketEvent()) {
    case wxSOCKET_CONNECTION: {
        wxPuts(L"[" VRT L"-" RESET L"] Connexion reussie");
        wxCharBuffer buf = dataToSend->To8BitData();
        Sock->Write(buf, buf.length());
        wxPuts(wxString(L"[" VRT L"-" RESET L"]    Envoye ") + *dataToSend);
        delete dataToSend;

        break;
    }

    case wxSOCKET_INPUT: {
        wxPuts(L"[" VRT L"-" RESET L"] Donnees recues");
        Sock->Read(buffer, sizeof(buffer));
        wxPuts(wxString(L"[" VRT L"-" RESET L"]    Recu ") + buffer);
        MessageRecu(new wxString(buffer));
        break;
    }

    case wxSOCKET_LOST: {
        wxPuts(L"[" RGE L"X" RESET L"] Connexion perdue");
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
        wxMessageBox(L"Le serveur na pas pu démarrer\nAttendez un peu puis redémarrez ipms");

        return;
    }

    wxPuts(L"[" VRT L"-" RESET L"] Le serveur a demarre");
}
void MainFrame::OnServerEvent(wxSocketEvent &event) {
    switch (event.GetSocketEvent()) {
    case wxSOCKET_CONNECTION: {
        wxPuts(L"[" VRT L"-" RESET L"] Connexion entrante");

        wxSocketBase *Sock = m_server->Accept(true);

        if (Sock == NULL) {
            wxPuts(L"[" RGE L"X" RESET L"] N'a pas pu accepter la connexion entrante");
            return;
        }

        wxPuts(L"[" VRT L"-" RESET L"]    Connexion acceptee\n");
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