#pragma once
#include <wx/wx.h>
#include <string>
#include "MusicPlayer.h"

class MainFrame : public wxFrame{
public:
	MainFrame(const wxString& title);

private:
	void ConstructComponents();                        // Create widgets   
	void ExtractAndAddSongs();                         // Extract songs in the folder
	void PlaySong(std::string filename);               // Play wav files
	void OnPlaySongButtonClicked(wxCommandEvent& evt); // Call the PlaySong function
	void BindFunctions();                              // Bind functions to buttons

	bool CheckSong(std::string filename); // Check weather file is a wav file or not
	MusicPlayer player;                   // Create an instance of the music player

	const unsigned short int PLAY_ID   = 0001;  // ID for playButton

	wxPanel*        panel;
	wxListBox*      songBox;     // List full of all the songs
	wxButton*       playButton;  // Play a selected song
	wxStaticText*   title;       // Create a title for the window
};

