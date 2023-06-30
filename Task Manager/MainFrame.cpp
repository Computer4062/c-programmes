#include "MainFrame.h"
#include <wx/wx.h>
#include <vector>
#include <string>
#include "Tasks.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{	
	CreateControls();
	BindEventHandlers();
	LoadTasks();
}

void MainFrame::LoadTasks()
{
	std::vector<Task> tasks = loadTasksFromFile("Text.txt");

	for (auto values : tasks) {                                         //iterate through all the values in tasks
		std::string info = values.description;
		std::replace(info.begin(), info.end(), '_', ' ');

		wxString description = info;
		bool done = values.done;

		checkListBox->Insert(description, checkListBox->GetCount());    //insert the item at the end of checkListBox
		checkListBox->Check(done, checkListBox->GetCount());            //check the item if it was checked
	}
}

void MainFrame::SaveTasks()
{
	std::vector<Task> tasks;

	for (int i = 0; i < checkListBox->GetCount(); i++) {
		std::string description = checkListBox->GetString(i).ToStdString();
		bool done = checkListBox->IsChecked(i);

		std::replace(description.begin(), description.end(), '_', ' ');
		tasks.push_back(Task{ description, done });
	}

	SaveTasksToFile(tasks, "Text.txt");
}

void MainFrame::CreateControls()
{
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List", 
		wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);                          	//-1 means defalt

	headlineText->SetFont(headlineFont);

	inputField = new wxTextCtrl(panel, wxID_ANY, "", 
		wxPoint(100, 80), wxSize(495, 35), wxTE_PROCESS_ENTER);                                //wxTE_PROCESS_ENTER allows to pick up enter key presses when inputField is in focus

	addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	checkListBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));  //checkListBox is a large box which allows us to add items into it
}

void MainFrame::BindEventHandlers()
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode()) {
	case WXK_DELETE:
		DeleteSelectedTask();
		break;

	case WXK_UP:
		MoveSelectedTask(-1);      //-1 means up
		break;

	case WXK_DOWN:
		MoveSelectedTask(1);       //1 mean down
		break;
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt)
{
	int deleted = 0;
	for (int i = 0; i < checkListBox->GetCount(); i++) {                //Iterate through every item in checkListBox
		if (checkListBox->IsChecked(i)) {                               //If item is ticked delete it
			checkListBox->Delete(i);
			deleted++;
		}
	}

	if (0 == deleted)                                                   //If nothing is checked clear everyting
		checkListBox->Clear();

	SaveTasks(); 
}

void MainFrame::AddTaskFromInput()
{
	wxString description = inputField->GetValue();

	if (!description.IsEmpty()) {
		checkListBox->Insert(description, checkListBox->GetCount());    //GetCount appends the description to the bottom of checkListBox
		inputField->Clear();                                            //remove text in the input box
	}

	inputField->SetFocus();                                             //set the typing cursor to the input field
	SaveTasks();                                                        //save the task to a text file
}

void MainFrame::DeleteSelectedTask()
{
	int selectedIndex = checkListBox->GetSelection();                   //get the index of the task you are focusing on
	
	if (wxNOT_FOUND == selectedIndex)                                   //if nothing is selected exit
		return;
	
	checkListBox->Delete(selectedIndex);
	SaveTasks();
}

void MainFrame::MoveSelectedTask(int offset)
{
	int selectedIndex = checkListBox->GetSelection();

	if (wxNOT_FOUND == selectedIndex)
		return;

	int newIndex = selectedIndex + offset;

	if (newIndex >= 0 && newIndex < checkListBox->GetCount()) {         //if newIndex is not at the begging or end of the checkListBox
		SwapTasks(selectedIndex, newIndex);                             //select newIndex after swaping
		checkListBox->SetSelection(newIndex, true);
	}
	SaveTasks();
}

void MainFrame::SwapTasks(int i, int j)
{
	Task taskI{ checkListBox->GetString(i).ToStdString(), checkListBox->IsChecked(i) };
	Task taskJ{ checkListBox->GetString(j).ToStdString(), checkListBox->IsChecked(j) };

	checkListBox->SetString(i, taskJ.description);
	checkListBox->Check(i, taskJ.done);

	checkListBox->SetString(j, taskI.description);
	checkListBox->Check(j, taskJ.done);

	SaveTasks();
}