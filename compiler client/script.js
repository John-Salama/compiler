const btn = document.querySelector(".btn");
var output = document.querySelector(".output");
var input = document.querySelector(".input");

var inputEditor = CodeMirror.fromTextArea(input, {
  mode: "text/x-c++src",
  lineNumbers: true,
  indentUnit: 4,
  theme: "material-darker",
  matchBrackets: true, // highlight matching brackets
  autoCloseBrackets: true, // automatically close brackets and quotes
});
inputEditor.setSize("30%");

var outputEditor = CodeMirror.fromTextArea(output, {
  mode: "text/x-python",
  lineNumbers: true,
  indentUnit: 4,
  theme: "material-darker",
  matchBrackets: true, // highlight matching brackets
  autoCloseBrackets: true, // automatically close brackets and quotes
});
outputEditor.setSize("30%");

btn.addEventListener("click", async function () {
  const code = inputEditor.getValue();
  console.log(inputEditor.getValue());
  await axios.post("http://127.0.0.1:3000/runEXE", { code: code });
  const res = await axios.get("http://127.0.0.1:3000/runEXE");
  outputEditor.setValue(res.data);
});
