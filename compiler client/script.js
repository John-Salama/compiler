const btn = document.querySelector(".btn");
const output = document.querySelector(".output");
const input = document.querySelector(".input");

btn.addEventListener("click", async function () {
  const code = input.value;
  console.log(code);
  await axios.post("http://127.0.0.1:3000/runEXE", { code: code });
  const res = await axios.get("http://127.0.0.1:3000/runEXE");
  output.textContent = res.data;
});
