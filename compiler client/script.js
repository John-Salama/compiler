const btn = document.querySelector(".btn");
var output = document.querySelector(".output");
var input = document.querySelector(".input");
var download = document.querySelector(".download-anchor");
var downloadBtn = document.querySelector(".download");
var copyBtn = document.querySelector(".copy");
var copy = document.querySelector(".copy-anchor");

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
  readOnly: true,
});
outputEditor.setSize("30%");

btn.addEventListener("click", async function () {
  const code = inputEditor.getValue();
  console.log(inputEditor.getValue());
  await axios.post("http://127.0.0.1:3000/runEXE", { code: code });
  const res = await axios.get("http://127.0.0.1:3000/runEXE");
  outputEditor.setValue(res.data);
  downloadBtn.style.visibility = "visible";
  copyBtn.style.visibility = "visible";
  copyBtn.src = "./img/copy.svg";
});

download.addEventListener("click", function () {
  var code = outputEditor.getValue();
  var blob = new Blob([code], { type: "text/x-python" });
  download.href = URL.createObjectURL(blob);
});

copy.addEventListener("click", function () {
  var code = outputEditor.getValue();
  var textarea = document.createElement("textarea");
  textarea.value = code;
  document.body.appendChild(textarea);
  textarea.select();
  document.execCommand("copy");
  document.body.removeChild(textarea);
  copyBtn.src = "./img/copied.svg";
});
