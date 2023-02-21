#ifndef PROFIL_HPP_INCLUDED
#define PROFIL_HPP_INCLUDED

#include <wx/wx.h>

class MainFrame;

class SettingsFrame : public wxFrame {
public:
    SettingsFrame(MainFrame *mainframe);
private:
    void OnClose(wxCloseEvent& event);
    void OnValider(wxCommandEvent& event);
    void OnSetMaj(wxCommandEvent& event);
    void OnSetCmd(wxCommandEvent& event);
    void OnSetSplashScreen(wxCommandEvent& event);
    wxMenu* editMenu;
    wxMenuBar *menuBar;
    wxBoxSizer *sizer;
    MainFrame *mainframe;
    bool validated;
    wxArrayInt settings;
    wxArrayInt oldSettings;
    wxCheckBox *maj;
    wxCheckBox *cmd;
    wxCheckBox *splash;
};
#endif // PROFIL_HPP_INCLUDED