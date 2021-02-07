[![pipeline status](https://gitlab.com/ubports/apps/teleports/badges/master/pipeline.svg)](https://gitlab.com/ubports/apps/teleports/commits/master)

# TELEports - An Ubuntu Touch Telegram client

[![OpenStore](https://open-store.io/badges/en_US.png)](https://open-store.io/app/teleports.ubports)

## Building

Teleports and its libraries are built using
[clickable](https://clickable.bhdouglass.com/en/latest/).

### Dependencies

Build tdlib (approx. 45 min per architecture):

    git submodule update --init
    clickable build-libs tdlib-prepare --arch amd64 # your host architecture (probably amd64)
    clickable build-libs tdlib --arch armhf # or arm64 or amd64

Alternatively one can download the tdlib build from our CI. Open the
[latest master pipeline](https://gitlab.com/ubports/apps/teleports/-/pipelines?page=1&scope=all&ref=master),
navigate to the respective job (`tdlib-armhf`, `tdlib-arm64` or `tdlib-amd64`),
download the Job Artifacts and extract them to your project directory conserving
the structure (`build/<ARCH_TRIPLET>/tdlib/install`).

Build rlottie:

    clickable build-libs rlottie --arch armhf # or arm64 or amd64

### App

Building the app is as simple as:

    clickable --arch armhf # or arm64 or amd64
    clickable desktop # build and run in desktop mode

When committing for the first time you need to apply two settings to your fork
of TELEports on Gitlab:

    Settings -> CI/CD -> General pipelines -> Expand -> Timeout = 2h -> Save Changes
    Settings -> CI/CD -> Runners -> Expand -> Shared Runners -> Enable shared runners

## Debugging

### Enable Tdlib Debug Output

Prepend the `clickable` call with `CLICKABLE_ENV_TDLIB_DEBUG=1` on building to produce
a build with Tdlib debug output enabled or trigger the debug CI job.

### Desktop Mode

Run debug build in Desktop Mode:

    clickable desktop --debug

### On Device

Make sure to build teleports clean and with `--debug`. In a first terminal run
the following commands:

    clickable --arch armhf --debug clean-build
    clickable --arch armhf install
    clickable --arch armhf gdbserver

After gdbserver has started, open a second terminal and start a gdb session with:

    clickable --arch armhf gdb

Replace `armhf` by `arm64` as needed. See clickable
[Debugging Documentation](https://clickable-ut.dev/en/latest/debugging.html) for
details.

Once you see the `<gdb>` prompt in the second terminal, you can start TELEports
by running the gdb command `continue`. For detailed instructions on how to use
gdb check out
[gdb documentation](https://sourceware.org/gdb/current/onlinedocs/gdb/).  

## C++ Code Style/Formatting

Clang-format is used to keep the code style consistent and should be run before
committing any changes to C++ code.

Install clang-format

```bash
$ sudo apt install clang-format
```

Then you can either run clang-format as a git pre commit hook (preferred) or run
the apply-format script to format any staged changes.

### Git pre commit hook

After checking out the repository run `./tools/git-pre-commit-format install`

Then during any commits where c++ code has changed you will be prompted to
accept any code formatting changes that will be applied.

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

As a community developed project, but somehow connected to ubports we follow the
[desgin rules](https://gitlab.com/ubports/teams/ux-des/ubuntu-touch-human-interface-guidelines---hib/wikis/home)
made by the ubports UX Team.
