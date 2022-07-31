# Terminal Minesweeper (Work in Progress)

![thumbnail](./thumbnail.png)

## To note
Works with Linux and MacOS platforms. **The program was not tested on Windows**

## Quick Start

```console
$ make
$ ./mine
```
### Optional Arguments

```
usage: mine [-r num] [-c num] [-p num]
       Optionally specify field [-r] rows and [-c] cols (20-128)
       Optionally specify [-p] percentage of mines (1-60) 
```

## Controls

| key                                                 | description        |
|-----------------------------------------------------|--------------------|
| <kbd>w</kbd>,<kbd>a</kbd>,<kbd>s</kbd>,<kbd>d</kbd> | Move cursor around |
| <kbd>SPACE</kbd>                                    | Open cell          |
| <kbd>f</kbd>                                        | Flag/unflag cell   |
| <kbd>r</kbd>                                        | Restart            |
| <kbd>q</kbd>                                        | Quit               |

## Debug Mode

Build with debug mode:

```console
$ make DEBUG=1
$ ./mine
```
