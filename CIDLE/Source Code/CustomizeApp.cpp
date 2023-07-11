#include "CustomizerApp.h"

bool CustomizerApp::OnInit(bool show)
{
	Customizer* customizer = new Customizer("Customizer");
	customizer->SetClientSize(400, 500);
	customizer->Center();
	customizer->Show(show);

	return show;
}
