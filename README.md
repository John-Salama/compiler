# C++ to Python Translator

This is a program that translates C++ statements (while, assignment statement, if/else if/else) to Python. It consists of a C++ program and a Node.js server.

![website](https://github.com/John-Salama/cpp_python_translator/assets/89663721/43f028f7-3f48-4977-a0aa-e0fb54655bc2)

## How it works

The user inputs their code in the front end using HTML, CSS, and JavaScript. The Node.js server takes the input code, creates an `input.cpp` file, and runs the C++ program. The C++ program generates an `output.py` file containing the translated Python code. The Node.js server then reads the output of the C++ program (i.e., the translated Python code) and sends it back to the front end for the user to see.

## Requirements

- C++ compiler (tested with g++)
- Node.js (tested with v16.0.0)

## How to use

1. Clone the repository.
2. Install `node.msi` .
3. Open `LUANCH.bat` .

## Contributing

If you want to contribute to this project, you can:

- Submit bug reports or feature requests in the issues section.
- Fork the repository and make changes.
- Submit pull requests.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
