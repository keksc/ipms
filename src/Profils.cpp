#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/file.h>
#include <wx/regex.h>
#include <wx/dir.h>

#include <filesystem>

#include "Profils.hpp"

#include "prefs.hpp"

ProfilsDialog::ProfilsDialog()
    : wxDialog(NULL, wxID_ANY, "Entrez les infos", wxDefaultPosition, wxSize(WINDOW_WIDTH, WINDOW_HEIGHT)), m_nom(new wxTextCtrl(this, wxID_ANY)), m_cle(new wxString), m_sizer(new wxBoxSizer(wxVERTICAL)) {
    wxPuts(L"[" VRT L"-" RESET L"] Fenetre edit profils cryptages affichee");
    wxButton *boutonOk = new wxButton(this, wxID_OK,"Valider");
    wxButton *boutonAnnuler = new wxButton(this, wxID_CANCEL,"Annuler");

    wxString fruit_choices[] = {L"Cryptage", L"Cryptage"};
    m_radioBox = new wxRadioBox(this, wxID_ANY, L"Choisis un cryptage :", wxDefaultPosition, wxDefaultSize, WXSIZEOF(fruit_choices), fruit_choices, 1, wxRA_SPECIFY_COLS);
    m_sizer->Add(m_nom);
    m_sizer->Add(m_radioBox);
    m_sizer->Add(boutonOk);
    m_sizer->Add(boutonAnnuler);
    /*sizer->Add(new wxStaticText(this, wxID_ANY, "Nom"));
    sizer->Add(m_nom,0,wxALL,5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Prenom"));
    sizer->Add(prenom,0,wxALL,5);
    sizer->Add(new wxStaticText(this, wxID_ANY, "IP"));
    sizer->Add(ip,0,wxALL,5);*/
    SetSizer(m_sizer);

    Center();
}
wxString ProfilsDialog::GetNom() {
  return m_nom->GetValue();
}
int ProfilsDialog::GetMode() {
  if(m_radioBox->GetSelection() == wxNOT_FOUND) {
    return -1;
  }
  return m_radioBox->GetSelection();
}
wxString* ProfilsDialog::GetCle() {
  return m_cle;
}