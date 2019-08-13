var addon = require('Lceb');
var express = require('express');
var bodyParser = require('body-parser');
var file = require('fs')
var handle = express();

handle.use(bodyParser.json())

handle.get('/', function(req,res)
{
  file.writeFile('Input.json',req.body,function(err)
  {
    if (err)
      console.log(err);
    else
      console.log('Write up');
  }
}                                 );
handle.listen(65535,function()
{
  console.log('Listening on port 65535');
}                             );

// -------------------------------------------------------------------------------
// var addon = require('Lceb');
// var express = require('express');
// var bodyParser = require("body-parser");
// var handle = express();
//
// handle.get('/', function(req,res)
// {
//
//  res.sendFile('C:/Users/Nicolas Grosjean/Desktop/Le-Compte-Est-Bon/Node/Windows/Input.json');
// }                                 );
// handle.listen(65535,function()
// {
//   console.log('Listening on port 65535');
// }                             );
