# unutf8
Convert utf-8 to stream of code points

## Synopsis 

    unutf8 < input > output
    
## Usage

    $ echo привет, мир ♥ | unutf8
    1087
    1088
    1080
    1074
    1077
    1090
    44
    32
    1084
    1080
    1088
    32
    9829
    10
    $ _

## Building

    make unutf8
    
## Formatting output

I recommend using `awk`. For example, if you want space-separated hex values instead of decimal numbers, say this:

    $ echo привет, мир ♥ | ./unutf8 | awk -e ' { printf "%04x ", $1 } '
    043f 0440 0438 0432 0435 0442 002c 0020 043c 0438 0440 0020 2665 000a
    $ _

## License

The author of this software cares exceptionally little about how, and why, and by whom this software is going to be used.
