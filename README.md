# TELEports - An Ubuntu Touch Telegram client

## Building
Teleports and its libraries are built using [clickable](https://clickable.bhdouglass.com/en/latest/).

You need to build the dependencies (tdlib) once:

    git submodule update --init
    clickable build-libs tdlib-prepare --arch amd64 # your host architecture (probably amd64)
    clickable build-libs tdlib --arch armhf # or arm64 or amd64
    clickable build-libs tdlib --arch amd64 #if you want to build teleports on desktop, too

From now on building the app is as simple as:

    clickable --arch armhf # or arm64 for the phone
    clickable desktop #for desktop

When commiting for the first time you need to apply two settings to your fork of TELEports:

    Settings -> CI/CD -> General pipelines -> Expand -> Timeout = 2h -> Save Changes
    Settings -> CI/CD -> Runners -> Expand -> Shared Runners -> Enable shared runners

## Debugging
Make sure to build teleports clean and with `--debug`:

    clickable --arch armhf --debug clean-build
    clickable --arch armhf install
    clickable --arch armhf gdbserver

In a second terminal:

    clickable --arch armhf gdb

Replace `armhf` by `arm64` as needed. See [Debugging Documentation](https://clickable-ut.dev/en/latest/debugging.html) for details.

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
## Design guidelines

As a community devloped project, but somehow connected to ubports we follow the [desgin rules](https://gitlab.com/ubports/teams/ux-des/ubuntu-touch-human-interface-guidelines---hib/wikis/home) made by the ubports ux-team.
