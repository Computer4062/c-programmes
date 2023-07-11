#include "MainFrame.h"
#include "CustomizerApp.h"
#include <wx/wx.h>
#include <fstream>
#include <vector>
#include <string>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	ConstructComponents();                     //Load all the components
	UpdateWindow();                            //update font, font colour, and background
	CreateMenuBar();                           //Create the menu
	BindMenuOptions();                         //Put Functions for the menu
	LoadCode();                                //Load the content of temp.txt to code box
}

void MainFrame::ConstructComponents()
{
	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	wxSize fullSize = wxGetDisplaySize();                      //Size of the enteir window

	codeBox = new wxTextCtrl(panel, wxID_ANY, " ",
		wxDefaultPosition, fullSize,
		wxTE_MULTILINE | wxTE_RICH | wxTE_NOHIDESEL
	);
}

void MainFrame::CreateMenuBar()
{
	menuBar = new wxMenuBar();

	//File menu
	fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW, "&New");          //Start a new project
	fileMenu->Append(wxID_OPEN, "&Open");        //Open a new project
	fileMenu->Append(SAVEAS_ID, "&Save As");     //Save the code to a file
	fileMenu->Append(wxID_SAVE, "&Save");        //Save the code to a temp file
	fileMenu->Append(wxID_EXIT, "&Exit");        //Exit the window

	menuBar->Append(fileMenu, "&File");          //Append fileMenu to main menu

	//Run menu
	runMenu = new wxMenu();
	runMenu->Append(RUN_ID, "&RunCode");         //Run the code
	runMenu->Append(CUSTOMIZER_ID, "&Customize");//Customize code editor

	menuBar->Append(runMenu, "&Code");           //Append runMenu to main menu

	//Help menu
	helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, "&About");

	menuBar->Append(helpMenu, "&Help");

	SetMenuBar(menuBar);
}

void MainFrame::UpdateWindow()                          //Update font , font colour and background
{
	std::vector <std::string> configs = LoadFile();
	std::string font                 = configs[0];
	std::string fontColor            = configs[1];
	std::string backgroundColor      = configs[2];

	mainFont = ReturnFont(font);
	background = ReturnColor(backgroundColor);
	codeColour = ReturnColor(fontColor);

	panel->SetFont(mainFont);
	codeBox->SetForegroundColour(codeColour);
	codeBox->SetBackgroundColour(background);
}

void MainFrame::LoadCode()                      //Load the code in temp.txt
{
	std::string content;
	std::ifstream file("temp.txt");

	if (!file.is_open()) {
		wxMessageBox("Cannot open temp.txt", "Error", wxICON_ERROR);
		return;
	}

	std::string line;
	while (std::getline(file, line))
		content += line + "\n";

	codeBox->Clear();
	codeBox->AppendText(content);
}

void MainFrame::Open(wxCommandEvent& evt)
{
	wxFileDialog fileDialog(                                                             //Create a file dialog to select a file to open from
		this,
		"Open Code File",
		"",
		"",
		"C++ files  (*.cpp) |*.cpp|C files (*.c) |*.c| header files (*.h)|*.h",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);

	if (wxID_CANCEL == fileDialog.ShowModal())                                          //If cancel button clicked exit
		return;

	const std::string selectedFile = fileDialog.GetPath().ToStdString();                //Get the file selected from the file dialog
	std::ifstream file(selectedFile);                                                   //Open that file

	std::string content;

	if (!file.is_open()) {                                                              //If file was not able to open
		wxMessageBox("Could not open " + selectedFile, "Error", wxICON_ERROR);
		return;
	}

	std::string line;
	while (std::getline(file, line)) {                                                  //Go through every line in the code file and add to content variable
		content += line + "\n";
	}

	file.close();

	std::fstream temp("temp.txt", std::ios::out);                                       //Open the temp file and add the content of the code file to the temp file

	if (!temp.is_open()) {
		wxMessageBox("Could not open temp.txt", "Error", wxICON_ERROR);
		return;
	}

	temp << content;

	temp.close();

	LoadCode();
	wxLogMessage("File Opened", "Opened", wxICON_INFORMATION);

	std::fstream locationFile("FileLocation.txt", std::ios::out);                       //Change content of location file

	if (!locationFile.is_open()) {
		wxMessageBox("Could not load FileLocation.txt", "Error", wxICON_ERROR);
		return;
	}

	locationFile << selectedFile;
	locationFile.close();
}

std::string MainFrame::ExtractFileName(const std::string& filePath)
{
	size_t dotIndex = filePath.find_last_of(".");
	if (dotIndex != std::string::npos) {
		std::string newPath = filePath.substr(0, dotIndex + 1) + "exe";
		return newPath;
	}

	return filePath;
}

void MainFrame::Run(wxCommandEvent& evt)
{
	std::ifstream file("FileLocation.txt");

	if (!file.is_open()) {
		wxMessageBox("Could not open FileLocation.txt", "Error", wxICON_ERROR);
		return;
	}

	std::string location;
	file >> location;

	file.close();

	std::string filePath = ExtractFileName(location);

	const std::string command = "g++ -o " + filePath + " " + location;
	const char* cmd = command.c_str();                         //convert std::string into char*

	system(cmd);
}

void MainFrame::BindMenuOptions()
{
	Bind(wxEVT_MENU, &MainFrame::Close, this, wxID_EXIT);
	Bind(wxEVT_MENU, &MainFrame::Customize, this, CUSTOMIZER_ID);
	Bind(wxEVT_MENU, &MainFrame::SaveAs, this, SAVEAS_ID);
	Bind(wxEVT_MENU, &MainFrame::Save, this, wxID_SAVE);
	Bind(wxEVT_MENU, &MainFrame::Open, this, wxID_OPEN);
	Bind(wxEVT_MENU, &MainFrame::New, this, wxID_NEW);
	Bind(wxEVT_MENU, &MainFrame::Run, this, RUN_ID);
	Bind(wxEVT_MENU, &MainFrame::About, this, wxID_ABOUT);

	codeBox->Bind(wxEVT_KEY_DOWN, &MainFrame::SaveKeyPressed, this);
}

void MainFrame::Close(wxCommandEvent& evt)
{
	exit(1);
}

void MainFrame::SaveAs(wxCommandEvent& evt)
{
	wxFileDialog fileDialog(
		this,
		"Save Code file",
		"",
		"",
		"C++ Files(*.cpp)|*.cpp|C Files(*.c)|*.c|Header Files(*.h)|*.h",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);

	if (wxID_CANCEL == fileDialog.ShowModal())
		return;

	const std::string fileLocation = fileDialog.GetPath().ToStdString();        //Get the file location of the newly created file
	std::fstream file(fileLocation, std::ios::out);                             //Open the newly created file

	if (!file.is_open()) {
		wxMessageBox("Could not open " + fileLocation, "Error", wxICON_ERROR);
		return;
	}

	std::string contentToSave = codeBox->GetValue().ToStdString();
	file << contentToSave;

	file.close();
	wxLogMessage("File Saved", "Saved", wxICON_INFORMATION);

	std::fstream locationFile("FileLocation.txt", std::ios::out);      //Change content of location file

	if (!locationFile.is_open()) {
		wxMessageBox("Could not load FileLocation.txt", "Error", wxICON_ERROR);
		return;
	}

	locationFile << fileLocation;
	locationFile.close();
}

void MainFrame::SaveFunction()
{
	std::ofstream file("temp.txt", std::ios::in);

	if (!file.is_open()) {
		wxMessageBox("Error cannot open temp.txt", "Error", wxICON_ERROR);
		return;
	}

	const std::string content = codeBox->GetValue().ToStdString();
	file << content;

	file.close();
}

void MainFrame::SaveKeyPressed(wxKeyEvent& evt)
{
	if (evt.ControlDown() && evt.GetKeyCode() == 'S')    //If Control S pressed
		SaveFunction();
	else
		evt.Skip();
}

void MainFrame::Save(wxCommandEvent& evt)               //Store content of file in temp.txt
{
	SaveFunction();
}

void MainFrame::New(wxCommandEvent& evt)
{
	codeBox->Clear();                                                                 //Clear text in code box

	std::fstream file("temp.txt", std::ios::out);                                     //Clear text in temp.txt

	if (!file.is_open()) {
		wxMessageBox("Cannot open temp.txt", "Error", wxICON_ERROR);
		return;
	}

	file << " ";
	file.close();

	std::fstream locationFile("FileLocation.txt", std::ios::out);                     //Clear text in FileLocation.txt

	if (!locationFile.is_open()) {
		wxMessageBox("Cannot open FileLocation.txt", "Error", wxICON_ERROR);
		return;
	}

	locationFile << " ";
	locationFile.close();

}

void MainFrame::About(wxCommandEvent& evt)
{
	std::ifstream file("About.txt");

	if (!file.is_open()) {
		wxMessageBox("Could not find About.txt", "Error", wxICON_ERROR);
		return;
	}

	std::string text;

	std::string line;
	while (std::getline(file, line)) {
		text += line + "\n";
	}

	file.close();

	wxMessageBox(text, "About", wxICON_INFORMATION);

}

void MainFrame::Customize(wxCommandEvent& evt)
{
	bool customizerOpen = customizer.OnInit(true);

	if (!customizerOpen) {
		wxMessageBox("Error could not open customizer", "Error", wxOK | wxICON_ERROR);
		return;
	}

	UpdateWindow();
}

std::vector<std::string> MainFrame::LoadFile()
{
	std::ifstream file("Config.txt");

	if (!file.is_open()) {
		wxMessageBox("Error could not open config.txt file", "Error", wxOK | wxICON_ERROR);
		exit(1);
	}

	std::vector<std::string> configs;
	
	std::string config;
	for (int i = 0; i < 3; i++) {
		file >> config;
		configs.push_back(config);
	}

	return configs;
}

wxColour MainFrame::ReturnColor(std::string color)
{
	if (color == "Black" || color == "Dark")
		return wxColour(0, 0, 0);

	else if (color == "White" || color == "Light")
		return wxColour(255, 255, 255);

	else if (color == "Yellow")
		return wxColour(255, 255, 0);

	else if (color == "Green")
		return wxColour(0, 255, 0);

	else if (color == "Pink")
		return wxColour(255, 192, 203);

	else
		return wxColour(255, 255, 255);
}

wxFont MainFrame::ReturnFont(std::string font)
{
	if (font == "monoscape")
		return wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	else if (font == "Ariel")
		return wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Arial");

	else if (font == "Fira")
		return wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Fira");
	else
		return wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

}