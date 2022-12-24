#ifndef NOUVEAUCONTACT_HPP_INCLUDED
#define NOUVEAUCONTACT_HPP_INCLUDED

#include <wx/wx.h>
class NouveauContactDialog : public wxDialog {
public:
    NouveauContactDialog();
    wxString GetNom();
    wxString GetPrenom();
    wxString GetIP();
private:
    wxTextCtrl *nom;
    wxTextCtrl *prenom;
    wxTextCtrl *ip;
};
#endif // NOUVEAUCONTACT_HPP_INCLUDED*/