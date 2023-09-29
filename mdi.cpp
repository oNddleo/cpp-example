#include <wx/wx.h>

class MyChildFrame : public wxMDIChildFrame {
public:
    MyChildFrame(wxMDIParentFrame* parent, const wxString& title)
        : wxMDIChildFrame(parent, wxID_ANY, title) {
        // Add child window content here
        wxTextCtrl* textCtrl = new wxTextCtrl(this, wxID_ANY, "Child Window Content", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    }
};

class MyMDIParentFrame : public wxMDIParentFrame {
public:
    MyMDIParentFrame(const wxString& title)
        : wxMDIParentFrame(nullptr, wxID_ANY, title) {
        // Create a menu bar with options to open child frames
        wxMenu* fileMenu = new wxMenu;
        fileMenu->Append(wxID_NEW, "&New Child Frame");
        fileMenu->Append(wxID_EXIT, "E&xit");

        wxMenuBar* menuBar = new wxMenuBar;
        menuBar->Append(fileMenu, "&File");
        SetMenuBar(menuBar);

        // Create a status bar (optional)
        CreateStatusBar();
    }

    void OnNewChildFrame(wxCommandEvent& event) {
        // Create a new child frame and show it
        MyChildFrame* childFrame = new MyChildFrame(this, "Child Frame");
        childFrame->Show();
    }

    void OnExit(wxCommandEvent& event) {
        // Exit the application
        Close(true);
    }

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyMDIParentFrame, wxMDIParentFrame)
    EVT_MENU(wxID_NEW, MyMDIParentFrame::OnNewChildFrame)
    EVT_MENU(wxID_EXIT, MyMDIParentFrame::OnExit)
wxEND_EVENT_TABLE();

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyMDIParentFrame* frame = new MyMDIParentFrame("MDI wxWidgets Example");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
