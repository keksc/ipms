#ifndef CONTACT_HPP_INCLUDED
#define CONTACT_HPP_INCLUDED

#include <wx/wx.h>
class ContactDialog : public wxDialog {
public:
    ContactDialog();
    wxString GetNom();
    wxString GetPrenom();
    wxString GetIP();
private:
    wxTextCtrl *nom;
    wxTextCtrl *prenom;
    wxTextCtrl *ip;

    wxBoxSizer *sizer1;
    wxBoxSizer *sizer2;
    wxBoxSizer *Psizer;
};
#endif // CONTACT_HPP_INCLUDED*/
