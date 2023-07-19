#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);

private:
	void ConstructComponents();               // Create widgets
	void ChangeColor(wxScrollEvent& evt);     // Find color from sliders
	void BindFunctions();                     // Bind FindColor function to sliders

	wxPanel* panel;
	wxSlider* rSlider;    // Slider for red
	wxSlider* gSlider;    // Slider for green
	wxSlider* bSlider;    // Slider for blue
	wxButton* canvas;     // This is where the color is loaded on to

	wxStaticText* color;  // Label that tells the color
	wxStaticText* labelR; // Label next to slider R
	wxStaticText* labelG; // Label next to slider G
	wxStaticText* labelB; // Label next to slider B
};