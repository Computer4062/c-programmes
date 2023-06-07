#include <wx/wx.h>
#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
	wxButton* button1 = new wxButton(panel, wxID_ANY, "Button 1",  wxPoint(300, 150), wxSize(200, 100));
	wxButton* button2 = new wxButton(panel, wxID_ANY, "Button 2", wxPoint(300, 350), wxSize(200, 100));

	button1->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);

	CreateStatusBar();
}

void MainFrame::OnKeyEvent(wxKeyEvent& evt) {
	//when tab pressed navigate to the other elements
	if (evt.GetKeyCode() == WXK_TAB) {
		//get current window
		wxWindow* window = (wxWindow*)evt.GetEventObject();
		window->Navigate();
	}

	wxChar keyChar = evt.GetUnicodeKey();

	if (evt.GetUnicodeKey() == 'E') {
		exit(0);
	}
	//get key code for non character keys
	else if (keyChar == WXK_NONE) {
		int keyCode = evt.GetKeyCode();
		wxLogStatus("Key Event: %d", keyCode);
	}
	else {
		wxLogStatus("Key Event %c", keyChar);
	}
}