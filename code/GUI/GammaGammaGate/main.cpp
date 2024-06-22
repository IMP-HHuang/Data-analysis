
#include "TRint.h"
#include "TObject.h"
#include "gui.h"

using namespace std;

int main(int argc, char *argv[])
{
		// Create an interactive ROOT application
		TApplication theApp("App", &argc, argv);

		gui mainWindow(gClient->GetRoot());

		// and enter the event loop...
		theApp.Run();
		//  delete mainWindow;

		return 0;
}

// 
// main.cc ends here
