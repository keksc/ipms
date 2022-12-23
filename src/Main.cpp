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
#include <wx/stdpaths.h>

#include <filesystem>

#include "Main.hpp"
#include "NouveauContact.hpp"

#include "settings.hpp"

MainFrame::MainFrame()
    : wxFrame(NULL, IDs::Fenetre, "ipms", wxPoint(-1, -1), wxSize(WINDOW_WIDTH, WINDOW_HEIGHT)) {

    //wxLogNull no_log; pour supprimer les logs localement, jusqu'à la fin du bloc
    /*wxString nomFichierConfig = wxString(std::filesystem::current_path().string()) + wxString("/config.ini");
    if(!wxFile::Exists(nomFichierConfig)) {
        wxFile creerFichierConfig;
        if(!creerFichierConfig.Create(nomFichierConfig, false, wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP | wxS_IROTH | wxS_IWOTH)) {
            wxPuts("erreur durant l ecriture du fichier");
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

    SetMenuBar(m_menuBar);
    CreateStatusBar();
    SetStatusText("Bienvenue dans ipms !");

    Bind(wxEVT_MENU, &MainFrame::AfficherMenuPrincipal, this, IDs::ListeContacts);
    Bind(wxEVT_BUTTON, &MainFrame::AfficherMenuPrincipal, this, IDs::ListeContacts);
    Bind(wxEVT_MENU, &MainFrame::OnNouveauContact, this, IDs::NouveauContact);
    Bind(wxEVT_MENU, &MainFrame::OnImportContact, this, IDs::ImportContact);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    //Bind(wxEVT_BUTTON, &MainFrame::OnNouveauContactEntrer, this, IDs::BtnEntrerNouveauContact);
    AfficherMenuPrincipal();
}
void MainFrame::AfficherMenuPrincipal(wxCommandEvent& event) {
    AfficherMenuPrincipal();
}
void MainFrame::AfficherMenuPrincipal() {
    wxString dir_root = wxGetCwd() + wxString("/Contacts/");
    wxArrayString files_result;
    wxDir::GetAllFiles(dir_root,&files_result,wxT("*.ctc"));
    for(size_t i=0; i<files_result.GetCount(); i++) {
        wxPuts(files_result[i]);
    }
    //std::vector<wxButton> *buttons;
    if(files_result.GetCount() < 1) {
        m_sizerMenuPrincipal = new wxGridSizer(1, 1, 1, 1);
        m_infoNoCtc = new wxStaticText(this, -1, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
        m_infoNoCtc->SetLabel(L"Aucun contact pour l'instant, pour en ajouter allez dans Fichier>Contact>Ajouter ou pressez Ctrl+A");
        Unbind(wxEVT_MENU, &MainFrame::AfficherMenuPrincipal, this, IDs::ListeContacts);
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
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    NouveauContactDialog dlgNouveauContact;
    if (dlgNouveauContact.ShowModal() == wxID_OK) {
        wxRegEx ipValide(wxString("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$"));
        if(ipValide.Matches(dlgNouveauContact.GetIP())) {
            if(not(dlgNouveauContact.GetNom().IsEmpty() | dlgNouveauContact.GetPrenom().IsEmpty())) {
                if(!wxDirExists(pathinfo.GetUserDataDir())) {
                    wxPuts(wxString(L"Dossier de configuration inexistant. Creation à ") + pathinfo.GetUserDataDir());
                    wxMkDir(pathinfo.GetUserDataDir(), 0777);
                }
                if(!wxDirExists(pathinfo.GetUserDataDir() + "/Contacts/")) {
                    wxPuts(wxString(L"Dossier de contacts dans le dossier de configuration inexistant. Creation à ") + pathinfo.GetUserDataDir());
                    wxMkDir(pathinfo.GetUserDataDir() + "/Contacts/", 0777);
                }
                wxString nomFichier(pathinfo.GetUserDataDir() + wxString("/Contacts/") + dlgNouveauContact.GetIP() + wxString(".ctc"));
                //std::filesystem::path cwd = std::filesystem::current_path();
                //wxString szUserName = wxGetTextFromUser("Please enter your name in the field below.", "Create a New User", wxEmptyString, this);
                wxFile fichierNouveauContact;
                if(!wxFile::Exists(nomFichier)) {
                    if(fichierNouveauContact.Create(nomFichier, false, wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP | wxS_IROTH | wxS_IWOTH)) {
                        fichierNouveauContact.Write(dlgNouveauContact.GetNom() + wxString("\n") + dlgNouveauContact.GetPrenom());
                        wxPuts(wxString("Fichier enregistre avec succes en tant que ") + nomFichier);
                    } else {
                        wxPuts("erreur durant l ecriture du fichier");
                        wxMessageBox(L"erreur durant la création du fichier");
                    }
                } else {
                    wxMessageBox(L"Ce contact existe déjà !", "Info", wxICON_INFORMATION);
                }
            } else {
                wxMessageBox(L"Veuillez renseigner les champs de nom et de prénom", "Erreur", wxICON_INFORMATION);
            }
        } else {
            wxMessageBox("Veuillez renseigner une adresse IP valide", "Erreur", wxICON_INFORMATION);
        }
    }
}

void MainFrame::OnImportContact(wxCommandEvent& event) {
    m_infoNoCtc->SetLabel("");
    wxPuts("menu import contact affiche");
    wxString source = wxFileSelector("Ouvrir", wxGetUserHome(), "", "", "Fiches de contact (*.ctc)|*.ctc");
    if(!source.empty()) {
        wxCopyFile(source, wxGetCwd() + wxString("/Contacts/") + wxFileNameFromPath(source));
        wxMessageDialog *dial = new wxMessageDialog(this, "Voulez-vous supprimer le fichier source ?", L"Fichier bien importé !", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
        int retour = dial->ShowModal();
        dial->Destroy();
        if(retour == wxID_YES) {
            wxRemoveFile(source);
            wxMessageBox(L"Fichier source bien supprimé !", "Info", wxICON_INFORMATION);
        }
    }
    AfficherMenuPrincipal();

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