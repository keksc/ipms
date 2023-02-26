#ifndef PROFIL_HPP_INCLUDED
#define PROFIL_HPP_INCLUDED

#include <wx/wx.h>

class MainFrame;

class SettingsFrame : public wxFrame {
public:
    SettingsFrame(MainFrame *mainframe);
private:
    void OnClose(wxCloseEvent& event);
    void OnValidate(wxCommandEvent& event);
    wxCheckBox* m_checkUpdates;
    wxCheckBox* m_checkDebug;
    wxCheckBox* m_checkIcon;

    int m_checkUpdatesState = 0;
    int m_checkDebugState = 0;
    int m_checkIconState = 0;
};
#endif // PROFIL_HPP_INCLUDED