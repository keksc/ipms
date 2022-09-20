#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/file.h>
#include <wx/fileconf.h>
#include <wx/regex.h>
#include <wx/log.h>
#include <wx/socket.h>
#include <filesystem>
#include <map>

#include "Main.h"

#inlude "settings.h"




enum IDs
{
    Fenetre = 1,
    NouveauContact = 2,
    ImportContact = 3,
    BtnEntrerNouveauContact = 4
};

MainFrame::MainFrame()
    : wxFrame(NULL, IDs::Fenetre, "mms", wxPoint(-1, -1), wxSize(WINDOW_WIDTH, WINDOW_HEIGHT))
{

    //wxLogNull no_log; pour supprimer les logs localement, jusqu'à la fin du bloc
    wxString nomFichierConfig = wxString(std::filesystem::current_path().string()) + wxString("/config.ini");
    if(!wxFile::Exists(nomFichierConfig))
    {
        wxFile creerFichierConfig;
        if(!creerFichierConfig.Create(nomFichierConfig, false, wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP | wxS_IROTH | wxS_IWOTH))
        {
#ifdef DEBUG
            wxPuts("erreur durant l ecriture du fichier");
#endif // DEBUG
            wxMessageBox(L"erreur durant la création du fichier de configuration");
        }
        else
        {
            //creerFichierConfig.Write("[Contacts]\nDossier=/home/pboursin/Bureau/ipms/debug/Contacts/");
        }
    }
    wxFileConfig fichierConfig("ipms", "pb", "config.ini", wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
 
    fichierConfig.SetPath(wxT("Contactw"));
    fichierConfig.Write(wxT("Server"), "ee");
    fichierConfig.Write(wxT("Database"), "ff");
    //delete file;
    config["dossierContacts"] = wxString("");

    SetIcon(wxIcon("res/icon.ico"));
    m_menuFile = new wxMenu;

    SetMinSize(wxSize(WINDOW_WIDTH, WINDOW_HEIGHT));

    m_menuContact = new wxMenu;
    m_menuContact->Append(IDs::NouveauContact, "Ajouter\tCtrl-A");
    m_menuContact->Append(IDs::ImportContact, "Importer\tCtrl-I");

    m_menuFile->AppendSubMenu(m_menuContact, "&Contact");
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

    Bind(wxEVT_MENU, &MainFrame::OnNouveauContact, this, IDs::NouveauContact);
    Bind(wxEVT_MENU, &MainFrame::OnImportContact, this, IDs::ImportContact);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_BUTTON, &MainFrame::OnNouveauContactEntrer, this, IDs::BtnEntrerNouveauContact);
}

void MainFrame::OnNouveauContact(wxCommandEvent& event)
{
    if(!printed_menuNouveauContact)
    {
#ifdef DEBUG
        wxPuts("menu nouveau contact affiche");
#endif // DEBUG
        DoSetClientSize(269, 220);//Sinon l'affichage de la grille bugue
        m_sizerNouveauContact = new wxGridSizer(4, 2, 5, 10);

        m_nomNouveauContact = new wxTextCtrl(this, -1);
        m_prenomNouveauContact = new wxTextCtrl(this, -1);
        m_IPNouveauContact = new wxTextCtrl(this, -1);

        m_sizerNouveauContact->Add(new wxStaticText(this, -1, "Nom du contact"), 0, wxEXPAND);
        m_sizerNouveauContact->Add(m_nomNouveauContact, 0, wxEXPAND);
        m_sizerNouveauContact->Add(new wxStaticText(this, -1, L"Prénom du contact"), 0, wxEXPAND);
        m_sizerNouveauContact->Add(m_prenomNouveauContact, 0, wxEXPAND);
        m_sizerNouveauContact->Add(new wxStaticText(this, -1, "IP du contact"), 0, wxEXPAND);
        m_sizerNouveauContact->Add(m_IPNouveauContact, 0, wxEXPAND);
        m_sizerNouveauContact->AddSpacer(1);
        m_sizerNouveauContact->Add(new wxButton(this, IDs::BtnEntrerNouveauContact, "Entrer"), 0, wxEXPAND);

        SetSizer(m_sizerNouveauContact);
        DoSetClientSize(WINDOW_WIDTH, WINDOW_HEIGHT);//Sinon l'affichage de la grille bugue

        //Centre();

        printed_menuNouveauContact = true;
    }
}

void MainFrame::OnNouveauContactEntrer(wxCommandEvent& event)
{
    wxRegEx ipValide(wxString("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$"));
    if(ipValide.Matches(m_IPNouveauContact->GetValue()))
    {
        printed_menuNouveauContact = false;
        wxString nomFichier(wxString(std::filesystem::current_path().string()) + wxString("/Contacts/") + m_IPNouveauContact->GetValue() + wxString(".ctc"));
        //std::filesystem::path cwd = std::filesystem::current_path();
        //wxString szUserName = wxGetTextFromUser("Please enter your name in the field below.", "Create a New User", wxEmptyString, this);
        wxFile fichierNouveauContact;
        if(!wxFile::Exists(nomFichier))
        {
            if(fichierNouveauContact.Create(nomFichier, false, wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP | wxS_IROTH | wxS_IWOTH))
            {
                fichierNouveauContact.Write(m_nomNouveauContact->GetValue() + wxString("\n") + m_prenomNouveauContact->GetValue());
#ifdef DEBUG
                wxPuts(wxString("Fichier enregistre avec succes en tant que ") + nomFichier);
#endif // DEBUG
            }
            else
            {
#ifdef DEBUG
                wxPuts("erreur durant l ecriture du fichier");
#endif // DEBUG
                wxMessageBox(L"erreur durant la création du fichier");
            }
        }
        else
        {
            wxMessageBox(L"Ce contact existe déjà !", "Info", wxICON_INFORMATION);
        }
        m_sizerNouveauContact->Clear(true);
    }
    else
    {
        wxMessageBox(L"Veuillez entrer une addresse IP valide", "Erreur", wxICON_INFORMATION);
    }
}

void MainFrame::OnImportContact(wxCommandEvent& event)
{
#ifdef DEBUG
    wxPuts("menu import contact affiche");
#endif // DEBUG
    wxString source = wxFileSelector("Ouvrir", wxGetUserHome(), "", "", "Fiches de contact (*.ctc)|*.ctc");
    if(!source.empty())
    {
        wxCopyFile(source, wxString(std::filesystem::current_path().string()) + wxString("/Contacts/") + wxFileNameFromPath(source));
        wxMessageDialog *dial = new wxMessageDialog(this, "Voulez-vous supprimer le fichier source ?", L"Fichier bien importé !", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
        int retour = dial->ShowModal();
        dial->Destroy();
        if(retour == wxID_YES)
        {
            wxRemoveFile(source);
            wxMessageBox(L"Fichier source bien supprimé !", "Info", wxICON_INFORMATION);
        }
    }


//wxString nomfichier = wxFileSelector("Enregistrer", cwd.string() + "/Contacts/",m_prenomNouveauContact->GetValue() + m_nomNouveauContact->GetValue() ,"", "cpp files (*.cpp;*.h)|*.h;*.cpp|resources files (*.rc)|*.rc", wxFD_SAVE);
}

void MainFrame::OnEnvoi(wxCommandEvent& event)
{
    wxTcpServer server();
    server.create("ipms server");
    wxIPV4address ip();
    ip.Service(PORT);
    
}

void MainFrame::OnMessageRecu(wxCommandEvent& event)
{
    wxTcpClient listener();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("mms 1.0", "A propos de mms", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
