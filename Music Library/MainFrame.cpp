#include <wx/wx.h>
#include <wx/dir.h>
#include <string>
#include <windows.h>
#include <fstream>
#include "MusicPlayer.h"
#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	ConstructComponents();
	ExtractAndAddSongs();
	BindFunctions();
}

void MainFrame::ConstructComponents()
{
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	title = new wxStaticText(panel, wxID_ANY, "Music Library", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	title->SetFont(headlineFont);

	songBox = new wxListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	playButton = new wxButton(panel, PLAY_ID, "Play", wxPoint(100, 525), wxSize(100, 35));
}

void MainFrame::ExtractAndAddSongs()
{
	/*
	* This function works by extracting all the files in a certain dirrectory.
	* Then going through those files to check weather they are wav files or not.
	* If they are wav files then they will get added to the songBox
	*/

	wxString currentDir = wxGetCwd(); // Current dirrectory
	wxDir dir(currentDir);            // Create a dirrectory object for the folder

	if (!dir.IsOpened()) {            // Check if dirrectory can be opened or exsists
		wxMessageBox("Failed to open dirrectory" + currentDir, "Error", wxICON_ERROR);
		return;
	}

	wxString filename;
	bool cont = dir.GetFirst(&filename); // Get the first entry (file or sub dirrectory)

	// Iterate over the files in the dirrectory
	while (cont) {

		// Add the file name to the list box if it is a .wav file
		if (CheckSong(filename.ToStdString())) 
			songBox->Append(filename);

		cont = dir.GetNext(&filename);
	}
}

bool MainFrame::CheckSong(std::string filename)
{
	size_t dotIndex = filename.find_last_of(".");

	if (dotIndex != std::string::npos) { // While dot index exists
		std::string extension = filename.substr(dotIndex + 1, sizeof(filename) - 1);

		if (extension == "wav")
			return true;
		else
			return false;
	}

	return false;
}

void MainFrame::PlaySong(const std::string filename)
{
	player.Play(filename);
}

void MainFrame::OnPlaySongButtonClicked(wxCommandEvent& evt)
{
	// Get ticked items
	int selectedIndex = songBox->GetSelection();
	std::string filename = songBox->GetString(selectedIndex).ToStdString();

	// Play ticked song
	PlaySong(filename);
}

void MainFrame::BindFunctions()
{
	Bind(wxEVT_BUTTON, &MainFrame::OnPlaySongButtonClicked, this, PLAY_ID);
}