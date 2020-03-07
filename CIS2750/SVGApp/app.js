'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

app.use(express.json());


app.post('/test', (req, res) => {
  // console.log("req: " + req.body + "\tres: " + res.body);
  // const test = res.json({
  //   status: "success",
  //   value: req.body
  // });
  // console.log(test.status);
  console.log("Received from user: " + JSON.stringify(req.body));
  const temp = {
    status: "success",
    chicken: "Chicken"
  };
  res.send(JSON.stringify(temp));
  console.log('sending: ' + JSON.stringify(temp) + " to the client.\n");
});

//Sample endpoint
app.get('/someendpoint', function(req , res){
  let retStr = req.query.name1 + " " + req.query.name2;
  res.send({
    foo: retStr
  });
});

app.get('/getSVGJSON', (req,res) => {
  let rects = ffi.Library('./libsvgparse', {
    'getSVGJSON': [ 'string', [ 'string' ] ]
  });
  let contents = rects.getSVGJSON(req.query.file);
  let temp = JSON.parse(contents);
  res.send(temp);
});

app.get('/getAllFiles', (req,res) => {
  fs.readdir('./uploads/', (err,items) => {
    if (err) {
      console.log("No files are uploaded.");
    }
    res.send(JSON.stringify(items));
  });
});

app.get('/getFileSize', (req, res) => {
  let h = fs.statSync(req.query.file);
  let size = h.size;
  res.send({
    size: size
  });
});

//Will return all valid files from the ./uploads/ folder
app.get('/returnValidSVGFiles', (req, res) => {
  fs.readdir('./uploads/', (err, items) => {
    if (err) {
      console.log("No files are available.");
    }
    let valid = ffi.Library('./libsvgparse', {
      'checkIfValid': ['bool', ['string']]
    });

    let allValidFiles = [];
    items.forEach(element => {
      let f = valid.checkIfValid("./uploads/"+element);
      if (f) {
        allValidFiles.push(element);
      } else {
        console.log("file is not valid. " + element);
      }
    });
    res.send({
      validFiles: allValidFiles
    })
  })
});

app.get('/getTitle', (req, res) => {
  let getTitle = ffi.Library('./libsvgparse', {
    'getTitleJSON': ['string', ['string']]
  });
  let title = getTitle.getTitleJSON("./uploads/" + req.query.fileName);
  res.send({
    title: title
  });
});

app.get('/getDescription', (req, res) => {
  let getDesc = ffi.Library('./libsvgparse', {
    'getDescJSON': ['string', ['string']]
  });
  let desc = getDesc.getDescJSON("./uploads/" + req.query.fileName);
  res.send({
    desc: desc
  });
});

app.get('/getRects', (req, res) => {
  let getRects = ffi.Library('./libsvgparse', {
    'getRectsJSON': ['string', ['string']]
  });
  let rects = getRects.getRectsJSON("./uploads/" + req.query.fileName);
  res.send({
    rects: rects
  });
});

app.get('/getCircs', (req, res) => {
  let getCircles = ffi.Library('./libsvgparse', {
    'getCircsJSON': ['string', ['string']]
  });
  let circs = getCircles.getCircsJSON("./uploads/" + req.query.fileName);
  res.send({
    circ: circs
  });
});

app.get('/getPaths', (req, res) => {
  let getPaths = ffi.Library('./libsvgparse', {
    'getPathsJSON': ['string', ['string']]
  });
  let paths = getPaths.getPathsJSON("./uploads/" + req.query.fileName);
  res.send({
    paths: paths
  });
});

app.get('/getGroups', (req, res) => {
  let getGroups = ffi.Library('./libsvgparse', {
    'getGroupsJSON': ['string', ['string']]
  });
  let groups = getGroups.getGroupsJSON("./uploads/" + req.query.fileName);
  res.send({
    groups: groups
  });
});

app.get('/JSONtoAttrsRect', (req, res) => {
  let getAttr = ffi.Library('./libsvgparse', {
    'getAttributesRect': ['string', ['string', 'string']]
  });
  let attrs = getAttr.getAttributesRect("./uploads/" + req.query.fileName, req.query.string);
  res.send({
    attrs: attrs
  });
});

app.get('/JSONtoAttrsCirc', (req, res) => {
  let getAttr = ffi.Library('./libsvgparse', {
    'getAttributesCirc': ['string', ['string', 'string']]
  });
  let attrs = getAttr.getAttributesCirc("./uploads/" + req.query.fileName, req.query.string);
  res.send({
    attrs: attrs
  });
});

app.get('/JSONtoAttrsPath', (req, res) => {
  let getAttr = ffi.Library('./libsvgparse', {
    'getAttributesPath': ['string', ['string', 'string']]
  });
  let attrs = getAttr.getAttributesPath("./uploads/" + req.query.fileName, req.query.string);
  res.send({
    attrs: attrs
  });
});

app.get('/JSONtoAttrsGroup', (req, res) => {
  let getAttr = ffi.Library('./libsvgparse', {
    'getAttributesGroup': ['string', ['string', 'string']]
  });
  let attrs = getAttr.getAttributesGroup("./uploads/" + req.query.fileName, req.query.string);
  res.send({
    attrs: attrs
  });
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);