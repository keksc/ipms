//TODO: ajouter un contrôle de la validité des addresses IP à la lecture du dossier dans AfficherMenuPrincipal
//TODO: fix bug quand je lance 2 fois fonction OnNouveauContact l'app se ferme

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/file.h>
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/regex.h>
#include <wx/log.h>
#include <wx/dir.h>
#include <wx/sckipc.h>

#include <filesystem>

#include "Main.h"

#include "settings.h"

enum IDs {
    Fenetre = 1,
    NouveauContact = 2,
    ImportContact = 3,
    BtnEntrerNouveauContact = 4,
    ListeContacts = 5
};

MainFrame::MainFrame()
    : wxFrame(NULL, IDs::Fenetre, "ipms", wxPoint(-1, -1), wxSize(WINDOW_WIDTH, WINDOW_HEIGHT)) {

    //wxLogNull no_log; pour supprimer les logs localement, jusqu'à la fin du bloc
    /*wxString nomFichierConfig = wxString(std::filesystem::current_path().string()) + wxString("/config.ini");
    if(!wxFile::Exists(nomFichierConfig)) {
        wxFile creerFichierConfig;
        if(!creerFichierConfig.Create(nomFichierConfig, false, wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP | wxS_IROTH | wxS_IWOTH)) {
    #ifdef DEBUG
            wxPuts("erreur durant l ecriture du fichier");
    #endif // DEBUG
            wxMessageBox(L"erreur durant la création du fichier de configuration");
        } else {
            //creerFichierConfig.Write("[Contacts]\nDossier=/home/pboursin/Bureau/ipms/debug/Contacts/");
        }
    }
    wxFileConfig fichierConfig("ipms", "pb", "config.ini", wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    fichierConfig.SetPath(wxT("Contact"));
    fichierConfig.Write(wxT("Server"), "ee");
    fichierConfig.Write(wxT("Database"), "ff");
    //delete file;
    config["dossierContacts"] = wxString("");*/

    SetIcon(wxIcon("res/icon.ico"));
    m_menuFile = new wxMenu;

    SetMinSize(wxSize(WINDOW_WIDTH, WINDOW_HEIGHT));

    m_menuFile->Append(IDs::ListeContacts, "Lister les contacts\tCtrl-L");

    m_menuContact = new wxMenu;
    m_menuContact->Append(IDs::NouveauContact, "Ajouter\tCtrl-A");
    m_menuContact->Append(IDs::ImportContact, "Importer\tCtrl-I");

    m_menuFile->AppendSubMenu(m_menuContact, "Contact");
    m_menuFile->AppendSeparator();
    m_menuFile->Append(wxID_EXIT, "Quitter\tCtrl-Q", "Quitter ce programme");

    m_menuHelp = new wxMenu;
    m_menuHelp->Append(wxID_ABOUT);

    m_menuBar = new wxMenuBar;
    m_menuBar->Append(m_menuFile, "&Fichier");
    m_menuBar->Append(m_menuHelp, "&Aide");

    printed_menuNouveauContact = false;

    SetMenuBar(m_menuBar);
    CreateStatusBar();
    SetStatusText("Bienvenue dans mms pour Linux !");

    Bind(wxEVT_MENU, &MainFrame::AfficherMenuPrincipal, this, IDs::ListeContacts);
    Bind(wxEVT_BUTTON, &MainFrame::AfficherMenuPrincipal, this, IDs::ListeContacts);
    Bind(wxEVT_MENU, &MainFrame::OnNouveauContact, this, IDs::NouveauContact);
    Bind(wxEVT_MENU, &MainFrame::OnImportContact, this, IDs::ImportContact);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_BUTTON, &MainFrame::OnNouveauContactEntrer, this, IDs::BtnEntrerNouveauContact);
    AfficherMenuPrincipal();
}
void MainFrame::AfficherMenuPrincipal(wxCommandEvent& event) {
    AfficherMenuPrincipal();
}
void MainFrame::AfficherMenuPrincipal() {
    if(printed_menuNouveauContact) {
        m_sizerNouveauContact->Clear(true);
        printed_menuNouveauContact = false;
    }
    wxString dir_root = wxString(std::filesystem::current_path().string()) + wxString("/Contacts/");
    wxArrayString files_result;
    wxDir::GetAllFiles(dir_root,&files_result,wxT("*.ctc"));
    for(size_t i=0; i<files_result.GetCount(); i++) {
        wxPuts(files_result[i]);
    }
    //std::vector<wxButton> *buttons;
    if(files_result.GetCount() < 1) {
        m_sizerMenuPrincipal = new wxGridSizer(1, 1, 1, 1);
        m_infoNoCtc = new wxStaticText(this, -1, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
        if(!infoNoCtcAffiche) {
            m_infoNoCtc->SetLabel(L"Aucun contact pour l'instant, pour en ajouter allez dans Fichier/Contact/Ajouter ou pressez Ctrl+A");
            infoNoCtcAffiche = true;
        }
        m_sizerMenuPrincipal->Add(m_infoNoCtc);
    } else {
        m_sizerMenuPrincipal = new wxGridSizer(3, files_result.GetCount(), 0, 0);
        for(size_t i=0; i<files_result.GetCount(); i++) {
            wxFileName filename(files_result[i]);
            m_sizerMenuPrincipal->Add(new wxButton(this, -1, filename.GetName()), 0, 0, wxEXPAND);
            //buttons->push_back(new wxButton(this, -1, filename.GetName()));
            //pour mettre autant d'ids de boutons que je veux faire un tableau de la taille de files_result.GetCount()
            m_sizerMenuPrincipal->AddSpacer(1);
        }
    }
    SetSizer(m_sizerMenuPrincipal);
}

void MainFrame::OnNouveauContact(wxCommandEvent& event) {
    m_infoNoCtc->SetLabel("");//Si j'utilise m_infoNoCtc->Destroy() et que je lance 3 fois OnNouveauContact l'app se ferme, car Destroy() ne peut pas être appelé si m_infoNoCtc n'existe pas
    infoNoCtcAffiche = false;
    if(!printed_menuNouveauContact) {
#ifdef DEBUG
        wxPuts("menu nouveau contact affiche");
#endif // DEBUG
        DoSetClientSize(269, 220);//Sinon l'affichage de la grille bugue
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

        m_sizerNouveauContact->Add(new wxButton(this, IDs::ListeContacts, "Retour"), 0, 0, wxEXPAND);
        m_sizerNouveauContact->Add(new wxButton(this, IDs::BtnEntrerNouveauContact, "Entrer"), 0, 0, wxEXPAND);

        SetSizer(m_sizerNouveauContact);
        DoSetClientSize(WINDOW_WIDTH, WINDOW_HEIGHT);//Sinon l'affichage de la grille bugue

        //Centre();

        printed_menuNouveauContact = true;
    }
}

void MainFrame::OnNouveauContactEntrer(wxCommandEvent& event) {
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
        printed_menuNouveauContact = false;
    } else {
        wxMessageBox(L"Veuillez entrer une addresse IP valide", "Erreur", wxICON_INFORMATION);
    }
    //AfficherMenuPrincipal();bug si je décommente, qd je lance OnNouveauContact() ferme l'app
}

void MainFrame::OnImportContact(wxCommandEvent& event) {
    m_infoNoCtc->SetLabel("");
#ifdef DEBUG
    wxPuts("menu import contact affiche");
#endif // DEBUG
    wxString source = wxFileSelector("Ouvrir", wxGetUserHome(), "", "", "Fiches de contact (*.ctc)|*.ctc");
    if(!source.empty()) {
        wxCopyFile(source, wxString(std::filesystem::current_path().string()) + wxString("/Contacts/") + wxFileNameFromPath(source));
        wxMessageDialog *dial = new wxMessageDialog(this, "Voulez-vous supprimer le fichier source ?", L"Fichier bien importé !", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
        int retour = dial->ShowModal();
        dial->Destroy();
        if(retour == wxID_YES) {
            wxRemoveFile(source);
            wxMessageBox(L"Fichier source bien supprimé !", "Info", wxICON_INFORMATION);
        }
    }


//wxString nomfichier = wxFileSelector("Enregistrer", cwd.string() + "/Contacts/",m_prenomNouveauContact->GetValue() + m_nomNouveauContact->GetValue() ,"", "cpp files (*.cpp;*.h)|*.h;*.cpp|resources files (*.rc)|*.rc", wxFD_SAVE);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("ipms 1.0", L"À propos d'ipms", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::VerifierMessageRecu() {
    wxTCPServer server;
    server.Create("4867");
}//it will be non-void to return to a loop true if a message has been received and false otherwise
