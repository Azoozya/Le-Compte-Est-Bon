// -------------------------------------------------------------------------------
var addon = require('Lceb');
  var express = require('express');
  var util = require('util');
  var handle = express();
  function getRandomInt(max) {
    return Math.floor(Math.random() * Math.floor(max));
  }
  handle.post('/', function(req,res,next) {
  console.log('Connection received');
  next();
  },function(req,res)
  {
    //On génère le nom de fichier
    //On crée le fichier avec le nom  qu'on vient de générer
  });
  handle.listen(65535,function()
  {
    console.log('Listening on port 65535');
  }                             );


