# ash: A Custom Shell in C

This project is a custom shell implementation in C, inspired by and building upon the tutorial by [Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c). It aims to provide a simple yet functional command-line interface for Unix-based systems.

## Features and Commands

The shell currently supports the following commands:

- `cd`: Change the current directory.
- `help`: Display information about builtin commands.
- `exit`: Exit the shell.
- `ls`: List directory contents.
- `touch`: Create a new file if it does not exist.

### Planned Commands

Future updates aim to introduce more functionality, including but not limited to:

- `mv`: Move or rename files and directories.
- `cp`: Copy files and directories.
- `rf`: Remove files.
- `rd`: Remove directories.
- `ra`: Rename all files in a directory.
- `cat`: Concatenate and display file content.
- `man`: Display manual pages for commands.


### Planned Features

The development roadmap for `ash` includes a variety of enhancements aimed at making the shell more powerful and user-friendly. Key planned features include:

- **Integration with External Commands**: Extend functionality by incorporating commands from external sources, such as `git`, `npm`, and others.
- **Scripting Capabilities**: Introduce scripting support to automate tasks and customize the shell environment, similar to `bash` scripting.
- **Command Piping and Redirection**: Implement the ability to pipe the output of one command as input to another and support redirection of input/output to and from files.
- **Cross-Platform Support**: While currently available on Linux, future versions aim to support Windows and MacOS.
- **Interactive Shell Features**: Add features like tab completion, command history, and syntax highlighting.
- **Enhanced File Management**: Beyond basic file operations, introduce advanced file management features such as batch renaming, file searching, and directory navigation enhancements.
- **Custom Themes and Prompts**: Allow users to customize the appearance of their shell prompt and interface, including themes and color schemes.

These features are part of our commitment to making `ash` a comprehensive and user-friendly shell that caters to the needs of developers and power users alike.

## Getting Started

To compile and run the shell:

1. Clone the repository to your local machine.
2. Navigate to the cloned directory.
3. Compile the shell using `gcc -o ash ash.c`
4. Run the shell by executing `./ash`.

## License

This project is open-source and available under the MIT License. See the LICENSE file for more details.