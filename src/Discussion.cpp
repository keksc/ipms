#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/regex.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>
#include <wx/statline.h>
#include <wx/scrolwin.h>
#include <wx/sstream.h>

#include "Discussion.hpp"
#include "Main.hpp"

#include "settings.hpp"

#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/undo.xpm"
#include "bitmaps/redo.xpm"
#include "bitmaps/bold.xpm"
#include "bitmaps/italic.xpm"
#include "bitmaps/underline.xpm"

#include "bitmaps/alignleft.xpm"
#include "bitmaps/alignright.xpm"
#include "bitmaps/centre.xpm"
#include "bitmaps/font.xpm"
#include "bitmaps/indentless.xpm"
#include "bitmaps/indentmore.xpm"

#include "wx/richtext/richtextctrl.h"
#include "wx/richtext/richtextformatdlg.h"
#include "wx/richtext/richtextxml.h"

DiscussionFrame::DiscussionFrame(wxString titre, wxString ip, MainFrame *mainframe)
    : wxFrame(NULL, wxID_ANY, titre, wxDefaultPosition, wxSize(DISCWINDOW_WIDTH, DISCWINDOW_HEIGHT)), mainframe(mainframe), m_ip(ip) {
    SetMinSize(wxSize(DISCWINDOW_WIDTH, DISCWINDOW_HEIGHT));

    editMenu = new wxMenu;
    editMenu->Append(wxID_UNDO, L"&Undo\tCtrl+Z");
    editMenu->Append(wxID_REDO, L"&Redo\tCtrl+Y");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_CUT, L"Cu&t\tCtrl+X");
    editMenu->Append(wxID_COPY, L"&Copy\tCtrl+C");
    editMenu->Append(wxID_PASTE, L"&Paste\tCtrl+V");

    /*editMenu->AppendSeparator();
    editMenu->Append(wxID_SELECTALL, _("Select A&ll\tCtrl+A"));
    editMenu->AppendSeparator();
    editMenu->Append(ID_SET_FONT_SCALE, _("Set &Text Scale..."));
    editMenu->Append(ID_SET_DIMENSION_SCALE, _("Set &Dimension Scale..."));*/

    menuBar = new wxMenuBar();
    menuBar->Append(editMenu, L"&Editer");

    SetMenuBar(menuBar);

    m_toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTB_FLAT|wxTB_NODIVIDER|wxTB_NOALIGN);
    m_toolBar->AddTool(wxID_CUT, wxEmptyString, wxBitmap(cut_xpm), L"Copier");
    m_toolBar->AddTool(wxID_COPY, wxEmptyString, wxBitmap(copy_xpm), L"Couper");
    m_toolBar->AddTool(wxID_PASTE, wxEmptyString, wxBitmap(paste_xpm), L"Coller");
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(wxID_UNDO, wxEmptyString, wxBitmap(undo_xpm), L"Annuler");
    m_toolBar->AddTool(wxID_REDO, wxEmptyString, wxBitmap(redo_xpm), L"Refaire");
    m_toolBar->AddSeparator();
    m_toolBar->AddCheckTool(IDs::FormatBold, wxEmptyString, wxBitmap(bold_xpm), wxNullBitmap, _("Bold"));
    m_toolBar->AddCheckTool(IDs::FormatItalic, wxEmptyString, wxBitmap(italic_xpm), wxNullBitmap, L"Italique");
    m_toolBar->AddCheckTool(IDs::FormatUnderline, wxEmptyString, wxBitmap(underline_xpm), wxNullBitmap, L"Souligner");
    m_toolBar->AddSeparator();
    m_toolBar->AddCheckTool(IDs::FormatAlignLeft, wxEmptyString, wxBitmap(alignleft_xpm), wxNullBitmap, L"Aligner à gauche");
    m_toolBar->AddCheckTool(IDs::FormatAlignCentre, wxEmptyString, wxBitmap(centre_xpm), wxNullBitmap, L"Aligner au centre");
    m_toolBar->AddCheckTool(IDs::FormatAlignRight, wxEmptyString, wxBitmap(alignright_xpm), wxNullBitmap, L"Aligner à droite");
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(IDs::FormatIndentLess, wxEmptyString, wxBitmap(indentless_xpm), L"Moins indenter");
    m_toolBar->AddTool(IDs::FormatIndentMore, wxEmptyString, wxBitmap(indentmore_xpm), L"Plus indenter");
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(IDs::FormatFont, wxEmptyString, wxBitmap(font_xpm), "Police");
    m_toolBar->Realize();

    wxFont font(wxFontInfo(12).Family(wxFONTFAMILY_ROMAN));
    m_richTextCtrl = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 300), wxVSCROLL|wxHSCROLL|wxWANTS_CHARS|wxNO_BORDER);
    m_richTextCtrl->SetMargins(10, 10);
    m_richTextCtrl->SetFont(font);

    m_sizer = new wxBoxSizer(wxVERTICAL);

    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    const wxString usrDataDir(pathinfo.GetUserDataDir());
    wxString folder(usrDataDir + "/Messages/");
    wxString path(folder + m_ip + ".msgs");
    wxTextFile file;
    bool empty;
    if(wxFile::Exists(path)) {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Ouverture de ") + path);
        file.Open(path);
        empty = false;
    } else {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Creation de ") + path);
        file.Create(path);
        empty = true;
    }
    wxRichTextCtrl *rline = nullptr;
    if(!empty) {
        wxString line(file.GetFirstLine());
        if(!line.IsEmpty()) wxPuts(wxString(L"[" JNE L"msg" RESET L"] ") + line);
        rline = new wxRichTextCtrl(this, wxID_ANY, line, wxDefaultPosition, wxDefaultSize, wxRE_READONLY | wxRE_MULTILINE);
        rline->EnableVerticalScrollbar(false);
        rline->SetBackgroundColour(GetBackgroundColour());
        m_sizer->Add(rline, 1, wxEXPAND);
        //m_sizer->Add(new wxRichTextCtrl(this, wxID_ANY, line, wxDefaultPosition, wxDefaultSize, wxRE_READONLY | wxRE_MULTILINE));
        //m_sizer->Add(new wxStaticText(this, wxID_ANY, line));
        while(!file.Eof()) {
            line = file.GetNextLine();
            if(!line.IsEmpty()) {
                wxPuts(wxString(L"[" JNE L"msg" RESET L"] ") + line);
                rline = new wxRichTextCtrl(this, wxID_ANY, line, wxDefaultPosition, wxDefaultSize, wxRE_READONLY | wxRE_MULTILINE);
                rline->EnableVerticalScrollbar(false);
                rline->SetBackgroundColour(GetBackgroundColour());
                m_sizer->Add(rline, 1, wxEXPAND);
            }
        }
    } else {
        wxPuts(wxString(L"[" VRT L"-" RESET L"] Aucun message pour l'instant"));
    }
    file.Close();
    m_sizer->Add(m_toolBar, 0, wxEXPAND);
    m_sizer->Add(m_richTextCtrl, 1, wxEXPAND);
    m_sizer->Add(new wxButton(this, IDs::ButEnvoyer, L"Envoyer"));
    SetSizer(m_sizer);

    Bind(wxEVT_CLOSE_WINDOW, &DiscussionFrame::OnExit, this);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnBold, this, IDs::FormatBold);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnItalic, this, IDs::FormatItalic);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnUnderline, this, IDs::FormatUnderline);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnCut, this, wxID_CUT);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnCopy, this, wxID_COPY);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnPaste, this, wxID_PASTE);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnUndo, this, wxID_UNDO);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnRedo, this, wxID_REDO);
    Bind(wxEVT_TOOL, &DiscussionFrame::OnFont, this, IDs::FormatFont);
    Bind(wxEVT_BUTTON, &DiscussionFrame::Envoyer, this, IDs::ButEnvoyer);
    m_richTextCtrl->SetValue("<span font='bold'>Item 1:</span> Description 1\n"
                         "<span font='bold'>Item 2:</span> Description 2\n"
                         "<span font='bold'>Item 3:</span> Description 3\n");
}

void DiscussionFrame::OnExit(wxCloseEvent& event) {
    //m_richTextCtrl->Unbind(wxEVT_RICHTEXT_SELECTION_CHANGED, &DiscussionFrame::OnTextSelectionChanged, this);
    Hide();
}

wxString DiscussionFrame::GetIP() {
    return m_ip;
}
void DiscussionFrame::OnBold(wxCommandEvent& event) {
    m_richTextCtrl->ApplyBoldToSelection();
}
void DiscussionFrame::OnItalic(wxCommandEvent& event) {
    m_richTextCtrl->ApplyItalicToSelection();
}

void DiscussionFrame::OnUnderline(wxCommandEvent& event) {
    m_richTextCtrl->ApplyUnderlineToSelection();
}

void DiscussionFrame::OnFont(wxCommandEvent& event) {
    if(m_richTextCtrl->GetStringSelection().IsEmpty()) return;
    wxRichTextRange range;
    if (m_richTextCtrl->HasSelection())
        range = m_richTextCtrl->GetSelectionRange();
    else
        range = wxRichTextRange(0, m_richTextCtrl->GetLastPosition()+1);

    int pages = wxRICHTEXT_FORMAT_FONT;

    wxRichTextFormattingDialog formatDlg(pages, this);
    formatDlg.SetOptions(wxRichTextFormattingDialog::Option_AllowPixelFontSize);
    formatDlg.GetStyle(m_richTextCtrl, range);

    if (formatDlg.ShowModal() == wxID_OK) {
        formatDlg.ApplyStyle(m_richTextCtrl, range, wxRICHTEXT_SETSTYLE_WITH_UNDO|wxRICHTEXT_SETSTYLE_OPTIMIZE|wxRICHTEXT_SETSTYLE_CHARACTERS_ONLY);
    }
}

void DiscussionFrame::OnCut(wxCommandEvent& event) {
    m_richTextCtrl->Cut();
}

void DiscussionFrame::OnCopy(wxCommandEvent& event) {
    m_richTextCtrl->Copy();
}

void DiscussionFrame::OnPaste(wxCommandEvent& event) {
    m_richTextCtrl->Paste();
}

void DiscussionFrame::OnUndo(wxCommandEvent& event) {
    m_richTextCtrl->Undo();
}
void DiscussionFrame::OnRedo(wxCommandEvent& event) {
    m_richTextCtrl->Redo();
}

void DiscussionFrame::Envoyer(wxCommandEvent& event) {
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString filename(pathinfo.GetUserDataDir() + wxString("/Messages/") + m_ip + ".msgs");
    wxFile *file;
    if(wxFile::Exists(filename)) {
        file = new wxFile(filename, wxFile::write_append);
        file->Write(m_richTextCtrl->GetValue() + "\n");
    } else {
        file = new wxFile(filename, wxFile::write);
        file->Write(m_richTextCtrl->GetValue() + "\n");
    }
    file->Close();
    m_sizer->Insert(m_sizer->GetItemCount() - 3, new wxStaticText(this, wxID_ANY, m_richTextCtrl->GetValue()));
    Layout();
    wxStringOutputStream out_stream;
    wxRichTextXMLHandler xml_handler;
    xml_handler.ExportXML(out_stream, m_richTextCtrl->GetBuffer(), 0);

// Get the serialized data as a wxString
    wxString serialized_data = out_stream.GetString();
    wxPuts(serialized_data);
    mainframe->Envoyer(m_richTextCtrl, m_ip);
}

void DiscussionFrame::MessageRecu(wxString& msg) {
    wxStandardPathsBase &pathinfo=wxStandardPaths::Get();
    wxString filename(pathinfo.GetUserDataDir() + wxString("/Messages/") + m_ip + ".msgs");
    wxFile *file;
    if(wxFile::Exists(filename)) {
        file = new wxFile(filename, wxFile::write_append);
        file->Write(msg + "\n");
    } else {
        file = new wxFile(filename, wxFile::write);
        file->Write(msg + "\n");
    }
    file->Close();
    m_sizer->Insert(m_sizer->GetItemCount() - 3, new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND);
    m_sizer->Insert(m_sizer->GetItemCount() - 3, new wxStaticText(this, wxID_ANY, msg), 0, wxALIGN_RIGHT);
    Layout();
}

wxArrayInt DiscussionFrame::GetWinSize() {
    int w;
    int h;
    DoGetClientSize(&w, &h);
    wxArrayInt size;
    size.Add(w);
    size.Add(h);
    return size;
}