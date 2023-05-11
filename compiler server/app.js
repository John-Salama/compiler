const express = require("express");
const fs = require("fs");
const cors = require("cors");
const { exec } = require("child_process");

const app = express();

app.use(express.json());
app.use(cors());

// define a route for running the executable file
app.get("/runEXE", function (req, res) {
  // call exec to run the executable file with arguments
  exec("compiler.exe", function (err, stdout, stderr) {
    if (err) {
      // if an error occurs, log it and return a 500 error response
      console.error(err);
      return res.send(stdout);
    }
    // if the command succeeds, log the output and return it to the client
    console.log(stdout);
    res.send(stdout);
  });
});

//write a route to accept the input data from the client
app.post("/runEXE", function (req, res) {
  // get the input data from the request body
  const inputData = req.body.code;
  // write the input data to a file
  fs.writeFile("input.cpp", inputData, function (err) {
    if (err) {
      // if an error occurs, log it and return a 500 error response
      console.error(err);
      return res.status(500).send("Error writing to file");
    }
    // if the command succeeds, log the output and return it to the client
    console.log("Input saved to file");
    res.send("Input saved to file");
  });
});

// start the server and listen for incoming requests
app.listen(3000, function () {
  console.log("Server started on port 3000");
});
