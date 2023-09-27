#include <wx/wx.h>
#include <thread>

namespace FrameExample {
  class Frame : public wxFrame {
  public:
    Frame() : wxFrame(nullptr, wxID_ANY, "Window example", wxDefaultPosition, {640, 480}) {
      buttonClose->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
        Close();
      });

      buttonExit->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
        wxTheApp->Exit();
      });
      
      buttonExitMainLoop->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
        wxTheApp->ExitMainLoop();
      });

      buttonThread->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event){
        std::thread t1([]{Frame::threadFunction1();});
        std::thread t2([]{Frame::threadFunction2();});

        // Wait for both threads to finish
        t1.join();
        t2.join();
      });

      Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent& event) {
        if (wxMessageBox("Are you sure you want exit?", "Close Window", wxICON_QUESTION|wxYES_NO) != wxYES) event.Veto();
        else event.Skip();
      });
    }

  private:
    wxPanel* panel = new wxPanel(this);
    wxButton* buttonClose = new wxButton(panel, wxID_ANY, "Close", {10, 10});
    wxButton* buttonExit = new wxButton(panel, wxID_ANY, "Exit", {100, 10});
    wxButton* buttonExitMainLoop = new wxButton(panel, wxID_ANY, "ExitMainLoop", {190, 10});
    wxButton* buttonThread = new wxButton(panel, wxID_ANY, "Run Thread", wxPoint(300, 10));
    static void threadFunction1() {
        // Code to be executed in thread 1
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 1: " << i << std::endl;
        }
    }

    static void threadFunction2() {
        // Code to be executed in thread 2
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 2: " << i << std::endl;
        }
    }
  };

  class Application : public wxApp {
    bool OnInit() override {
      (new Frame())->Show();
      return true;
    }
  };
}

wxIMPLEMENT_APP(FrameExample::Application);
