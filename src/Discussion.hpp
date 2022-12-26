#ifndef DISCUSSION_HPP_INCLUDED
#define DISCUSSION_HPP_INCLUDED

#include <wx/wx.h>
#include <wx/file.h>
#include <wx/socket.h>

#include "NouveauContact.hpp"

class DiscussionFrame : public wxFrame {
public:
    DiscussionFrame(wxString titre);

private:
    void OnExit(wxCommandEvent& event);
    wxTextCtrl *m_titre;
    wxTextCtrl *m_message;

    wxBoxSizer *m_sizer;

    int m_index;
};

#endif // DISCUSSION_HPP_INCLUDED