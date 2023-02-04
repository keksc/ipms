#ifndef PROFIL_HPP_INCLUDED
#define PROFIL_HPP_INCLUDED

#include <wx/wx.h>

class ProfilFrame : public wxFrame {
public:
    ProfilFrame();
private:
    void OnClose(wxCloseEvent& event);
    void OnAbout(wxCommandEvent& event);
};
#endif // PROFIL_HPP_INCLUDED