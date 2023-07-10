#pragma once
#include <wx/wx.h>

class Customizer : public wxFrame
{
public:
	Customizer(const wxString& title);         //Window to customize the code editor

private:
	void RenderComponents();                   //Render widgets
	void FillComboBoxes();                     //Function to put text into combo boxes
	void SaveConfig();                         //Function to save settings 
	void BindCommands();                       //Bind Save button command
	void OnSaveButton(wxCommandEvent& evt);    //Add Save Button command

	wxArrayString fontChoices;                 //Combo box choices for fonts
	wxArrayString fontColorChoices;            //Combo box choices for font color
	wxArrayString backgroundChoices;           //Combo box choices for background

	wxPanel*        panel;                     //panel with all the components
	wxStaticText*   fontLabel;                 //Label to select font
	wxStaticText*   fontColorLabel;            //Label to select font color
	wxStaticText*   backgroundLabel;           //Label to select background
	wxComboBox*     fontBox;                   //Combo box to select font
	wxComboBox*     fontColorBox;              //Combo box to select font color
	wxComboBox*     backgroundBox;             //Combo box to select background
	wxButton*       saveButton;                //Button to save the config

	const std::string fileName = "Config.txt"; //File location of config.txt
};