#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/regex.h>

#include "Discussion.hpp"

#include "settings.hpp"

DiscussionFrame::DiscussionFrame(wxString titre)
    : wxFrame(NULL, wxID_ANY, "", wxDefaultPosition, wxSize(WINDOW_WIDTH, WINDOW_HEIGHT)) {
    m_sizer = new wxBoxSizer(wxVERTICAL);
    m_titre = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    m_message = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_AUTO_URL);
    m_sizer->Add(m_titre);
    m_sizer->Add(m_message);
    SetSizer(m_sizer);
    SetTitle(titre);
}

void DiscussionFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}