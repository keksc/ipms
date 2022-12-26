#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/file.h>
#include <wx/filename.h>
#include <wx/regex.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>

#include "Main.hpp"
#include "NouveauContact.hpp"
#include "Discussion.hpp"

#include "settings.hpp"

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, WINDOW_NAME, wxDefaultPosition, wxSize(WINDOW_WIDTH, WINDOW_HEIGHT)), m_printedListBox(false) {

    //wxLogNull no_log; pour supprimer les logs localement, jusqu'à la fin du bloc

    SetIcon(wxIcon("res/icon.ico"));
    m_menuFile = new wxMenu;

    SetMinSize(wxSize(WINDOW_WIDTH, WINDOW_HEIGHT));

    m_menuContact = new wxMenu;
    m_menuContact->Append(IDs::NouveauContact, "Ajouter\tCtrl-A");
    m_menuContact->Append(IDs::ImportContact, "Importer\tCtrl-I");

    m_menuSocket = new wxMenu;
    m_menuSocket->Append(IDs::ButConn, "Connecter");

    m_menuFile->AppendSubMenu(m_menuContact, "Contact");
    m_menuFile->AppendSeparator();
    m_menuFile->Append(wxID_EXIT, "Quitter\tCtrl-Q", "Quitter ce programme");

    m_menuHelp = new wxMenu;
    m_menuHelp->Append(wxID_ABOUT);

    m_menuBar = new wxMenuBar;
    m_menuBar->Append(m_menuFile, "&Fichier");
    m_menuBar->Append(m_menuHelp, "&Aide");
    m_menuBar->Append(m_menuSocket, "&Socket");

    SetMenuBar(m_menuBar);
    CreateStatusBar();
    SetStatusText("Bienvenue dans ipms !");

    Bind(wxEVT_MENU, &MainFrame::AfficherMenuPrincipal, this, IDs::ListeContacts);
    Bind(wxEVT_MENU, &MainFrame::OnNouveauContact, this, IDs::NouveauContact);
    Bind(wxEVT_MENU, &MainFrame::OnImportContact, this, IDs::ImportContact);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    //socket stuff
    Bind(wxEVT_MENU, &MainFrame::Connect, this, IDs::ButConn);
    Bind(wxEVT_SOCKET, &MainFrame::OnSocketEvent, this, IDs::Socket);
    Bind(wxEVT_SOCKET, &MainFrame::OnServerEvent, this, IDs::Server);
    Bind(wxEVT_SOCKET, &MainFrame::OnSocketEvent, this, IDs::SrvSock);
    Bind(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, &MainFrame::OnListBoxEvent, this, IDs::ListBox);
    Bind(wxEVT_SIZE, &MainFrame::OnResize, this, wxID_ANY);
    SrvStart();
    AfficherMenuPrincipal();
}
void MainFrame::AfficherMenuPrincipal(wxCommandEvent& WXUNUSED(event)) {
    AfficherMenuPrincipal();
}
void MainFrame::AfficherMenuPrincipal() {
    CreateConfFolders();
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxDir dir(pathinfo.GetUserDataDir() + "/Contacts/");
    m_infoNoCtc = new wxStaticText(this, -1, wxEmptyString);
    if(!dir.HasFiles("*.ctc")) {
        m_sizerMenuPrincipal = new wxGridSizer(1, 1, 1, 1);
        m_infoNoCtc->SetLabel(L"Aucun contact pour l'instant, pour en ajouter allez dans\nFichier>Contact>Ajouter ou pressez Ctrl+A");
        Unbind(wxEVT_MENU, &MainFrame::AfficherMenuPrincipal, this, IDs::ListeContacts);
        m_sizerMenuPrincipal->Add(m_infoNoCtc);
    } else {
        wxArrayString arrayOfFiles;
        wxDir::GetAllFiles(pathinfo.GetUserDataDir() + "/Contacts/", &arrayOfFiles, "*.ctc");
        m_sizerMenuPrincipal = new wxGridSizer(3, arrayOfFiles.GetCount(), 0, 0);
        m_listBox = new wxListBox(this, IDs::ListBox, wxDefaultPosition, wxSize(GetWinSize()[0], GetWinSize()[1]));
        m_printedListBox = true;
        m_sizerMenuPrincipal->Add(m_listBox, wxEXPAND);
        wxString filename;
        bool cont = dir.GetFirst(&filename);
        while (cont) {
            m_listBox->Append(filename);
            cont = dir.GetNext(&filename);
        }
        SetSizer(m_sizerMenuPrincipal);
    }
}
void MainFrame::OnNouveauContact(wxCommandEvent& event) {
    m_infoNoCtc->SetLabel("");
    if(m_printedListBox) {
        m_listBox->Clear();
        m_printedListBox = false;
    }
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    NouveauContactDialog dlgNouveauContact;
    if (dlgNouveauContact.ShowModal() == wxID_OK) {
        wxRegEx ipValide(wxString("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$"));
        if(ipValide.Matches(dlgNouveauContact.GetIP())) {
            if(not(dlgNouveauContact.GetNom().IsEmpty() | dlgNouveauContact.GetPrenom().IsEmpty())) {
                CreateConfFolders();
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
    AfficherMenuPrincipal();
}

void MainFrame::OnImportContact(wxCommandEvent& event) {
    m_infoNoCtc->SetLabel("");
    if(m_printedListBox) {
        m_listBox->Clear();
        m_printedListBox = false;
    }
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

void MainFrame::OnClose(wxCloseEvent& event) {
    for (auto& frame : discFrames)
    {
        frame->Close(true);
    }
    wxPuts("closed main frame");
    Destroy();
}

void MainFrame::CreateConfFolders() {
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    if(!wxDirExists(pathinfo.GetUserDataDir())) {
        wxPuts(wxString(L"Dossier de configuration inexistant. Creation à ") + pathinfo.GetUserDataDir());
        wxMkDir(pathinfo.GetUserDataDir(), 777);
    }
    if(!wxDirExists(pathinfo.GetUserDataDir() + "/Contacts/")) {
        wxPuts(wxString(L"Dossier de contacts dans le dossier de configuration inexistant. Creation à ") + pathinfo.GetUserDataDir());
        wxMkDir(pathinfo.GetUserDataDir() + "/Contacts/", 777);
    }
}

void MainFrame::OnListBoxEvent(wxCommandEvent& event) {
    wxString name = m_listBox->GetStringSelection();
    discFrames.push_back(new DiscussionFrame(name));
    // Get the selected file name
    wxString fileName = m_listBox->GetStringSelection();

    // Check if a frame already exists for the selected file name
    DiscussionFrame* frame = nullptr;
    for (auto& f : discFrames) {
        if (f->GetTitle() == fileName) {
            frame = f;
            break;
        }
    }

    // If a frame does not exist, create one
    if (!frame) {
        frame = new DiscussionFrame(m_listBox->GetStringSelection());
        discFrames.push_back(frame);
    } else {
        frame->SetFocus();
    }

    // Show the frame
    frame->Show(true);
}

wxArrayInt MainFrame::GetWinSize() {
    int w;
    int h;
    DoGetClientSize(&w, &h);
    wxArrayInt tab;
    tab.Add(w);
    tab.Add(h);
    return tab;
}

void MainFrame::OnResize(wxSizeEvent& event) {
    m_listBox->SetSize(GetWinSize()[0], GetWinSize()[1]);
}