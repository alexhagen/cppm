# cppm
**c++ objects for people with no time**

I write a lot of firmware for the detectors I make.  And I have to say, `c++`
has some of the most convoluted classes imaginable. So, everytime I have to use
one of the complex `c++` objects, or - even worse - use the old non-semantic
`c` APIs for things, I try to write it into a little bit more pythonic object.
If you really want the full power of these objects, don't use this library.
But if you want the object to be exactly what it says it is, and no more, this
library may be one of the faster ways.

## Installation

Installation is easy.  Just clone, copy into `/usr/inclue/c++`, and go. If
you're on Windows, good luck.  Maybe someone can tell me where Windows
developers might keep their header files.

**Step-By-Step**

```bash

git clone https://github.com/alexhagen/cppm
cp cppm/*.h /usr/includes/
```

## Usage

There's a bunch of different interfaces defined here, categorized roughly as
*memory objects*, *tcp objects*, *udp objects*, and *signals and timing*.
Specific documentation on all of those interfaces are included in the readmes
folder.
