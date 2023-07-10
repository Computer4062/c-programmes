#include "Customizer.h"
#include <wx/wx.h>
#include <fstream>

Customizer::Customizer(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	FillComboBoxes();              //Fill Combo box choices
	RenderComponents();            //Render all the components
	BindCommands();                //Bind button commands
}

void Customizer::RenderComponents()
{
	wxFont mainFont(wxFontInfo(wxSize(0, 18)));            //main Font used through out the customizer

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	fontBox = new wxComboBox(           //create combo box for rendering fonts
		panel,
		wxID_ANY,
		wxEmptyString,                   //no title
		wxPoint(200, 100),                
		wxDefaultSize,                   //default size
		fontChoices,                     //choices 
		wxCB_DROPDOWN | wxCB_READONLY    //styles 
	);

	fontColorBox = new wxComboBox(       //create combo box for rendering font color
		panel,
		wxID_ANY,
		wxEmptyString,                   //no title
		wxPoint(200, 200),
		wxDefaultSize,                   //default size
		fontColorChoices,                //choices 
		wxCB_DROPDOWN | wxCB_READONLY    //styles 
	);

	backgroundBox = new wxComboBox(      //create combo box for rendering background
		panel,
		wxID_ANY,
		wxEmptyString,                   //no title
		wxPoint(200, 300),
		wxDefaultSize,                   //default size
		backgroundChoices,               //choices 
		wxCB_DROPDOWN | wxCB_READONLY    //styles 
	);

	fontLabel = new wxStaticText(panel, wxID_ANY, "Font: ", wxPoint(50, 100), wxDefaultSize);
	fontColorLabel = new wxStaticText(panel, wxID_ANY, "Font Color: ", wxPoint(50, 200), wxDefaultSize);
	backgroundLabel = new wxStaticText(panel, wxID_ANY, "Background: ", wxPoint(50, 300), wxDefaultSize);

	saveButton = new wxButton(panel, wxID_SAVE, "Save", wxPoint(150, 400), wxDefaultSize);
}

void Customizer::FillComboBoxes()
{
	fontChoices.Add("monoscape");
	fontChoices.Add("Ariel");
	fontChoices.Add("Fira");

	fontColorChoices.Add("Black");
	fontColorChoices.Add("White");
	fontColorChoices.Add("Yellow");
	fontColorChoices.Add("Green");
	fontColorChoices.Add("Pink");

	backgroundChoices.Add("Dark");
	backgroundChoices.Add("Light");
}

void Customizer::SaveConfig()
{
	std::fstream file(fileName, std::ios::out);

	if (!file.is_open()) {
		wxMessageBox("Error could not open config.txt file", "Error", wxOK | wxICON_ERROR);
		return;
	}
	
	const int fontIndex = fontBox->GetCurrentSelection();                                          //Get current font
	const std::string font = fontChoices[fontIndex].ToStdString();

	const int fontColorIndex = fontColorBox->GetCurrentSelection();                                //Get current font color
	const std::string fontColor = fontColorChoices[fontColorIndex].ToStdString();

	const int  backgroundIndex = backgroundBox->GetCurrentSelection();                             //Get current background
	const std::string background = backgroundChoices[backgroundIndex].ToStdString();

	file << font        << "\n";
	file << fontColor   << "\n";
	file << background;

}

void Customizer::BindCommands()
{
	Bind(wxEVT_BUTTON, &Customizer::OnSaveButton, this, wxID_SAVE);
}

void Customizer::OnSaveButton(wxCommandEvent& evt)
{
	SaveConfig();
}
