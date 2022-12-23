//TODO: ajouter un contrôle de la validité des addresses IP à la lecture du dossier dans AfficherMenuPrincipal
//TODO: fix bug quand je lance 2 fois fonction OnNouveauContact l'app se ferme

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/file.h>
#include <wx/regex.h>
#include <wx/dir.h>

#include <filesystem>

#include "NouveauContact.h"

#include "settings.h"

NouveauContactDialog::NouveauContactDialog()
    : wxDialog(NULL, -1, "Entrez les infos", wxDefaultPosition, wxSize(250, 230)) {
    wxPuts("menu nouveau contact affiche");
    /*//DoSetClientSize(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);//Sinon l'affichage de la grille bugue
    m_sizerNouveauContact = new wxGridSizer(4, 2, 5, 10);

    m_nomNouveauContact = new wxTextCtrl(this, -1);
    m_prenomNouveauContact = new wxTextCtrl(this, -1);
    m_IPNouveauContact = new wxTextCtrl(this, -1);

    m_sizerNouveauContact->Add(new wxStaticText(this, -1, "Nom du contact"), 0, 0, wxEXPAND);
    m_sizerNouveauContact->Add(m_nomNouveauContact, 0, wxEXPAND);

    m_sizerNouveauContact->Add(new wxStaticText(this, -1, L"Prénom du contact"), 0, 0, wxEXPAND);
    m_sizerNouveauContact->Add(m_prenomNouveauContact, 0, wxEXPAND);

    m_sizerNouveauContact->Add(new wxStaticText(this, -1, "IP du contact"), 0, 0, wxEXPAND);
    m_sizerNouveauContact->Add(m_IPNouveauContact, 0, wxEXPAND);

    m_sizerNouveauContact->Add(new wxButton(this, wxID_OK, "Annuler"), 0, 0, wxEXPAND);
    m_sizerNouveauContact->Add(new wxButton(this, wxID_CANCEL, "Valider"), 0, 0, wxEXPAND);
    Centre();
    SetSizer(m_sizerNouveauContact);
    //DoSetClientSize(WINDOW_WIDTH, WINDOW_HEIGHT);//Sinon l'affichage de la grille bugue*/
    nom = new wxTextCtrl(this,-1);
    prenom = new wxTextCtrl(this,-1);
    ip = new wxTextCtrl(this,-1);
    wxButton *boutonOk = new wxButton(this,wxID_OK,"Valider");
    wxButton *boutonAnnuler = new wxButton(this,wxID_CANCEL,"Annuler");

    wxGridSizer *sizer1 = new wxGridSizer( 3, 2, 2, 2 );
    sizer1->Add(-1,25);
    sizer1->Add(new wxStaticText(this, -1, "Nom"));
    sizer1->Add(nom,0,wxALL,5);
    sizer1->Add(new wxStaticText(this, -1, "Prenom"));
    sizer1->Add(prenom,0,wxALL,5);
    sizer1->Add(new wxStaticText(this, -1, "IP"));
    sizer1->Add(ip,0,wxALL,5);

    wxBoxSizer *sizer2 = new wxBoxSizer( wxHORIZONTAL );
    sizer2->Add(boutonAnnuler,0,wxBOTTOM | wxLEFT | wxRIGHT,5);
    sizer2->Add(boutonOk,0,wxBOTTOM | wxLEFT | wxRIGHT,5);

    wxBoxSizer *Psizer = new wxBoxSizer( wxVERTICAL );
    Psizer->Add(sizer1,3,wxALIGN_CENTER);
    Psizer->Add(sizer2,1,wxALIGN_CENTER);
    SetSizer(Psizer);
}
wxString NouveauContactDialog::GetNom() {
    return nom->GetValue();
}
wxString NouveauContactDialog::GetPrenom() {
    return prenom->GetValue();
}
wxString NouveauContactDialog::GetIP() {
    return ip->GetValue();
}

/*void NouveauContactDialog::OnEntrer(wxCommandEvent& event) {
    wxRegEx ipValide(wxString("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$"));
    if(ipValide.Matches(m_IPNouveauContact->GetValue())) {
        wxString nomFichier(wxString(std::filesystem::current_path().string()) + wxString("/Contacts/") + m_IPNouveauContact->GetValue() + wxString(".ctc"));
        //std::filesystem::path cwd = std::filesystem::current_path();
        //wxString szUserName = wxGetTextFromUser("Please enter your name in the field below.", "Create a New User", wxEmptyString, this);
        wxFile fichierNouveauContact;
        if(!wxFile::Exists(nomFichier)) {
            if(fichierNouveauContact.Create(nomFichier, false, wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP | wxS_IROTH | wxS_IWOTH)) {
                fichierNouveauContact.Write(m_nomNouveauContact->GetValue() + wxString("\n") + m_prenomNouveauContact->GetValue());
#ifdef DEBUG
                wxPuts(wxString("Fichier enregistre avec succes en tant que ") + nomFichier);
#endif // DEBUG
            } else {
#ifdef DEBUG
                wxPuts("erreur durant l ecriture du fichier");
#endif // DEBUG
                wxMessageBox(L"erreur durant la création du fichier");
            }
        } else {
            wxMessageBox(L"Ce contact existe déjà !", "Info", wxICON_INFORMATION);
        }
        m_sizerNouveauContact->Clear(true);
    } else {
        wxMessageBox(L"Veuillez entrer une addresse IP valide", "Erreur", wxICON_INFORMATION);
    }
    //AfficherMenuPrincipal();bug si je décommente, qd je lance OnNouveauContact() ferme l'app
}
*/