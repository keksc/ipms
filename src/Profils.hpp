#ifndef PROFILS_HPP_INCLUDED
#define PROFILS_HPP_INCLUDED

#include <wx/wx.h>
class ProfilsDialog : public wxDialog {
public:
    ProfilsDialog();
    wxString GetNom();
    int GetMode();
    wxString* GetCle();
private:
    wxTextCtrl *m_nom;
    int m_mode;
    wxRadioBox *m_radioBox;
    wxString *m_cle;

    wxBoxSizer *m_sizer;
};
#endif // PROFILS_HPP_INCLUDED*/
