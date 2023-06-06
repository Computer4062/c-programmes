#include <wx/wx.h>
#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	wxButton* button1 = new wxButton(panel, wxID_ANY, "Button", wxPoint(300, 275),  wxSize(200, 50));
	wxButton* button2 = new wxButton(panel, wxID_ANY, "Button", wxPoint(300, 350), wxSize(200, 50));

	this->Bind(wxEVT_BUTTON, &MainFrame::OnAnyButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
	button1->Bind(wxEVT_BUTTON, &MainFrame::OnButton1Clicked, this);
	button2->Bind(wxEVT_BUTTON, &MainFrame::OnButton2Clicked, this);

	CreateStatusBar();
}

void MainFrame::OnButton1Clicked(wxCommandEvent& evt) {
	wxLogStatus("Button 1 Clicked");
}

void MainFrame::OnButton2Clicked(wxCommandEvent& evt) {
	wxLogStatus("Button 2 Clicked");
}

void MainFrame::OnAnyButtonClicked(wxCommandEvent& evt) {
	wxLogStatus("Clicked");
}

void MainFrame::OnClose(wxCloseEvent& evt) {
	wxLogMessage("Frame Closing");
	evt.Skip();
}