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
#include "Contact.hpp"
#include "Profils.hpp"
#include "Discussion.hpp"
#include "Settings.hpp"

#include "prefs.hpp"

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
    m_menuContact->Append(IDs::EditProfils, L"Editer les profils de cryptage\tCtrl-E");

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
    Bind(wxEVT_MENU, &MainFrame::OnEditProfils, this, IDs::EditProfils);
    Bind(wxEVT_MENU, &MainFrame::OnPreferences, this, IDs::Preferences);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    //socket stuff
    Bind(wxEVT_SOCKET, &MainFrame::OnSocketEvent, this, IDs::Socket);
    Bind(wxEVT_SOCKET, &MainFrame::OnServerEvent, this, IDs::Server);
    Bind(wxEVT_SOCKET, &MainFrame::OnSocketEvent, this, IDs::SrvSock);
    Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &MainFrame::OnListBoxEvent, this, IDs::ListBox);
    Bind(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, &MainFrame::OnClicListBox, this, IDs::ListBox);
    Bind(wxEVT_BUTTON, &MainFrame::AfficherMenuPrincipal, this, IDs::ButReload);
    m_sizerMenuPrincipal = new wxBoxSizer(wxHORIZONTAL);
    settingsFrame = new SettingsFrame(this);
    SrvStart();
    AfficherMenuPrincipal();
}
void MainFrame::AfficherMenuPrincipal(wxCommandEvent& event) {
    AfficherMenuPrincipal();
}
void MainFrame::AfficherMenuPrincipal() {
    m_sizerMenuPrincipal->Clear();
    ReloadDiscFrames();
    CreateConfFolders();
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString usrDataDir(pathinfo.GetUserDataDir());
    wxDir dir(usrDataDir + "/Contacts/");
    if(!dir.HasFiles("*.ctc")) {
        wxMessageBox(L"Aucun contact pour l'instant, pour en ajouter allez dans Fichier>Contact>Ajouter ou pressez Ctrl+A", L"Info", wxICON_INFORMATION);
    } else {
        if(m_printedListBox) delete m_listBox;
        m_listBox = new wxListBox(this, IDs::ListBox/*, wxDefaultPosition, wxSize(GetWinSize()[0], GetWinSize()[1])*/);
        m_printedListBox = true;
        wxFileName name;
        wxString filename;
        wxTextFile file;
        wxRegEx ipValide(wxString("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$"));
        bool cont = dir.GetFirst(&filename);
        while (cont) {
            name.Assign(usrDataDir + wxString("/Contacts/") + filename);
            if(ipValide.Matches(name.GetName()) or name.GetName() == "localhost") {
                file.Open(usrDataDir + wxString("/Contacts/") + filename);
                m_listBox->Append(file.GetFirstLine() + wxString(" ") + file.GetNextLine() + wxString("|") + name.GetName());
                file.Close();
            } else {
                wxMessageBox(wxString(L"Un des fichiers n'a pas une adresse IP valide (il s'agit de ") + filename + ")", L"Attention", wxICON_WARNING);//TODO: PROPOSER DE LE SUPPR, aussi ya un core dump qd je lance app si un dossier est présent
            }
            cont = dir.GetNext(&filename);
        }
        m_sizerMenuPrincipal->Add(m_listBox, 0, wxEXPAND);
        m_sizerMenuPrincipal->Add(new wxButton(this, IDs::ButReload, L"Recharger"));
        SetSizer(m_sizerMenuPrincipal);
        Layout();
    }
}
void MainFrame::OnNouveauContact(wxCommandEvent& event) {
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    ContactDialog dlg;
    if (dlg.ShowModal() == wxID_OK) {
        wxRegEx ipValide(wxString("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$"));
        if(ipValide.Matches(dlg.GetIP()) or dlg.GetIP() == "localhost") {
            if(not(dlg.GetNom().IsEmpty() | dlg.GetPrenom().IsEmpty())) {
                CreateConfFolders();
                wxString nomFichier(pathinfo.GetUserDataDir() + wxString("/Contacts/") + dlg.GetIP() + wxString(".ctc"));
                //std::filesystem::path cwd = std::filesystem::current_path();
                //wxString szUserName = wxGetTextFromUser("Please enter your name in the field below.", "Create a New User", wxEmptyString, this);
                wxFile fichierNouveauContact;
                if(!wxFile::Exists(nomFichier)) {
                    if(fichierNouveauContact.Create(nomFichier, false, wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP | wxS_IROTH | wxS_IWOTH)) {
                        fichierNouveauContact.Write(dlg.GetNom() + wxString("\n") + dlg.GetPrenom());
                        wxPuts(wxString(L"[" VRT L"-" RESET L"] Fichier enregistre avec succes en tant que ") + nomFichier);
                        fichierNouveauContact.Close();
                    } else {
                        wxMessageBox(L"Erreur durant la création du fichier", L"Erreur", wxICON_ERROR);
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
    settingsFrame->Destroy();
    wxPuts(L"[" VRT L"-" RESET L"] Fermeture de la fenetre principale");
    Destroy();
}

void MainFrame::CreateConfFolders() {
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString usrDataDir(pathinfo.GetUserDataDir());
    if(!wxDirExists(usrDataDir)) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Dossier de configuration inexistant. Creation a ") + usrDataDir);
        wxDir::Make(usrDataDir);
    }
    if(!wxDirExists(usrDataDir + "/Contacts/")) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Dossier de contacts inexistant. Creation a ") + usrDataDir + "/Contacts/");
        wxDir::Make(usrDataDir + "/Contacts/");
    }
    if(!wxDirExists(usrDataDir + "/Messages/")) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Dossier de messages inexistant. Creation a ") + usrDataDir + "/Messages/");
        wxDir::Make(usrDataDir + "/Messages/");
    }
}

void MainFrame::OnListBoxEvent(wxCommandEvent& event) {
    wxString name = m_listBox->GetStringSelection().AfterFirst('|') + ".ctc";
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString path(pathinfo.GetUserDataDir() + wxString("/Contacts/") + name);
    wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
    DiscussionFrame *frame = nullptr;
    for (auto& f : discFrames) {
        if (f->GetIP() + ".ctc" == name) {
            frame = f;
            break;
        }
    }

    // If a frame does not exist, create one
    if (frame == nullptr) {
        wxFileName filename(path);
        wxTextFile file(path);
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
    wxString framename;
    if(wxFile::Exists(path)) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
        file.Open();
        if (file.GetLineCount() >= 2) {
            framename = file.GetFirstLine() + wxString(L" ") + file.GetNextLine();
        } else {
            wxPuts(wxString(L"[" RGE L"X" RESET L"] Pas assez de lignes dans ") + path);
            wxMessageBox(wxString(L"Pas assez de lignes dans le fichier de contacts ") + path + L", il doit y avoir au moins deux lignes (une avec le nom, une avec le prénom)", L"Erreur", wxICON_ERROR);
            return;
        }
    } else {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Creation de ") + path);
        file.Create();
        file.AddLine(L"Nouveau");
        file.AddLine(L"Contact");
        file.Write();
        AfficherMenuPrincipal();
        framename = file.GetFirstLine() + wxString(L" ") + file.GetNextLine();
    }
    file.Close();
    wxPuts(wxString(L"[" JNE L"?" RESET L"] Nom de l'envoyeur : ") + framename);
    wxPuts(wxString(L"[" JNE L"?" RESET L"] Message : ") + message);
    wxPuts(wxString(L"[" JNE L"?" RESET L"] Adresse IP de l'envoyeur : ") + ip);
    for(auto& f : discFrames) {
        if (f->GetTitle() == framename) {
            f->Show(true);
            f->MessageRecu(message);
            break;
        }
    }
}

void MainFrame::OnPreferences(wxCommandEvent& event) {
    wxPuts(L"[" VRT L"-" RESET L"] Fenetre de preferences affichee");
    settingsFrame->Show(true);
}

void MainFrame::OnClicListBox(wxCommandEvent& event) {
    wxString name = m_listBox->GetStringSelection().AfterFirst('|') + ".ctc";
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString path(pathinfo.GetUserDataDir() + wxString(L"/Contacts/") + name);
    if(!wxFile::Exists(path)) {
        wxMessageBox(L"Le contact est inexistant (suppression du fichier durant l'exécution de l'application ?)", L"Erreur", wxICON_ERROR);
        return;
    }

    wxPuts(wxString(L"[" VRT L"-" RESET L"] Edition de ") + path);

    ContactDialog dlg;
    if (dlg.ShowModal() == wxID_OK) {
        wxRegEx ipValide(wxString("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$"));
        if(ipValide.Matches(dlg.GetIP()) or dlg.GetIP() == "localhost") {
            if(not(dlg.GetNom().IsEmpty() | dlg.GetPrenom().IsEmpty())) {
                CreateConfFolders();

                //std::filesystem::path cwd = std::filesystem::current_path();
                //wxString szUserName = wxGetTextFromUser("Please enter your name in the field below.", "Create a New User", wxEmptyString, this);
                wxTextFile file(path);
                wxString newpath(pathinfo.GetUserDataDir() + wxString(L"/Contacts/") + dlg.GetIP() + ".ctc");
                if(!wxFile::Exists(newpath)) {
                    if(file.Open()) {
                        wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture du fichier ") + path);
                        file.Clear();
                        file.AddLine(dlg.GetNom());
                        file.AddLine(dlg.GetPrenom());
                        file.Write();
                        file.Close();
                        wxRenameFile(path, newpath);
                    } else {
                        wxMessageBox(L"Erreur durant l'ouverture du fichier", L"Erreur", wxICON_ERROR);
                    }
                } else {
                    wxMessageBox(L"Cette IP est déjà prise par un autre contact ! Réessayez avec une IP libre.", L"Impossible de changer l'adresse", wxICON_WARNING);
                }
            } else {
                wxMessageBox(L"Veuillez renseigner les champs de nom et de prénom", "Erreur", wxICON_INFORMATION);
            }
        } else {
            wxMessageBox("Veuillez renseigner une adresse IP valide", "Erreur", wxICON_INFORMATION);
        }
    }
    AfficherMenuPrincipal();
    // If a frame does not exist, create one
    /*if (frame == nullptr) {
        wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
        wxString folder(pathinfo.GetUserDataDir() + "/Contacts/");
        wxFileName filename(folder + name);
        wxTextFile file(folder + name);
        file.Open();
        frame = new DiscussionFrame(file.GetFirstLine() + wxString(" ") + file.GetNextLine(), filename.GetName(), this);
        discFrames.push_back(frame);
    } else {
        frame->SetFocus();
    }*/

    // Show the frame
    //frame->Show(true);
}

void MainFrame::ReloadDiscFrames() {
    for(auto& frame : discFrames) {
        frame->Destroy();
    }
    discFrames.clear();
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString usrDataDir(pathinfo.GetUserDataDir());
    wxDir dir(usrDataDir + "/Contacts/");
    wxString filename;
    wxFileName name;
    wxTextFile file;
    bool cont = dir.GetFirst(&filename);
    while (cont) {
        name.Assign(usrDataDir + wxString("/Contacts/") + filename);
        file.Open(usrDataDir + wxString("/Contacts/") + filename);
        discFrames.push_back(new DiscussionFrame(file.GetFirstLine() + wxString(" ") + file.GetNextLine(), name.GetName(), this));
        file.Close();
        cont = dir.GetNext(&filename);
    }
}

void MainFrame::OnEditProfils(wxCommandEvent& event) {
    ProfilsDialog dlg;
    if (dlg.ShowModal() == wxID_OK) {
        if(dlg.GetMode() == 0) {
            wxPuts(L"Mode de cryptage");
        } else {
            wxPuts(L"Mode de cryptage xxx");
        }
    }
}