#pragma once
#include <wx/wx.h>
#include <vector>
#include <string>
#include "CustomizerApp.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);       //Main window with code in it

private:
	void ConstructComponents();              //function for loading all the widgets
	void BindMenuOptions();                  //Bind Menu Functions
	void CreateMenuBar();                    //Create Menus
	void UpdateWindow();                     //Update font, fontcolour and background
	void LoadCode();                         //Load temp file

	void SaveFunction();                    //Save code to a temp.txt file
	void SaveKeyPressed(wxKeyEvent& evt);   //Run Save function when CRTL + S is pressed

	void Open(wxCommandEvent& evt);          //Opening code file
	void Run(wxCommandEvent& evt);           //Running code file
	void Close(wxCommandEvent& evt);         //exit the code editor
	void SaveAs(wxCommandEvent& evt);        //Saving code file
	void Save(wxCommandEvent& evt);          //Call SaveFunction function
	void New(wxCommandEvent& evt);           //Clear data from temp.txt, filelocation.txt, clear text in codebox
	void About(wxCommandEvent& evt);         //Creates a message box explaining about the app

	void Customize(wxCommandEvent& evt);     //Run the customizer
	std::vector<std::string> LoadFile();     //Load configs

	wxColour ReturnColor(std::string color); //Return a wxColor depending on a given colour name
	wxFont   ReturnFont(std::string font);   //Return a font depending on a given font name
	std::string ExtractFileName(const std::string& filePath); //Extract filename from FileLocation.txt

	wxPanel* panel;                        //Place where all the widgets are dropped into
	wxMenuBar* menuBar;                      //A place for the menus to stay in
	wxMenu* fileMenu;                     //Menu for opening and saving
	wxMenu* helpMenu;                     //Menu for about and help
	wxMenu* runMenu;                      //Menu for run and terminal options
	wxTextCtrl* codeBox;                      //Main textbox with code in it

	const unsigned short int RUN_ID        = 0001;
	const unsigned short int TERMINAL_ID   = 0002;
	const unsigned short int HELP_ID       = 0003;
	const unsigned short int ABOUT_ID      = 0004;
	const unsigned short int CUSTOMIZER_ID = 0005;
	const unsigned short int SAVEAS_ID     = 0006;

	CustomizerApp customizer;                //Create customizerApp Object for opening customizer

	wxFont   mainFont;                       //Font that is going to be used
	wxColour codeColour;                     //colour of the code in the editor box
	wxColour background;                     //colour of the background
};