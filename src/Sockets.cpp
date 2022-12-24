#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/sckipc.h>

#include "Main.hpp"

#include "settings.hpp"

void MainFrame::Connect(wxCommandEvent& event) {
    wxPuts("Connecting to the server...");

    //Connecting to the server
    wxIPV4address adr;
    adr.Hostname("localhost");
    adr.Service(3000);

    //Create the socket
    wxSocketClient *Socket = new wxSocketClient();

    Socket->SetEventHandler(*this, IDs::Socket);
    Socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    Socket->Notify(TRUE);

    Socket->Connect(adr, false);
    wxPuts("Connecting...");

    return;
}
void MainFrame::OnSocketEvent(wxSocketEvent &event) {
    wxSocketBase *Sock = event.GetSocket();

    char buffer[10];

    switch(event.GetSocketEvent()) {
    case wxSOCKET_CONNECTION: {
        wxPuts("Connexion reussie");

        char mychar = '0';

        for (int i=0; i<10; ++i) {
            buffer[i] = mychar++;
        }

        Sock->Write(buffer, sizeof(buffer));

        char cstring[256];
        sprintf(cstring, "%c%c%c%c%c%c%c%c%c%c\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5],
                buffer[6], buffer[7], buffer[8], buffer[9]);
        wxPuts(wxString("    Sent ") + cstring);

        break;
    }

    case wxSOCKET_INPUT: {
        wxPuts("Donnees recues");
        Sock->Read(buffer, sizeof(buffer));

        char cstring[256];
        sprintf(cstring, "%c%c%c%c%c%c%c%c%c%c\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5],
                buffer[6], buffer[7], buffer[8], buffer[9]);
        wxPuts(wxString("    recu ") + cstring);

        break;
    }

    case wxSOCKET_LOST: {
        wxPuts("connexion perdue");
        Sock->Destroy();
        break;
    }

    default: {
        break;
    }
    }
}

void MainFrame::SrvStart(wxCommandEvent& event) {
    wxIPV4address adr;
    adr.Service(3000);

    m_server = new wxSocketServer(adr);


    m_server->SetEventHandler(*this, IDs::Server);
    m_server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_server->Notify(true);

    if(!m_server->Ok()) {
        wxPuts(wxT("Could not start server :(\n"));
        return;
    }

    wxPuts(wxT("Server started\n"));

    return;
}
void MainFrame::OnServerEvent(wxSocketEvent &event) {
    switch (event.GetSocketEvent()) {
    case wxSOCKET_CONNECTION: {
        wxPuts("OnServerEvent : wxSOCKET_CONNECTION\n");

        wxSocketBase *Sock = m_server->Accept(true);

        if (Sock == NULL) {
            wxPuts("Failed to accept incoming connection\n");
            return;
        }

        wxPuts("    Accepted Connection\n");
        Sock->SetEventHandler(*this, IDs::SrvSock);
        Sock->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_OUTPUT_FLAG | wxSOCKET_LOST_FLAG);
        Sock->Notify(true);
        break;
    }
    default: {
        wxPuts("OnServerEvent : unknown event\n");
        break;
    }
    }
}