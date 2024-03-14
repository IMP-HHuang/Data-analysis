// This macro setup.C is called in Analysis constructor
// These settings are used in batch mode.
// When started from GUI, these settings are default, but can be overwritten.

void setup()
{
  TGo4AnalysisStep        * step;
  TGo4StepFactory         * fact;
  TGo4FileStoreParameter  * store;

  Text_t autosave[]="result";   // name of autosave file (.root)
  Text_t outfile[]="tree";      // name of output file (.root)
  Text_t infile[]="tree";       // name of input file (previous output)
  Text_t node[]="nodename";     // name of MBS or event server node
  Int_t  port=0;                // depends on the remote event server

  go4->SetAutoSaveFile(autosave); // optional
  go4->SetAutoSaveInterval(0);    // after n seconds , 0 = at end of event loop   
  go4->SetAutoSave(kTRUE);        // optional

  // Get access to step and factory:
  step = go4->GetAnalysisStep("Unpack");
  fact=(TGo4StepFactory *)step->GetStepFactory();

  // Analysis scenarios:
  // 1. Input MBS events, default output event class, no output
  // 2. Input MBS events, user output event class, optional output
  // 3. Input from output of 2, user input event class, default output event class
  //    The input class of 3 must be the same as the output class of 2.

  // tell the framework the name and class of our event processor:
  fact->DefEventProcessor("Proc","Proc");

  // tell the framework the name and class of our output event.
  //--------------------------------------------------
  // Case 1 and 3:
  // Use default event class (no data):
  fact->DefOutputEvent("Dummy","TGo4EventElement");
  //--------------------------------------------------
  // Case 2:
  // Use our event class:
  // fact->DefOutputEvent("Event","Event");

  // tell the framework the name and class of our input event.
  //--------------------------------------------------
  // Case 3 only:
  // fact->DefInputEvent("Event","Event");
  // TGo4FileSourceParameter * source = new TGo4FileSourceParameter(infile);

  //--------------------------------------------------
  // Case 1 and 2:
  // activate one of the following MBS event sources:
  //--------------------------------------------------
  TGo4MbsRandomParameter * source = new TGo4MbsRandomParameter("Random");
  //--------------------------------------------------
  // TGo4MbsEventServerParameter * source = new TGo4MbsEventServerParameter(node);
  //--------------------------------------------------
  // TGo4MbsStreamParameter * source = new TGo4MbsStreamParameter(node);
  //--------------------------------------------------
  // TGo4MbsTransportParameter * source = new TGo4MbsTransportParameter(node);
  //--------------------------------------------------
  // TGo4MbsFileParameter * source = new TGo4MbsFileParameter(node);
  //--------------------------------------------------
  // TGo4RevServParameter * source=new TGo4RevServParameter(node);
  // source->SetPort(port);
  //--------------------------------------------------

  step->SetEventSource(source);  // register event source
  step->SetSourceEnabled(kTRUE);
  step->SetProcessEnabled(kTRUE);
  step->SetErrorStopEnabled(kTRUE);

  //--------------------------------------------------
  // optional output (event object needed):
  // Case 2:
  // store = new TGo4FileStoreParameter(outfile);
  // step->SetEventStore(store);
  step->SetStoreEnabled(kFALSE);  // en-disable output
  printf("Setup done!\n");

}
