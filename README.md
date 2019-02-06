# TELEports - An Ubuntu Touch Telegram client

## Building
Teleports and its libraries are built using [clickable](https://clickable.bhdouglass.com/en/latest/). 

You need to build the dependencies (tdlib) once:

    git submodule update --init
    clickable build-libs tdlib-prepare --arch amd64
    clickable build-libs tdlib --arch armhf #if you want to build teleports for the device
    clickable build-libs tdlib --arch amd64 #if you want to build teleports on desktop, too

From now on building the app is as simple as:

    clickable #for the phone
    clickable desktop #for desktop

If you get a `teleports: error while loading shared libraries: libQt5QuickControls2.so.5: cannot open shared object file: No such file or directory`, 
remove the `.clickable` directory and try again. This may happen when
switching between phone and desktop build.

## C++ Code Style/Formatting

Clang-format is used to keep the code style consistent and should be run before committing any changes to C++ code.

Install clang-format

```bash
$ sudo apt install clang-format
```

Then you can either run clang-format as a git pre commit hook (preffered) or run the apply-format script to format any staged
changes.

### Git pre commit hook

After checking out the repository run `./tools/git-pre-commit-format install`

Then during any commits where c++ code has changed you will be prompted to accept any code formatting changes that will be applied.

### Run manually

To apply the format manually you could do something similar to:

```bash
$ # Make some changes.
$ # Stage changes you want to commit.
$ git add -p
$ # Reformat the staged changes you made.
$ ./tools/apply-format -i --staged
$ # Commit the result.
$ git commit
```

## Known issues

* No media attachments or photos are supported right now
* During login you need to resize the window after telephone number input or code input if you see a white box only. On the phone: *Rotate* the phone briefly to get the window resized
* No notifications
* Messages are not always refreshing, you have to back and forth between chat list and message window
* Messages are not being marked as read

## Logging out
**Note:** At the current stage it is adviseable to log out again from the App, especially if you are working on the Telegram test server. You can do this in the Account page.
