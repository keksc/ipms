//TODO: ajouter un contrôle de la validité des addresses IP à la lecture du dossier dans AfficherMenuPrincipal

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/file.h>
#include <wx/regex.h>
#include <wx/dir.h>

#include <filesystem>

#include "EditContact.hpp"

#include "settings.hpp"

EditContactDialog::EditContactDialog()
    : wxDialog(NULL, wxID_ANY, "Entrez les infos", wxDefaultPosition, wxSize(WINDOW_WIDTH, WINDOW_HEIGHT)) {
    wxPuts(L"[" VRT L"-" RESET L"] Fenetre editer contact affichee");
    nom = new wxTextCtrl(this, wxID_ANY);
    prenom = new wxTextCtrl(this, wxID_ANY);
    ip = new wxTextCtrl(this, wxID_ANY);
    wxButton *boutonOk = new wxButton(this, wxID_OK,"Valider");
    wxButton *boutonAnnuler = new wxButton(this, wxID_CANCEL,"Annuler");

    sizer1 = new wxBoxSizer(wxVERTICAL);
    sizer1->Add(new wxStaticText(this, wxID_ANY, "Nom"));
    sizer1->Add(nom,0,wxALL,5);
    sizer1->Add(new wxStaticText(this, wxID_ANY, "Prenom"));
    sizer1->Add(prenom,0,wxALL,5);
    sizer1->Add(new wxStaticText(this, wxID_ANY, "IP"));
    sizer1->Add(ip,0,wxALL,5);

    sizer2 = new wxBoxSizer( wxHORIZONTAL );
    sizer2->Add(boutonAnnuler,0,wxBOTTOM | wxLEFT | wxRIGHT,5);
    sizer2->Add(boutonOk,0,wxBOTTOM | wxLEFT | wxRIGHT,5);

    Psizer = new wxBoxSizer( wxVERTICAL );
    Psizer->Add(sizer1,3,wxALIGN_CENTER);
    Psizer->Add(sizer2,1,wxALIGN_CENTER);
    SetSizer(Psizer);

    Center();
}
wxString EditContactDialog::GetNom() {
    return nom->GetValue();
}
wxString EditContactDialog::GetPrenom() {
    return prenom->GetValue();
}
wxString EditContactDialog::GetIP() {
    return ip->GetValue();
}