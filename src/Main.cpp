#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/file.h>
#include <wx/filename.h>
#include <wx/regex.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include "wx/richtext/richtextctrl.h"
#include <wx/textfile.h>

#include "Main.hpp"
#include "NouveauContact.hpp"
#include "Discussion.hpp"
#include "Settings.hpp"

#include "settings.hpp"

#include "res/icon.xpm"

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, WINDOW_NAME, wxDefaultPosition, wxSize(WINDOW_WIDTH, WINDOW_HEIGHT)), m_printedListBox(false), settingsFrame(nullptr) {

    //wxLogNull no_log; pour supprimer les logs localement, jusqu'à la fin du bloc
    wxIPV4address remote;
    remote.Hostname("www.google.com");
    remote.Service(80);

    wxIPV4address local;

    wxSocketClient client;
    if(client.Connect(remote)) client.GetLocal(local);

    myPublicAddr = new wxString(local.IPAddress());
    wxPuts(wxString(L"[" BLU L"?" RESET L"] Votre adresse IP : ") + *myPublicAddr);

    SetIcon(wxIcon(icon));
    m_menuFile = new wxMenu;

    SetMinSize(wxSize(WINDOW_WIDTH, WINDOW_HEIGHT));

    m_menuContact = new wxMenu;
    m_menuContact->Append(IDs::NouveauContact, L"Ajouter\tCtrl-A");
    m_menuContact->Append(IDs::ImportContact, L"Importer\tCtrl-I");

    m_menuFile->AppendSubMenu(m_menuContact, L"Contact");
    m_menuFile->AppendSeparator();
    m_menuFile->Append(IDs::Preferences, L"Préférences\tCtrl-S");
    m_menuFile->Append(wxID_EXIT, L"Quitter\tCtrl-Q");

    m_menuHelp = new wxMenu;
    m_menuHelp->Append(wxID_ABOUT, L"A propos");


    m_menuBar = new wxMenuBar;
    m_menuBar->Append(m_menuFile, "&Fichier");
    m_menuBar->Append(m_menuHelp, "&Aide");

    SetMenuBar(m_menuBar);
    CreateStatusBar();
    SetStatusText("Bienvenue dans ipms !");

    Bind(wxEVT_MENU, &MainFrame::OnNouveauContact, this, IDs::NouveauContact);
    Bind(wxEVT_MENU, &MainFrame::OnImportContact, this, IDs::ImportContact);
    Bind(wxEVT_MENU, &MainFrame::OnPreferences, this, IDs::Preferences);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    //socket stuff
    Bind(wxEVT_SOCKET, &MainFrame::OnSocketEvent, this, IDs::Socket);
    Bind(wxEVT_SOCKET, &MainFrame::OnServerEvent, this, IDs::Server);
    Bind(wxEVT_SOCKET, &MainFrame::OnSocketEvent, this, IDs::SrvSock);
    Bind(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, &MainFrame::OnListBoxEvent, this, IDs::ListBox);
    m_sizerMenuPrincipal = new wxBoxSizer(wxHORIZONTAL);

    SrvStart();
    AfficherMenuPrincipal();
}

void MainFrame::AfficherMenuPrincipal() {
    m_sizerMenuPrincipal->Clear();
    CreateConfFolders();
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString usrDataDir(pathinfo.GetUserDataDir());
    wxDir dir(usrDataDir + "/Contacts/");
    if(!dir.HasFiles("*.ctc")) {
        wxMessageBox(L"Aucun contact pour l'instant, pour en ajouter allez dans Fichier>Contact>Ajouter ou pressez Ctrl+A");
    } else {
        m_listBox = new wxListBox(this, IDs::ListBox/*, wxDefaultPosition, wxSize(GetWinSize()[0], GetWinSize()[1])*/);
        m_printedListBox = true;
        wxFileName name;
        wxString filename;
        wxTextFile file;
        bool cont = dir.GetFirst(&filename);
        while (cont) {
            name.Assign(usrDataDir + wxString("/Contacts/") + filename);
            file.Open(usrDataDir + wxString("/Contacts/") + filename);
            m_listBox->Append(file.GetFirstLine() + wxString(" ") + file.GetNextLine() + wxString("|") + name.GetName());
            file.Close();
            cont = dir.GetNext(&filename);
        }
        m_sizerMenuPrincipal->Add(m_listBox, 0, wxEXPAND);
        SetSizer(m_sizerMenuPrincipal);
        m_sizerMenuPrincipal->FitInside(this);
        Layout();
    }
}
void MainFrame::OnNouveauContact(wxCommandEvent& event) {
    if(m_printedListBox) {
        m_listBox->Clear();
        m_printedListBox = false;
    }
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    NouveauContactDialog dlgNouveauContact;
    if (dlgNouveauContact.ShowModal() == wxID_OK) {
        wxRegEx ipValide(wxString("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$"));
        if(ipValide.Matches(dlgNouveauContact.GetIP()) or dlgNouveauContact.GetIP() == "localhost") {
            if(not(dlgNouveauContact.GetNom().IsEmpty() | dlgNouveauContact.GetPrenom().IsEmpty())) {
                CreateConfFolders();
                wxString nomFichier(pathinfo.GetUserDataDir() + wxString("/Contacts/") + dlgNouveauContact.GetIP() + wxString(".ctc"));
                //std::filesystem::path cwd = std::filesystem::current_path();
                //wxString szUserName = wxGetTextFromUser("Please enter your name in the field below.", "Create a New User", wxEmptyString, this);
                wxFile fichierNouveauContact;
                if(!wxFile::Exists(nomFichier)) {
                    if(fichierNouveauContact.Create(nomFichier, false, wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP | wxS_IROTH | wxS_IWOTH)) {
                        fichierNouveauContact.Write(dlgNouveauContact.GetNom() + wxString("\n") + dlgNouveauContact.GetPrenom());
                        wxPuts(wxString(L"[" VRT L"-" RESET L"] Fichier enregistre avec succes en tant que ") + nomFichier);
                        fichierNouveauContact.Close();
                    } else {
                        wxMessageBox(L"Erreur durant la création du fichier");
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
    AfficherMenuPrincipal();
}

void MainFrame::OnImportContact(wxCommandEvent& event) {
    if(m_printedListBox) {
        m_listBox->Clear();
        m_printedListBox = false;
    }
    wxPuts(L"[" VRT L"-" RESET L"] Menu import contact affiche");
    wxString source = wxFileSelector("Ouvrir", wxGetUserHome(), "", "", "Fiches de contact (*.ctc)|*.ctc");
    if(!source.empty()) {
        wxCopyFile(source, wxGetCwd() + wxString("/Contacts/") + wxFileNameFromPath(source));
        wxMessageDialog *dial = new wxMessageDialog(this, L"Voulez-vous supprimer le fichier source ?", L"Fichier bien importé !", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
        int retour = dial->ShowModal();
        dial->Destroy();
        if(retour == wxID_YES) {
            wxRemoveFile(source);
            wxMessageBox(L"Fichier source bien supprimé !", L"Info", wxICON_INFORMATION);
        }
    }
    AfficherMenuPrincipal();

//wxString nomfichier = wxFileSelector("Enregistrer", cwd.string() + "/Contacts/",m_prenomNouveauContact->GetValue() + m_nomNouveauContact->GetValue() ,"", "cpp files (*.cpp;*.h)|*.h;*.cpp|resources files (*.rc)|*.rc", wxFD_SAVE);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("ipms 1.0", L"À propos d'ipms", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnClose(wxCloseEvent& event) {
    for (auto& frame : discFrames) {
        if(frame != nullptr) frame->Destroy();
    }
    if(settingsFrame != nullptr) settingsFrame->Destroy();
    wxPuts(L"[" VRT L"-" RESET L"] Fermeture de la fenetre principale");
    Destroy();
}

void MainFrame::CreateConfFolders() {
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString usrDataDir(pathinfo.GetUserDataDir());
    if(!wxDirExists(usrDataDir)) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Dossier de configuration inexistant. Creation a ") + usrDataDir);
        wxMkDir(usrDataDir, 777);
    }
    if(!wxDirExists(usrDataDir + "/Contacts/")) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Dossier de contacts inexistant. Creation a ") + usrDataDir + "/Contacts/");
        wxMkDir(usrDataDir + "/Contacts/", 777);
    }
    if(!wxDirExists(usrDataDir + "/Messages/")) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Dossier de messages inexistant. Creation a ") + usrDataDir + "/Messages/");
        wxMkDir(usrDataDir + "/Messages/", 777);
    }
}

void MainFrame::OnListBoxEvent(wxCommandEvent& event) {
    wxString name = m_listBox->GetStringSelection().AfterFirst('|') + ".ctc";
    wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + name);

    DiscussionFrame *frame = nullptr;
    for (auto& f : discFrames) {
        if (f->GetIP() + ".ctc" == name) {
            frame = f;
            break;
        }
    }

    // If a frame does not exist, create one
    if (frame == nullptr) {
        wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
        wxString folder(pathinfo.GetUserDataDir() + "/Contacts/");
        wxFileName filename(folder + name);
        wxTextFile file(folder + name);
        file.Open();
        frame = new DiscussionFrame(file.GetFirstLine() + wxString(" ") + file.GetNextLine(), filename.GetName(), this);
        discFrames.push_back(frame);
    } else {
        frame->SetFocus();
    }

    // Show the frame
    frame->Show(true);
}

/*wxArrayInt MainFrame::GetWinSize() {
    int w;
    int h;
    DoGetClientSize(&w, &h);
    wxArrayInt size;
    size.Add(w);
    size.Add(h);
    return size;
}*/

void MainFrame::Envoyer(wxRichTextCtrl *text, wxString ip) {
    dataToSend = new wxString(*myPublicAddr + wxString(L"^") + text->GetValue());
    ipToSend = new wxString(ip);
    Connect();
}

void MainFrame::MessageRecu(wxString *buffer) {
    wxString ip(buffer->BeforeFirst(L'^'));
    wxString message(buffer->AfterFirst(L'^'));
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString folder(pathinfo.GetUserDataDir() + "/Contacts/");
    wxString path(folder + ip + ".ctc");
    wxFileName filename(path);
    wxTextFile file(path);
    if(!file.Exists()) {
        file.Create();
    } else {
        file.Open();
    }
    wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
    wxString framename;
    if (file.GetLineCount() >= 2) {
        framename = file.GetFirstLine() + wxString(" ") + file.GetNextLine();
    } else {
        wxPuts(wxString(L"[" RGE L"-" RESET L"] Pas assez de lignes dans ") + path); // TODO: handle error
    }
    file.Close();
    wxPuts(wxString(L"[" JNE L"?" RESET L"] Nom de l'envoyeur : ") + framename);
    wxPuts(wxString(L"[" JNE L"?" RESET L"] Message : ") + message);
    wxPuts(wxString(L"[" JNE L"?" RESET L"] Adresse IP de l'envoyeur : ") + ip);
    DiscussionFrame *frame = nullptr;
    for (auto& f : discFrames) {
        if (f->GetTitle() == framename) {
            frame = f;
            break;
        }
    }
    frame->MessageRecu(message);
}

void MainFrame::OnPreferences(wxCommandEvent& event) {
    wxPuts(L"[" VRT L"-" RESET L"] Fenetre de preferences affichee");
    settingsFrame = new SettingsFrame(this);
    settingsFrame->Show(true);
}