#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <chrono>
#include <thread>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);

	wxButton* button = new wxButton(panel, wxID_ANY, "Button", wxPoint(150, 50), 
		wxSize(100, 35), wxBU_LEFT);

	wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "CheckBox", 
		wxPoint(550, 55), wxDefaultSize, wxCHK_3STATE | wxCHK_ALLOW_3RD_STATE_FOR_USER);

	wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "StaticText", 
		wxPoint(120, 150), wxSize(200, -1), wxALIGN_CENTER_HORIZONTAL);

	staticText->SetBackgroundColour(*wxLIGHT_GREY);

	wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "Text Ctrl", 
		wxPoint(500, 145), wxSize(200, -1), wxTE_PASSWORD);

	wxSlider* slider = new wxSlider(panel, wxID_ANY, 50, 0, 100, 
		wxPoint(100, 250), wxSize(200, -1), wxSL_VALUE_LABEL);

	wxGauge* gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(500, 255), wxSize(200, -1));
	for (int i = 0; i < 100; i++) {
		gauge->SetValue(i);
		std::this_thread::sleep_for(std::chrono::nanoseconds(10));
	}

	wxArrayString choices;
	choices.Add("Item A");
	choices.Add("Item B");
	choices.Add("Item C");

	wxChoice* choice = new wxChoice(panel, wxID_ANY, wxPoint(150, 375), 
		wxSize(150, -1), choices, wxCB_SORT);

	choice->Select(0);

	wxSpinCtrl* spinCtrl = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(550, 375), 
		wxSize(150, -1), wxSP_WRAP);

	wxListBox* listBox = new wxListBox(panel, wxID_ANY, wxPoint(150, 475), wxSize(100, -1), 
		choices, wxLB_MULTIPLE);

	wxRadioBox* radioBox = new wxRadioBox(panel, wxID_ANY, "Choice Box", wxPoint(550, 450), 
		wxDefaultSize, choices, 1, wxRA_SPECIFY_ROWS);

}