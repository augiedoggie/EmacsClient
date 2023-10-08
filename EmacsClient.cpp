// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 Chris Roberts

#include <Alert.h>
#include <Application.h>
#include <Entry.h>
#include <Path.h>
#include <PathFinder.h>
#include <Roster.h>
#include <String.h>
#include <stdlib.h>


class EmacsClientApp : public BApplication {
public:
	EmacsClientApp()
		:
		BApplication("application/x-vnd.cpr.EmacsClient")
	{
	}


	status_t
	_LaunchEmacs(const BString& argString)
	{
		BString commandString;
		if (BRoster().IsRunning("application/x-vnd.GNU-emacs")) {
			//TODO wait for server mode if Emacs is still loading
			BStringList pathList;
			if (BPathFinder::FindPaths(B_FIND_PATH_BIN_DIRECTORY, "emacsclient", B_FIND_PATH_EXISTING_ONLY, pathList) == B_OK
				&& !pathList.IsEmpty())
				commandString.SetToFormat("%s -n %s", pathList.StringAt(0).String(), argString.String());
			else {
				(new BAlert("ERROR", "Error finding emacsclient command!", "Ok"))->Go();
				return B_ERROR;
			}
		} else {
			BStringList pathList;
			if (BPathFinder::FindPaths(B_FIND_PATH_APPS_DIRECTORY, "Emacs", B_FIND_PATH_EXISTING_ONLY, pathList) == B_OK
				&& !pathList.IsEmpty())
				commandString.SetToFormat("%s %s &", pathList.StringAt(0).String(), argString.String());
			else {
				(new BAlert("ERROR", "Error finding Emacs application!", "Ok"))->Go();
				return B_ERROR;
			}
		}

		if (system(commandString.String()) != 0) {
			(new BAlert("ERROR", "Error executing command!", "Ok"))->Go();
			return B_ERROR;
		}

		return B_OK;
	}


	virtual void
	RefsReceived(BMessage* message)
	{
		int32 line = message->GetInt32("be:line", 0);
		entry_ref ref;
		BString commandArgs;

		for (int32 index = 0; message->FindRef("refs", index, &ref) == B_OK; index++) {
			BPath filePath(&ref);

			if (index != 0)
				commandArgs << " ";

			if (line > 0)
				commandArgs << "+" << line << " ";

			// escape any single quotes
			BString pathString(filePath.Path());
			pathString.ReplaceAll("'", "'\\''");

			commandArgs << "'" << pathString << "'";
		}

		_LaunchEmacs(commandArgs);
	}


	virtual void
	ArgvReceived(int32 /*argc*/, char** argv)
	{
		//TODO Parse line information "/foo/bar:#" and handle multiple args
		if (argv[1] == NULL)
			return;

		_LaunchEmacs(argv[1]);
	}


	virtual void
	ReadyToRun()
	{
		be_app->PostMessage(B_QUIT_REQUESTED);
	}
};


int
main(int /*argc*/, char** /*argv*/)
{
	EmacsClientApp app;
	app.Run();

	return 0;
}
