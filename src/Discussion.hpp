#ifndef DISCUSSION_HPP_INCLUDED
#define DISCUSSION_HPP_INCLUDED

#include <wx/wx.h>

class MainFrame;

#include "wx/richtext/richtextctrl.h"

class DiscussionFrame : public wxFrame {
public:
    wxString GetIP();
    void OnExit(wxCloseEvent& event);
    DiscussionFrame(wxString title, wxString ip, MainFrame *mainframe);

    wxArrayInt GetWinSize();
    void OnResize(wxSizeEvent& event);

    void OnBold(wxCommandEvent& event);
    void OnItalic(wxCommandEvent& event);
    void OnUnderline(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnFont(wxCommandEvent& event);
    void Envoyer(wxCommandEvent& event);
    void MessageRecu(wxString& msg);
private:
    MainFrame *mainframe;
    wxMenu* editMenu;
    wxMenuBar *menuBar;
    wxBoxSizer *m_sizer;
    wxToolBar *m_toolBar;
    wxRichTextCtrl* m_richTextCtrl;
    wxString m_ip;
};
#endif // DISCUSSION_HPP_INCLUDED