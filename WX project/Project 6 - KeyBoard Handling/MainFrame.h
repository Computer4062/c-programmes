#include <wx/wx.h>

#pragma once
class MainFrame : public wxFrame{
public:
	MainFrame(const wxString& title);

private:
	void OnKeyEvent(wxKeyEvent& evt);
};

