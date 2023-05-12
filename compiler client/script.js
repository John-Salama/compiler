const btn = document.querySelector(".btn");
var output = document.querySelector(".output");
var input = document.querySelector(".input");

btn.addEventListener("click", async function () {
  const code = inputEditor.getValue();
  console.log(inputEditor.getValue());
  await axios.post("http://127.0.0.1:3000/runEXE", { code: code });
  const res = await axios.get("http://127.0.0.1:3000/runEXE");
  outputEditor.setValue(res.data);
});

/***********************************************/

var inputEditor = CodeMirror.fromTextArea(input, {
  mode: "text/x-c++src",
  lineNumbers: true,
  indentUnit: 4,
  theme: "material-darker",
  matchBrackets: true, // highlight matching brackets
  autoCloseBrackets: true, // automatically close brackets and quotes
});

// inputEditor.setOption("mode", "text/x-c++src");
var outputEditor = CodeMirror.fromTextArea(output, {
  mode: "text/x-python",
  lineNumbers: true,
  indentUnit: 4,
  theme: "material-darker",
  matchBrackets: true, // highlight matching brackets
  autoCloseBrackets: true, // automatically close brackets and quotes
});
// outputEditor.setOption("mode", "text/x-c++src");
inputEditor.setSize("30%");
outputEditor.setSize("30%");
