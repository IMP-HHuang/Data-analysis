
#include "TRint.h"
#include "TObject.h"
#include "gui.h"

using namespace std;

int main(int argc, char *argv[])
{
		// Create an interactive ROOT application
		//   TRint *theApp = new TRint("Rint", &argc, argv);
		TApplication theApp("App", &argc, argv);
		//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

		//   new gui(gClient->GetRoot());
		//   gui *mainWindow = new gui(gClient->GetRoot());
		gui mainWindow(gClient->GetRoot());

		// and enter the event loop...
		theApp.Run();
		//  delete mainWindow;

		return 0;
}

// 
// main.cc ends here
