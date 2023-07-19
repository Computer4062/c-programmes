#include "MainFrame.h"
#include <wx/wx.h>
#include <iostream>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	ConstructComponents();
	BindFunctions();
}

void MainFrame::ConstructComponents()
{
	wxFont font(wxFontInfo(wxSize(0, 24)).Bold());

	panel = new wxPanel(this);
	panel->SetFont(font);

	labelR = new wxStaticText(panel, wxID_ANY, "Red: ",   wxPoint(10, 100));
	labelG = new wxStaticText(panel, wxID_ANY, "Green: ", wxPoint(10, 300));
	labelB = new wxStaticText(panel, wxID_ANY, "Blue: ",  wxPoint(10, 500));

	rSlider = new wxSlider(panel, wxID_ANY, 0, 0, 255, wxPoint(90, 100), wxSize(300, 50));
	gSlider = new wxSlider(panel, wxID_ANY, 0, 0, 255, wxPoint(90, 300), wxSize(300, 50));
	bSlider = new wxSlider(panel, wxID_ANY, 0, 0, 255, wxPoint(90, 500), wxSize(300, 50));

	canvas = new wxButton(panel, wxID_ANY, "", wxPoint(400, 50), wxSize(300, 500));
	canvas->Enable(false);
	canvas->SetBackgroundColour(wxColor(0, 0, 0));

	color = new wxStaticText(panel, wxID_ANY, "0 0 0", wxPoint(450, 550));
}

void MainFrame::ChangeColor(wxScrollEvent& evt)
{
	int r = rSlider->GetValue();
	int b = bSlider->GetValue();
	int g = gSlider->GetValue();

	std::string red   = std::to_string(r);
	std::string green = std::to_string(g);
	std::string blue  = std::to_string(b);

	wxString text = red + " " + green + " " + blue;

	color->SetLabelText(text);
	canvas->SetBackgroundColour(wxColor(r, g, b));
}

void MainFrame::BindFunctions()
{
	rSlider->Bind(wxEVT_SCROLL_THUMBTRACK, &MainFrame::ChangeColor, this);
	gSlider->Bind(wxEVT_SCROLL_THUMBTRACK, &MainFrame::ChangeColor, this);
	bSlider->Bind(wxEVT_SCROLL_THUMBTRACK, &MainFrame::ChangeColor, this);
}