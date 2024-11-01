# WireWatch

Hello my friend! :wave:

This is a little project i started more or less spontaneously during a week with too much freetime, even though i had no experience with network programming in C whatsoever.

## Introduction

This little piece of software is basically a very simple network scanner build in C and only for Linux at the moment (until i find the time and energy to look into the Windows specific network APIs :smile:).

## Installation

You can download the `wirewatch-linux` executable from the release assets!
> Note that you might need to execute following command on Linux `chmod +x wirewatch-linux` 😊


Or you can install it from source via CMake using gcc compiler:

```
git clone https://github.com/fepauly/wirewatch.git

cd wirewatch
mkdir build
cd build

cmake ..
make
```

Now you have this little `wiwa` executable that you can execute.

## Usage

```
./wiwa <command> [arguments]
```

Run `./wiwa help` to get the list of supported commands! :blush:

